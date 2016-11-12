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


class Level {

public:

    Level(std::string filename);
    ~Level();

    void draw(sf::RenderWindow& window);

    void update();

private:

    void load_entity_textures();

    void init_physics();

    cpSpace* space;
    cpShape* ground;
    cpBody* ball_body;
    cpShape* ball_shape;
    sf::Sprite ball_sprite;
    sf::Texture ball_texture;

    int width, height;

    std::vector<std::vector<Block*>> grid;

    std::vector<Entity*> entities;

    std::map<Entity*, std::vector<Entity*>> interaction_map;

    sf::Texture tile_texture;

    std::map<std::string, sf::Texture> entity_textures;

    void physics();
};

#endif
