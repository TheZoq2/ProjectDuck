#ifndef H_AUDIO_ZONE
#define H_AUDIO_ZONE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AudioZone 
{
public:
    AudioZone(sf::Vector2<float> position, sf::Vector2<float> size, bool duck, bool crab, std::string file);

    void try_play(sf::Vector2<float> crab_pos, sf::Vector2<float> duck_pos);
private:
    void try_play_generic(sf::Vector2<float> pos);

    bool has_played = false;

    sf::Vector2<float> position;
    sf::Vector2<float> size;
    bool duck;
    bool crab;

    sf::SoundBuffer sound_buffer;
    sf::Sound sound;
};
#endif
