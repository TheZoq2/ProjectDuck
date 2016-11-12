#include "wave.h"
#include <math.h>
#include <iostream>

#include <chrono>

const float WAVE_AMPLITUDE = 5;

Wave::Wave(float length, float height, std::size_t segment_amount) :
	vertices(sf::TrianglesStrip)
{
	float segment_length = length / segment_amount;

	for(std::size_t i = 0; i < segment_amount; ++i)
	{
		this->vertices.append(sf::Vector2f(i * segment_length, height));
		this->vertices.append(sf::Vector2f(i * segment_length, 0));
	}

	this->segment_amount = segment_amount;
	this->height = height;
	this->segment_length = segment_length;

    //Loading the water shader
    shader.loadFromFile("assets/water_shader.vert", sf::Shader::Vertex);
    shader.loadFromFile("assets/water_shader.frag", sf::Shader::Fragment);
}


void Wave::update()
{
	this->internal_time = this->internal_time + 1/60.0;

	for(std::size_t i = 0; i < segment_amount; i++)
	{
		float time_modifier = 1.5;
		float offset1 =  WAVE_AMPLITUDE * 0.75 * sin(internal_time * time_modifier + i * segment_length / 20.);
		float offset2 = WAVE_AMPLITUDE * 0.5 * sin(internal_time * time_modifier + 1.2 + i * segment_length / 10.);
		float offset3 = WAVE_AMPLITUDE * 0.5 * sin(-internal_time * time_modifier + 1.4 + i * segment_length / 10.);
		float offset = offset1 + offset2*0.5 + offset3 * 0.5;

		vertices[i * 2 + 1].position.y = offset;
	}
}


void Wave::draw(sf::RenderWindow& window)
{
    shader.setParameter("iGlobalTime", internal_time);

	sf::RenderStates render_states;
	render_states.transform.translate(0, 200);;
    render_states.shader = &shader;

	window.draw(this->vertices, render_states);
}
