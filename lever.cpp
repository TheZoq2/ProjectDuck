#include "lever.hpp"

Lever::Lever(sf::Sprite sprite, sf::Vector2<double> position) : 
    Entity(position, 0),
    sprite(sprite)
{
    
}

void Lever::draw(sf::RenderWindow& window) const 
{
    //this->sprite.move(position.x, position.y);
}

std::vector<Block> Lever::get_blocks() const
{
    return std::vector<Block>();
}

sf::Vector2<double> Lever::wants_to_move() const
{
    return sf::Vector2<double>(0,0);
}



