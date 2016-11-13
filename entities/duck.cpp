#include "duck.hpp"
#include <SFML/System.hpp>
#include <vectorutils.hpp>

Duck::Duck(sf::Vector2<float> position, cpSpace* space, std::string name)
    : Entity(position, DUCK_MASS, name)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    textures[0].loadFromFile("assets/duck1.png");
    textures[1].loadFromFile("assets/duck2.png");
    textures[2].loadFromFile("assets/duck3.png");
    for (int i = 0; i < NUM_DUCK_SPRITES; ++i) {
        sprites[i].setTexture(textures[i]);
        sprites[i].setScale(DUCK_CRAB_SCALE, DUCK_CRAB_SCALE);
    }
    current_sprite = 0;
    delay = 0;
}

void Duck::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprites[current_sprite].setPosition(position.x, position.y);
    window.draw(sprites[current_sprite]);
    delay++;
    if (delay == DUCK_CRAB_DELAY) {
        delay = 0;
        current_sprite = (current_sprite + 1) % 3;
    }
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

