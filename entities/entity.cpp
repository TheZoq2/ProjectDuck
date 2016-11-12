#include "entity.hpp"

Entity::Entity(sf::Vector2<double> position, int mass) {
    this->mass = mass;
    this->position = position;
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

