#include "vectorutils.hpp"
#include <cmath>

float compute_distance(float x1, float x2, float y1, float y2) {
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

float distance(sf::Vector2<int> v1, sf::Vector2<int> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

float distance(sf::Vector2<float> v1, sf::Vector2<int> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

float distance(sf::Vector2<int> v1, sf::Vector2<float> v2) {
    return compute_distance(v1.x, v2.x, v1.y, v2.y);
}

float distance(sf::Vector2<float> v1, sf::Vector2<float> v2) {
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
