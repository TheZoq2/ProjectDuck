#ifndef DEBUG_DRAW_HPP
#define DEBUG_DRAW_HPP

struct cpShape;
struct cpSpace;

namespace sf {
    class RenderWindow;
}

void draw_shape(cpShape* shape, void* data);
void draw_shapes(cpSpace* space, sf::RenderWindow* window);

#endif
