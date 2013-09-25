// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Health Bar             #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __HEALTH_BAR_H__
#define __HEALTH_BAR_H__

#include "Modes.h"
#include "Angel.h"
#include "Camera.h"
#include "Light.h"

class HealthBar
{
public:
	HealthBar(float ambient, float diffuse, float specular, float shininess, 
		Camera *camera, Light *light, float height, float width);

	// Creating a health bar
	void generateHealthBar();
	
	// Accessors/Modifiers
	void change(float health);
	bool dead();

	// Send data to GPU
	void init();

	// Drawing tools
	void draw(bool edges, bool mesh);

private:
	// Health bar details
	int m_health;
	float m_ambient;
	float m_diffuse;
	float m_specular;
	float m_shininess;
	float m_height;
	float m_width;

	// Point holders and attribs
	vec4 *m_vertices;
	vec4 *m_edges;
	vec3 *m_normals;
	int m_numVertices;
	int m_numEdges;

	// Shader data
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_program;

	// Pointers to other classes
	Camera *m_camPtr;
	Light *m_lightPtr;
};

#endif  // __HEALTH_BAR_H__
