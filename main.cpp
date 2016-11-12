#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "wave.h"
#include "entities/entity.hpp"
#include "level.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::Vector2i screenDimensions(800,400);
    sf::RenderWindow window(screenDimensions, "Hello my friend! :)", sf::Style::Close, settings);

	/*sf::Texture bTexture;
	sf::Sprite bImage;

	if(!bTexture.loadFromFile("assets/background.png")){
		std::cout << "Could not load background image" << std::endl;
	}

	bImage.setTexture(bTexture);
	bImage.setScale(1.0f
*/
	Wave wave(800, 200, 100);

    Level level("assets/level.json");

    auto vec = sf::Vector2<double>(5, 5);


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


		window.display();
    }

    return 0;
}
