#ifndef CRAB_H
#define CRAB_H

#include "entity.hpp"

const static int CRAB_MASS = 3;

class Crab : public Entity {

public:

    Crab(sf::Vector2<double>& position) 
        : Entity(position, CRAB_MASS) {}

    virtual void draw(sf::RenderWindow& window) const override;
    virtual std::vector<Block> get_blocks() const override;
    virtual sf::Vector2<double> wants_to_move() const override;

};

#endif
