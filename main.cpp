#include <iostream>
#include <SFML/Window.hpp>
#include "entity.h"
#include "duck.h"
#include "level.h"
#include  "wave.h"

int main() {

    std::cout << "Hello World!" << std::endl;

	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close, settings);

	Wave wave(800, 200, 100);

    while (window.isOpen())
    {
        sf::Event event;
		window.clear();

		wave.draw(window);
		wave.update();

		window.display();

        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
    }

    return 0;
}
