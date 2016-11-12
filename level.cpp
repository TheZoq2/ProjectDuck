#include "level.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "entities/crab.hpp"
#include "entities/duck.hpp"
#include "entities/crate.hpp"
#include "entities/lever.hpp"

using namespace std;
// For JSON parsing
using namespace nlohmann;

Level::Level(std::string filename)
{
    load_entity_textures();

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

    for(auto entity : entity_layer_data)
    {
        int x = entity["x"];
        int y = entity["y"];

        std::string type = entity["type"];

        Entity* new_entity = nullptr;

        if (type == "lever")
        {
            sf::Sprite sprite(entity_textures["lever"]);
            new_entity = new Lever(sprite, sf::Vector2<double>(x, y));
        } else if (type == "crab") {
            new_entity = new Crab(sf::Vector2<double>(x, y));
            this->crab = (Crab*)new_entity;
        } else if (type == "duck") {
            new_entity = new Duck(sf::Vector2<double>(x, y));
            this->duck = (Duck*)new_entity;
        } else if (type == "crate") {
            new_entity = new Crate(sf::Vector2<double>(x, y));
        }

        if (new_entity != nullptr)
        {
            entities.push_back(new_entity);
        }
    }
}

Level::~Level() {
    for (auto col : grid) {
        for (auto block : col) {
            delete block;
        }
    }

    cpShapeFree(ball_shape);
    cpBodyFree(ball_body);
    cpShapeFree(ground);
    cpSpaceFree(space);
}

void Level::draw(sf::RenderWindow& window)
{

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

    window.draw(ball_sprite);
}


void Level::load_entity_textures()
{
    sf::Texture lever_texture;
    if(!lever_texture.loadFromFile("assets/lever.png"))
    {
        std::cout << "failed to load lever texture" << std::endl;
    }
    this->entity_textures["lever"] = lever_texture;
}

void Level::init_physics() {
    // Create physics stuff
    space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, 100));

    ground = cpSegmentShapeNew(space->staticBody, cpv(-20, 50), cpv(20, 100), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    float mass = 1;
    float radius = 5;
    float moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    ball_body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ball_body, cpv(0, 15));

    ball_shape = cpSpaceAddShape(space, cpCircleShapeNew(ball_body, radius, cpvzero));
    cpShapeSetFriction(ball_shape, 0.7);

    ball_texture.loadFromFile("assets/crab.png");
    ball_sprite.setTexture(ball_texture);
}

void Level::physics() {
    float time_step = 1.0f / 60.0f;
    cpSpaceStep(space, time_step);
    cpVect pos = cpBodyGetPos(ball_body);
    ball_sprite.setRotation(cpBodyGetAngle(ball_body));
    ball_sprite.setPosition(sf::Vector2f(pos.x, pos.y));
}

void Level::move_camera() {

}

void Level::update() {
    physics();
    // TODO collision detection
    for (Entity* entity : entities) {
        entity->set_position(entity->wants_to_move() + entity->get_position());
        if (entity->can_interact_with(
                    PlayerType::DUCK, duck->get_position())) {
            entity->interact();
            if (interaction_map.count(entity) == 1) {
                // call the interact function of all the 
                // things this entity interacts with
                for (Entity* e : interaction_map[entity]) {
                    e->interact();
                }
            }
        }
    }

    move_camera();
}

