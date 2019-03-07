#include <App.h>
#include <Core.h>
#include <Robot.h>

void Setup();
void Display();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int) {}
void MousePassive(int x, int y);

Robot* robot;

int main(int argc, char** argv)
{
   App(argc, argv, argv[0], 600, 400);
   App::RegisterCallbackFuncs(Display, Mouse, NULL, MousePassive);
   App::Init(0.f);
   Setup();
   App::Loop();
   
   return 0;
}

void Setup()
{
   robot = new Robot();
   robot->SetScale(GLScale(100.f, 100.f, 100.f));
}

void Display()
{
   App::Display([]() { // Put Your code for displaying objects here.
	 if(robot) robot->Tick();
      });
}

void Mouse(int button, int state, int x, int y)
{
   switch (button)
   {
      case  GLUT_LEFT_BUTTON:
	 if (state == GLUT_DOWN)
	 {

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

}

