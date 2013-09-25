// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Bunny                  #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __BUNNY_H__
#define __BUNNY_H__

#include "Modes.h"
#include <vector>
#include "Angel.h"
#include "Camera.h"
#include "Light.h"

class Bunny
{
public:
	Bunny(float size, vec3 location, vec4 color, float ambient, float diffuse, 
		float specular, float shininess, Camera *camera, Light *light);

	// Load vertices
	void readVertices(const char *filename);
	
	// Buffer operations
	void init(const char *textureFile);

	// Drawing tools
	void draw(bool mesh);

	// Modifiers
	void setSize(float size);
	void setColor(vec4 color);
	void setLocation(vec3 location);
	void setAmbient(float ambient);
	void setDiffuse(float diffuse);
	void setSpecular(float specular);
	void setShininess(float shininess);
	void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);

	// Accessors
	vec3 getLocation();
	float getSize();

private:
	// Shader data
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_textureBufferObject;
	GLuint m_program;

	// Point holders and attribs
	vec4 *m_vertices;
	vec3 *m_normals;
//	vec2 *m_textureUV;
	int m_numVertices;

	// Bunny details
	float m_size;
	vec4 m_location;
	vec4 m_color;
	float m_ambient;
	float m_diffuse;
	float m_specular;
	float m_shininess;
	vec3 m_rotAngle;

	// Pointers to other classes
	Camera *m_camPtr;
	Light *m_lightPtr;
};

#endif  // __BUNNY_H__
