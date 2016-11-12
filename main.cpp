#include <iostream>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "entity.hpp"
#include "duck.hpp"
#include "level.hpp"

int main() {

    std::cout << "Hello World!" << std::endl;

    sf::RenderWindow window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close);

    Level level;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

	window.clear(sf::Color::Black);

        level.draw(window);

	window.display();
    }
    return 0;
}
