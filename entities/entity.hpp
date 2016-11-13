#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include "block.hpp"
#include <chipmunk/chipmunk.h>

enum PlayerType {DUCK, CRAB};

class Entity {

public:

    Entity(sf::Vector2<float> position, float mass, std::string name);
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
        can_interact_with(PlayerType type, sf::Vector2<float> position);

    sf::Vector2<float> get_position() const;
    virtual void set_position(const sf::Vector2<float>& position);
    int get_mass() const;
    std::string get_name() const;

protected:

    std::string name;
    
    sf::Vector2<float> position;
    float mass;

    virtual void body_init(int width, int height, cpSpace* space);

    cpBody* body;

};

#endif
