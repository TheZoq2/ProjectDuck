#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "block.h"

class Entity {

    public:
        
        void draw(sf::Window& window);
        std::vector<Block> get_blocks();

    private:
        
        sf::Vector2<int> position;
};

#endif
