#include "pressure_plate.hpp"

bool handle_pressure(cpArbiter* arb, cpSpace* space, void* ptr) {
    PressurePlate* plate = (PressurePlate*)ptr;
    CP_ARBITER_GET_SHAPES(arb, plate_shape, obj_shape);
    cpBody* obj_body = cpShapeGetBody(obj_shape);
    float mass = cpBodyGetMass(obj_body);

    plate->has_enough_weight = mass >= MASS_THREASHOLD;
    
    return true;
}

PressurePlate::PressurePlate(sf::Vector2<float> position, 
            std::string name, cpSpace* space) 
            : Entity(position, 0, name) {
    cpVect v = graphics_to_physics(position);
    cpBB bb = cpBBNew(v.x, v.y, v.x + PLATE_WIDTH, v.y + PLATE_HEIGHT);
    body = cpSpaceGetStaticBody(space);

    cpShape* shape = cpSpaceAddShape(space,
            cpBoxShapeNew2(body, bb));
    cpShapeSetSensor(shape, true);
    cpShapeSetCollisionType(shape, 2);

    cpSpaceAddCollisionHandler(space, 2, 0, NULL, 
            (cpCollisionPreSolveFunc)handle_pressure, NULL, nullptr, this);

    main_texture.loadFromFile("assets/pressure_plate.png");
    pushed_texture.loadFromFile("assets/pressure_plate_pushed.png");
    main_sprite.setTexture(main_texture);
    pushed_sprite.setTexture(pushed_texture);

    main_sprite.setPosition(sf::Vector2f(position.x, position.y));
    pushed_sprite.setPosition(sf::Vector2f(position.x, position.y));
    current_sprite = &main_sprite;
}

void PressurePlate::draw(sf::RenderWindow& window) 
{
    window.draw(*current_sprite);
}

std::vector<sf::Vector2<int>> PressurePlate::get_blocks() const
{
    return std::vector<sf::Vector2<int>>();
}

bool PressurePlate::can_interact_with(PlayerType type,
        sf::Vector2<float> position) {
    return has_enough_weight;
}


