#include "seaweed.hpp"
#include "../vectorutils.hpp"

#include <chipmunk/chipmunk.h>

SeaWeed::SeaWeed(sf::Vector2f position, sf::Vector2f size, std::string name, cpSpace* space) :
    Entity(position, 0, name)
{
    std::string filename = "assets/seaweed.png";
    texture.loadFromFile(filename);
    sprite.setTexture(texture);
    sprite.setPosition(position);
    
    this->body = cpBodyNewStatic();
    cpBodySetPos(body, graphics_to_physics(sf::Vector2f(position.x + size.x / 2, position.y + size.y/ 2)));
    cpShape* shape = cpSpaceAddShape(space,
            cpBoxShapeNew(body, size.x, size.y));
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

