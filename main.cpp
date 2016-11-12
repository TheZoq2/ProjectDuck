#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

#include "wave.h"
#include "entities/entity.hpp"
#include "level.hpp"

int main() {
	sf::ContextSettings settings;
	settings.antialiasingLevel = 16;
//	sf::Vector2i screenDimensions(800,400);
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

    cpSpace *space = cpSpaceNew();
    cpSpaceSetGravity(space, cpv(0, 100));

    cpShape* ground = cpSegmentShapeNew(space->staticBody, cpv(-20, 50), cpv(20, 100), 0);
    cpShapeSetFriction(ground, 1);
    cpSpaceAddShape(space, ground);

    float mass = 1;
    float radius = 5;
    float moment = cpMomentForCircle(mass, 0, radius, cpvzero);

    cpBody* ballBody = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(ballBody, cpv(0, 15));

    cpShape* ballShape = cpSpaceAddShape(space, cpCircleShapeNew(ballBody, radius, cpvzero));
    cpShapeSetFriction(ballShape, 0.7);

    float time_step = 1.0 / 600.0;

    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("assets/crab.png");
    sprite.setTexture(texture);

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
        sprite.setPosition(sf::Vector2f(pos.x, pos.y));

	window.draw(bImage);
		wave.update();


		window.clear(sf::Color::Blue);
		wave.draw(window);
        level.update();
        level.draw(window);
        window.draw(sprite);

		window.display();
    }

    cpShapeFree(ballShape);
    cpBodyFree(ballBody);
    cpShapeFree(ground);
    cpSpaceFree(space);

    return 0;
}
