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

    const int LEVEL_WIDTH = layer["width"];
    const int LEVEL_HEIGHT = layer["height"];
    const int TILE_WIDTH = 32;
    for (int tile_index : layer_data) {
        sf::Sprite sprite;
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect(tile_index * TILE_WIDTH, 0, TILE_WIDTH, TILE_WIDTH));

        // TODO: create block here and push it into the vector
    }
}

void Level::draw(sf::Window& window)
{
}
