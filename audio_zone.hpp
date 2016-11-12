#ifndef H_AUDIO_ZONE
#define H_AUDIO_ZONE

#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

class AudioZone 
{
public:
    AudioZone(sf::Vector2<double> position, sf::Vector2<double> size, bool duck, bool crab, std::string file);

    void try_play(sf::Vector2<double> crab_pos, sf::Vector2<double> duck_pos);
private:
    void try_play_generic(sf::Vector2<double> pos);

    bool has_played = false;

    sf::Vector2<double> position;
    sf::Vector2<double> size;
    bool duck;
    bool crab;

    sf::SoundBuffer sound_buffer;
};
#endif
