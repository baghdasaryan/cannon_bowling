// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Cylinder               #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include "Modes.h"
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "HealthBar.h"
#include "tga.h"

class Cylinder
{
public:
	Cylinder(int divs, float size, float radius, vec3 location, vec4 color, float ambient, 
		float diffuse, float specular, float shininess, Camera *camera, Light *light);

	// Creating a cylinder
	void generateCircle(float angle = 360.0f);
	void generateWall(float z, int rectangles);
	void generateSide(int rectangles);
	virtual void generateCylinder();

	// Send data to GPU
	virtual void memAlloc();
	void init(const char *textureFile);

	// Modifiers
	void setSize(float size);
	void setColor(vec4 color);
	void setLocation(vec3 location);
	void setAmbient(float ambient);
	void setDiffuse(float diffuse);
	void setSpecular(float specular);
	void setShininess(float shininess);
	virtual void rotateX(float angle);
	void rotateY(float angle);
	void rotateZ(float angle);
	void setAngleX(float angle) {m_rotAngle.x = angle;}

	// Accessors
	vec3 getLocation();
	float getRadius();

	// Drawing tools
	virtual void draw(bool edges, bool mesh);


protected:
	// Point holders and attribs
	vec4 *m_vertices;
	vec4 *m_edges;
	vec3 *m_normals;
	vec2 *m_textureUV;
	vec2 *m_circlePoints;
	int m_numDivisions;
	int m_numVertices;
	int m_numEdges;

private:
	// Shader data
	GLuint m_vertexArrayObject;
	GLuint m_vertexBufferObject;
	GLuint m_textureBufferObject;
	GLuint m_program;

	// Cylinder details
	float m_size;
	float m_radius;
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
};


struct collisionHolder
{
	vec3 n;
	float d;
};

class CylinderSlice: public Cylinder
{
public:
	CylinderSlice(int divs, float size, float radius, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light, float angle);
	void generateCylinder();
	void memAlloc();
	collisionHolder *getCollisions();

private:
	float m_sliceAngle;
	collisionHolder m_cols[3];
};

// -------------------------------------- Special class ---------------------------------
class Cheese: public CylinderSlice
{
public:
	Cheese(int divs, float size, float radius, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light, float angle);

	void hit(float healthLost);
	bool gameOver();
	virtual void draw(bool edges, bool mesh);

private:
	HealthBar *m_healthPtr;
};

class Cannon: public Cylinder
{
public:
	Cannon(int divs, float size, float radius, vec3 location, vec4 color, float ambient, 
		float diffuse, float specular, float shininess, Camera *camera, Light *light)
		: Cylinder(divs, size, radius, location, color, ambient, diffuse, specular, shininess, camera, light)
	{
		m_angle = 0.0f;
	}

	void rotateX(float angle)
	{
		if((angle > 0.0f && m_angle <= (90.0f - angle)) || (angle < 0.0f && m_angle >= (-90.0f + angle)))
		{
			m_angle += angle;
			if (m_angle >= 22.5f)
				m_angle = 22.5f;
			else if (m_angle <= 10.0f)
				m_angle = 10.0f;
		}
		Cylinder::setAngleX(m_angle);
	}

private:
	float m_angle;
};

#endif  // __CYLINDER_H__
