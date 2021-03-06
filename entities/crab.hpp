#ifndef CRAB_H
#define CRAB_H

#include "entity.hpp"
#include "constants.hpp"

const static int CRAB_MASS = 1;
const static int NUM_CRAB_SPRITES = 3;

class Crab : public Entity {

public:

    Crab(sf::Vector2<float> position, std::string name, cpSpace* space);

    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;
    virtual void move() override;

private:
    sf::Sprite sprites[NUM_CRAB_SPRITES];
    sf::Texture textures[NUM_CRAB_SPRITES];
    int current_sprite;
    int delay;
};

#endif
