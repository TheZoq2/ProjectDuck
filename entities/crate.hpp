#ifndef CRATE_H
#define CRATE_H

#include "entity.hpp"

const static int CRATE_MASS = 2;

class Crate : public Entity {
public:

    Crate(sf::Vector2<double> position);
    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;

private:
    sf::Sprite sprite;

};

#endif
