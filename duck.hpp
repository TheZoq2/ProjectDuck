#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"

const static int DUCK_MASS = 5;

class Duck : public Entity {

    public:
        
        Duck(sf::Vector2<double>& position, sf::Vector2<double>& velocity) 
            : Entity(position, velocity, DUCK_MASS) {}

        virtual void draw(sf::Window& window) const override;
        virtual std::vector<Block> get_blocks() const override;

};

#endif
