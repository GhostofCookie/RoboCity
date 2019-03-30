#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <vector>
#include <cmath>

#include <string>
#include <time.h>

#include <Camera.h>
#include <Robot.h>

#include <StreetGenerator.h>
#include <Building.h>

#define FPS 60

// Functions
void Setup();
void Display();
void Idle(int);
void init(void);
void reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int);
void SpecialKey(int key, int x, int y);
void SpecialKeyUp(int key, int x, int y);
void MousePassive(int x, int y);
void ProcessHits(GLint hits, GLuint buffer[]);
void drawObjects(GLenum mode);
void PrintToScreen(const char * str, float x, float y, float[]);
Building::BuildingType ChooseBuildingType();

// Variables
GLfloat _width = 780, _height = 500;
int _prevTime = time(NULL), _curTime, _frameCount = 0;
int _tick = 0;
bool pause = false;
int gridX, gridZ;
bool selectReady = true;

Robot* _robot = new Robot();
Camera* _camera = new Camera(_robot);

int cSize = 0;
float gridScale = 10.0;
StreetGenerator* _city = new StreetGenerator(20, gridScale, gridScale);
std::vector<std::vector<Building>> buildings;

float worldOffset;


// MAIN
int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(_width, _height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("RoboCity");
	init();
	Setup();
	glutDisplayFunc(Display);
	glutReshapeFunc(reshape);
	glutMouseFunc(Mouse);
	glutKeyboardFunc(Keyboard);
	glutSpecialFunc(SpecialKey);
	glutSpecialUpFunc(SpecialKeyUp);
	glutTimerFunc(1000 / FPS, Idle, 0);
	//glutIdleFunc(Display); // call display while idle
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glutMainLoop();
	return 0;
}

//
void init(void)
{
	// Set random seed
	srand(time(0));

	glClearColor(0.468, 0.839, 0.976, 1.0);
	glClearDepth(1.0);

	// Enable depth testing.
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// Enable transparency.
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat lpos[4] = { 100, 100, 100, 100 };
	GLfloat white[4] = { 1.f, 1.f, 1.f, 1.f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white);
	glLightfv(GL_LIGHT0, GL_POSITION, lpos);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	// City Generator info
	_city->CreateStreets_Simple();
	gridX = gridZ = _city->CitySize() / 2;
	cSize = _city->CitySize();

	worldOffset = (-cSize / 2 * gridScale) - (gridScale / 2);

	// Init buildings per block
	for (int z = 0; z < cSize; z++)
	{
		buildings.push_back(std::vector<Building>());
		for (int x = 0; x < cSize; x++)
		{
			Block b = _city->grid[z][x];

			if (!b.isStreet)
			{
				int heightMult;

				// For each of the 4 building locations:
				if (b.buildings[0].canBuild)
				{
					heightMult = (rand() % 3) + 1;
					buildings[z].push_back(Building(
						b.buildings[0].x + worldOffset, b.buildings[0].y + worldOffset,
						gridScale / 3, gridScale / heightMult, gridScale / 3, ChooseBuildingType()));
				}
				if (b.buildings[1].canBuild)
				{
					heightMult = (rand() % 4) + 1;
					buildings[z].push_back(Building(
						b.buildings[1].x + worldOffset, b.buildings[1].y + worldOffset,
						gridScale / 3, gridScale / heightMult, gridScale / 3, ChooseBuildingType()));
				}
				if (b.buildings[2].canBuild)
				{
					heightMult = (rand() % 4) + 1;
					buildings[z].push_back(Building(
						b.buildings[2].x + worldOffset, b.buildings[2].y + worldOffset,
						gridScale / 3, gridScale / heightMult, gridScale / 3, ChooseBuildingType()));
				}
				if (b.buildings[3].canBuild)
				{
					heightMult = (rand() % 4) + 1;
					buildings[z].push_back(Building(
						b.buildings[3].x + worldOffset, b.buildings[3].y + worldOffset,
						gridScale / 3, gridScale / heightMult, gridScale / 3, ChooseBuildingType()));
				}
			}
		}
	}
}

//
void Setup()
{
}

