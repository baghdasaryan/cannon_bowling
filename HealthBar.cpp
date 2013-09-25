// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Health Bar             #### //
// ####    Language: C++                   #### //
// ####    Author: Georgi Baghdasaryan     #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#include "HealthBar.h"

HealthBar::HealthBar(float ambient, float diffuse, float specular, float shininess, Camera *camera, Light *light, float height, float width)
{
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_camPtr = camera;
	m_lightPtr = light;
	m_height = height;
	m_width = width;

	m_numVertices = 0;
	m_numEdges = 0;
	m_health = 100;
}

void HealthBar::change(float health)
{
	if(m_health + health <= 100)
		m_health += health;
	if(m_health < 0)
		m_health = 0;
}

bool HealthBar::dead()
{
	return (m_health <= 0.0f);
}

void HealthBar::generateHealthBar()
{
	// Create vertices for inside
	float dx = m_width/100.0f;
	for(float i = -m_width/2.0; i < m_width/2.0; i+= dx)
	{
		m_vertices[m_numVertices] = vec4(i, -m_height/2.0, 0.0f, 1.0f);
		m_normals[m_numVertices] = vec3(0.0f, 0.0f, 1.0f); 
		m_numVertices++;

		m_vertices[m_numVertices] = vec4(i, m_height/2.0, 0.0f, 1.0f);		
		m_normals[m_numVertices] = vec3(0.0f, 0.0f, 1.0f); 
		m_numVertices++;
	}

	// Create edge vertices
	int m_numEdges = 0;
	m_edges[m_numEdges++] = vec4(-m_width/2.0, -m_height/2.0, 0.0f, 1.0f);
	m_edges[m_numEdges++] = vec4(-m_width/2.0,  m_height/2.0, 0.0f, 1.0f);
	m_edges[m_numEdges++] = vec4( m_width/2.0,  m_height/2.0, 0.0f, 1.0f);
	m_edges[m_numEdges++] = vec4( m_width/2.0, -m_height/2.0, 0.0f, 1.0f);
	m_edges[m_numEdges++] = vec4(-m_width/2.0, -m_height/2.0, 0.0f, 1.0f);
}

void HealthBar::init()
{
	// Allocate space for vertices
	m_vertices = new vec4[202];
	m_edges = new vec4[5];
	m_normals = new vec3[202];

	// Create points
	generateHealthBar();

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
	glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices + sizeof(m_edges[0])*5, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0])*m_numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices, sizeof(m_edges[0])*5, m_edges);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices + sizeof(m_edges[0])*5, sizeof(m_normals[0])*m_numVertices, m_normals);

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
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0])*m_numVertices + sizeof(m_edges[0])*5));

	// Clean up
	delete [] m_vertices;
	delete [] m_edges;
	delete [] m_normals;
}

void HealthBar::draw(bool edges, bool mesh)
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
	mat4 transform = Translate(vec4(-0.7f, 0.85f, 0.0f, 1.0f));
		
	// Pass uniform data to the shader program
	glUniformMatrix4fv(glGetUniformLocation(m_program, "wMo"), 1, GL_TRUE, transform);
	glUniformMatrix4fv(glGetUniformLocation(m_program, "cMw"), 1, GL_TRUE, mat4());
	glUniformMatrix4fv(glGetUniformLocation(m_program, "proj"), 1, GL_TRUE, mat4());

	// vShader
	glUniform4fv(glGetUniformLocation(m_program, "camPos"), 1, m_camPtr->getPosition());
	glUniform4fv(glGetUniformLocation(m_program, "lightPos"), 1, vec4(0.0f, 0.0f, 1.0f, 1.0f));
	
	// fShader
	glUniform1f(glGetUniformLocation(m_program, "Ambient"), m_ambient);
	glUniform1f(glGetUniformLocation(m_program, "Diffuse"), m_diffuse);
	glUniform1f(glGetUniformLocation(m_program, "Specular"), m_specular);
	glUniform1f(glGetUniformLocation(m_program, "Shininess"), m_shininess);
	glUniform4fv(glGetUniformLocation(m_program, "lightColor"), 1, vec4(0.0f, 0.0f, 0.0f, 1.0f));

	glDisable(GL_DEPTH_TEST);

	// Draw edges
	if(edges)
	{
		glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, vec4(0.0, 0.0, 0.0, 1.0));
		glEnable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LINE_SMOOTH);
		glLineWidth(5);
		glPolygonOffset(0.5, 0.5);
		glDrawArrays(GL_LINE_STRIP, m_numVertices, 5);
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_OFFSET_FILL);
		glLineWidth(1);
	}

	// Draw objects
	glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, vec4(0.0, 1.0, 0.0, 1.0));
	if(mesh)
		glDrawArrays(GL_LINE_STRIP, 0, (2 + m_health*2));
	else
		glDrawArrays(GL_TRIANGLE_STRIP, 0, (2 + m_health*2));

	glEnable(GL_DEPTH_TEST);
}
