#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"

const static int DUCK_MASS = 5;

class Duck : public Entity {

public:
    
    Duck(sf::Vector2<double>& position) 
        : Entity(position, DUCK_MASS) {}

    virtual void draw(sf::RenderWindow& window) const override;
    virtual std::vector<Block> get_blocks(int grid_height, int grid_width)
        const override;
    virtual sf::Vector2<double> wants_to_move() const override;

};

#endif
