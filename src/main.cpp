#include <App.h>
#include <Core.h>
#include <Robot.h>

#include <string>

void Setup();
void Display();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int) {}
void MousePassive(int x, int y);

Robot* robot;
float ez;

int main(int argc, char** argv)
{
   App(argc, argv, argv[0], 980, 720);
   App::RegisterCallbackFuncs(Display, Mouse, NULL, MousePassive);
   App::Init(0.f);
   Setup();
   App::Loop();
   
   return 0;
}

void Setup()
{
   robot = new Robot(0.f, 0.f, -30.f);
   robot->SetScale(GLScale(0.5f));
}

void Display()
{
   App::Display([]() { // Put Your code for displaying objects here.
      });
}

void Mouse(int button, int state, int x, int y)
{
   switch (button)
   {
      case  GLUT_LEFT_BUTTON:
	 if (state == GLUT_DOWN)
	 {
	    ez += 1;
	 }
	 break;
      case GLUT_RIGHT_BUTTON:
	 if (state == GLUT_DOWN)
	 {

	 }
	 break;
   }
}

void MousePassive(int x, int y)
{
   robot->SetLocation(GLVector(((float)x * 2)-(App::Width()/2), 0, -30.f));
}

