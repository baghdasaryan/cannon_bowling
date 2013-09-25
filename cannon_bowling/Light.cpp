// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Object Lighting        #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#include "Light.h"

Light::Light(vec4 position, vec4 color)
{
	m_position = position;
	m_color = color;
}

void Light::setPosition(vec4 position)
{
	m_position = position;
}

void Light::setColor(vec4 color)
{
	m_color = color;
}

vec4 Light::getPosition()
{
	return m_position;
}

vec4 Light::getColor()
{
	return m_color;
}
