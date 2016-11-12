#ifndef BLOCK_H
#define BLOCK_H

#include <SFML/Graphics.hpp>

class Block {
public:
    void draw(sf::Window& window, sf::Vector2<int> position);
private:
};

void draw(sf::Window& window, sf::Vector2<int> position){
	sf::RectangleShape rectangle(sf::Vector2f(50, 50));
	rectangle.setPosition(position.x, position.y);
}

#endif
