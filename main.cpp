#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "wave.h"
#include "entities/entity.hpp"
#include "level.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close, settings);
sf::View viewLeft(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y/2));
viewLeft.setViewport(sf::FloatRect(0, 0, 1, 0.5));
sf::View viewRight(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y/2));
viewRight.setViewport(sf::FloatRect(0, 0.5, 1, 0.5));

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

		window.clear(sf::Color::Blue);

		wave.update();


		wave.draw(window);
        level.draw(window);
window.setView(viewLeft);
//window.draw(leftSprite);
window.setView(viewRight);
//window.draw(rightSprite);
		window.display();
    }

    return 0;
}
