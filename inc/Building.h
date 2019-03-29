#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Building
{
private:
	unsigned _health;
	bool _isCollapsing = false;
	bool _isDead = false;
	float _ogHeight;
	float _shakeBound = 0.5;
	float _shakex = 0.0;
	float _shakez = 0.0;
	float _shakexValue = 3.0;
	float _shakezValue = 3.3;

public:
	enum BuildingType {
		Weak = 0,
		Strong,
		Indestructable
	};

	float cx;
	float cy;
	float cz;
	float Width;
	float Height;
	float Depth;
	float R;
	float G;
	float B;
	bool Destructable;
	void Draw();
	void Collapse(int tick);

	Building(float centerx, float centerz, float w, float h, float d, BuildingType bt);
	void Display(int tick);
	void Hit();
	float Left();
	float Right();
	float Bottom();
	float Top();
	float Back();
	float Front();
	bool IsDead();
};