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


#include "Cylinder.h"

// **************************************************************** //
// *********************** Cylinder ******************************* //
// **************************************************************** //

Cylinder::Cylinder(int divs, float size, float radius, vec3 location, vec4 color, float ambient, 
				   float diffuse, float specular, float shininess, Camera *camera, Light *light)
{
	m_numDivisions = divs;
	m_numVertices = 0;
	m_numEdges = 0;
	m_size = size;
	m_radius = radius;
	m_location = location;
	m_color = color;
	m_ambient = ambient;
	m_diffuse = diffuse;
	m_specular = specular;
	m_shininess = shininess;
	m_camPtr = camera;
	m_lightPtr = light;
	m_rotAngle = 0.0f;
	m_textureOn = false;
}

// ---------------------------- CREATE A CYLINDER ----------------------------

void Cylinder::generateCircle(float angle)
{
	float theta;
    for(int i = 0; i < m_numDivisions; i++)
    {
        theta = (i * angle * 2.0 * M_PI) / (360.0f * m_numDivisions);
		m_circlePoints[i] = m_radius * vec2(cosf(theta), sinf(theta));
    }
}

void Cylinder::generateWall(float z, int rectangles)
{
	vec3 n;
	for(int i = 0; i < rectangles; i++)
	{
		vec3 p1(m_circlePoints[i].x, m_circlePoints[i].y, z);
		vec3 p2(0.0f, 0.0f, z);
		vec3 p3(m_circlePoints[(i+1)%m_numDivisions].x, m_circlePoints[(i+1)%m_numDivisions].y, z);

		if(z < 0.0f)
		{
			vec3 temp = p1;
			p1 = p3;
			p3 = temp;
		}

		n = cross(p1-p2, p3-p2);
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(acosf(p3.x/m_radius)/M_PI, 0.5 + (asinf(p3.y/m_radius)/M_PI)); m_numVertices++;
		m_edges[m_numEdges++] = p1;
		m_vertices[m_numVertices] = p2; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.5f, 0.5f); m_numVertices++;
		m_vertices[m_numVertices] = p3; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(acosf(p3.x/m_radius)/M_PI, 0.5 + (asinf(p3.y/m_radius)/M_PI)); m_numVertices++;
		m_edges[m_numEdges++] = p3;
	}
}

void Cylinder::generateSide(int rectangles)
{
	for(int i = 0; i < rectangles; i++)
    {
		int i2 = (i+1)%m_numDivisions;

        vec3 p1(m_circlePoints[i2].x, m_circlePoints[i2].y, -1.0f);
        vec3 p2(m_circlePoints[i2].x, m_circlePoints[i2].y,  1.0f);
        vec3 p3(m_circlePoints[i].x,  m_circlePoints[i].y,   1.0f);

		//point3 n = cross(p3-p2, p1-p2);	// use for flat shading
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = vec3(p1.x, p1.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p1.y/m_radius, p1.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p1.z)/M_PI); m_numVertices++;
		m_vertices[m_numVertices] = p2; m_normals[m_numVertices] = vec3(p2.x, p2.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p2.y/m_radius, p2.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p2.z)/M_PI); m_numVertices++;
		m_vertices[m_numVertices] = p3; m_normals[m_numVertices] = vec3(p3.x, p3.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p3.y/m_radius, p3.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p3.z)/M_PI); m_numVertices++;
		
		p1 = vec3(m_circlePoints[i2].x, m_circlePoints[i2].y, -1.0f);
        p2 = vec3(m_circlePoints[i].x,  m_circlePoints[i].y,   1.0f);
        p3 = vec3(m_circlePoints[i].x,  m_circlePoints[i].y,  -1.0f);

        //n = cross(p3-p2, p1-p2);			// use for flat shading
        m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = vec3(p1.x, p1.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p1.y/m_radius, p1.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p1.z)/M_PI); m_numVertices++;
        m_vertices[m_numVertices] = p2; m_normals[m_numVertices] = vec3(p2.x, p2.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p2.y/m_radius, p2.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p2.z)/M_PI); m_numVertices++;
        m_vertices[m_numVertices] = p3; m_normals[m_numVertices] = vec3(p3.x, p3.y, 0.0f); m_textureUV[m_numVertices] = vec2(4.0f*0.5f*atan2f(p3.y/m_radius, p3.x/m_radius)/M_PI + 1.0f, 0.5f - asin(p3.z)/M_PI); m_numVertices++;
    }
}

void Cylinder::generateCylinder()
{
    generateCircle();
    generateWall( 1.0f, m_numDivisions);
    generateWall(-1.0f, m_numDivisions);
	generateSide(m_numDivisions);
}

// ---------------------------------------------------------------------------


// ---------------------------- UPLOAD DATA TO GPU ---------------------------

