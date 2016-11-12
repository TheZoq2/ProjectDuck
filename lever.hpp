#ifndef H_LEVER
#define H_LEVER

#include "entity.hpp"

class Lever: public Entity
{
    public:
        Lever(sf::Sprite sprite, sf::Vector2<double> position);

        void draw(sf::RenderWindow& window) const;
        std::vector<Block> get_blocks() const;
        sf::Vector2<double> wants_to_move() const;
    
    private:
        sf::Sprite sprite;
};

#endif
