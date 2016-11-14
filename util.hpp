#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>

std::string stringFindAndReplace(std::string original, std::string toFind, std::string toReplace);

std::string stringFindAndReplaceAll(std::string original, std::string toFind, std::string toReplace);

std::vector<std::string> splitString(std::string original, char splitChar);

sf::Sprite load_sprite(sf::Texture& texture, std::string filename);

#endif // UTIL_H
