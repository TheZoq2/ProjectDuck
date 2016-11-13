#ifndef WATER_PHYSICS_H
#define WATER_PHYSICS_H

#include <chipmunk/chipmunk.h>

void init_water_physics(cpSpace* space, float map_width);
cpBody* create_floating_box(float width, float height, cpSpace* space);

void ChipmunkFreeSpaceChildren(cpSpace *space);

#endif
