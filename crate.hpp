#ifndef CRATE_H
#define CRATE_H

#include "entity.hpp"

const static int CRATE_MASS = 2;

class Crate : public Entity {
public:
    
    Crate(sf::Vector2<double>& position, sf::Vector2<double>& velocity) 
        : Entity(position, velocity, CRATE_MASS) {}
    
};

#endif
