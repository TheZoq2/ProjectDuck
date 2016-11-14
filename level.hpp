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

    bool is_finished();
private:
    bool is_pos_in_finish(sf::Vector2f pos);

    void init_physics();

    cpSpace* space;

    int width, height;

    Duck* duck = nullptr;
    Crab* crab = nullptr;

    std::vector<std::vector<Block*>> grid;

    std::vector<Entity*> entities;

    std::map<std::string, std::vector<Entity*>> interaction_map;

    sf::Texture tile_texture;

    std::vector<AudioZone> audio_zones;

    void physics();

    void move_camera(sf::RenderWindow& window);

    Entity* find_entity(std::string name);

    int camera_x;

    struct FinishArea
    {
        int x;
        int y;
        int width;
        int height;
    };

    FinishArea finish;
};


#endif
