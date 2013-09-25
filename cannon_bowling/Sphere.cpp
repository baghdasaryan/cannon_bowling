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


#include "Sphere.h"

Sphere::Sphere(int divs, float size, vec3 location, vec4 color, float ambient, float diffuse,
			   float specular, float shininess, Camera *camera, Light *light)
{
	// Set drawing data
	m_numDivisions = divs;
	m_numVertices = 0;			// can we still do this???
	m_size = size;
	m_color = color;
	m_location = location;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_camPtr = camera;
	m_lightPtr = light;
	m_rotAngle = 0.0f;
	m_textureOn = false;
}

// ---------------------------- CREATE A SPHERE ------------------------------

vec4 Sphere::unit(const vec4 &p)
{
	double len = sqrt(p.x*p.x + p.y*p.y + p.z*p.z);

	vec4 u;
	if(len > DivideByZeroTolerance)
	{
		u = p/len;
		u.w = 1.0f;
	}

	return u;
}

void Sphere::divideTriangle(const vec4 &a, const vec4 &b, const vec4 &c, int n)
{
    if(n > 0)
    {
		vec4 v1 = unit(a + b);
		vec4 v2 = unit(a + c);
		vec4 v3 = unit(b + c);

		divideTriangle(a, v1, v2, n - 1);
		divideTriangle(c, v2, v3, n - 1);
		divideTriangle(b, v3, v1, n - 1);
		divideTriangle(v1, v3, v2, n - 1);
    }
	else
	{
		m_vertices[m_numVertices] = a; m_normals[m_numVertices] = normalize(vec3(a.x, a.y, a.z)); m_textureUV[m_numVertices] = vec2(0.5f*(atan2f(a.z,a.x)/M_PI + 1), 0.5f - asinf(a.y)/M_PI); m_numVertices++;
		m_vertices[m_numVertices] = b; m_normals[m_numVertices] = normalize(vec3(b.x, b.y, b.z)); m_textureUV[m_numVertices] = vec2(0.5f*(atan2f(b.z,b.x)/M_PI + 1), 0.5f - asinf(b.y)/M_PI); m_numVertices++;
		m_vertices[m_numVertices] = c; m_normals[m_numVertices] = normalize(vec3(c.x, c.y, c.z)); m_textureUV[m_numVertices] = vec2(0.5f*(atan2f(c.z,c.x)/M_PI + 1), 0.5f - asinf(c.y)/M_PI); m_numVertices++;
	}
}

void Sphere::generateSphere()
{
	vec4 v[4] = {
		vec4( 0.0f,       0.0f,       1.0f,      1.0f),
		vec4( 0.0f,       0.942809f, -0.333333f, 1.0f),
		vec4(-0.816497f, -0.471405f, -0.333333f, 1.0f),
		vec4( 0.816497f, -0.471405f, -0.333333f, 1.0f)
	};

	divideTriangle(v[0], v[1], v[2], m_numDivisions);
	divideTriangle(v[3], v[2], v[1], m_numDivisions);
	divideTriangle(v[0], v[3], v[1], m_numDivisions);
	divideTriangle(v[0], v[2], v[3], m_numDivisions);
}

// ---------------------------------------------------------------------------


// ---------------------------- UPLOAD DATA TO GPU ---------------------------

void Sphere::init(const char *textureFile)
{   
	m_textureOn = (textureFile[0] != '\0')? true: false;

	// Allocate space for vertices
	int numPoints = pow(4.0, m_numDivisions + 1) * 3;
	m_vertices = new vec4[numPoints];
	m_textureUV = new vec2[numPoints];
	m_normals = new vec3[numPoints];

	generateSphere();	// generate sphere points

	// Create a vertex array object
#ifdef __APPLE__
    glGenVertexArraysAPPLE(1, &m_vertexArrayObject);
    glBindVertexArrayAPPLE(m_vertexArrayObject);
#else
	glGenVertexArrays(1, &m_vertexArrayObject);
    glBindVertexArray(m_vertexArrayObject);
#endif

	// Create a buffer object
    glGenBuffers(1, &m_vertexBufferObject);
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);    

	// Initialize buffer
	glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]) + sizeof(m_textureUV[0]))*m_numVertices, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0])*m_numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices, sizeof(m_normals[0])*m_numVertices, m_normals);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices, sizeof(m_textureUV[0])*m_numVertices, m_textureUV);
    
	// Load shaders and use the resulting shader program
    m_program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(m_program);

	// Specify vertex positions as an input to the shader program
    GLuint position = glGetAttribLocation(m_program, "vPosition");
    glEnableVertexAttribArray(position);
    glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); 

	// Set up normal vectors as an input to the shader program
	GLuint normal = glGetAttribLocation(m_program, "vNormal");
    glEnableVertexAttribArray(normal);
    glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0])*m_numVertices));

	// Specify texture positions as an input to the shader program
	GLuint texture = glGetAttribLocation(m_program, "vTexture");
	glEnableVertexAttribArray(texture);
	glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices)); 

	// Load image
	TgaImage image;
	if(m_textureOn && !image.loadTGA(textureFile))
	{
		printf("Error loading image file\n");
		exit(1);
	}

	// Bind texture
	glGenTextures(1, &m_textureBufferObject);
	glBindTexture(GL_TEXTURE_2D, m_textureBufferObject);
	glTexImage2D(GL_TEXTURE_2D, 0, 4, image.width, image.height, 0, (image.byteCount == 3) ? GL_BGR : GL_BGRA, GL_UNSIGNED_BYTE, image.data);
    
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glUniform1i(glGetUniformLocation(m_program, "Tex"), 0);
    
	// Clean up
	delete [] m_vertices;
	delete [] m_textureUV;
	delete [] m_normals;
}

