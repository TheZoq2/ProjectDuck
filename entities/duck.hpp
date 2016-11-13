#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"
#include "constants.hpp"

const static int DUCK_MASS = 5;

class Duck : public Entity {

public:

    Duck(sf::Vector2<double> position, cpSpace* space);

    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;
    virtual void move() override;
    virtual void set_position(const sf::Vector2<double>& position) override;

private:
    sf::Sprite sprite;
    sf::Texture texture;

};

#endif
