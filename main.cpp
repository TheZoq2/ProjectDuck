#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include  "wave.h"
#include "entity.hpp"
#include "duck.hpp"
#include "level.hpp"
#include "crab.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close, settings);

	Wave wave(800, 200, 100);

    Level level("assets/level.json");


    while (window.isOpen())
    {
        sf::Event event;


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		window.clear(sf::Color::Black);

		wave.update();

        level.draw(window);
		wave.draw(window);

		window.display();
    }

    return 0;
}
