#ifndef VECTORUTILS_H
#define VECTORUTILS_H
#include <SFML/Graphics.hpp>

double distance(sf::Vector2<int> v1, sf::Vector2<int> v2);
double distance(sf::Vector2<double> v1, sf::Vector2<int> v2);
double distance(sf::Vector2<int> v1, sf::Vector2<double> v2);
double distance(sf::Vector2<double> v1, sf::Vector2<double> v2);

#endif
