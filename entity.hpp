#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "block.hpp"

class Entity {

public:

    Entity(sf::Vector2<double>& position,
            sf::Vector2<double>& velocity, int mass);
    ~Entity();

    virtual void draw(sf::Window& window) const = 0;
    virtual std::vector<Block> get_blocks() const = 0;

    sf::Vector2<double> get_position() const;
    sf::Vector2<double> get_velocity() const;
    void set_position(sf::Vector2<double>& position);
    void set_velocity(sf::Vector2<double>& velocity);
    int get_mass() const;

private:

    sf::Vector2<double> position;
    sf::Vector2<double> velocity;
    int mass;

};

#endif
