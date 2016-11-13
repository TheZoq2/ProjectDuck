#include "duck.hpp"
#include <SFML/System.hpp>
#include <vectorutils.hpp>

Duck::Duck(sf::Vector2<float> position, cpSpace* space, std::string name)
    : Entity(position, DUCK_MASS, name)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    texture.loadFromFile("assets/duck.png");
    sprite.setTexture(texture);
}

void Duck::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprite.setPosition(position.x, position.y);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Duck::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

void Duck::move() {
    cpVect velocity = cpv(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        velocity.x += DUCK_CRAB_SPEED;
    }

	cpBodyApplyImpulse(body, cpvmult(velocity, DT), cpv(0, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        cpVect vel = cpBodyGetVel(body);
        vel.y = 400;
        cpBodySetVel(body, vel);
    }
}

void Duck::set_position(const sf::Vector2<float>& position) {
    this->position = position;
    cpBodySetPos(body, graphics_to_physics(sf::Vector2f(position.x, position.y)));
}

