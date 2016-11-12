#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.hpp"
#include <map>
#include <string>
#include "block.hpp"
#include "constants.hpp"

class Level {

public:

    Level(std::string filename);

    void draw(sf::RenderWindow& window);

private:

    int width, height;

    std::vector<std::vector<Block>> grid;

    std::vector<Entity*> entities;

    std::map<Entity*, std::vector<Entity*>> interaction_map;
};

#endif
