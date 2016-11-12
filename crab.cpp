#include "crab.hpp"

Crab::Crab(sf::Vector2<double>& position, sf::Vector2<double>& velocity)
    : Entity(position, velocity, CRAB_MASS)
{
    sf::Texture texture;
    texture.create(32, 32);

    sprite.setTexture(texture);
}

void Crab::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

std::vector<Block> Crab::get_blocks() const {
    // TODO implement
}

sf::Vector2<double> Crab::wants_to_move() const {
    // TODO implement
}
