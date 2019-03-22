#include <App.h>
#include <Core.h>
#include <Robot.h>
//#include <Building.h>

#include <string>

void Setup();
void Display();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int);
void MousePassive(int x, int y);

Robot* robot;

int main(int argc, char** argv)
{
   App(argc, argv, argv[0], 980, 720);
   App::RegisterCallbackFuncs(Display, Mouse, Keyboard, MousePassive);
   App::Init(0.f, GL_SMOOTH);
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
	 std::string str = std::to_string(robot->GetLocation().X);
	 App::PrintToScreen(str.c_str(), 0, 0, GL_Colour::Emerald);
	 str = std::to_string(robot->GetLocation().Z);
	 App::PrintToScreen(str.c_str(), 0, -10, GL_Colour::Emerald);
	 str = std::to_string(robot->GetRotation().Y);
	 App::PrintToScreen(str.c_str(), 0, -15, GL_Colour::Emerald);
	   
	 //robot->Rotate(GLRotator(0.f, 5.f, 0.f));
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

void Keyboard(unsigned char key, int x, int y)
{
   switch(key)
   {
      case 'w': case 'W':
	 robot->MoveForward(10.f);
	 break;
	 
      case 's': case 'S':
	 robot->MoveForward(-10.f);
	 break;
      case 'd': case 'D':
	 robot->Rotate(90.f, GLRotator(0.f, 90.f, 0.f));
	 break;
   };
}

