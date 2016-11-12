#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.hpp"
#include <map>
#include <string>
#include "block.hpp"

class Level {

public:

    Level(std::string filename);

    void draw(sf::Window& window);

private:

    std::vector<std::vector<Block>> grid;

    std::vector<Entity*> entities;

    std::map<std::string, std::vector<Entity*>> interaction_map;
};

#endif
