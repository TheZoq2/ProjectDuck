#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "block.hpp"

enum PlayerType {DUCK, CRAB};

class Entity {

public:

    Entity(sf::Vector2<double>& position, int mass);
    virtual ~Entity() {}

    virtual void draw(sf::RenderWindow& window) const = 0;
    virtual std::vector<sf::Vector2<int>> get_blocks() const = 0;

    /*
     * Gets the velocity vector of the direction and speed
     * in which it wants to move.
     */
    virtual sf::Vector2<double> wants_to_move() const = 0;
    virtual void interact();

    virtual bool 
        can_interact_with(PlayerType type, sf::Vector2<double> position);

    sf::Vector2<double> get_position() const;
    void set_position(const sf::Vector2<double>& position);
    int get_mass() const;

protected:

    sf::Vector2<double> position;
    int mass;

};

#endif
