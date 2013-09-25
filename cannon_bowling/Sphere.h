// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Sphere                 #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "Modes.h"
#include "Windows.h"
#include "Angel.h"
#include "Camera.h"
#include "Light.h"
#include "Cylinder.h"
#include "tga.h"

class Sphere
{
public:
	Sphere(int divs, float size, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light);

	// Creating a sphere
	vec4 unit(const vec4 &p);
	void divideTriangle(const vec4 &a, const vec4 &b, const vec4 &c, int n);
	void generateSphere();

	// Send data to GPU
	void init(const char *textureFile);

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
	float getRadius();

	// Drawing tools
	virtual void draw(bool mesh);
			
private:
	// Shader data
    GLuint m_vertexArrayObject;
    GLuint m_vertexBufferObject;
	GLuint m_textureBufferObject;
    GLuint m_program;

	// Point holders and attribs
	vec4 *m_vertices;
	vec3 *m_normals;
	vec2 *m_textureUV;
	int m_numDivisions;
	int m_numVertices;

	// Sphere details
	float m_size;
	vec4 m_location;
	vec4 m_color;
	float m_ambient;
	float m_diffuse;
	float m_specular;
	float m_shininess;
	vec3 m_rotAngle;

	bool m_textureOn;

protected:
	// Pointers to other classes
	Camera *m_camPtr;
	Light *m_lightPtr;
};

// Special class
class SphereBall: public Sphere
{
public:
	SphereBall(int divs, float size, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light, Cheese *cheese, bool bounce = false, float vDamp = 0.3, float hDamp = 0.5);
	void launch();
	void dismiss();
	bool launched();
	void update(double dt);
	void altitude(float angle);
	void draw(bool mesh);

private:
	float m_radius;
	float m_mass;
	float m_speed;
	float m_vDamp;
	float m_hDamp;
	vec3 m_velocity;
	float m_angle;
	bool m_launched;
	bool m_bounceOn;

	// Pointers to other classes
	Cheese *m_cheesePtr;
};

#endif  // __SPHERE_H__
