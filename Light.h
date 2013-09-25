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



#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "Modes.h"
#include "Angel.h"

class Light
{
public:
	Light(vec4 position, vec4 color);

	void setPosition(vec4 position);
	void setColor(vec4 color);

	vec4 getPosition();
	vec4 getColor();

private:
	vec4 m_position;
	vec4 m_color;
};

#endif  // __LIGHT_H__
