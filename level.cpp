#include "level.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>
#include "entities/lever.hpp"

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
        sf::Sprite sprite;
        sprite.setTexture(tile_texture);
        sprite.setTextureRect(sf::IntRect(tile_index * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
        sprite.setPosition(sf::Vector2f(tile_x * TILE_SIZE, tile_y * TILE_SIZE));

        Block block(sprite);
        grid[tile_x].push_back(block);

        tile_x++;
        if (tile_x >= width) {
            tile_y++;
            tile_x = 0;
        }
    }


    //Loading entities
    auto entity_layer = level_json_data["layers"][1];
    auto entity_layer_data = entity_layer["objects"];

    for(auto entity : entity_layer_data)
    {
        int x = entity["x"];
        int y = entity["y"];

        std::string type = entity["type"];

        Entity* new_entity = nullptr;

        if(type == "lever")
        {
            sf::Sprite sprite(entity_textures["lever"]);
            new_entity = new Lever(sprite, sf::Vector2<double>(x, y));
			std::cout << "adding lever" << std::endl;
        }
    }
}

void Level::draw(sf::RenderWindow& window)
{
    for (int tile_x = 0; tile_x < width; tile_x++) {
        for (int tile_y = 0; tile_y < height; tile_y++) {
            grid[tile_x][tile_y].draw(window, sf::Vector2i(tile_x * TILE_SIZE, tile_y * TILE_SIZE));
        }
    }
}


void Level::load_entity_textures()
{
    sf::Texture lever_texture;
    lever_texture.loadFromFile("assets/lever.png");
    this->entity_textures["lever"] = lever_texture;
}

void Level::update() {
    for (Entity* entity : entities) {
        entity->set_position(entity->wants_to_move() + entity->get_position());
    }
}