// ---------------------------------------------------------------------------


// ------------------------------- MODIFIERS ---------------------------------

void Sphere::setSize(float size)
{
	m_size = size;
}

void Sphere::setColor(vec4 color)
{
	m_color = color;
}

void Sphere::setLocation(vec3 location)
{
	m_location = location;
}

void Sphere::setAmbient(float ambient)
{
	m_ambient = ambient;
}

void Sphere::setDiffuse(float diffuse)
{
	m_diffuse = diffuse;
}

void Sphere::setSpecular(float specular)
{
	m_specular = specular;
}

void Sphere::setShininess(float shininess)
{
	m_shininess = shininess;
}

void Sphere::rotateX(float angle)
{
	m_rotAngle.x += angle;
	if(m_rotAngle.x >= 360.0f)
		m_rotAngle.x -= 360.0f;
}

void Sphere::rotateY(float angle)
{
	m_rotAngle.y += angle;
	if(m_rotAngle.y >= 360.0f)
		m_rotAngle.y -= 360.0f;
}

void Sphere::rotateZ(float angle)
{
	m_rotAngle.z += angle;
	if(m_rotAngle.z >= 360.0f)
		m_rotAngle.z -= 360.0f;
}

// ---------------------------------------------------------------------------


// ------------------------------- ACCESSORS ---------------------------------

vec3 Sphere::getLocation()
{
	return vec3(m_location.x, m_location.y, m_location.z);
}

float Sphere::getRadius()
{
	return m_size;
}

//void Cylinder::setColor(vec4 color)
//{
//	m_color = color;
//}
//
//void Cylinder::setLocation(vec3 location)
//{
//	m_location = location;
//}
//
//void Cylinder::setAmbient(float ambient)
//{
//	m_ambient = ambient;
//}
//
//void Cylinder::setDiffuse(float diffuse)
//{
//	m_diffuse = diffuse;
//}
//
//void Cylinder::setSpecular(float specular)
//{
//	m_specular = specular;
//}
//
//void Cylinder::setShininess(float shininess)
//{
//	m_shininess = shininess;
//}
//
//void Cylinder::rotateX(float angle)
//{
//	m_rotAngle.x += angle;
//	if(m_rotAngle.x >= 360.0)
//		m_rotAngle.x -= 360.0;
//}
//
//void Cylinder::rotateY(float angle)
//{
//	m_rotAngle.y += angle;
//	if(m_rotAngle.y >= 360.0)
//		m_rotAngle.y -= 360.0;
//}
//
//void Cylinder::rotateZ(float angle)
//{
//	m_rotAngle.z += angle;
//	if(m_rotAngle.z >= 360.0)
//		m_rotAngle.z -= 360.0;
//}

// ---------------------------------------------------------------------------


void Sphere::draw(bool mesh)
{
	// Use current object's program
	glUseProgram(m_program);

	// Bind buffers
#ifdef __APPLE__
    glBindVertexArrayAPPLE(m_vertexArrayObject); 
#else
	glBindVertexArray(m_vertexArrayObject);
#endif
    glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);
	glBindTexture(GL_TEXTURE_2D, m_textureBufferObject);

	// Get transformation matrices
	mat4 transform = RotateZ(m_rotAngle.z) * RotateY(m_rotAngle.y) * RotateX(m_rotAngle.x) * Translate(m_location) * Scale(m_size);

	// Pass uniform data to the shader program lighting and color data to the GPU
	glUniformMatrix4fv(glGetUniformLocation(m_program, "wMo"), 1, GL_TRUE, transform);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "cMw"), 1, GL_TRUE, m_camPtr->getTranslation());
	glUniformMatrix4fv(glGetUniformLocation(m_program, "proj"), 1, GL_TRUE, m_camPtr->getPerspective());

	// vShader
	glUniform4fv(glGetUniformLocation(m_program, "camPos"), 1, m_camPtr->getPosition());
	glUniform4fv(glGetUniformLocation(m_program, "lightPos"), 1, m_lightPtr->getPosition());
	
	// fShader
	glUniform1f(glGetUniformLocation(m_program, "Ambient"), m_ambient);
	glUniform1f(glGetUniformLocation(m_program, "Diffuse"), m_diffuse);
	glUniform1f(glGetUniformLocation(m_program, "Specular"), m_specular);
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), m_shininess);
	glUniform4fv(glGetUniformLocation(m_program, "lightColor"), 1, m_lightPtr->getColor());

	// Draw objects
	if(m_textureOn)
		glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 1);
	glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, m_color);
	if(mesh)
		glDrawArrays(GL_LINES, 0, m_numVertices);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 0);
}



