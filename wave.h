#ifndef H_WAVE_RENDERER
#define H_WAVE_RENDERER

#include <SFML/Graphics.hpp>

class Wave 
{
public:
	Wave(float length, float height, size_t segment_amount);

	void draw(sf::RenderWindow& window);
	void update();

    void set_camera_position(float x);
private:
	sf::VertexArray vertices;
	float height;
	float segment_length;

	float internal_time = 0;

	std::size_t segment_amount;

    sf::Shader shader;

    float camera_x;
};
#endif
