#include "seaweed.hpp"

SeaWeed::SeaWeed(sf::Vector2f position, std::string name) :
    Entity(position, 0, name)
{
    std::string filename = "assets/seaweed.png";
    texture.loadFromFile(filename);
    sprite.setTexture(texture);
    sprite.setPosition(position);
}

void SeaWeed::draw(sf::RenderWindow& window) 
{
    window.draw(sprite);
}

void SeaWeed::interact()
{
    
}

bool SeaWeed::can_interact_with(PlayerType type, sf::Vector2f position)
{
    return false;
}