//
void reshape(int w, int h)
{
	if (!h) h = 1;
	glViewport(0, 0, _width, _height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.f, (GLfloat)w / (GLfloat)h, 0.01, 1000);

	_width = (GLfloat)w;
	_height = (GLfloat)h;

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void drawObjects(GLenum mode)
{
	int i = 1;
	for (int z = 0; z < buildings.size(); z++)
		for (int x = 0; x < buildings[z].size(); x++)
		{
			if (mode == GL_SELECT)
			{
				glLoadName(z + (x * buildings.size()));
			}
			buildings[z][x].Display(_tick);
		}
}

//
void Display()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float robotx = _robot->GetLocation().x;
	float robotz = _robot->GetLocation().z;

	glLoadIdentity();
	_camera->Display(_tick);
	//gluLookAt(robotx,0,5+robotz,robotx,0,robotz,0,1,0);
	//glTranslatef(0.f,0.f,-5.f);

	// Center point
	glColor3f(1, 1, 1);
	glutWireCube(1.0);

	_robot->Render();

	// Debug comments
	//std::cout << gridX << std::endl;
	//std::cout << gridZ << std::endl << std::endl;

	// City Grid
	glPushMatrix();
	//glTranslatef((-cSize / 2 * gridScale) - (gridScale / 2),
	//	-0.5,
	//	(-cSize / 2 * gridScale) - (gridScale / 2));

	glBegin(GL_QUADS);
	// Creates the grid of quads using the city information
	for (int z = 0; z < cSize; z++)
	{
		for (int x = 0; x < cSize; x++)
		{
			Block b = _city->grid[z][x];
			// Decide on color - can be cleaned up
			if (b.isIntersection)
				//glColor3f(0.6, 0.6, 0.6);
				glColor3f(0.1, 0.1, 0.1);
			else if (b.isStreet)
				//glColor3f(0.55, 0.55, 0.55);
				glColor3f(0.1, 0.1, 0.1);
			else
				//glColor3f(0.05, 0.05, 0.05);
				glColor3f(0.5, 0.5, 0.5);

			float startx = b.startX + worldOffset;
			float endx = b.endX + worldOffset;
			float starty = b.startY + worldOffset;
			float endy = b.endY + worldOffset;
			glVertex3f(startx, -0.5, endy);
			glVertex3f(endx, -0.5, endy);
			glVertex3f(endx, -0.5, starty);
			glVertex3f(startx, -0.5, starty);
		}
	}
	// All polygons have been drawn.
	glEnd();

	drawObjects(GL_RENDER);

	glPopMatrix();


	glFlush();
	glutSwapBuffers();

	_frameCount++;
	_curTime = time(NULL);
	if (_curTime - _prevTime > 0)
	{
		//std::cout<< "FPS" << _frameCount / (_curTime - _prevTime) << std::endl;
		_tick = _frameCount / (_curTime - _prevTime);
		_frameCount = 0;
		_prevTime = _curTime;
	}
}

//
void Mouse(int button, int state, int x, int y)
{
	// overObject = if x and y are on a building
	switch (button)
	{
	case  GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN)
		{
			if (selectReady)
			{
				selectReady = false;
				const int buff = 128;
				GLuint selectBuf[buff];
				GLint hits;
				GLint viewport[4];
				glGetIntegerv(GL_VIEWPORT, viewport);
				glSelectBuffer(buff, selectBuf);

				glRenderMode(GL_SELECT);
				glInitNames();
				glPushName(0);

				glMatrixMode(GL_PROJECTION);
				glPushMatrix();
				glLoadIdentity();
				/*  create 5x5 pixel picking region near cursor location	*/
				gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.f, 1.f, viewport);
				gluPerspective(45.f, _width / _height, 0.01, 1000);
				drawObjects(GL_SELECT);
				glMatrixMode(GL_PROJECTION);
				glPopMatrix();
				glFlush();

				hits = glRenderMode(GL_RENDER);
				ProcessHits(hits, selectBuf);

				glMatrixMode(GL_MODELVIEW);
				glLoadIdentity();
				glutPostRedisplay();
			}
		}
		break;
	}
}

void ProcessHits(GLint hits, GLuint buffer[])
{
	float diffLength = 9999999.0f;
	float minLength = 9999999.0f;
	float idxX = 0;
	float idxY = 0;
	bool isHit = false;

	unsigned int i, j;
	GLint names, *ptr;
	
	ptr = (GLint *)buffer;
	for (i = 0; i < hits; i++)
	{	/*  for each hit  */
		names = *ptr;
		ptr += 3;
		for (j = 0; j < names; j++)
		{ /*  for each name */
		   /*
		 nest for loop for all buildings
		 of each building calc closest to robot
		 if closer replace closesst

	   */
			int x = *ptr % buildings.size();
			int y = *ptr / buildings.size();
			Building b = buildings[x][y];

			diffLength = std::sqrt(std::pow(b.cx - _robot->GetLocation().x, 2) + std::pow(b.cz - _robot->GetLocation().z, 2));
			if (diffLength < minLength)
			{
				minLength = diffLength;
				idxX = x;
				idxY = y;
				isHit = true;
			}

			ptr++;
		}
	}

	if (isHit) buildings[idxX][idxY].Hit();

	selectReady = true;
}

