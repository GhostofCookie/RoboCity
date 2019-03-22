#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <fbxsdk.h>

#include <stdlib.h>
#include <stdio.h>    
#include <cstring>

#include <string>

#include <Core.h>
#include <Robot.h>
#include <Object.h>
//#include <Building.h>

GLfloat _width = 780, _height = 500;
std::vector<Object*> _object_pool;

void Setup();
void Display();
void init(void);
void reshape(int w, int h);
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int);
void MousePassive(int x, int y);
void AddObjectToPool(Object* obj);
void RemoveObject(Object * obj);
void PrintToScreen(const char * str, float x, float y, GL_Font font, GL_Colour colour);

Robot* robot;

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (_width, _height); 
   glutInitWindowPosition (100, 100);
   glutCreateWindow ("RoboCity");
init ();
Setup();
   glutDisplayFunc(Display);
   glutReshapeFunc(reshape);
   glutMouseFunc(Mouse);
   glutKeyboardFunc(Keyboard);
   //glutIdleFunc(Display); // call display while idle
   //Setup();
   glEnable(GL_DEPTH_TEST);
   glEnable(GL_CULL_FACE);
   glCullFace(GL_BACK);
   glutMainLoop();
   return 0;
}

void Setup()
{
   robot = new Robot(0.f, 0.f, -30.f);
   robot->SetScale(GLScale(2.f));
}

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
   glLightfv(GL_LIGHT0, GL_DIFFUSE, GL_Colour::White.RGBA);
   glLightfv(GL_LIGHT0, GL_SPECULAR, GL_Colour::White.RGBA);
   glLightfv(GL_LIGHT0, GL_POSITION, lpos);
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT, GL_Colour::Grey.RGBA);
   glEnable(GL_COLOR_MATERIAL);
	

   glShadeModel(GL_FLAT);

   Setup();
}

void reshape(int w, int h)
{
   if (!h) h = 1;
   glViewport(0, 0, _width, _height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-_width/10, _width/10, -_height/10, _height/10, -1000, 1000);
   
   _width = (GLfloat) w;
   _height = (GLfloat) h;   

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void Display()
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   for (auto it = _object_pool.begin(); it != _object_pool.end(); ++it)
      if(*it) (*it)->Tick();

// print stuff();
   glColor3f(1,1,1);
   glutWireCube(1.0);

   glutSwapBuffers();
}

void Mouse(int button, int state, int x, int y)
{
   switch (button)
   {
      case  GLUT_LEFT_BUTTON:
	 if (state == GLUT_DOWN)
	 {
		 robot->RotateHead(30.f);
	 }
	 else
		 robot->RotateHead(-30.f);
	 break;
      case GLUT_RIGHT_BUTTON:
	 if (state == GLUT_DOWN)
	 {
		 robot->RotateHead(-30.f);
	 }
	 else
		 robot->RotateHead(30.f);
	 break;
	 }
}

void Keyboard(unsigned char key, int x, int y)
{
   switch (key)
   {
      case 'z':
	 robot->MoveForward(10.f);
	 break;
      case 'q':
	 //if at intersection
	 robot->RotateBody(90.f);
	 //robot->MoveForward(10.f);
	 break;
      case 'a':
	 //if at intersection
	 robot->RotateBody(-90.f);
	 //robot->MoveForward(10.f);
	 break;
      case 'p':
	 //pause the game
	 break;
      case 'r':
	 //return robot to original position
	 break;
   }
	 
}


void MousePassive(int x, int y)
{
   
}

void AddObjectToPool(Object* obj)
{
   _object_pool.push_back(obj);
}

void RemoveObject(Object * obj)
{
   if(obj)
      for(auto it = _object_pool.begin(); it != _object_pool.end(); ++it)
	 if (*it == obj)
	 {
	    _object_pool.erase(it);
	    break;
	 }
}

void PrintToScreen(const char * str, float x, float y, GL_Font font, GL_Colour colour)
{
   glDisable(GL_LIGHTING);
   colour.UseColour();
   glRasterPos3f(x, y, 1000);
   int len = strlen(str);
   for (int i = 0; i < len; i++)
      glutBitmapCharacter(font.GetFont(), *str++);
   glEnable(GL_LIGHTING);
}


