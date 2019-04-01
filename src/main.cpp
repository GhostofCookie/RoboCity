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
void Display();
void Idle(int);
void init(void);
void Reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int);
void SpecialKey(int key, int x, int y);
void SpecialKeyUp(int key, int x, int y);
void ProcessHits(GLint hits, GLuint buffer[]);
void DrawObjects(GLenum mode);
void PrintToScreen(const char * str, float x, float y, float[]);
void CalculateMoveGrid();
Building::BuildingType ChooseBuildingType();

// Variables
int cSize = 0;
int gridX, gridZ;
int _tick = 0;
int _prevTime = time(NULL), _curTime, _frameCount = 0;
bool canFire = true;
bool pause = false;
bool selectReady = true;
float worldOffset;
float gridScale = 10.0;
GLfloat _width = 1000, _height = 700;

std::vector<std::vector<Building>> buildings;
Robot* _robot = new Robot();
Camera* _camera = new Camera(_robot);
StreetGenerator* _city = new StreetGenerator(2, gridScale, gridScale);

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize(_width, _height);
   glutInitWindowPosition(100, 100);
   glutCreateWindow("RoboCity");
   init();
   glutDisplayFunc(Display);
   glutReshapeFunc(Reshape);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(Keyboard);
   glutSpecialFunc(SpecialKey);
   glutSpecialUpFunc(SpecialKeyUp);
   glutTimerFunc(1000 / FPS, Idle, 0);
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glutMainLoop();
   return 0;
}

void init(void)
{
   // Set random seed
   srand(time(0));

   glClearColor(0.468, 0.839, 0.976, 1.0);
   glClearDepth(1.0);

   // Enable depth testing.
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_NORMALIZE);
   glDepthFunc(GL_LESS);

   // Enable transparency.
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Enable Lighting
   glEnable(GL_LIGHTING);
   glEnable(GL_LIGHT0);

   glEnable(GL_COLOR_MATERIAL);

   glShadeModel(GL_SMOOTH);

   // Initialize camera placement.
   _camera->MoveCamera(0, 2, gridScale);

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
	       buildings[z].push_back(
		  Building(
		     b.buildings[0].x + worldOffset, b.buildings[0].y + worldOffset,
		     gridScale/3, gridScale/heightMult, gridScale/3, ChooseBuildingType()));
	    }
	    if (b.buildings[1].canBuild)
	    {
	       heightMult = (rand() % 4) + 1;
	       buildings[z].push_back(
		  Building(
		     b.buildings[1].x + worldOffset, b.buildings[1].y + worldOffset,
		     gridScale/3, gridScale/heightMult, gridScale/3, ChooseBuildingType()));
	    }
	    if (b.buildings[2].canBuild)
	    {
	       heightMult = (rand() % 4) + 1;
	       buildings[z].push_back(
		  Building(
		     b.buildings[2].x + worldOffset, b.buildings[2].y + worldOffset,
		     gridScale/3, gridScale/heightMult, gridScale/3, ChooseBuildingType()));
	    }
	    if (b.buildings[3].canBuild)
	    {
	       heightMult = (rand() % 4) + 1;
	       buildings[z].push_back(
		  Building(
		     b.buildings[3].x + worldOffset, b.buildings[3].y + worldOffset,
		     gridScale/3, gridScale/heightMult, gridScale/3, ChooseBuildingType()));
	    }
	 }
      }
   }
}

void Reshape(int w, int h)
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

void DrawObjects(GLenum mode)
{
   int i = 1;
   for (int z = 0; z < buildings.size(); z++)
      for (int x = 0; x < buildings[z].size(); x++)
      {
	 if (mode == GL_SELECT)
	 {
	    // calculate the 1D index from a 2D index
	    glLoadName(z + (x * buildings.size()));
	 }
	 buildings[z][x].Display(_tick);
      }
}

