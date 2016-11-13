#include "level.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>    // std::max
#include "entities/crab.hpp"
#include "entities/duck.hpp"
#include "entities/crate.hpp"
#include "entities/lever.hpp"
#include "entities/wall.hpp"
#include "entities/seaweed.hpp"
#include "util.hpp"
#include "vectorutils.hpp"
#include "water_physics.hpp"

using namespace std;
// For JSON parsing
using namespace nlohmann;

Level::Level(std::string filename)
{
    const auto tile_file = "assets/tiles.png";
    if (!tile_texture.loadFromFile(tile_file)) {
        cerr << "ERROR: could not load " << tile_file << std::endl;
    }

    ifstream file(filename);
    json level_json_data;
    file >> level_json_data;
    auto tile_layer = level_json_data["layers"][0];
    auto tile_layer_data = tile_layer["data"];

    width = tile_layer["width"];
    height = tile_layer["height"];

    grid.resize(width);

    int tile_x = 0, tile_y = 0;
    for (int tile_index : tile_layer_data) {
        Block* block = nullptr;
        if (tile_index != 0) {
            sf::Sprite sprite;
            sprite.setTexture(tile_texture);
            sprite.setTextureRect(sf::IntRect((tile_index - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            sprite.setPosition(sf::Vector2f(tile_x * TILE_SIZE, tile_y * TILE_SIZE));

            block = new Block(sprite);
        }
        grid[tile_x].push_back(block);

        tile_x++;
        if (tile_x >= width) {
            tile_y++;
            tile_x = 0;
        }
    }

    init_physics();


    //Loading entities
    auto entity_layer = level_json_data["layers"][1];
    auto entity_layer_data = entity_layer["objects"];

    std::string default_name = "a";
    // load the entities in the list of entities
    for(auto entity : entity_layer_data)
    {
        int x = entity["x"];
        int y = entity["y"];

        std::string type = entity["type"];

        Entity* new_entity = nullptr;

        sf::Vector2<float> pos(x, y);

        std::string name;
        if (entity.count("name") != 0) {
            name = entity["name"];
        } else {
            name = default_name;
            default_name += "a";
        }

        if (type == "lever")
        {
            new_entity = new Lever(pos, name);
        } else if (type == "crab") {
            new_entity = new Crab(pos, name, space);
            this->crab = (Crab*)new_entity;
        } else if (type == "duck") {
            new_entity = new Duck(pos, space, name);
            this->duck = (Duck*)new_entity;
        } else if (type == "crate") {
            new_entity = new Crate(pos, name);
        } else if (type == "wall") {
            new_entity = new Wall(pos, entity["height"],
                     entity["width"], name);
        } else if (type == "seaweed"){
            new_entity = new SeaWeed(pos, sf::Vector2f(entity["width"], entity["height"]), name, space);
        }

        if (new_entity != nullptr)
        {
            interaction_map[name] = std::vector<Entity*>();
            entities.push_back(new_entity);
        }
    }

    // load them into the interaction map
    for (auto entity : entity_layer_data) {
        if (entity["properties"].count("links") != 0) {
            vector<std::string> links = 
                splitString(entity["properties"]["links"], ',');

            // find the entity key
            for (auto& pair : interaction_map) {
                if (pair.first == entity["name"]) {
                    for (std::string link_name : links) {
                        Entity* link = find_entity(link_name);
                        if (link != nullptr) {
                            pair.second.push_back(link);
                        }
                    }
                }
            }
        }
    }

    //Loading audio zones
    auto audio_layer = level_json_data["layers"][2];
    auto audio_layer_data = audio_layer["objects"];

    std::cout << audio_layer["name"] << std::endl;

    for(auto zone : audio_layer_data)
    {
        int height = zone["height"];
        int width = zone["width"];
        int x = zone["x"];
        int y = zone["y"];

        auto properties = zone["properties"];
        std::string filename = properties["audio_name"];
        bool duck = true;
        bool crab = true;

        AudioZone result(sf::Vector2<float>(x, y), sf::Vector2<float>(width, height), duck, crab, "assets/" + filename);

        this->audio_zones.push_back(result);
    }

    auto finish_layer = level_json_data["layers"][3];
    auto finish_data = finish_layer["objects"];

    for(auto finish : finish_data)
    {
        this->finish.x = finish["x"];
        this->finish.y = finish["y"];
        this->finish.width = finish["width"];
        this->finish.height = finish["height"];
    }
}

Entity* Level::find_entity(std::string name) {
    for (Entity* e : entities) {
        if (e->get_name() == name) {
            return e;
        }
    }
    return nullptr;
}

Level::~Level() {
    for (auto col : grid) {
        for (auto block : col) {
            delete block;
        }
    }

    ChipmunkFreeSpaceChildren(space);
    cpSpaceFree(space);
}

void Level::draw(sf::RenderWindow& window)
{
    move_camera(window);

    for (int tile_x = 0; tile_x < width; tile_x++) {
        for (int tile_y = 0; tile_y < height; tile_y++) {
            Block* block = grid[tile_x][tile_y];
            if (block != nullptr)
                block->draw(window, sf::Vector2i(tile_x * TILE_SIZE, tile_y * TILE_SIZE));
        }
    }

    //Drawing entities
    for(auto entity : entities)
    {
        entity->draw(window);
    }

    sf::RectangleShape rectangle1(sf::Vector2f(200, 50));
    sf::RectangleShape rectangle2(sf::Vector2f(40, 80));
    rectangle1.setOrigin(100, 25);
    rectangle2.setOrigin(20, 40);

    cpVect pos = cpBodyGetPos(box_body1);
    rectangle1.setRotation(cpBodyGetAngle(box_body1));
    rectangle1.setPosition(physics_to_graphics(pos));

    pos = cpBodyGetPos(box_body2);
    rectangle2.setRotation(cpBodyGetAngle(box_body2));
    rectangle2.setPosition(physics_to_graphics(pos));

    window.draw(rectangle1);
    window.draw(rectangle2);
}

void Level::init_physics() {
    space = cpSpaceNew();
	cpSpaceSetIterations(space, 30);
	cpSpaceSetGravity(space, cpv(0, -500));
//	cpSpaceSetDamping(space, 0.5);
	cpSpaceSetSleepTimeThreshold(space, 0.5f);
	cpSpaceSetCollisionSlop(space, 0.5f);

    init_water_physics(space, width * TILE_SIZE);

    box_body1 = create_floating_box(200, 50, space);
    box_body2 = create_floating_box(40, 80, space);
}

void Level::physics() {
    float time_step = 1.0f / 60.0f;
    cpSpaceStep(space, time_step);
}

void Level::move_camera(sf::RenderWindow& window) {
    int camera_center = ((duck->get_position().x - crab->get_position().x))/(2);

    sf::View camera_view(sf::FloatRect(0, 0, 800, 600));
    // we keep our view centered on the player //as 400 is the half of the scrensize
    camera_view.setCenter(duck->get_position().x - camera_center,300);

    if ((duck->get_position().x - camera_center) > (window.getView().getCenter().x + 200)) {
        camera_view.setCenter(duck->get_position().x - camera_center - 200,300);
        window.setView(camera_view);
    }

    if ((duck->get_position().x - camera_center) < (window.getView().getCenter().x - 200)){
        camera_view.setCenter(duck->get_position().x - camera_center+200,300);
        window.setView(camera_view);
    }

    camera_x = window.getView().getCenter().x;
    
    if(camera_x < 400)
    {
        camera_x = 400;
        auto view = window.getView();
        view.setCenter(camera_x, view.getCenter().y);
        window.setView(view);
    }
}

void Level::update() {
    physics();
    // TODO collision detection
    for (Entity* entity : entities) {
        entity->move();

        if (entity->can_interact_with(
                    PlayerType::DUCK, duck->get_position())) {
            entity->interact();
            if (interaction_map.count(entity->get_name()) == 1) {
                // call the interact function of all the 
                // things this entity interacts with
                for (Entity* e : interaction_map[entity->get_name()]) {
                    e->interact();
                }
            }
        } else if (entity->can_interact_with(
                    PlayerType::CRAB, crab->get_position())) {
            entity->interact();
            if (interaction_map.count(entity->get_name()) == 1) {
                // call the interact function of all the 
                // things this entity interacts with
                for (Entity* e : interaction_map[entity->get_name()]) {
                    e->interact();
                }
            }
        }
    }

    for(auto& zone : audio_zones)
    {
        zone.try_play(crab->get_position(), duck->get_position());
    }
}


int Level::get_camera_x()
{
    return camera_x;
}

bool Level::is_finished()
{
    return is_pos_in_finish(crab->get_position()) && is_pos_in_finish(duck->get_position());
}

bool Level::is_pos_in_finish(sf::Vector2f pos)
{
    return pos.x > finish.x && pos.x < finish.x + finish.width &&
           pos.y > finish.y && pos.y < finish.y + finish.height;
}
