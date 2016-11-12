#include <iostream>
#include <SFML/Window.hpp>
#include "entity.hpp"
#include "duck.hpp"
#include "level.hpp"

int main() {

    std::cout << "Hello World!" << std::endl;

    sf::Window window(sf::VideoMode(800, 600), "Hello my friend! :)", sf::Style::Close);

    Level level("assets/level.json");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        level.draw(window);
    }
    return 0;
}
