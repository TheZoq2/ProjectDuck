#include "water_physics.hpp"
//#include <chipmunk/constraints/util.h>
#include "constants.hpp"

static inline cpFloat
k_scalar_body(cpBody *body, cpVect point, cpVect n)
{
    cpFloat rcn = cpvcross(cpvsub(point, cpBodyGetPosition(body)), n);
    return 1.0f/cpBodyGetMass(body) + rcn*rcn/cpBodyGetMoment(body);
}

static cpBool water_pre_solve(cpArbiter* arb, cpSpace *space, void* ptr) {
    CP_ARBITER_GET_SHAPES(arb, water, poly);
    cpBody *body = cpShapeGetBody(poly);

    // Get the top of the water sensor bounding box to use as the water level.
    cpFloat level = cpShapeGetBB(water).t;

    // Clip the polygon against the water level
    int count = cpPolyShapeGetCount(poly);
    int clippedCount = 0;
    cpVect clipped[count + 1];

    for(int i=0, j=count-1; i<count; j=i, i++){
        cpVect a = cpBodyLocalToWorld(body, cpPolyShapeGetVert(poly, j));
        cpVect b = cpBodyLocalToWorld(body, cpPolyShapeGetVert(poly, i));

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
    cpFloat clippedArea = cpAreaForPoly(clippedCount, clipped, 0.0f);
    cpFloat displacedMass = clippedArea*FLUID_DENSITY;
    cpVect centroid = cpCentroidForPoly(clippedCount, clipped);

    cpFloat dt = cpSpaceGetCurrentTimeStep(space);
    cpVect g = cpSpaceGetGravity(space);

    // Apply the buoyancy force as an impulse.
    cpBodyApplyImpulseAtWorldPoint(body, cpvmult(g, -displacedMass*dt), centroid);

    // Apply linear damping for the fluid drag.
    cpVect v_centroid = cpBodyGetVelocityAtWorldPoint(body, centroid);
    cpFloat k = k_scalar_body(body, centroid, cpvnormalize(v_centroid));
    cpFloat damping = clippedArea*FLUID_DRAG*FLUID_DENSITY;
    cpFloat v_coef = cpfexp(-damping*dt*k); // linear drag
//	cpFloat v_coef = 1.0/(1.0 + damping*dt*cpvlength(v_centroid)*k); // quadratic drag
    cpBodyApplyImpulseAtWorldPoint(body, cpvmult(cpvsub(cpvmult(v_centroid, v_coef), v_centroid), 1.0/k), centroid);
	
    // Apply angular damping for the fluid drag.
    cpVect cog = cpBodyLocalToWorld(body, cpBodyGetCenterOfGravity(body));
    cpFloat w_damping = cpMomentForPoly(FLUID_DRAG*FLUID_DENSITY*clippedArea, clippedCount, clipped, cpvneg(cog), 0.0f);
    cpBodySetAngularVelocity(body, cpBodyGetAngularVelocity(body)*cpfexp(-w_damping*dt/cpBodyGetMoment(body)));
    return cpTrue;
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
        shape = cpSpaceAddShape(space, cpBoxShapeNew2(staticBody, bb, 0.0));
        cpShapeSetSensor(shape, true);
        cpShapeSetCollisionType(shape, 1);
    }

    cpCollisionHandler *handler = cpSpaceAddCollisionHandler(space, 1, 0);
    handler->preSolveFunc = (cpCollisionPreSolveFunc)water_pre_solve;
}

cpBody* create_floating_box(float width, float height, cpSpace* space) {
    cpFloat mass = 0.3*FLUID_DENSITY*width*height;
    cpFloat moment = cpMomentForBox(mass, width, height);

    cpBody* body = cpSpaceAddBody(space, cpBodyNew(mass, moment));
    cpBodySetPosition(body, cpv(700, 300));
    cpBodySetVelocity(body, cpv(0, 100));
    cpBodySetAngularVelocity(body, 1);

    cpShape* shape = cpSpaceAddShape(space, cpBoxShapeNew(body, width, height, 0.1f));
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
