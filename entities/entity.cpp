#include "entity.hpp"
#include <vectorutils.hpp>

Entity::Entity(sf::Vector2<float> position, float mass) {
    this->mass = mass;
    this->position = position;
    this->body = nullptr;
}

bool Entity::can_interact_with(PlayerType type,
        sf::Vector2<float> position) {
    return false;
}

// does nothing by default
void Entity::interact() {}

sf::Vector2<float> Entity::get_position() const {
    return position;
}


void Entity::set_position(const sf::Vector2<float>& position) {
    this->position = position;
}


int Entity::get_mass() const {
    return mass;
}

void Entity::body_init(int width, int height, cpSpace* space) {
    this->body = cpSpaceAddBody(space,
            cpBodyNew(mass, cpMomentForBox(mass, width, height)));
    cpBodySetPos(body, graphics_to_physics(sf::Vector2f(position.x, position.y)));
    cpShape* shape = cpSpaceAddShape(space,
            cpBoxShapeNew(body, width, height));
}

