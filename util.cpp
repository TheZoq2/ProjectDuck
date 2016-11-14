
#include "util.hpp"

#include <iostream>

std::string stringFindAndReplace(std::string original, std::string toFind, std::string toReplace)
{
    std::size_t start = original.find(toFind);

    if(start != std::string::npos)
    {
        return original.replace(start, toFind.length(), toReplace);
    }
    return original;
}

std::string stringFindAndReplaceAll(std::string original, std::string toFind, std::string toReplace)
{
    std::string result = original;

    std::size_t start = result.find(toFind);

    while(start != std::string::npos)
    {
        result = stringFindAndReplace(result, toFind, toReplace);

        start = result.find(toFind);
    }

    return result;
}

std::vector<std::string> splitString(std::string original, char splitChar)
{
    std::vector<std::string> result;
    std::string buffer;

    for(auto c : original)
    {
        if(c == splitChar)
        {
            result.push_back(buffer);
            buffer = "";
        }
        else
        {
            buffer.push_back(c);
        }
    }

    result.push_back(buffer);
    return result;
}

sf::Sprite load_sprite(sf::Texture& texture, std::string filename)
{
    texture.loadFromFile(filename);
    sf::Vector2u size = texture.getSize();
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setOrigin(size.x / 2, size.y / 2);

    return sprite;
}
