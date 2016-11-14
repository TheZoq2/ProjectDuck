#include "chipmunk/chipmunk_private.h"
#include "vectorutils.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

static void draw_segment(cpVect a, cpVect b, float radius, sf::RenderWindow* window) {
    radius = fmax(radius, 1);

    sf::Vertex line[] = {
        sf::Vertex(physics_to_graphics(a)),
        sf::Vertex(physics_to_graphics(b))
    };

    window->draw(line, 2, sf::Lines);
}

static void draw_poly(int vert_count, cpVect* verts, sf::RenderWindow* window) {
    if (vert_count <= 0)
        return;

    cpVect first = verts[0];
    cpVect previous = first;
    for (int i = 1; i < vert_count; i++) {
        draw_segment(previous, verts[i], 1, window);
        previous = verts[i];
    }
    draw_segment(previous, first, 1, window);
}

void draw_shape(cpShape* shape, void* data) {
    sf::RenderWindow* window = (sf::RenderWindow*)data;

    //cpBody *body = shape->body;

    switch(shape->klass->type) {
    case CP_SEGMENT_SHAPE: {
        cpSegmentShape* seg = (cpSegmentShape*) shape;
        draw_segment(seg->ta, seg->tb, seg->r, window);
        break;
    }

    case CP_POLY_SHAPE: {
        cpPolyShape* poly = (cpPolyShape*) shape;
        draw_poly(poly->numVerts, poly->tVerts, window);
        break;
    }

    default:
        break;
    }
}

void draw_shapes(cpSpace* space, sf::RenderWindow* window) {
    cpSpaceEachShape(space, draw_shape, window);
}
