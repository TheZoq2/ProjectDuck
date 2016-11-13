#include "crab.hpp"
#include "vectorutils.hpp"

Crab::Crab(sf::Vector2<float> position, std::string name, cpSpace* space)
    : Entity(position, CRAB_MASS, name)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    texture.loadFromFile("assets/crab.png");
    sprite.setTexture(texture);
}

void Crab::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprite.setPosition(position.x, position.y);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Crab::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

void Crab::move() {
    cpVect velocity;
    velocity.y = cpBodyGetVel(body).y;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += DUCK_CRAB_SPEED;
    }
	cpBodySetVel(body, velocity);
}
