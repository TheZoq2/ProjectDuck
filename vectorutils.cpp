#include "vectorutils.hpp"
#include <cmath>

double compute_distance(double x1, double x2, double y1, double y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

double distance(sf::Vector2<int> v1, sf::Vector2<int> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

double distance(sf::Vector2<double> v1, sf::Vector2<int> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

double distance(sf::Vector2<int> v1, sf::Vector2<double> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

double distance(sf::Vector2<double> v1, sf::Vector2<double> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

sf::Vector2f physics_to_graphics(cpVect vec) {
    return sf::Vector2f(vec.x, 600-vec.y);
}

cpVect graphics_to_physics(sf::Vector2f vec) {
    cpVect newVec;
    newVec.x = vec.x;
    newVec.y = vec.y;
    return newVec;
}