//
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'q':
		//if at intersection, turn left
		if (!pause)
		{
			if (_city->grid[gridX][gridZ].isIntersection) {
				_robot->Rotate(90.f);
				_camera->RotateCamera(0.f, -90.f, 0.f);
			}
		}
		break;
	case 'a':
		if (!pause)
		{
			if (_city->grid[gridX][gridZ].isIntersection) {
				_robot->Rotate(-90.f);
				_camera->RotateCamera(0.f, 90.f, 0.f);
			}
		}
		break;
	case 'p':
		//pause the game
		pause = !pause;
		break;
	case 'r':
		//return robot to original position
		if (!pause)
			_robot->SetLocation(0.0, 0.0, 0.0);
		break;
	case 'z':
		// TODO uncomment after robot updates in main
		//gridX = (_robot->GetLocation().x - (-cSize / 2 * gridScale)) / gridScale;
		//gridZ = (_robot->GetLocation().z - (-cSize / 2 * gridScale)) / gridScale;

		float nextX = gridX + _robot->ForwardVector().x;
		float nextZ = gridZ + _robot->ForwardVector().z;

		// move robot forward
		if ((nextX >= 0 && nextX < cSize) && (nextZ >= 0 && nextZ < cSize))
		{
			if (_city->grid[nextX][nextZ].isStreet)
			{
				_robot->MoveForward(gridScale);
				gridX = (_robot->GetLocation().x - (-cSize / 2 * gridScale)) / gridScale;
				gridZ = (_robot->GetLocation().z - (-cSize / 2 * gridScale)) / gridScale;
			}
		}
		break;
	}

}

//
void SpecialKey(int key, int x, int y)
{

	float robotx = _robot->GetLocation().x;
	float robotz = _robot->GetLocation().z;
	if (pause == false) {
		switch (key)
		{
		case GLUT_KEY_F1:
			// turn head of robot to face forward
			SpecialKeyUp(key, x, y);
			break;
		case GLUT_KEY_F2:
			// turn head of robot to the right (clockwise)
			_robot->RotateHead(-45.f);
			break;
		case GLUT_KEY_F3:
			// turn head of robot to the left (counterclockwise)
			_robot->RotateHead(45.f);
			break;
		case GLUT_KEY_F4:
			// reutrns LookAt view to default
			_camera->MoveCamera(0, 1, gridScale);
			std::cout << robotz - 5 << std::endl;
			break;
		case GLUT_KEY_F5:
			// move LookAt to behind left shoulder
			_camera->MoveCamera(-2 * gridScale, 10, gridScale);
			break;
		case GLUT_KEY_F6:
			// move LookAt to behind right shoulder
			_camera->MoveCamera(2 * gridScale, 10, gridScale);
			break;
		case GLUT_KEY_F7:
			// move LookAt to in front of right shoulder
			_camera->MoveCamera(2 * gridScale, 10, -gridScale);
			break;
		case GLUT_KEY_F8:
			// move LookAt to in front of left shoulder
			_camera->MoveCamera(-2 * gridScale, 10, -gridScale);
			break;
		case GLUT_KEY_F9:
			// F5 but far away
			_camera->MoveCamera(-4 * gridScale, 20, 4 * gridScale);
			break;
		case GLUT_KEY_F10:
			// F6 but far away
			_camera->MoveCamera(4 * gridScale, 20, 4 * gridScale);
			break;
		case GLUT_KEY_F11:
			// F7 but far away
			_camera->MoveCamera(4 * gridScale, 20, -4 * gridScale);
			break;
		case GLUT_KEY_F12:
			// F8 but far away
			_camera->MoveCamera(-4 * gridScale, 20, -4 * gridScale);
			break;
		}
	}
}

void SpecialKeyUp(int key, int x, int y)
{
	_robot->ResetHead();
}

//
void MousePassive(int x, int y)
{

}

//
void PrintToScreen(const char * str, float x, float y, float c[])
{
	glDisable(GL_LIGHTING);
	glColor3f(c[0], c[1], c[3]);
	glRasterPos3f(x, y, 1000);
	int len = strlen(str);
	for (int i = 0; i < len; i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, *str++);
	glEnable(GL_LIGHTING);
}

Building::BuildingType ChooseBuildingType()
{
	int num = rand() % 3;
	num = 1;

	Building::BuildingType bt;
	switch (num)
	{
	case 0:
		bt = Building::Weak;
		break;
	case 1:
		bt = Building::Strong;
		break;
	case 2:
		bt = Building::Indestructable;
		break;
	default:
		bt = Building::Weak;
		break;
	}
	return bt;
}

//
void Idle(int)
{
	glutPostRedisplay();
	glutTimerFunc(1000 / FPS, Idle, 0);
}


