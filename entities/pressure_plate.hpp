#ifndef PRESSURE_PLATE_H
#define PRESSURE_PLATE_H

#include "entities/entity.hpp"
#include "constants.hpp"
#include "vectorutils.hpp"

const float MASS_THREASHOLD = 2;

const int PLATE_WIDTH = TILE_SIZE;
const int PLATE_HEIGHT = TILE_SIZE/8;

class PressurePlate : public Entity {
    
public:

    PressurePlate(sf::Vector2<float> position, 
            std::string name, cpSpace* space);
    void draw(sf::RenderWindow& window) override;
    std::vector<sf::Vector2<int>> get_blocks() const override;

    virtual bool
        can_interact_with(PlayerType type,
                sf::Vector2<float> position) override;

    bool has_enough_weight;

private:
    sf::Texture main_texture;
    sf::Texture pushed_texture;
    sf::Sprite main_sprite;
    sf::Sprite pushed_sprite;

    sf::Sprite* current_sprite;

};

#endif /* ifndef PRESSURE_PLATE_H */
