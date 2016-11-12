#include "duck.hpp"
#include <SFML/System.hpp>

Duck::Duck(sf::Vector2<double>& position)
    : Entity(position, DUCK_MASS)
{
}

void Duck::draw(sf::RenderWindow& window) const {
    // TODO implement
}

std::vector<sf::Vector2<int>> Duck::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Duck::wants_to_move() const {
    sf::Vector2<double> velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        velocity.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        velocity.x += 1;
    }
    return velocity;
}

