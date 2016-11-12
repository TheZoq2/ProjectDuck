#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.hpp"
#include <map>
#include <string>
#include "block.hpp"

const int TILE_SIZE = 32;

class Level {

public:

    Level(std::string filename);

    void draw(sf::Window& window);

private:

    int width, height;

    std::vector<std::vector<Block>> grid;

    std::vector<Entity*> entities;

    std::map<Entity*, std::vector<Entity*>> interaction_map;
};

#endif
