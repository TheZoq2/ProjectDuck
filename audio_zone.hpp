#ifndef H_AUDIO_ZONE
#define H_AUDIO_ZONE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AudioZone 
{
public:
    AudioZone(sf::Vector2f position, sf::Vector2f size, bool duck, bool crab, std::string file);

    void try_play(sf::Vector2f crab_pos, sf::Vector2f duck_pos);
private:
    void try_play_generic(sf::Vector2f pos);

    bool has_played = false;

    sf::Vector2f position;
    sf::Vector2f size;
    bool duck;
    bool crab;

    sf::SoundBuffer sound_buffer;
};
#endif
