#include "crab.hpp"

Crab::Crab(sf::Vector2<double>& position)
    : Entity(position, CRAB_MASS)
{
    sf::Texture texture;
    texture.create(32, 32);

    sprite.setTexture(texture);
}

void Crab::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

std::vector<Block> Crab::get_blocks(int grid_height, int grid_width) const {
    // TODO implement
}

sf::Vector2<double> Crab::wants_to_move() const {
    // TODO implement
}
