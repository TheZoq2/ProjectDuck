#include "crab.hpp"

Crab::Crab(sf::Vector2<double> position)
    : Entity(position, CRAB_MASS)
{
    
    texture.loadFromFile("assets/crab.png");
    sprite.setTexture(texture);
}

void Crab::draw(sf::RenderWindow& window) {
    sprite.setPosition(position.x, position.y);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Crab::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Crab::wants_to_move() const {
    sf::Vector2<double> velocity;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= 1;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += 1;
    }
    return velocity;
}