void Display()
{
   GLfloat mat_spec[4] = { 0.1f, 0.1f, 0.1f, 1.f }, shine[] = {128.f};
   
   glMaterialfv(GL_FRONT, GL_SPECULAR, mat_spec);
   glMaterialfv(GL_FRONT, GL_SHININESS, shine);

   float robotx = _robot->GetLocation().x;
   float robotz = _robot->GetLocation().z;

   glLoadIdentity();
   _camera->Display(_tick);

   _robot->Render(_tick);

   // City Grid
   glPushMatrix();

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

   DrawObjects(GL_RENDER);
   glPopMatrix();

   glFlush();
   glutSwapBuffers();
   
   GLfloat lpos[4] = { 80.f, 100, 100.f, 0.f };
   GLfloat white[4] = { 1.5f, 1.5f, 1.5f, 1.f };
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
   glLightfv(GL_LIGHT0, GL_SPECULAR, white);
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
   
   // Calculate frames per second
   _frameCount++;
   _curTime = time(NULL);
   if (_curTime - _prevTime > 0)
   {
      _tick = _frameCount / (_curTime - _prevTime);
      _frameCount = 0;
      _prevTime = _curTime;
   }
}

void Mouse(int button, int state, int x, int y)
{
   if(canFire)
   {
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
		  gluPickMatrix((GLdouble)x, (GLdouble)(viewport[3] - y), 1.f, 1.f, viewport);
		  gluPerspective(45.f, _width / _height, 0.01, 1000);
		  DrawObjects(GL_SELECT);
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
   {
      names = *ptr;
      ptr += 3;
      for (j = 0; j < names; j++)
      {
	 int x = *ptr % buildings.size();
	 int y = *ptr / buildings.size();
	 Building b = buildings[x][y];

	 // Find the closest hit object
	 // Get Vector length between robot and current building.
	 diffLength = std::sqrt(
	    std::pow(b.cx - _robot->GetLocation().x, 2) + // (b.x)^2 - (r.x)^2
	    std::pow(b.cz - _robot->GetLocation().z, 2)); // (b.z)^2 - (r.z)^2
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

   // Damage the building
   if (isHit) buildings[idxX][idxY].Hit();
   selectReady = true;
}

void Keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'q':
	 //if at intersection, turn left
	 if (!pause && _robot->IsMoving() == false)
	 {
	    CalculateMoveGrid();
	    if (_city->grid[gridX][gridZ].isIntersection) {
	       _robot->Rotate(90.f);
	       _camera->RotateCamera(0.f, -90.f, 0.f);
	    }
	 }
	 break;
      case 'a':
	 if (!pause && _robot->IsMoving() == false)
	 {
	    CalculateMoveGrid();
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
	 if (!pause && !_robot->IsMoving())
	    _robot->SetLocation(0.0, 0.0, 0.0);
	 break;
      case 'z':
	 if (!_robot->IsMoving() && !pause)
	 {
	    CalculateMoveGrid();
	    float nextX = gridX + _robot->ForwardVector().x;
	    float nextZ = gridZ + _robot->ForwardVector().z;

	    // move robot forward
	    if ((nextX >= 0 && nextX < cSize) && (nextZ >= 0 && nextZ < cSize))
	    {
	       if (_city->grid[nextX][nextZ].isStreet)
	       {
		  _robot->MoveForward(gridScale);
	       }
	    }
	 }
	 break;
   }
}

void CalculateMoveGrid()
{
   gridX = (_robot->GetLocation().x - (-cSize / 2 * gridScale)) / gridScale;
   gridZ = (_robot->GetLocation().z - (-cSize / 2 * gridScale)) / gridScale;
}

void SpecialKey(int key, int x, int y)
{

   float robotx = _robot->GetLocation().x;
   float robotz = _robot->GetLocation().z;
   if (pause == false) {
      canFire = false;
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
	    _camera->MoveCamera(0, 2, gridScale);
	    canFire = true;
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

// This is what sets the fps to a hard value of the defined FPS
void Idle(int)
{
   if(!pause) glutPostRedisplay();
   glutTimerFunc(1000 / FPS, Idle, 0);
}


