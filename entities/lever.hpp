#ifndef H_LEVER
#define H_LEVER

#include "entity.hpp"

const double INTERACT_DISTANCE = 20.0;

class Lever: public Entity
{
    public:
        Lever(sf::Vector2<double> position);

        void draw(sf::RenderWindow& window) override;
        std::vector<sf::Vector2<int>> get_blocks() const override;
        sf::Vector2<double> wants_to_move() const override;
        virtual void interact() override;

        virtual bool
            can_interact_with(PlayerType type,
                    sf::Vector2<double> position) override;
    
    private:
        sf::Texture main_texture;
        sf::Texture pulled_texture;
        sf::Sprite main_sprite;
        sf::Sprite pulled_sprite;

        sf::Sprite* current_sprite;
        
};

#endif
