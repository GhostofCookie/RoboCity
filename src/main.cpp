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

#include <string>
#include <time.h>

#include <Camera.h>
#include <Robot.h>

#include <StreetGenerator.h>

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
void MousePassive(int x, int y);
void PrintToScreen(const char * str, float x, float y, float[]);

// Variables
int _prevTime = time(NULL), _curTime, _frameCount = 0;
int _tick = 0;

GLfloat _width = 780, _height = 500;

Robot* _robot = new Robot();
Camera* _camera = new Camera(_robot);

float gridScale = 5.0;
StreetGenerator* _city = new StreetGenerator(20, gridScale, gridScale);


// MAIN
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (_width, _height); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("RoboCity");
   init();
   Setup();
   glutDisplayFunc(Display);
   glutReshapeFunc(reshape);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(Keyboard);
   glutSpecialFunc(SpecialKey);
   glutTimerFunc(1000/FPS, Idle, 0);
   //glutIdleFunc(Display); // call display while idle
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   
   glutMainLoop();
   return 0;
}

//
void init (void) 
{
   glClearColor(0.0, 0.0, 0.0, 1.0);
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

   GLfloat lpos[4] = {100, 100, 100, 100};
   GLfloat white[4] = {1.f, 1.f, 1.f, 1.f};
   glLightfv(GL_LIGHT0, GL_DIFFUSE, white);
   glLightfv(GL_LIGHT0, GL_SPECULAR, white);
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
   glEnable(GL_COLOR_MATERIAL);	

   glShadeModel(GL_FLAT);

   // City Generator info
   //_city->CreateStreets_Complex(2);
   _city->CreateStreets_Simple();
   //_city->PrintGridStates();

   Setup();
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
   //glOrtho(-_width/10, _width/10, -_height/10, _height/10, -1000, 1000);
   gluPerspective(45.f, (GLfloat) w / (GLfloat) h, 0.01, 1000);
   
   _width = (GLfloat) w;
   _height = (GLfloat) h;   

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
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

   glColor3f(1,1,1);
   glutWireCube(1.0);

   _robot->Render();

   //std::cout<<_robot->GetLocation().x<<std::endl;
   //std::cout<<_robot->GetLocation().z<<std::endl;
   //std::cout<<_robot->GetRotation().y<<std::endl;

   // City Grid
   glPushMatrix();

   int cSize = _city->CitySize();
   glTranslatef(-cSize/2-(gridScale/2), -0.5, -cSize/2-(gridScale/2));
   // Move the object back from the screen.
   
   glBegin(GL_QUADS);
   // Creates the grid of quads using the city information
   for(int y = 0; y < cSize; y++)
   {
      for(int x = 0; x < cSize; x++)
      {
	 Block b = _city->grid[y][x];
	 // Decide on color - can be cleaned up
	 if(b.isIntersection)
	    glColor3f(0.6, 0.6, 0.6);
	 else if(b.isStreet)
	    glColor3f(0.5, 0.5, 0.5);
	 else
	    glColor3f(0.1, 0.1, 0.1);

	 glVertex3f(b.startX, 0.0, b.endY);
	 glVertex3f(b.endX, 0.0, b.endY);
	 glVertex3f(b.endX, 0.0, b.startY);
	 glVertex3f(b.startX, 0.0, b.startY);
      }
   }
   // All polygons have been drawn.
   glEnd();
   glPopMatrix();

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
	 if (state == GLUT_UP)
	    //if (overObject)
	 {
	 }
	 break;
   }
}

//
void Keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'q':
	 //if at intersection, turn left
	 _robot->Rotate(90.f);
	 _camera->RotateCamera(0.f, -90.f, 0.f);
	 //_robot->MoveForward(10.f);
	 break;
      case 'a':
	 //if at intersection, turn right
	 _robot->Rotate(-90.f);
	 _camera->RotateCamera(0.f, 90.f, 0.f);
	 break;
      case 'p':
	 //pause the game
	 break;
      case 'r':
	 //return robot to original position
	 break;
      case 'z':
	 // move robot forward
	 _robot->MoveForward(gridScale);
	 break;
   }
	 
}

//
void SpecialKey(int key, int x, int y)
{

   float robotx = _robot->GetLocation().x;
   float robotz = _robot->GetLocation().z;
   
   switch (key)
   {
      case GLUT_KEY_F1:
	 // turn head of robot to face forward
	 std::cout<<"F1"<<std::endl;
	 break;
      case GLUT_KEY_F2:
	 // turn head of robot to the right (clockwise)
	 _robot->RotateHead(-45.f);
	 std::cout<<"F2"<<std::endl;
	 break;
      case GLUT_KEY_F3:
	 // turn head of robot to the left (counterclockwise)
	 _robot->RotateHead(45.f);
	 std::cout<<"F3"<<std::endl;
	 break;
      case GLUT_KEY_F4:
	 // reutrns LookAt view to default
	 _camera->MoveCamera(robotx, 0, robotz+5);
	 std::cout<<"F4"<<std::endl;
	 break;
      case GLUT_KEY_F5:
	 // move LookAt to behind left shoulder
	 _camera->MoveCamera(robotx-10, 10, robotz+5);
	 std::cout<<"F5"<<std::endl;
	 break;
      case GLUT_KEY_F6:
	 // move LookAt to behind right shoulder
	 _camera->MoveCamera(robotx+10, 10, robotz+5);
	 std::cout<<"F6"<<std::endl;
	 break;
      case GLUT_KEY_F7:
	 // move LookAt to in front of right shoulder
	 _camera->MoveCamera(robotx+10, 10, robotz-5);
	 std::cout<<"F7"<<std::endl;
	 break;
      case GLUT_KEY_F8:
	 // move LookAt to in front of left shoulder
	 _camera->MoveCamera(robotx-10, 10, robotz-5);
	 std::cout<<"F8"<<std::endl;
	 break;
      case GLUT_KEY_F9:
	 // F5 but far away
	 _camera->MoveCamera(robotx-20, 20, robotz+20);
	 std::cout<<"F9"<<std::endl;
	 break;
      case GLUT_KEY_F10:
	 // F6 but far away
	 _camera->MoveCamera(robotx+20, 20, robotz+20);
	 std::cout<<"F10"<<std::endl;
	 break;
      case GLUT_KEY_F11:
	 // F7 but far away
	 _camera->MoveCamera(robotx+20, 20, robotz-20);
	 std::cout<<"F11"<<std::endl;
	 break;
      case GLUT_KEY_F12:
	 // F8 but far away
	 _camera->MoveCamera(robotx-20, 20, robotz-20);
	 std::cout<<"F12"<<std::endl;
	 break;
	 
   }

		
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

//
void Idle(int)
{
   glutPostRedisplay();
   glutTimerFunc(1000 / FPS, Idle, 0);
}