// **************************************************************** //
// *********************** SphereBall****************************** //
// **************************************************************** //

SphereBall::SphereBall(int divs, float size, vec3 location, vec4 color, float ambient, float diffuse,
	float specular, float shininess, Camera *camera, Light *light, Cheese *cheese, bool bounce, float vDamp, float hDamp)
	: Sphere(divs, size, location, color, ambient, diffuse,	specular, shininess, camera, light)
{
	m_radius = size;
	m_launched = false;
	m_angle = -8.0f;
	m_speed = 18.5f;
	m_vDamp = vDamp;
	m_hDamp = hDamp;
	m_bounceOn = bounce;
	m_cheesePtr = cheese;
}

void SphereBall::launch()
{
	if(!m_launched)
	{
		m_launched = true;
		vec3 curloc(m_camPtr->getPosition().x, m_camPtr->getPosition().y - 4.0f, m_camPtr->getPosition().z);
		setLocation(curloc);
		//curloc = vec3(0.0, 0.0, 0.0) - curloc;
		//vec2 dir = vec2(curloc.x >= 0.0? 1.0:-1.0, curloc.z >= 0.0? 1.0:-1.0) * (normalize(vec2(curloc.x, curloc.z)) * normalize(vec2(curloc.x, curloc.z)));
		//m_velocity = vec3(dir.x*m_speed*cosf(m_angle*M_PI/180.0), m_speed*sinf(m_angle*M_PI/180.0), dir.y*m_speed*cosf(m_angle*M_PI/180.0));
		m_velocity = vec3(-6*m_speed*cosf(m_angle*M_PI/180.0) * sin(m_camPtr->getPhi()*DegreesToRadians), m_speed*sinf(m_angle*M_PI/180.0), -6*m_speed*cosf(m_angle*M_PI/180.0) * cos(m_camPtr->getPhi()*DegreesToRadians)) ;
		PlaySound(TEXT("sound_cannon.wav"), NULL, SND_ASYNC | SND_FILENAME);
	}
}

void SphereBall::dismiss()
{
	m_launched = false;
}

bool SphereBall::launched()
{
	return m_launched;
}

void SphereBall::update(double dt)
{
	if(!m_launched)
		return;

	vec3 loc = getLocation();

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

	if(cheeseFlag)
	{
		m_cheesePtr->hit(20.0f);
		dismiss();
		return;
	}

	if((!m_bounceOn && loc.y <= 1.5f + m_radius) || (m_bounceOn && loc.y <= 1.0f + m_radius && -0.05f <= m_velocity.y && m_velocity.y <= 0.05f))
	{
		dismiss();
		return;
	}
	else if(m_bounceOn && loc.y <= 1.0f + m_radius && m_velocity.y < -0.5f)
	{
		m_velocity.y = -m_vDamp*m_velocity.y;
		m_velocity.x =  m_hDamp*m_velocity.x;
		m_velocity.z =  m_hDamp*m_velocity.z;
	}

	dt /= 2.0;		// used for slowing down cannonball animation
	loc.x += m_velocity.x*dt;
	loc.y += m_velocity.y*dt - Gravity*pow(dt, 2)/2.0f;
	loc.z += m_velocity.z*dt;
	m_velocity.y -= Gravity*dt;
	setLocation(loc);

	// Use for debugging purposes only
	//std::cout << "Velocity: m_velocity.y = " << m_velocity.y << "; Location: loc.y = " << loc.y << "." << std::endl;
}
	
void SphereBall::altitude(float angle)
{
	if((angle > 0.0f && m_angle <= (90.0f - angle)) || (angle < 0.0f && m_angle >= (-90.0f + angle)))
	{
		m_angle += angle;
		if (m_angle >= 2.0f)
			m_angle = 2.0f ;
		else if (m_angle <= -10.0f)
			m_angle = -10.0f;
	}
}

void SphereBall::draw(bool mesh)
{
	if(m_launched)
		Sphere::draw(mesh);
}
