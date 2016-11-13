#include "level.hpp"
#include "json.hpp"
#include <chipmunk/constraints/util.h>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>    // std::max
#include "entities/crab.hpp"
#include "entities/duck.hpp"
#include "entities/crate.hpp"
#include "entities/lever.hpp"
#include "entities/wall.hpp"
#include "vectorutils.hpp"

using namespace std;
// For JSON parsing
using namespace nlohmann;

const float FLUID_DENSITY = 0.00014;
const float FLUID_DRAG = 1.0;

static bool water_pre_solve(cpArbiter* arb, cpSpace *space, void* ptr) {
    CP_ARBITER_GET_SHAPES(arb, water, poly)
	cpBody *body = cpShapeGetBody(poly);

	// Get the top of the water sensor bounding box to use as the water level.
	cpFloat level = cpShapeGetBB(water).t;

	// Clip the polygon against the water level
	int count = cpPolyShapeGetNumVerts(poly);
	int clippedCount = 0;
	cpVect clipped[count + 1];

	for(int i=0, j=count-1; i<count; j=i, i++){
		cpVect a = cpBodyLocal2World(body, cpPolyShapeGetVert(poly, j));
		cpVect b = cpBodyLocal2World(body, cpPolyShapeGetVert(poly, i));

		if(a.y < level){
			clipped[clippedCount] = a;
			clippedCount++;
		}

		cpFloat a_level = a.y - level;
		cpFloat b_level = b.y - level;

		if(a_level*b_level < 0.0f){
			cpFloat t = cpfabs(a_level)/(cpfabs(a_level) + cpfabs(b_level));

			clipped[clippedCount] = cpvlerp(a, b, t);
			clippedCount++;
		}
	}

	// Calculate buoyancy from the clipped polygon area
	cpFloat clippedArea = cpAreaForPoly(clippedCount, clipped);
	cpFloat displacedMass = clippedArea*FLUID_DENSITY;
	cpVect centroid = cpCentroidForPoly(clippedCount, clipped);
	cpVect r = cpvsub(centroid, cpBodyGetPos(body));

	cpFloat dt = cpSpaceGetCurrentTimeStep(space);
	cpVect g = cpSpaceGetGravity(space);

	// Apply the buoyancy force as an impulse.
	apply_impulse(body, cpvmult(g, -displacedMass*dt), r);

	// Apply linear damping for the fluid drag.
	cpVect v_centroid = cpvadd(body->v, cpvmult(cpvperp(r), body->w));
	cpFloat k = k_scalar_body(body, r, cpvnormalize_safe(v_centroid));
	cpFloat damping = clippedArea*FLUID_DRAG*FLUID_DENSITY;
	cpFloat v_coef = cpfexp(-damping*dt*k); // linear drag
//	cpFloat v_coef = 1.0/(1.0 + damping*dt*cpvlength(v_centroid)*k); // quadratic drag
	apply_impulse(body, cpvmult(cpvsub(cpvmult(v_centroid, v_coef), v_centroid), 1.0/k), r);

	// Apply angular damping for the fluid drag.
	cpFloat w_damping = cpMomentForPoly(FLUID_DRAG*FLUID_DENSITY*clippedArea, clippedCount, clipped, cpvneg(body->p));
    body->w *= cpfexp(-w_damping*dt*body->i_inv);

    return true;
}

Level::Level(std::string filename)
{
    const auto tile_file = "assets/tiles.png";
    if (!tile_texture.loadFromFile(tile_file)) {
        cerr << "ERROR: could not load " << tile_file << std::endl;
    }

    ifstream file(filename);
    json level_json_data;
    file >> level_json_data;
    auto tile_layer = level_json_data["layers"][0];
    auto tile_layer_data = tile_layer["data"];

    width = tile_layer["width"];
    height = tile_layer["height"];

    grid.resize(width);

    int tile_x = 0, tile_y = 0;
    for (int tile_index : tile_layer_data) {
        Block* block = nullptr;
        if (tile_index != 0) {
            sf::Sprite sprite;
            sprite.setTexture(tile_texture);
            sprite.setTextureRect(sf::IntRect((tile_index - 1) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE));
            sprite.setPosition(sf::Vector2f(tile_x * TILE_SIZE, tile_y * TILE_SIZE));

            block = new Block(sprite);
        }
        grid[tile_x].push_back(block);

        tile_x++;
        if (tile_x >= width) {
            tile_y++;
            tile_x = 0;
        }
    }

    init_physics();


    //Loading entities
    auto entity_layer = level_json_data["layers"][1];
    auto entity_layer_data = entity_layer["objects"];

    for(auto entity : entity_layer_data)
    {
        int x = entity["x"];
        int y = entity["y"];

        std::string type = entity["type"];

        Entity* new_entity = nullptr;
        
        sf::Vector2<float> pos(x, y);

        if (type == "lever")
        {
            new_entity = new Lever(pos);
        } else if (type == "crab") {
            new_entity = new Crab(pos, space);
            this->crab = (Crab*)new_entity;
        } else if (type == "duck") {
            new_entity = new Duck(pos, space);
            this->duck = (Duck*)new_entity;
        } else if (type == "crate") {
            new_entity = new Crate(pos);
        } else if (type == "wall") {
            new_entity = new Wall(pos, entity["height"], entity["width"]);
        }

        if (new_entity != nullptr)
        {
            entities.push_back(new_entity);
        }
    }

    //Loading audio zones
    auto audio_layer = level_json_data["layers"][2];
    auto audio_layer_data = audio_layer["objects"];

    std::cout << audio_layer["name"] << std::endl;

    for(auto zone : audio_layer_data)
    {
        int height = zone["height"];
        int width = zone["width"];
        int x = zone["x"];
        int y = zone["y"];

        auto properties = zone["properties"];
        std::string filename = properties["audio_name"];
        bool duck = true;
        bool crab = true;

        AudioZone result(sf::Vector2<float>(x, y), sf::Vector2<float>(width, height), duck, crab, "assets/" + filename);

        this->audio_zones.push_back(result);
    }
}

