#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>

#include "wave.h"
#include "entities/entity.hpp"
#include "level.hpp"


int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close, settings);

	sf::Texture bTexture;
	sf::Sprite bImage;

	if(!bTexture.loadFromFile("assets/background.png")){
		std::cout << "Could not load background image" << std::endl;
	}

	bImage.setTexture(bTexture);
	bImage.setScale(1.0f, 600.0f/ bTexture.getSize().y);

	Wave wave(800, 200, 100);

	Level level("assets/level.json");

	auto vec = sf::Vector2<double>(5, 5);

	while (window.isOpen())
	{
		sf::Event event;

		clock_t timer_start = clock();
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        cpSpaceStep(space, time_step);
        cpVect pos = cpBodyGetPos(ballBody);
        sprite.setRotation(cpBodyGetAngle(ballBody));
        sprite.setPosition(sf::Vector2f(pos.x, pos.y));
		window.clear(sf::Color::Blue);

		window.draw(bImage);
		wave.update();

		wave.draw(window);
		level.update();
		level.draw(window);

		window.display();
		clock_t timer_end = clock();

		std::cout << (float)(timer_end - timer_start)/(CLOCKS_PER_SEC/1000) << std::endl;
	}

	return 0;
}
