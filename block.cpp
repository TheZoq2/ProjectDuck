#include "block.hpp"

Block::Block(sf::Sprite sprite) : sprite(sprite) {
}

void Block::draw(sf::Window& window, sf::Vector2<int> position){
	sf::RectangleShape rectangle(sf::Vector2f(50,50));
	rectangle.setPosition(position.x, position.y);
}
