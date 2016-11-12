#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <chrono>
#include <thread>
#include <chipmunk/chipmunk.h>

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

    // Create physics stuff
    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, 100));

    cpShape* ground = cpSegmentShapeNew(space->staticBody, cpv(-20, 50), cpv(20, 100), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    float mass = 1;
    float radius = 5;
    float moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    cpBody* ball_body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ball_body, cpv(0, 15));

    cpShape* ball_shape = cpSpaceAddShape(space, cpCircleShapeNew(ball_body, radius, cpvzero));
    cpShapeSetFriction(ball_shape, 0.7);
    float time_step = 1.0 / 600.0;

    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("assets/crab.png");
    sprite.setTexture(texture);

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

        cpSpaceStep(space, time_step);
        cpVect pos = cpBodyGetPos(ball_body);
        sprite.setRotation(cpBodyGetAngle(ball_body));
        sprite.setPosition(sf::Vector2f(pos.x, pos.y));
		window.clear(sf::Color::Blue);

		window.draw(bImage);
		wave.update();

		wave.draw(window);
		level.update();
		level.draw(window);
        window.draw(sprite);

		window.display();


		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end-start;
		std::cout << elapsed_seconds.count() << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(16 - (int)elapsed_seconds.count())); 
	}

    cpShapeFree(ball_shape);
    cpBodyFree(ball_body);
    cpShapeFree(ground);
    cpSpaceFree(space);

	return 0;
}
