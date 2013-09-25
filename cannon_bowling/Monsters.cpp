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


#include "Monsters.h"

Monster::Monster(float size, float radius, float speed, vec4 color, float ambient, float diffuse,
	float specular, float shininess, Camera *camera, Light *light, SphereBall *cannonBall, Cheese *cheese, const char *textureFile)
{
#ifdef GOOD_GPU
	m_bunnyPtr = new Bunny(size, vec3(0.0f, 0.0f, 0.0f), color, ambient, diffuse, specular, shininess, camera, light);
#else // GOOD_GPU
	m_cubePtr = new Cube(size, vec3(0.0f, 0.0f, 0.0f), color, ambient, diffuse, specular, shininess, camera, light);
	m_cubePtr->init(textureFile);
#endif // GOOD_GPU

	m_launched = false;
	m_speed = speed;
	m_radius = radius;
	m_spherePtr = cannonBall;
	m_cheesePtr = cheese;
}

Monster::~Monster()
{

#ifdef GOOD_GPU
	delete m_bunnyPtr;
#else // GOOD_GPU
	delete m_cubePtr;
#endif // GOOD_GPU

}

void Monster::launch()
{
	if(!m_launched)
	{
		m_launched = true;
		m_launchAngle = float (rand() % 360);
		vec3 loc(m_radius * sinf(m_launchAngle*DegreesToRadians), 1.4578f, m_radius * cos(m_launchAngle*DegreesToRadians));
		
#ifdef GOOD_GPU
		m_bunnyPtr->rotateY(m_launchAngle - 90.0f);
		m_bunnyPtr->setLocation(loc);
#else // GOOD_GPU
		m_cubePtr->rotateY(m_launchAngle);
		m_cubePtr->setLocation(loc);
#endif // GOOD_GPU

		m_velocity = vec3(-m_speed * sinf(m_launchAngle*DegreesToRadians), 0.0f, -m_speed * cos(m_launchAngle*DegreesToRadians));
		PlaySound(TEXT("sound_monster.wav"), NULL, SND_ASYNC | SND_NOSTOP | SND_FILENAME);
	}
}

void Monster::dismiss()
{
	m_launched = false;
}

bool Monster::launched()
{
	return m_launched;
}

void Monster::update(double dt)
{
	if(!m_launched)
		return;

#ifdef GOOD_GPU	
	vec3 loc = m_bunnyPtr->getLocation();
	float diameter = 1.0f; //m_bunnyPtr->getSize();
#else // GOOD_GPU
	vec3 loc = m_cubePtr->getLocation();
	float diameter = m_cubePtr->getSize()/2.0f;
#endif // GOOD_GPU

	if(length(loc - (m_spherePtr->getLocation())) <= diameter)
	{
		dismiss();
		m_spherePtr->dismiss();
		return;
	}

	bool cheeseFlag = false;
	for(int i = 0; i < 3; i++)
	{
		float dist = length(loc - vec3(0.0f, 0.0f, 0.0f));
		if(dist <= m_cheesePtr->getRadius())
		{
			cheeseFlag = true;
			break;
		}
	}

	if(cheeseFlag) // hit cheese
	{
		m_cheesePtr->hit(10.0f);
		dismiss();
		return;
	}

	// Move towards center
	dt /= 5.0f;
	loc.x += m_velocity.x*dt;
	loc.y += m_velocity.y*dt;
	loc.z += m_velocity.z*dt;

#ifdef GOOD_GPU
		m_bunnyPtr->setLocation(loc);
#else // GOOD_GPU
		m_cubePtr->setLocation(loc);
#endif // GOOD_GPU

	// Use for debugging purposes only
	//std::cout << "Velocity: m_velocity.y = " << m_velocity.y << "; Location: loc.y = " << loc.y << "." << std::endl;
}

void Monster::draw(bool edges, bool mesh)
{
	if(m_launched)
	{
#ifdef GOOD_GPU
		m_bunnyPtr->draw(mesh);
#else // GOOD_GPU
		m_cubePtr->draw(edges, mesh);
#endif // GOOD_GPU
	}
}