static void ShapeFreeWrap(cpSpace *space, cpShape *shape, void *unused){
	cpSpaceRemoveShape(space, shape);
	cpShapeFree(shape);
}

static void PostShapeFree(cpShape *shape, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)ShapeFreeWrap, shape, NULL);
}

static void ConstraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused){
	cpSpaceRemoveConstraint(space, constraint);
	cpConstraintFree(constraint);
}

static void PostConstraintFree(cpConstraint *constraint, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)ConstraintFreeWrap, constraint, NULL);
}

static void BodyFreeWrap(cpSpace *space, cpBody *body, void *unused){
	cpSpaceRemoveBody(space, body);
	cpBodyFree(body);
}

static void PostBodyFree(cpBody *body, cpSpace *space){
	cpSpaceAddPostStepCallback(space, (cpPostStepFunc)BodyFreeWrap, body, NULL);
}

void ChipmunkDemoFreeSpaceChildren(cpSpace *space)
{
	// Must remove these BEFORE freeing the body or you will access dangling pointers.
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)PostShapeFree, space);
	cpSpaceEachConstraint(space, (cpSpaceConstraintIteratorFunc)PostConstraintFree, space);

	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc)PostBodyFree, space);
}

Level::~Level() {
    for (auto col : grid) {
        for (auto block : col) {
            delete block;
        }
    }

    ChipmunkDemoFreeSpaceChildren(space);
    cpSpaceFree(space);
}

void Level::draw(sf::RenderWindow& window)
{
    move_camera(window);

    for (int tile_x = 0; tile_x < width; tile_x++) {
        for (int tile_y = 0; tile_y < height; tile_y++) {
            Block* block = grid[tile_x][tile_y];
            if (block != nullptr)
                block->draw(window, sf::Vector2i(tile_x * TILE_SIZE, tile_y * TILE_SIZE));
        }
    }

    //Drawing entities
    for(auto entity : entities)
    {
        entity->draw(window);
    }

    // sf::Vertex line[] = {
    //     sf::Vertex(sf::Vector2f(400, 200)),
    //     sf::Vertex(sf::Vector2f(400, 600)),
    //     sf::Vertex(sf::Vector2f(1200, 600)),
    //     sf::Vertex(sf::Vector2f(1200, 200))
    // };
    // window.draw(line, 4, sf::Lines);

    sf::RectangleShape rectangle1(sf::Vector2f(200, 50));
    sf::RectangleShape rectangle2(sf::Vector2f(40, 80));
    rectangle1.setOrigin(100, 25);
    rectangle2.setOrigin(20, 40);

    cpVect pos = cpBodyGetPos(box_body1);
    rectangle1.setRotation(cpBodyGetAngle(box_body1));
    rectangle1.setPosition(physics_to_graphics(pos));

    pos = cpBodyGetPos(box_body2);
    rectangle2.setRotation(cpBodyGetAngle(box_body2));
    rectangle2.setPosition(physics_to_graphics(pos));

    window.draw(rectangle1);
    window.draw(rectangle2);

    // window.draw(box_sprite1);
    // window.draw(box_sprite2);
}


