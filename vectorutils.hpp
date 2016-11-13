#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

float distance(sf::Vector2<int> v1, sf::Vector2<int> v2);
float distance(sf::Vector2<float> v1, sf::Vector2<int> v2);
float distance(sf::Vector2<int> v1, sf::Vector2<float> v2);
float distance(sf::Vector2<float> v1, sf::Vector2<float> v2);


sf::Vector2f physics_to_graphics(cpVect vec);
cpVect graphics_to_physics(sf::Vector2f vec);

#endif
