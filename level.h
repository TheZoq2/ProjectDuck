#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "entity.h"
#include <map>
#include <string>

struct Block;

class Level {

    public:

        void draw(sf::Window& window);

    private:

        std::vector<std::vector<Block>> grid;

        std::vector<Entity*> entities;

        std::map<std::string, std::vector<Entity*>> interaction_map;
};

#endif