void Level::init_physics() {
    space = cpSpaceNew();
	cpSpaceSetIterations(space, 30);
	cpSpaceSetGravity(space, cpv(0, -500));
//	cpSpaceSetDamping(space, 0.5);
	cpSpaceSetSleepTimeThreshold(space, 0.5f);
	cpSpaceSetCollisionSlop(space, 0.5f);

	cpBody *body, *staticBody = cpSpaceGetStaticBody(space);
	cpShape *shape;

    float map_width = width * TILE_SIZE;
    float border_height = 2000;

	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(0, border_height), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(map_width, 0), cpv(map_width, border_height), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(map_width, 0), 0.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);

	{
        float water_height = 400;
		cpBB bb = cpBBNew(0, 0, map_width, water_height);

		// Add the sensor for the water.
		shape = cpSpaceAddShape(space, cpBoxShapeNew2(staticBody, bb));
		cpShapeSetSensor(shape, true);
		cpShapeSetCollisionType(shape, 1);
	}

    ball_texture.loadFromFile("assets/crab.png");
    box_sprite1.setTexture(ball_texture);
    box_sprite2.setTexture(ball_texture);

	{
		cpFloat width = 200.0f;
		cpFloat height = 50.0f;
		cpFloat mass = 0.3*FLUID_DENSITY*width*height;
		cpFloat moment = cpMomentForBox(mass, width, height);

		body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPos(body, cpv(700, 300));
		cpBodySetVel(body, cpv(0, 100));
		cpBodySetAngVel(body, 1);

		shape = cpSpaceAddShape(space, cpBoxShapeNew(body, width, height));
		cpShapeSetFriction(shape, 0.8f);

        box_body1 = body;
        box_sprite1.setScale(width/32.0f, height/32.0f);
        box_sprite1.setOrigin(16, 16);
	}

	{
		cpFloat width = 40.0f;
		cpFloat height = width*2;
		cpFloat mass = 0.3*FLUID_DENSITY*width*height;
		cpFloat moment = cpMomentForBox(mass, width, height);

		body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
		cpBodySetPos(body, cpv(800, 300));
		cpBodySetVel(body, cpv(0, 100));
		cpBodySetAngVel(body, 1);

		shape = cpSpaceAddShape(space, cpBoxShapeNew(body, width, height));
		cpShapeSetFriction(shape, 0.8f);

        box_body2 = body;
        box_sprite2.setScale(width/32.0f, height/32.0f);
        box_sprite2.setOrigin(16, 16);
	}

    cpSpaceAddCollisionHandler(space, 1, 0, NULL, (cpCollisionPreSolveFunc)water_pre_solve, NULL, NULL, NULL);
}

void Level::physics() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        cpBodySetVel(box_body1, cpv(0, 400));
        cpBodySetAngVel(box_body1, 10);
    }

    float time_step = 1.0f / 60.0f;
    cpSpaceStep(space, time_step);

    // cpVect pos = cpBodyGetPos(box_body1);
    // box_sprite1.setRotation(cpBodyGetAngle(box_body1));
    // box_sprite1.setPosition(physics_to_graphics(pos));

    // pos = cpBodyGetPos(box_body2);
    // box_sprite2.setRotation(cpBodyGetAngle(box_body2));
    // box_sprite2.setPosition(physics_to_graphics(pos));
}

void Level::move_camera(sf::RenderWindow& window) {
    int camera_center = ((duck->get_position().x - crab->get_position().x))/(2);

    sf::View camera_view(sf::FloatRect(0, 0, 800, 600));
    // we keep our view centered on the player //as 400 is the half of the scrensize
    camera_view.setCenter(duck->get_position().x - camera_center,300);

    if ((duck->get_position().x - camera_center) > (window.getView().getCenter().x + 200)) {
        camera_view.setCenter(duck->get_position().x - camera_center - 200,300);
        window.setView(camera_view);
    }

    if ((duck->get_position().x - camera_center) < (window.getView().getCenter().x - 200)){
        camera_view.setCenter(duck->get_position().x - camera_center+200,300);
        window.setView(camera_view);
    }

    camera_x = window.getView().getCenter().x;
}

void Level::update() {
    physics();
    // TODO collision detection
    for (Entity* entity : entities) {
        entity->move();

        if (entity->can_interact_with(
                    PlayerType::DUCK, duck->get_position())) {
            entity->interact();
            if (interaction_map.count(entity) == 1) {
                // call the interact function of all the 
                // things this entity interacts with
                for (Entity* e : interaction_map[entity]) {
                    e->interact();
                }
            }
        } else if (entity->can_interact_with(
                    PlayerType::CRAB, crab->get_position())) {
            entity->interact();
            if (interaction_map.count(entity) == 1) {
                // call the interact function of all the 
                // things this entity interacts with
                for (Entity* e : interaction_map[entity]) {
                    e->interact();
                }
            }
        }
    }

    for(auto& zone : audio_zones)
    {
        zone.try_play(crab->get_position(), duck->get_position());
    }
}


int Level::get_camera_x()
{
    return camera_x;
}
