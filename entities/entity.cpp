#include "entity.hpp"

Entity::Entity(sf::Vector2<double> position, int mass, std::string name) {
    this->mass = mass;
    this->position = position;
    this->body = nullptr;
    this->name = name;
}

bool Entity::can_interact_with(PlayerType type, 
        sf::Vector2<double> position) {
    return false;
}

// does nothing by default
void Entity::interact() {}

sf::Vector2<double> Entity::get_position() const {
    return position;
}


void Entity::set_position(const sf::Vector2<double>& position) {
    this->position = position;
}


int Entity::get_mass() const {
    return mass;
}

void Entity::body_init(int width, int height, cpSpace* space) {
    this->body = cpSpaceAddBody(space,
            cpBodyNew(mass, cpMomentForBox(mass, width, height)));
    cpBodySetPos(body, cpv(position.x, position.y));
    cpShape* shape = cpSpaceAddShape(space,
            cpBoxShapeNew(body, width, height));
}

std::string Entity::get_name() const {
    return name;
}
