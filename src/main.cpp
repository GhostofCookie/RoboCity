#include <App.h>
#include <Core.h>

void Setup();
void Display();
void Mouse(int button, int state, int x, int y);
void Keyboard(unsigned char, int, int) {}
void MousePassive(int x, int y);

int main(int argc, char** argv)
{
	App(argc, argv, argv[0], 1280, 720);
	App::RegisterCallbackFuncs(Display, Mouse, NULL, MousePassive);
	App::Init(0.f);

	Setup();

	App::Loop();
	return 0;
}

void Setup()
{

}

void Display(void)
{
	App::Display([]() {

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

