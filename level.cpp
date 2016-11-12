#include "level.hpp"
#include "json.hpp"
#include <fstream>
#include <iostream>

using namespace std;
// For JSON parsing
using namespace nlohmann;

Level::Level(std::string filename)
{
    sf::Texture texture;
    const auto tile_file = "assets/tiles.png";
    if (!texture.loadFromFile(tile_file)) {
        cerr << "ERROR: could not load " << tile_file << std::endl;
    }

    ifstream file(filename);
    json level_json_data;
    file >> level_json_data;
    auto layer = level_json_data["layers"][0];
    auto layer_data = level_json_data["data"];

    width = layer["width"];
    height = layer["height"];

    grid.resize(width);

    int tile_x = 0, tile_y = 0;
    for (int tile_index : layer_data) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(tile_index * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
        sprite.setPosition(sf::Vector2f(tile_x * TILE_SIZE, tile_y * TILE_SIZE));

        Block block(sprite);
        grid[tile_x].push_back(block);

        tile_x++;
        if (tile_x > width) {
            tile_y++;
            tile_x = 0;
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
