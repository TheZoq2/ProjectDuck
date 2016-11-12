#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block {
public:
    Block(sf::Sprite sprite);
    void draw(sf::RenderWindow& window, sf::Vector2<int> position);
private:
    sf::Sprite sprite;
};

#endif
