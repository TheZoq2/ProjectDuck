#include "crab.hpp"

Crab::Crab(sf::Vector2<double>& position)
    : Entity(position, CRAB_MASS)
{
    texture.create(32, 32);
    sprite.setTexture(texture);
    //sprite.setColor(sf::Color(255, 255, 255));
}

void Crab::draw(sf::RenderWindow& window) const {
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Crab::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Crab::wants_to_move() const {
    // TODO implement
}
