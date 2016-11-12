#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"

const static int DUCK_MASS = 5;

class Duck : public Entity {

public:

    Duck(sf::Vector2<double> position);

    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;
    virtual sf::Vector2<double> wants_to_move() const override;

};

#endif