void Cylinder::memAlloc()
{
	m_vertices = new vec4[m_numDivisions * 12];
	m_edges = new vec4[m_numDivisions * 4];
	m_textureUV = new vec2[m_numDivisions * 12];
	m_normals = new vec3[m_numDivisions * 12];
}

void Cylinder::init(const char *textureFile)
{
	m_textureOn = (textureFile[0] != '\0')? true: false;

	// Allocate space for vertices
	memAlloc();
	m_circlePoints = new vec2[m_numDivisions];

	generateCylinder();	// generate cylinder points

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
	glBufferData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]) + sizeof(m_textureUV[0]))*m_numVertices + sizeof(m_edges[0])*m_numEdges, NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(m_vertices[0])*m_numVertices, m_vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices, sizeof(m_edges[0])*m_numEdges, m_edges);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(m_vertices[0])*m_numVertices + sizeof(m_edges[0])*m_numEdges, sizeof(m_normals[0])*m_numVertices, m_normals);
	glBufferSubData(GL_ARRAY_BUFFER, (sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices + sizeof(m_edges[0])*m_numEdges, sizeof(m_textureUV[0])*m_numVertices, m_textureUV);

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
	glVertexAttribPointer(normal, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(sizeof(m_vertices[0])*m_numVertices + sizeof(m_edges[0])*m_numEdges));

	// Specify texture positions as an input to the shader program
	GLuint texture = glGetAttribLocation(m_program, "vTexture");
	glEnableVertexAttribArray(texture);
	glVertexAttribPointer(texture, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET((sizeof(m_vertices[0]) + sizeof(m_normals[0]))*m_numVertices + sizeof(m_edges[0])*m_numEdges)); 

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
	delete [] m_edges;
	delete [] m_textureUV;
	delete [] m_normals;
	delete [] m_circlePoints;
}

// ---------------------------------------------------------------------------


// ------------------------------- MODIFIERS ---------------------------------

void Cylinder::setSize(float size)
{
	m_size = size;
}

void Cylinder::setColor(vec4 color)
{
	m_color = color;
}

void Cylinder::setLocation(vec3 location)
{
	m_location = location;
}

void Cylinder::setAmbient(float ambient)
{
	m_ambient = ambient;
}

void Cylinder::setDiffuse(float diffuse)
{
	m_diffuse = diffuse;
}

void Cylinder::setSpecular(float specular)
{
	m_specular = specular;
}

void Cylinder::setShininess(float shininess)
{
	m_shininess = shininess;
}

void Cylinder::rotateX(float angle)
{
	m_rotAngle.x += angle;
	if(m_rotAngle.x >= 360.0)
		m_rotAngle.x -= 360.0;
}

void Cylinder::rotateY(float angle)
{
	m_rotAngle.y += angle;
	if(m_rotAngle.y >= 360.0)
		m_rotAngle.y -= 360.0;
}

void Cylinder::rotateZ(float angle)
{
	m_rotAngle.z += angle;
	if(m_rotAngle.z >= 360.0)
		m_rotAngle.z -= 360.0;
}

// ---------------------------------------------------------------------------


// ------------------------------- ACCESSORS ---------------------------------

vec3 Cylinder::getLocation()
{
	return vec3(m_location.x, m_location.y, m_location.z);
}

float Cylinder::getRadius()
{
	return m_size*m_radius;
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


void Cylinder::draw(bool edges, bool mesh)
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

	// Get transformation matrices (POSSIBLE ERRORS -> ROTATION AROUND Y)
	mat4 transform = RotateY(m_rotAngle.y) * Translate(m_location) * RotateZ(m_rotAngle.z) * RotateX(m_rotAngle.x) * Scale(m_size);

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

	// Draw edges
	if(edges)
	{
		glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, vec4(0.0, 0.0, 1.0, 1.0));
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonOffset(0.5, 0.5);
		glDrawArrays(GL_LINES, m_numVertices, m_numEdges);
		glDisable(GL_POLYGON_OFFSET_FILL);
	}

	// Draw objects
	if(m_textureOn)
		glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 1);
	glUniform4fv(glGetUniformLocation(m_program, "Color"), 1, m_color);
	if(mesh)
		glDrawArrays(GL_LINE_STRIP, 0, m_numVertices);
	else
		glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
	glUniform1i(glGetUniformLocation(m_program, "EnableTex"), 0);
}



// **************************************************************** //
// *********************** CylinderSlice ************************** //
// **************************************************************** //

CylinderSlice::CylinderSlice(int divs, float size, float radius, vec3 location, vec4 color, float ambient, float diffuse,
							 float specular, float shininess, Camera *camera, Light *light, float angle)
							 : Cylinder(divs, size, radius, location, color, ambient, diffuse, specular, shininess, camera, light)
{
	m_sliceAngle = angle;
}

