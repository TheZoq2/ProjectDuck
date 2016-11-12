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

    auto vec = sf::Vector2<double>(5, 5);
    Crab crab(vec);


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

		wave.draw(window);
        level.draw(window);

        crab.draw(window);

		window.display();
    }

    return 0;
}
