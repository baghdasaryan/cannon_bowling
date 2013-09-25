// ############################################ //
// ############################################ //
// ####                                    #### //
// ####    Program: Mickey's Game          #### //
// ####    Language: C++                   #### //
// ####    Authors: Georgi Baghdasaryan    #### //
// ####    Email: baghdasaryan@ucla.edu    #### //
// ####                                    #### //
// ############################################ //
// ############################################ //


#include "main.h"

void printInstructions()
{
	// Print instructions
	std::cout << "Instructions:" << std::endl;
	std::cout << "    Press ESC, 'q' or 'Q' to exit the program." << std::endl;
	std::cout << "    Press Arrow Up to increase cannon altitude." << std::endl;
	std::cout << "    Press Arrow Down to decrease cannon altitude." << std::endl;
	std::cout << "    Press Arrow Left to rotate left around the table." << std::endl;
	std::cout << "    Press Arrow Right to rotate right around the table." << std::endl;

	// Print author info
	std::cout << "\nCreated by:" << std::endl;
	std::cout << "    Georgi Baghdasaryan    baghdasaryan@ucla.edu" << std::endl;
}

// Create and send objects to the GPU
void initScene()
{
	cylinders[0] = new Cylinder(32768, 1.0f, 20.0f, vec3(0.0f, 0.0f, 0.0f), vec4(188.0f/255.0f, 116.0f/255.0f, 43.0f/255.0f, 1.0f), 0.2f, 1.0f, 0.3f, 1000.0f, &camera, &light);
	cylinders[0]->init("image_table.tga");
	cylinders[0]->rotateX(90.0f);

	cylinders[1] = new Cheese(256, 1.0f, 3.0f, vec3(-3.0f/2.0f, 2.0f, 0.0f), vec4(1.0f, 1.0f, 0.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light, 30.0f);
	cylinders[1]->init("image_swiss_cheese.tga");
	cylinders[1]->rotateX(90.0f);

	cylinders[2] = new Cannon(32768, 2.0f, 0.5f, vec3(0.0f, 8.0f, 95.0f), vec4(1.0f, 0.0f, 0.0f, 0.5f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light);
	cylinders[2]->init("image_cannon.tga");
	cylinders[2]->rotateX(15.0f);

	spheres[0] = new SphereBall(5, 0.4f, vec3(camera.getPosition().x, camera.getPosition().y-0.5, camera.getPosition().z-5), vec4(0.0f, 0.0f, 1.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light, static_cast<Cheese*>(cylinders[1]));
	spheres[0]->init("");

	//spheres[1] = new Sphere(5, 1.0, vec3(0.0f, 3.0f, 3.0f), vec4(239.0f/255.0f, 121.0f/255.0f, 33.0f/255.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light);
	//spheres[1]->init("image_challenge.tga");

	//cubes[0] = new Cube(1.0f, vec3(0.0f, 1.5f, 3.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light);
	//cubes[0]->init("image_challenge.tga");

	monsters[0] = new Monster(0.5f, 18.5f, 3.0f, vec4(1.0f, 0.0f, 1.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light, static_cast<SphereBall*>(spheres[0]), static_cast<Cheese*>(cylinders[1]), "image_challenge.tga");
	monsters[1] = new Monster(0.5f, 18.5f, 5.0f, vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light, static_cast<SphereBall*>(spheres[0]), static_cast<Cheese*>(cylinders[1]), "image_challenge.tga");

	//bunnies[0] = new Bunny(0.5f, vec3(0.0f, 1.4578f, 23.0f), vec4(1.0f, 0.0f, 0.0f, 1.0f), 0.2f, 0.6f, 0.2f, 100.0f, &camera, &light);
	//bunnies[0]->rotateY(-90.0f);

	// Set OpenGL parameters
	glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

// Draw the scene
void drawScene()
{
	// Draw the objects
	cylinders[0]->draw(true, 0);	// bool => edges, number => mesh
	cylinders[1]->draw(true, 0);	// bool => edges, number => mesh
	cylinders[2]->draw(true, 0);	// bool => edges, number => mesh
	spheres[0]->draw(0);			// number => mesh
	//spheres[1]->draw(0);			// number => mesh
	//cubes[0]->draw(true, 0);		// bool => edges, number => mesh
	monsters[0]->draw(true, 0);		// bool => edges, number => mesh
	monsters[1]->draw(true, 0);		// bool => edges, number => mesh
	//bunnies[0]->draw(0);			// number => mesh
	cylinders[2]->draw(true, 0);	// bool => edges, number => mesh
}

// Clean up
void cleanUp()
{
	delete cylinders[0];
	delete cylinders[1];
	delete cylinders[2];

	delete spheres[0];

	delete monsters[0];
	delete monsters[1];
}


// ----------------------------------Callback functions-----------------------------------------

void displayCallback()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	drawScene();
	glutSwapBuffers();

	if(static_cast<Cheese*>(cylinders[1])->gameOver())
	{
		Sleep(2 * 1000); // Sleep for 2 seconds

		cleanUp();
		exit(0);
	}
}

void keyboardCallback(unsigned char key, int x, int y)
{
	switch(key)
	{
	case 033:				// Exit on ESC
	case 113:				// Exit on 'q'
	case 81:				// Exit on 'Q'
		cleanUp();
		exit(0);
		break;
	case 32:				// reset default positioning on Space
		static_cast<SphereBall*>(spheres[0])->launch();
		break;

#ifdef DEBUG_MODE_ON
	case 119:				// move camera up on 'w'
	case 87:				// move camera up on 'W'
		camera.vertical(0.01f);
		light.setPosition(camera.getPosition());
		break;
	case 115:				// move camera down on 's'
	case 83:				// move camera down on 'S'
		camera.vertical(-0.01f);
		light.setPosition(camera.getPosition());
		break;
	case 97:				// move camera left on 'a'
	case 65:				// move camera left on 'A'
		camera.horizontal(-0.01f);
		light.setPosition(camera.getPosition());
		break;
	case 100:				// move camera right on 'd'
	case 68:				// move camera right on 'D'
		camera.horizontal(0.01f);
		light.setPosition(camera.getPosition());
		break;
	case 'x':				// move camera forward on 'x'
	case 'X':				// move camera forward on 'X'
		camera.distance(-2.0f);
		break;
	case 'z':				// move camera backwards on 'z'
	case 'Z':				// move camera backwards on 'Z'
		camera.distance(2.0f);
		break;
	case 'r':				// reset camera position on 'r'
	case 'R':				// reset camera position on 'R'
		camera.reset();
		light.setPosition(camera.getPosition());
		break;
	case 'u':				// increase camera altitude on 'u'
	case 'U':				// increase camera altitude on 'U'
		camera.altitude(-0.5);
		light.setPosition(camera.getPosition());
		break;
	case 'j':				// decrease camera altitude on 'd'
	case 'J':				// decrease camera altitude on 'D'
		camera.altitude(0.5);
		light.setPosition(camera.getPosition());
		break;
#endif // DEBUG_MODE_ON

	default:
		break;
	};

	// Set redisplay flag
	glutPostRedisplay();
}

void keyboardUpCallback(unsigned char key, int x, int y)
{
	// Set redisplay flag
	glutPostRedisplay();
}

void specialCallback(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_UP:			// negative rotation around X-axis
		static_cast<SphereBall*>(spheres[0])->altitude(2.0f);
		cylinders[2]->rotateX(2.0f);
		break;
	case GLUT_KEY_DOWN:			// positive rotation around X-axis
		static_cast<SphereBall*>(spheres[0])->altitude(-2.0f);
		cylinders[2]->rotateX(-2.0f);
		break;
	case GLUT_KEY_LEFT:			// rotate cubes left around Y-axis
		if(!soundOn)
		{
			soundOn = true;
			PlaySound(TEXT("sound_move_start.wav"), NULL, SND_ASYNC | SND_FILENAME);
			PlaySound(TEXT("sound_move.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		}
		camera.azimuth(-1.0f);
		cylinders[2]->rotateY(-1.0f);
		light.setPosition(camera.getPosition());
		break;
	case GLUT_KEY_RIGHT:		// rotate cubes right around Y-axis
		if(!soundOn)
		{
			soundOn = true;
			PlaySound(TEXT("sound_move_start.wav"), NULL, SND_ASYNC | SND_FILENAME);
			PlaySound(TEXT("sound_move.wav"), NULL, SND_ASYNC | SND_FILENAME | SND_LOOP);
		}
		camera.azimuth(1.0f);
		cylinders[2]->rotateY(1.0f);
		light.setPosition(camera.getPosition());
		break;
	default:
		break;
	};

	// Set redisplay flag
	glutPostRedisplay();
}

void specialUpCallback(int key, int x, int y)
{
	switch(key)
	{
	case GLUT_KEY_LEFT:
		PlaySound(NULL, 0, 0);
		//PlaySound(TEXT("sound_move_end.wav"), NULL, SND_ASYNC | SND_FILENAME);
		soundOn = false;
		break;
	case GLUT_KEY_RIGHT:
		PlaySound(NULL, 0, 0);
		//PlaySound(TEXT("sound_move_end.wav"), NULL, SND_ASYNC | SND_FILENAME);
		soundOn = false;
		break;
	default:
		break;
	};

	// Set redisplay flag
	glutPostRedisplay();
}

void reshapeCallback(int w, int h)
{
	// Store new window size information
	window_width = w;
	window_height = h;

	// Change the aspect ratio if the window is reshaped
	glViewport( 0, 0, w, h );
	camera.updateAspect((GLfloat(w))/h);

	// Set redisplay flag
	glutPostRedisplay();
}

void idleCallback()
{
	TIME = TM.GetElapsedTime() ;
	DTIME = TIME - TIME_LAST;		// ANIMATE = DTIME * degrees per second
	TIME_LAST = TIME;
	
	MONSTER_TIME += DTIME;
	monsters[0]->launch();
	monsters[0]->update(DTIME);
	if (MONSTER_TIME >= 15)
	{
		MONSTER_TIME = 0;
		monsters[1]->launch();		
	}
	monsters[1]->update(DTIME);

	static_cast<SphereBall*>(spheres[0])->update(DTIME);
	
	// Set redisplay flag
	glutPostRedisplay();
}

// ---------------------------------------------------------------------------------------------


int main(int argc, char **argv)
{
	printInstructions();

	// Setup glut
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH |GLUT_MULTISAMPLE);
	glutInitWindowSize(window_width, window_height);
	//glutInitContextVersion(3, 2);
	//glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow("Mickey's Cannon Bowling");

	// Setup glew
	glewExperimental = GL_TRUE;
	glewInit();

	// Initialize objects
	initScene();

	// Setup callback functions
	glutDisplayFunc(displayCallback);
	glutKeyboardFunc(keyboardCallback);
	glutKeyboardUpFunc(keyboardUpCallback);
	glutSpecialFunc(specialCallback);
	glutSpecialUpFunc(specialUpCallback);
	glutReshapeFunc(reshapeCallback);
	glutIdleFunc(idleCallback);

	glutMainLoop();
	return 0;
}
