// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Camera Positioning     #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "Modes.h"
#include <math.h>
#include "Angel.h"

class Camera
{
public:
	Camera(float radius, float initAngle,
		GLfloat zNear, GLfloat zFar, GLfloat fovy);

	mat4 getPerspective();
	mat4 getTranslation();
	vec4 getPosition();
	void updateLocs();
	void updateAspect(GLfloat aspect);
	void updateAspect(double mult);
	void updateFovy(GLfloat fovy);

	void horizontal(float delta);
	void vertical(float delta);
	void distance(float delta);

	void altitude(float angle);
	void azimuth(float angle);

	float getPhi()
	{
		return m_phi;
	}

	void reset();

private:
	vec4 m_eye;
	vec4 m_at;
	vec4 m_up;
	
	float m_angle;
	float m_theta;
	float m_phi;

	double m_initRadius;
	double m_radius;

	float m_moveVert;
	float m_moveHor;

	// Projection attributes
	GLfloat m_zNear;
	GLfloat m_zFar;
	GLfloat m_fovy;
	GLfloat m_initAspect;
	GLfloat m_aspect;

	mat4 m_perspective;
};

#endif  // __CAMERA_H__
