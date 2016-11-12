#include "crate.hpp"

Crate::Crate(sf::Vector2<double> position) : Entity(position, CRATE_MASS) {
    sf::Texture texture;
    texture.create(32, 32);

    sprite.setTexture(texture);
}

void Crate::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Crate::get_blocks() const {
    std::vector<sf::Vector2<int>> blocks;
    int x = (int)position.x;
    int y = (int)position.y;
    blocks.push_back(sf::Vector2<int>(x, y));
    return blocks;
}

sf::Vector2<double> Crate::wants_to_move() const {
    // shouldn't move by itself
    return sf::Vector2<double>(0, 0);
}

