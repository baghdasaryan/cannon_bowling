// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Cube                   #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __CUBE_H__
#define __CUBE_H__

#include "Modes.h"
#include <math.h>
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "tga.h"

// Point details
const int numVertices = 36;
const int numEdges = 24;
const vec4 vertices[8] = {
	vec4( -0.5, -0.5,  0.5, 1.0 ),
	vec4( -0.5,  0.5,  0.5, 1.0 ), 
	vec4(  0.5,  0.5,  0.5, 1.0 ), 
	vec4(  0.5, -0.5,  0.5, 1.0 ),
	vec4( -0.5, -0.5, -0.5, 1.0 ),
	vec4( -0.5,  0.5, -0.5, 1.0 ),
	vec4(  0.5,  0.5, -0.5, 1.0 ), 
	vec4(  0.5, -0.5, -0.5, 1.0 )
};


class Cube
{
public:
	Cube(float size, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light);

	// Generate cube vertices
	void genPoints(vec3 norm, int a, int b, int c, int d, bool faceTriangles);
	void genCube();

	// Buffer operations
	void init(const char *textureFile);

	// Drawing tools
	void draw(bool edges, bool mesh);

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
	vec4 *m_edges;
	vec3 *m_normals;
	vec2 *m_textureUV;
	int m_numVertices;

	// Cube details
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

	bool m_textureOn;

	// Helper variables
	int m_index;
};

#endif  // __CUBE_H__
