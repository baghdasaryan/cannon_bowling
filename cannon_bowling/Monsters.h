// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Monsters               #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __MONSTERS_H__
#define __MONSTERS_H__

#include "Modes.h"
#include <stdlib.h>
#include "Windows.h"
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Cube.h"
#include "Sphere.h"
#include "Cylinder.h"
#include "Bunny.h"

class Monster
{
public:
	Monster(float size, float radius, float speed, vec4 color, float ambient, float diffuse, float specular, 
		float shininess, Camera *camera, Light *light, SphereBall *cannonBall, Cheese *cheese, const char *textureFile);
	~Monster();
	void launch();
	void dismiss();
	bool launched();
	void update(double dt);
	void draw(bool edges, bool mesh);

private:
	// Shader data
    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
    GLuint m_program;

	// Point holders and attribs
	vec4 *m_vertices;
	vec3 *m_normals;
	int m_numDivisions;
	int m_numVertices;

	// Monster details
	bool m_launched;
	float m_mass;
	float m_speed;
	float m_launchAngle;
	float m_radius;
	vec3 m_velocity;

	// Pointers to other classes
	Camera *m_camPtr;
	Light *m_lightPtr;
	Cube *m_cubePtr;
	Bunny *m_bunnyPtr;
	SphereBall *m_spherePtr;
	Cheese *m_cheesePtr;
};

#endif  // __MONSTERS_H__