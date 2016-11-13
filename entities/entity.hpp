#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "block.hpp"
#include <chipmunk/chipmunk.h>

enum PlayerType {DUCK, CRAB};

class Entity {

public:

    Entity(sf::Vector2<double> position, int mass);
    virtual ~Entity() {}

    virtual void draw(sf::RenderWindow& window) = 0;
    virtual std::vector<sf::Vector2<int>> get_blocks() const = 0;

    /*
     * Gets the velocity vector of the direction and speed
     * in which it wants to move.
     */
    virtual void move() {}
    virtual void interact();

    virtual bool
        can_interact_with(PlayerType type, sf::Vector2<double> position);

    sf::Vector2<double> get_position() const;
    virtual void set_position(const sf::Vector2<double>& position);
    int get_mass() const;

protected:

    sf::Vector2<double> position;
    int mass;

    virtual void body_init(int width, int height, cpSpace* space);

    cpBody* body;

};

#endif
