#include "duck.hpp"
#include <SFML/System.hpp>

Duck::Duck(sf::Vector2<double> position)
    : Entity(position, DUCK_MASS)
{
    texture.loadFromFile("assets/duck.png");
    sprite.setTexture(texture);
}

void Duck::draw(sf::RenderWindow& window) {
    sprite.setPosition(position.x, position.y);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Duck::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Duck::wants_to_move() const {
    sf::Vector2<double> velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        velocity.x += DUCK_CRAB_SPEED;
    }
    return velocity;
}

