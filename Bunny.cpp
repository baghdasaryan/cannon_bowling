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


#include "Bunny.h"

Bunny::Bunny(float size, vec3 location, vec4 color, float ambient, float diffuse, 
	float specular, float shininess, Camera *camera, Light *light)
{
	m_numVertices = 0;
	m_rotAngle = 0.0f;
	m_size = size;
	m_location = location;
	m_color = color;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_camPtr = camera;
	m_lightPtr = light;

	// Create a bunny
	init("bunny.obj");
}
	
void Bunny::readVertices(const char *filename)
{
	FILE *file;
	fopen_s(&file, filename, "r");

	if(!file)
	{
		fprintf(stderr, "BUNNY ERROR: Was not able to open file: \"%s\".\n", filename);
		exit(1);
	}

	char buf[128];
	std::vector<vec4> vertices;
	struct int_arr{int vertex[3];};
	std::vector<int_arr> faces;
	vec3 v;
	int_arr f;
	while(fscanf_s(file, "%s", buf, 2) != EOF)
	{
		switch(buf[0])
		{
		case 'v':
			if(buf[1] == '\0')	// vertex
			{
				fscanf_s(file, "%f %f %f", &v.x, &v.y, &v.z);
				vertices.push_back(vec4(v, 1.0f));
			}
			else
			{
				printf("BUNNY ERROR: Unknown token \"%s\".\n", buf);
				exit(1);
			}
			break;
		case 'f':
			if(buf[1] == '\0')	// face of 3 vertices
			{
				fscanf_s(file, "%d %d %d", &f.vertex[0], &f.vertex[1], &f.vertex[2]);
				faces.push_back(f);
				m_numVertices += 3;
			}
			else
			{
				printf("BUNNY ERROR: Unknown token \"%s\".\n", buf);
				exit(1);
			}
			break;
		default:
			break;
		}
	}

	// Allocate memory for vertices
	m_vertices = new vec4[m_numVertices];
	m_normals = new vec3[m_numVertices];
	vec4 p1, p2, p3;
	for(int i = 0; i < faces.size(); i++)
	{
		p1 = vertices.at(faces.at(i).vertex[0] - 1);
		p2 = vertices.at(faces.at(i).vertex[1] - 1);
		p3 = vertices.at(faces.at(i).vertex[2] - 1);

		vec3 n = cross(p2 - p1, p3 - p1);
		m_vertices[i*3 + 0] = p1; m_normals[i*3 + 0] = n;
		m_vertices[i*3 + 1] = p2; m_normals[i*3 + 0] = n;
		m_vertices[i*3 + 2] = p3; m_normals[i*3 + 0] = n;
	}
}

void Bunny::init(const char *textureFile)
{
	readVertices(textureFile);

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

	// Create a buffer object
	glGenBuffers(1, &m_vertexBufferObject);
	glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferObject);    

	// Initialize buffer
	glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0])*m_numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices, sizeof(m_normals[0])*m_numVertices, m_normals);

	// Load shaders and use the resulting shader program
	m_program = InitShader("vshader.glsl", "fshader.glsl");
	glUseProgram(m_program);

	// Specify vertex positions as an input to the shader program
	GLuint position = glGetAttribLocation(m_program, "vPosition");
	glEnableVertexAttribArray(position);
	glVertexAttribPointer(position, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0)); 

	// Specify normal vectors as an input to the shader program
	GLuint normal = glGetAttribLocation(m_program, "vNormal");
	glEnableVertexAttribArray(normal);
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0])*m_numVertices));

	// Clean up
	delete [] m_vertices;
	delete [] m_normals;
}

void Bunny::draw(bool mesh)
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

	// Get transformation matrices
	mat4 transform = Translate(m_location) * RotateZ(m_rotAngle.z) * RotateY(m_rotAngle.y) * RotateX(m_rotAngle.x) * Scale(m_size);

	// Pass uniform data to the shader program
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
	glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, m_color);
	if(mesh)
		glDrawArrays(GL_LINE_STRIP, 0, m_numVertices);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
}

	// ------------------------------- MODIFIERS ---------------------------------

void Bunny::setSize(float size)
{
	m_size = size;
}

void Bunny::setColor(vec4 color)
{
	m_color = color;
}

void Bunny::setLocation(vec3 location)
{
	m_location = location;
}

void Bunny::setAmbient(float ambient)
{
	m_ambient = ambient;
}

void Bunny::setDiffuse(float diffuse)
{
	m_diffuse = diffuse;
}

void Bunny::setSpecular(float specular)
{
	m_specular = specular;
}

void Bunny::setShininess(float shininess)
{
	m_shininess = shininess;
}

void Bunny::rotateX(float angle)
{
	m_rotAngle.x += angle;
	if(m_rotAngle.x >= 360.0f)
		m_rotAngle.x -= 360.0f;
}

void Bunny::rotateY(float angle)
{
	m_rotAngle.y = angle;
	//m_rotAngle.y += angle;
	//if(m_rotAngle.y >= 360.0f)
	//	m_rotAngle.y -= 360.0f;
}

void Bunny::rotateZ(float angle)
{
	m_rotAngle.z += angle;
	if(m_rotAngle.z >= 360.0f)
		m_rotAngle.z -= 360.0f;
}

// ---------------------------------------------------------------------------


// ------------------------------- ACCESSORS ---------------------------------

vec3 Bunny::getLocation()
{
	return vec3(m_location.x, m_location.y, m_location.z);
}

float Bunny::getSize()
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