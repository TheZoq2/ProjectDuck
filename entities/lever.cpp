#include "lever.hpp"

#include <iostream>

Lever::Lever(sf::Sprite sprite, sf::Vector2<double> position) : 
    Entity(position, 0),
    sprite(sprite)
{
    this->sprite.setPosition(sf::Vector2f(position.x, position.y));
}

void Lever::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Lever::get_blocks() const
{
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Lever::wants_to_move() const
{
    return sf::Vector2<double>(0,0);
}



