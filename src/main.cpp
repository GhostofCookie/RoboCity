#include <App.h>
#include <Core.h>
#include <Robot.h>
//#include <Building.h>

#include <string>

void Setup();
void Display();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int) {}
void MousePassive(int x, int y);

Robot* robot;

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
	 /*  std::string str = std::to_string(b->GetHealth());
	   App::PrintToScreen(str.c_str(), 0, 0, GL_Colour::Emerald);*/
      });
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

void MousePassive(int x, int y)
{
   
}

