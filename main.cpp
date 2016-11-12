#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>
#include <thread>

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

	while (window.isOpen())
	{

		std::chrono::time_point<std::chrono::system_clock> start, end;
		start = std::chrono::system_clock::now();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

		window.clear(sf::Color::Blue);

		window.draw(bImage);
		wave.update();

		wave.draw(window);
		level.update();
		level.draw(window);

		window.display();


		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << elapsed_seconds.count() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(16 - (int)elapsed_seconds.count())); 
	}

	return 0;
}
