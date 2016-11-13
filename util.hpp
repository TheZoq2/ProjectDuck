#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

std::string stringFindAndReplace(std::string original, std::string toFind, std::string toReplace);

std::string stringFindAndReplaceAll(std::string original, std::string toFind, std::string toReplace);

std::vector<std::string> splitString(std::string original, char splitChar);

#endif // UTIL_H
