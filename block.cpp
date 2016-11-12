#include "block.hpp"

Block::Block(sf::Sprite sprite) : sprite(sprite) {
}

void Block::draw(sf::RenderWindow& window, sf::Vector2<int> position){
	sprite.move(sf::Vector2f(position.x,position.y));
	window.draw(sprite);
}
