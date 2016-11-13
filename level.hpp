#ifndef LEVEL_H
#define LEVEL_H

#include <SFML/Graphics.hpp>
#include <chipmunk/chipmunk.h>
#include <vector>
#include "entities/entity.hpp"
#include <map>
#include <string>
#include "block.hpp"
#include "constants.hpp"
#include "audio_zone.hpp"

#include "entities/crab.hpp"
#include "entities/duck.hpp"

class Level {

public:

    Level(std::string filename);
    ~Level();

    void draw(sf::RenderWindow& window);

    void update();

    int get_camera_x();

private:

    void init_physics();

    cpSpace* space;
    sf::Sprite box_sprite1;
    sf::Sprite box_sprite2;
    sf::Texture ball_texture;
    cpBody* box_body1;
    cpBody* box_body2;

    int width, height;

    Duck* duck = nullptr;
    Crab* crab = nullptr;

    std::vector<std::vector<Block*>> grid;

    std::vector<Entity*> entities;

    std::map<Entity*, std::vector<Entity*>> interaction_map;

    sf::Texture tile_texture;

    std::vector<AudioZone> audio_zones;

    void physics();

    void move_camera(sf::RenderWindow& window);

    int camera_x;
};

#endif
