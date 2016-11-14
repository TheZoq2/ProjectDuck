
#include "lever.hpp"
#include "vectorutils.hpp"
#include "util.hpp"

#include <iostream>

Lever::Lever(sf::Vector2<float> position, std::string name) :
    Entity(position, 0, name)
{
    main_sprite = load_sprite(main_texture, "assets/lever.png");
    pulled_sprite = load_sprite(main_texture, "assets/lever_pulled.png");
    this->main_sprite.setPosition(sf::Vector2f(position.x, position.y));
    this->pulled_sprite.setPosition(sf::Vector2f(position.x, position.y));
    this->current_sprite = &main_sprite;

	state = OFF;
}

void Lever::draw(sf::RenderWindow& window)
{
    window.draw(*current_sprite);
	
	if(switch_cd > 0)
	{
		switch_cd--;
	}
}

std::vector<sf::Vector2<int>> Lever::get_blocks() const
{
    return std::vector<sf::Vector2<int>>();
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
	switch_cd = 60;
}

bool Lever::can_interact_with(PlayerType type, sf::Vector2<float> position) {
    return (distance(position, this->position) < INTERACT_DISTANCE)&&
		switch_cd == 0;
}

