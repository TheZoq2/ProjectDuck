#include "crab.hpp"
#include "vectorutils.hpp"
#include "util.hpp"

Crab::Crab(sf::Vector2<float> position, std::string name, cpSpace* space)
    : Entity(position, CRAB_MASS, name)
{
    std::string files[] = {
        "assets/crab1.png",
        "assets/crab2.png",
        "assets/crab3.png"
    };
    for (int i = 0; i < NUM_CRAB_SPRITES; ++i) {
        sprites[i] = load_sprite(textures[i], files[i]);
        sprites[i].setScale(DUCK_CRAB_SCALE, DUCK_CRAB_SCALE);
    }
    sf::Vector2u textureSize = textures[0].getSize();
    body_init(textureSize.x * DUCK_CRAB_SCALE, textureSize.y * DUCK_CRAB_SCALE, space);
    current_sprite = 0;
    delay = 0;
}

void Crab::draw(sf::RenderWindow& window) {
    cpVect pos = cpBodyGetPos(body);
    position = physics_to_graphics(pos);
    sprites[current_sprite].setPosition(position.x, position.y);
    sprites[current_sprite].setRotation((-180 / 3.14) * cpBodyGetAngle(body));
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
    cpVect velocity = cpv(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
        velocity.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
        velocity.x += DUCK_CRAB_SPEED;
    }

	cpBodyApplyImpulse(body, cpvmult(velocity, DT), cpv(0, 0));

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
        cpVect vel = cpBodyGetVel(body);
        vel.y = 400;
        cpBodySetVel(body, vel);
    }
}

