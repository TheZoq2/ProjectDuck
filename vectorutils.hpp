#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>

double distance(sf::Vector2<int> v1, sf::Vector2<int> v2);
double distance(sf::Vector2<double> v1, sf::Vector2<int> v2);
double distance(sf::Vector2<int> v1, sf::Vector2<double> v2);
double distance(sf::Vector2<double> v1, sf::Vector2<double> v2);


sf::Vector2f physics_to_graphics(cpVect vec);
cpVect graphics_to_physics(sf::Vector2f vec);

#endif
