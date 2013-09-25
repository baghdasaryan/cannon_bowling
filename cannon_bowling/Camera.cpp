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


#include "Camera.h"

Camera::Camera(float radius, float initAngle, 
			   GLfloat zNear, GLfloat zFar, GLfloat fovy)
{
	m_angle = 90.0 - initAngle;

	m_phi = 0.0;
	m_theta = m_angle;

	m_at = vec4(0.0, 0.0, 0.0, 1.0);
	m_up = vec4(0.0, 1.0, 0.0, 0.0);

	m_initRadius = radius;
	m_radius = m_initRadius;

	m_zNear = zNear;
	m_zFar = zFar;
	m_fovy = fovy;

	m_perspective = Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

mat4 Camera::getPerspective()
{
	return m_perspective;
}

mat4 Camera::getTranslation()
{
	updateLocs();
	return LookAt(m_eye, m_at, m_up);
}

vec4 Camera::getPosition()
{
	updateLocs();
	return m_eye;
}

void Camera::updateLocs()
{
	m_eye.x = m_radius * sin(m_theta*DegreesToRadians) * sin(m_phi*DegreesToRadians) + m_moveHor;
	m_eye.y = m_radius * cos(m_theta*DegreesToRadians) + m_moveVert;
	m_eye.z = m_radius * sin(m_theta*DegreesToRadians) * cos(m_phi*DegreesToRadians);

	m_up.x = 1.0 * sin((m_theta - 90.0)*DegreesToRadians) * sin(m_phi*DegreesToRadians);
	m_up.y = 1.0 * cos((m_theta - 90.0)*DegreesToRadians);
	m_up.z = 1.0 * sin((m_theta - 90.0)*DegreesToRadians) * cos(m_phi*DegreesToRadians);
}

void Camera::updateAspect(GLfloat aspect)
{
	m_initAspect = aspect;
	m_aspect = m_initAspect;
	m_perspective = Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

void Camera::updateAspect(double mult)
{
	m_aspect *= mult;
	m_perspective = Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

void Camera::updateFovy(GLfloat fovy)
{
	m_fovy = fovy;
	m_perspective = Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}

void Camera::horizontal(float delta)
{
	m_moveHor += delta;
	m_at.x += delta;
}

void Camera::vertical(float delta)
{
	m_moveVert += delta;
	m_at.y += delta;
}
	
void Camera::distance(float delta)
{
	m_radius += delta;
}

void Camera::altitude(float angle)
{
	m_theta += angle;
}

void Camera::azimuth(float angle)
{
	m_phi += angle;

	if(m_phi >= 360.0)
		m_phi -= 360.0;
	else if(m_phi <= -360.0)
		m_phi += 360.0;
}

void Camera::reset()
{
	m_theta = m_angle;
	m_phi = 0.0;
	m_radius = m_initRadius;

	m_at = vec4(0.0, 0.0, 0.0, 1.0);
	m_up = vec4(0.0, 1.0, 0.0, 0.0);

	m_moveHor = 0.0;
	m_moveVert = 0.0;

	m_aspect = m_initAspect;
	m_perspective = Perspective(m_fovy, m_aspect, m_zNear, m_zFar);
}
