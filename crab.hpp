#ifndef CRAB_H
#define CRAB_H

#include "entity.hpp"

class Crab : public Entity {

    public:
        virtual void draw(sf::Window& window) const override;

};

#endif
