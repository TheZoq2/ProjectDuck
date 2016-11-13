#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "wave.h"
#include "entities/entity.hpp"
#include "level.hpp"
#include "sfml_icon.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
	sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close, settings);
	window.setFramerateLimit(60);

	//window.SetIcon( sfml_icon.width,  sfml_icon.height,  sfml_icon.pixel_data ); 

	sf::Texture bTexture;
	sf::Sprite bImage;

	if(!bTexture.loadFromFile("assets/background.png")){
		std::cout << "Could not load background image" << std::endl;
	}

	bImage.setTexture(bTexture);
	bImage.setScale(1.0f, 600.0f/ bTexture.getSize().y);

	Wave wave(3300, 400, 400);
	Wave wavebg(3300, 400, 400);

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

		//window.draw(bImage);
		wave.update();
        wave.set_camera_position(level.get_camera_x());
		wavebg.update();
        wavebg.set_camera_position(level.get_camera_x());

		level.update();
		wavebg.draw(window);
		level.draw(window);
		wave.draw(window);

		window.display();
	}

	return 0;
}
