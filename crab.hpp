#ifndef CRAB_H
#define CRAB_H

#include "entity.hpp"

class Crab : public Entity {

    public:
        virtual void draw(sf::Window& window) const override;
        virtual std::vector<Block> get_blocks() const override;

};

#endif
