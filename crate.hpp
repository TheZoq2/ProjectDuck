#ifndef CRATE_H
#define CRATE_H

#include "entity.hpp"

const static int CRATE_MASS = 2;

class Crate : public Entity {
public:
    
    Crate(sf::Vector2<double>& position);
    virtual void draw(sf::RenderWindow& window) const override;
    virtual std::vector<Block> get_blocks(int grid_height, int grid_width)
        const override;
    virtual sf::Vector2<double> wants_to_move() const override;

private:
    sf::Sprite sprite;
    
};

#endif
