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


#include "cube.h"

// Initialize data
Cube::Cube(float size, vec3 location, vec4 color, float ambient, float diffuse,
		float specular, float shininess, Camera *camera, Light *light)
{
	m_size = size;
	m_color = color;
	m_location = location;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_camPtr = camera;
	m_lightPtr = light;
	m_textureOn = false;
	m_rotAngle = 0.0f;
}

void Cube::genCube()
{
	// generate points for drawing cube faces
	m_index = 0;
	genPoints(vec3( 0.0,  0.0,  1.0), 1, 0, 3, 2, true);
	genPoints(vec3( 1.0,  0.0,  0.0), 2, 3, 7, 6, true);
	genPoints(vec3( 0.0, -1.0,  0.0), 3, 0, 4, 7, true);
	genPoints(vec3( 0.0,  1.0,  0.0), 6, 5, 1, 2, true);
	genPoints(vec3( 0.0,  0.0, -1.0), 6, 7, 4, 5, true);
	genPoints(vec3(-1.0,  0.0,  0.0), 5, 4, 0, 1, true);
	
	// generate points for drawing cube edges
	m_index = 0;
	genPoints(vec3( 0.0, 0.0, 0.0), 0, 1, 1, 2, false);
	genPoints(vec3( 0.0, 0.0, 0.0), 2, 3, 3, 0, false);
	genPoints(vec3( 0.0, 0.0, 0.0), 0, 4, 4, 5, false);
	genPoints(vec3( 0.0, 0.0, 0.0), 5, 6, 6, 7, false);
	genPoints(vec3( 0.0, 0.0, 0.0), 7, 4, 1, 5, false);
	genPoints(vec3( 0.0, 0.0, 0.0), 2, 6, 3, 7, false);
}

void Cube::genPoints(vec3 norm, int a, int b, int c, int d, bool faceTriangles)
{
	if(faceTriangles)	// for cube vertices
	{
		m_vertices[m_index] = vertices[a];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(0.0, 1.0);
		m_vertices[m_index] = vertices[b];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(0.0, 0.0);
		m_vertices[m_index] = vertices[c];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(1.0, 0.0);
		m_vertices[m_index] = vertices[a];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(0.0, 1.0);
		m_vertices[m_index] = vertices[c];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(1.0, 0.0);
		m_vertices[m_index] = vertices[d];	m_normals[m_index] = norm;	m_textureUV[m_index++] = vec2(1.0, 1.0);
	}
	else				// for edge vertices
	{
		m_edges[m_index++] = vertices[a];
		m_edges[m_index++] = vertices[b];
		m_edges[m_index++] = vertices[c];
		m_edges[m_index++] = vertices[d];
	}
}

void Cube::init(const char *textureFile)
{
	m_textureOn = (textureFile[0] != '\0')? true: false;

	// Allocate space for vertices
	m_vertices = new vec4[numVertices];
	m_normals = new vec3[numVertices];
	m_textureUV = new vec2[numVertices];
	m_edges = new vec4[numEdges];

	genCube();	// generate cube points

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
	glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]) + sizeof(m_textureUV[0]))*numVertices + sizeof(m_edges[0])*numEdges, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0])*numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*numVertices, sizeof(m_edges[0])*numEdges, m_edges);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*numVertices + sizeof(m_edges[0])*numEdges, sizeof(m_normals[0])*numVertices, m_normals);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]))*numVertices + sizeof(m_edges[0])*numEdges, sizeof(m_textureUV[0])*numVertices, m_textureUV);

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
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0])*numVertices + sizeof(m_edges[0])*numEdges));

	// Specify texture positions as an input to the shader program
	GLuint texture = glGetAttribLocation(m_program, "vTexture");
	glEnableVertexAttribArray(texture);
	glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(m_vertices[0]) + sizeof(m_normals[0]))*numVertices + sizeof(m_edges[0])*numEdges)); 

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
	delete [] m_normals;
	delete [] m_textureUV;
	delete [] m_edges;
}

void Cube::draw(bool edges, bool mesh)
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
	mat4 transform = Translate(m_location) *  RotateZ(m_rotAngle.z) * RotateY(m_rotAngle.y) * RotateX(m_rotAngle.x) * Scale(m_size);

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

	// Draw cube edges
	if(edges)
	{
		glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, vec4(0.0, 0.0, 1.0, 1.0));
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 0.5);
		glDrawArrays(GL_LINES, numVertices, numEdges);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	// Draw objects
	if(m_textureOn)
		glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 1);
	glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, m_color);
	if(mesh)
		glDrawArrays(GL_LINE_STRIP, 0, numVertices);
	else
		glDrawArrays(GL_TRIANGLES, 0, numVertices);
	glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 0);
}

// ------------------------------- MODIFIERS ---------------------------------

void Cube::setSize(float size)
{
	m_size = size;
}

void Cube::setColor(vec4 color)
{
	m_color = color;
}

void Cube::setLocation(vec3 location)
{
	m_location = location;
}

void Cube::setAmbient(float ambient)
{
	m_ambient = ambient;
}

void Cube::setDiffuse(float diffuse)
{
	m_diffuse = diffuse;
}

void Cube::setSpecular(float specular)
{
	m_specular = specular;
}

void Cube::setShininess(float shininess)
{
	m_shininess = shininess;
}

void Cube::rotateX(float angle)
{
	m_rotAngle.x += angle;
	if(m_rotAngle.x >= 360.0f)
		m_rotAngle.x -= 360.0f;
}

void Cube::rotateY(float angle)
{
	m_rotAngle.y = angle;
	//m_rotAngle.y += angle;
	//if(m_rotAngle.y >= 360.0f)
	//	m_rotAngle.y -= 360.0f;
}

void Cube::rotateZ(float angle)
{
	m_rotAngle.z += angle;
	if(m_rotAngle.z >= 360.0f)
		m_rotAngle.z -= 360.0f;
}

// ---------------------------------------------------------------------------


// ------------------------------- ACCESSORS ---------------------------------

vec3 Cube::getLocation()
{
	return vec3(m_location.x, m_location.y, m_location.z);
}

float Cube::getSize()
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
