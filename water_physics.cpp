#include "water_physics.hpp"
#include <chipmunk/constraints/util.h>
#include "constants.hpp"

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


void init_water_physics(cpSpace* space, float map_width) {
	cpBody *staticBody = cpSpaceGetStaticBody(space);
	cpShape *shape;

    float border_height = 2000;

	// Create segments around the edge of the screen.
	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(0, border_height), 5.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(map_width, 0), cpv(map_width, border_height), 5.0f));
	cpShapeSetElasticity(shape, 1.0f);
	cpShapeSetFriction(shape, 1.0f);

	shape = cpSpaceAddShape(space, cpSegmentShapeNew(staticBody, cpv(0, 0), cpv(map_width, 0), 5.0f));
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

    cpSpaceAddCollisionHandler(space, 1, 0, NULL, (cpCollisionPreSolveFunc)water_pre_solve, NULL, NULL, NULL);
}

cpBody* create_floating_box(float width, float height, cpSpace* space) {
    cpFloat mass = 0.3*FLUID_DENSITY*width*height;
    cpFloat moment = cpMomentForBox(mass, width, height);

    cpBody* body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPos(body, cpv(700, 300));
    cpBodySetVel(body, cpv(0, 100));
    cpBodySetAngVel(body, 1);

    cpShape* shape = cpSpaceAddShape(space, cpBoxShapeNew(body, width, height));
    cpShapeSetFriction(shape, 0.8f);
    return body;
}

// Below are functions for freeing memory
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

void ChipmunkFreeSpaceChildren(cpSpace *space)
{
	// Must remove these BEFORE freeing the body or you will access dangling pointers.
	cpSpaceEachShape(space, (cpSpaceShapeIteratorFunc)PostShapeFree, space);
	cpSpaceEachConstraint(space, (cpSpaceConstraintIteratorFunc)PostConstraintFree, space);

	cpSpaceEachBody(space, (cpSpaceBodyIteratorFunc)PostBodyFree, space);
}
