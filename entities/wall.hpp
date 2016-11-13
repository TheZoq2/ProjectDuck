#ifndef WALL_H
#define WALL_H 

#include "entities/entity.hpp"
#include "constants.hpp"

const static int WALL_MASS = 100;

class Wall : public Entity {

public:

    Wall(sf::Vector2<float> position, int height, int width);

    virtual void draw(sf::RenderWindow& window) override;
    virtual std::vector<sf::Vector2<int>> get_blocks() const override;

    virtual void interact() override;

private:

    sf::Texture texture;
    sf::Sprite sprite;
    int height;
    int width;
    bool moved;

};

#endif /* ifndef WALL_H */
