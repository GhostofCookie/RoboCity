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
	bool Destructible;
	void Draw();
	void Collapse(int tick);
	BuildingType Type;

	Building(float centerx, float centerz, float w, float h, float d, BuildingType bt);
	void Display(int tick);
	void Hit();
	void DrawWeak();
	void DrawStrong();
	void DrawIndestructable();
	float Left();
	float Right();
	float Bottom();
	float Top();
	float Back();
	float Front();
	bool IsDead();
	void CalculateNormal(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2);
};