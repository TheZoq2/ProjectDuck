
#include "lever.hpp"
#include "vectorutils.hpp"

#include <iostream>

Lever::Lever(sf::Vector2<double> position) : 
    Entity(position, 0)
{
    main_texture.loadFromFile("assets/lever.png"); 
    pulled_texture.loadFromFile("assets/lever_pulled.png");
    main_sprite.setTexture(main_texture);
    pulled_sprite.setTexture(pulled_texture);
    this->main_sprite.setPosition(sf::Vector2f(position.x, position.y));
    this->pulled_sprite.setPosition(sf::Vector2f(position.x, position.y));
    this->current_sprite = &main_sprite;

	state = OFF;
}

void Lever::draw(sf::RenderWindow& window) 
{
    window.draw(*current_sprite);
}

std::vector<sf::Vector2<int>> Lever::get_blocks() const
{
    return std::vector<sf::Vector2<int>>();
}

sf::Vector2<double> Lever::wants_to_move() const
{
    return sf::Vector2<double>(0,0);
}

void Lever::interact() {
	if(state == ON)
	{
		state = OFF;
    	this->current_sprite = &main_sprite;
	}
	else
	{
		state = ON;
		this->current_sprite = &pulled_sprite;
	}
}

bool Lever::can_interact_with(PlayerType type, sf::Vector2<double> position) {
    return (distance(position, this->position) < INTERACT_DISTANCE);
}

