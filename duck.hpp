#ifndef DUCK_H
#define DUCK_H

#include "entity.hpp"

class Duck : public Entity {

    public:
        virtual void draw(sf::Window& window) const override;
        virtual std::vector<Block> get_blocks() const override;

};

#endif
