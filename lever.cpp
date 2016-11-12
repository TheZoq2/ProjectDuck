#include "lever.hpp"

Lever::Lever(sf::Sprite sprite, sf::Vector2<double> position) : 
    Entity(position, 0),
    sprite(sprite)
{
    sprite.setPosition(sf::Vector2f(position.x, position.y));
}

void Lever::draw(sf::RenderWindow& window) const 
{
    window.draw(sprite);
}

std::vector<Block> Lever::get_blocks() const
{
    return std::vector<Block>();
}

sf::Vector2<double> Lever::wants_to_move() const
{
    return sf::Vector2<double>(0,0);
}



