#ifndef ENTITY_H
#define ENTITY_H

#include <sfml/graphics.hpp>
#include <vector>

struct Block;

class Entity {

    public:
        
        std::vector<Block> get_blocks();

    private:
        
        sf::Vector2 position;
};

#endif
