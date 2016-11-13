#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"
#include "constants.hpp"

const static float DUCK_MASS = 0.1;
const static int NUM_DUCK_SPRITES = 3;

class Duck : public Entity {

public:

    Duck(sf::Vector2<float> position, cpSpace* space, std::string name);

    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;
    virtual void move() override;
    virtual void set_position(const sf::Vector2<float>& position) override;

private:
    sf::Sprite sprites[NUM_DUCK_SPRITES];
    sf::Texture textures[NUM_DUCK_SPRITES];
    int current_sprite;
    int delay;
};

#endif
