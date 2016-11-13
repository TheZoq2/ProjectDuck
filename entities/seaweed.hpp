#ifndef H_SEAWEED
#define H_SEAWEED

#include "entity.hpp"

class SeaWeed : public Entity
{
public:

    SeaWeed(sf::Vector2<float> position, std::string name);

    virtual void draw(sf::RenderWindow& window) override;

    /*
     * Gets the velocity vector of the direction and speed
     * in which it wants to move.
     */
    virtual void interact() override;

    virtual bool
        can_interact_with(PlayerType type, sf::Vector2<float> position) override;
private:
    sf::Texture texture;
    sf::Sprite sprite;
};
#endif