void CylinderSlice::memAlloc()
{
	m_vertices = new vec4[m_numDivisions * 12 + 6];
	m_edges = new vec4[m_numDivisions * 4 + 10];
	m_textureUV = new vec2[m_numDivisions * 12 + 6];
	m_normals = new vec3[m_numDivisions * 12 + 6];
}

void CylinderSlice::generateCylinder()
{
	generateCircle(m_sliceAngle);
	generateWall( 1.0f, m_numDivisions-1);
	generateWall(-1.0f, m_numDivisions-1);
	generateSide(m_numDivisions-1);

	// build extra sides
	if(m_sliceAngle < 360.0)
	{
		vec3 p1(m_circlePoints[0].x, m_circlePoints[0].y,  1.0f);
		vec3 p2(0.0f, 0.0f,  1.0f);
		vec3 p3(m_circlePoints[0].x, m_circlePoints[0].y, -1.0f);
		vec3 p4(0.0f, 0.0f, -1.0f);

		// Side 1
		vec3 n = cross(p4-p2, p1-p2);
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 0.0f); m_numVertices++;
		m_vertices[m_numVertices] = p2; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 0.0f); m_numVertices++;
		m_vertices[m_numVertices] = p4; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 1.0f); m_numVertices++;
		n = cross(p3-p4, p1-p4);	// value does not change
		m_vertices[m_numVertices] = p4; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 1.0f); m_numVertices++;
		m_vertices[m_numVertices] = p3; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 1.0f); m_numVertices++;
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 0.0f); m_numVertices++;

		m_edges[m_numEdges++] = p1;
		m_edges[m_numEdges++] = p2; 
		m_edges[m_numEdges++] = p2;
		m_edges[m_numEdges++] = p4;
		m_edges[m_numEdges++] = p4;
		m_edges[m_numEdges++] = p3;
		m_edges[m_numEdges++] = p3;
		m_edges[m_numEdges++] = p1;

		m_cols[0].n = normalize(n);
		//m_cols[0].n.y = 0.0f; m_cols[0].n.z = -1.0f;
		m_cols[0].d = dot(m_cols[0].n, p4);

		p1 = vec3(m_circlePoints[m_numDivisions-1].x, m_circlePoints[m_numDivisions-1].y,  1.0f);
		p3 = vec3(m_circlePoints[m_numDivisions-1].x, m_circlePoints[m_numDivisions-1].y, -1.0f);

		// Side 2
		n = cross(p4-p3, p1-p3);
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 0.0f); m_numVertices++;
		m_vertices[m_numVertices] = p3; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 1.0f); m_numVertices++;
		m_vertices[m_numVertices] = p4; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 1.0f); m_numVertices++;
		n = cross(p1-p2, p4-p2);	// value does not change
		m_vertices[m_numVertices] = p4; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 1.0f); m_numVertices++;
		m_vertices[m_numVertices] = p2; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(0.0f, 0.0f); m_numVertices++;
		m_vertices[m_numVertices] = p1; m_normals[m_numVertices] = n; m_textureUV[m_numVertices] = vec2(1.0f, 0.0f); m_numVertices++;

		m_edges[m_numEdges++] = p1;
		m_edges[m_numEdges++] = p2;
		m_edges[m_numEdges++] = p4;
		m_edges[m_numEdges++] = p3;
		m_edges[m_numEdges++] = p3;
		m_edges[m_numEdges++] = p1;

		m_cols[1].n = normalize(n);
		m_cols[1].n.z = m_cols[1].n.y; m_cols[1].n.y = 0.0f;
		m_cols[1].d = dot(m_cols[1].n, p2);

		p2 = vec3(m_circlePoints[0].x, m_circlePoints[0].y, 1.0f);

		m_cols[2].n = p1;//normalize(cross(p2 - p1, p3 - p1));
		m_cols[2].d = dot(m_cols[2].n, p1);
	}
}

collisionHolder *CylinderSlice::getCollisions()
{
	return m_cols;
}



// **************************************************************** //
// *********************** Cheese ********************************* //
// **************************************************************** //

Cheese::Cheese(int divs, float size, float radius, vec3 location, vec4 color, float ambient, float diffuse,
			   float specular, float shininess, Camera *camera, Light *light, float angle)
			   : CylinderSlice(divs, size, radius, location, color, ambient, diffuse, specular, shininess, camera, light, angle)
{
	m_healthPtr = new HealthBar(ambient, diffuse, specular, shininess, camera, light, 0.0625, 0.5);
	m_healthPtr->init();
}

void Cheese::hit(float healthLost)
{
	m_healthPtr->change(-healthLost);
}

bool Cheese::gameOver()
{
	return (m_healthPtr->dead());
}

void Cheese::draw(bool edges, bool mesh)
{
	Cylinder::draw(edges, mesh);
	m_healthPtr->draw(true, mesh);
}
