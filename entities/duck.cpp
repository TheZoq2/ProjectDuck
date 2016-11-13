#include "duck.hpp"
#include <SFML/System.hpp>
#include <vectorutils.hpp>

Duck::Duck(sf::Vector2<float> position, cpSpace* space, std::string name)
    : Entity(position, DUCK_MASS, name)
{
    body_init(TILE_SIZE, TILE_SIZE, space);
    textures[0].loadFromFile("assets/duck1.png");
    textures[1].loadFromFile("assets/duck2.png");
    textures[2].loadFromFile("assets/duck3.png");
    for (int i = 0; i < NUM_DUCK_SPRITES; ++i) {
        sprites[i].setTexture(textures[i]);
        sprites[i].setScale(DUCK_CRAB_SCALE, DUCK_CRAB_SCALE);
    }
    current_sprite = 0;
    delay = 0;

    this->space = space;
}

void Duck::draw(sf::RenderWindow& window) {
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

std::vector<sf::Vector2<int>> Duck::get_blocks() const {
    return std::vector<sf::Vector2<int>>();
}

void Duck::move() {
    cpVect impulse = cpv(0, 0);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::J)) {
        impulse.x -= DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::L)) {
        impulse.x += DUCK_CRAB_SPEED;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::I)) {
        if(can_jump())
        {
            impulse.y = 400;
        }
    }

	cpBodyApplyImpulse(body, cpvmult(impulse, DT), cpv(0, 0));

    cpVect vel = cpBodyGetVel(body);

    if(vel.x > DUCK_CRAB_SPEED)
    {
        vel.x = DUCK_CRAB_SPEED;
    }
    else if(vel.x < -DUCK_CRAB_SPEED)
    {
        vel.x = -DUCK_CRAB_SPEED;
    }



    cpBodySetVel(body, vel);
}

void Duck::set_position(const sf::Vector2<float>& position) {
    this->position = position;
    cpBodySetPos(body, graphics_to_physics(sf::Vector2f(position.x, position.y)));
}


bool Duck::can_jump()
{
    cpVect point = cpBodyGetPos(body);
    point.y -= 20;
    cpShape* shape = cpSpaceNearestPointQueryNearest(space, point, 1, CP_ALL_LAYERS, CP_NO_GROUP, nullptr);

    return shape != nullptr;
}
