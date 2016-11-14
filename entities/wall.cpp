#include "wall.hpp"
#include <iostream>
#include "util.hpp"

Wall::Wall(sf::Vector2<float> position, int height,
        int width, std::string name, cpSpace* space)
    : Entity(position, WALL_MASS, name) {
    texture.setRepeated(true);
    sprite = load_sprite(texture, "assets/wall.png");
    sprite.setTexture(texture);
    sprite.setPosition(position.x, position.y);
    //sprite.setOrigin(width/2, height/2);

    this->body = cpBodyNew(mass, cpMomentForBox(mass, width, height));
    cpBodySetPos(body,
            graphics_to_physics(sf::Vector2f(position.x, position.y)));
    cpShape* shape = cpSpaceAddShape(space,
            cpBoxShapeNew(body, width, height));

    this->moved = false;
    this->height = height;
    this->width = width;
}

void Wall::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprite.setPosition(position);
    window.draw(sprite);
}

std::vector<sf::Vector2<int>> Wall::get_blocks() const
{
    return std::vector<sf::Vector2<int>>();
}

void Wall::interact() {
    moved = !moved;
    cpVect pos = cpBodyGetPos(body);
    if (moved) {
        pos.y += TILE_SIZE * 3;
        //std::cout << "Moving " << name << "..." << std::endl;
    } else {
        pos.y -= TILE_SIZE * 3;
    }
    cpBodySetPos(body, pos);
}

