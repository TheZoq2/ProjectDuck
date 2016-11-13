#include "duck.hpp"
#include <SFML/System.hpp>

Duck::Duck(sf::Vector2<double> position, cpSpace* space)
    : Entity(position, DUCK_MASS)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    texture.loadFromFile("assets/duck.png");
    sprite.setTexture(texture);
}

void Duck::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    sprite.setPosition(pos.x, pos.y);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Duck::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Duck::wants_to_move() const {
    cpVec velocity;
    cpVec.y = cpBodyGetVel(body).y;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        velocity.x += DUCK_CRAB_SPEED;
    }
	cpBodySetVel(body, cpVec);
}

void Duck::set_position(const sf::Vector2<double>& position) {
    this->position = position;
    cpBodySetPos(body, graphics_to_physics(position));
}

