#include "crab.hpp"
#include "vectorutils.hpp"

Crab::Crab(sf::Vector2<float> position, std::string name, cpSpace* space)
    : Entity(position, CRAB_MASS, name)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    textures[0].loadFromFile("assets/crab1.png");
    textures[1].loadFromFile("assets/crab2.png");
    textures[2].loadFromFile("assets/crab3.png");
    for (int i = 0; i < NUM_CRAB_SPRITES; ++i) {
        sprites[i].setTexture(textures[i]);
        sprites[i].setScale(DUCK_CRAB_SCALE, DUCK_CRAB_SCALE);
    }
    current_sprite = 0;
    delay = 0;
}

void Crab::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprites[current_sprite].setPosition(position.x, position.y);
    window.draw(sprites[current_sprite]);
    delay++;
    if (delay == DUCK_CRAB_DELAY) {
        delay = 0;
        current_sprite = (current_sprite + 1) % 3;
    }
}

std::vector<sf::Vector2<int>> Crab::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

void Crab::move() {
    cpVect velocity;
    velocity.y = cpBodyGetVel(body).y;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += DUCK_CRAB_SPEED;
    }
	cpBodySetVel(body, velocity);
}

