#include "Building.h"

Building::Building(float centerx, float centerz, float w, float h, float d, BuildingType bt)
{
	cx = centerx;
	cz = centerz;
	cy = h / 2 - 0.5;


	Width = w;
	Height = h;
	Depth = d;
	_ogHeight = h;

	Destructable = true;
	switch (bt)
	{
	case Building::Weak:
		_health = 1;
		R = 0.5f;
		G = 0.5f;
		B = 0.5f;
		break;
	case Building::Strong:
		_health = 3;
		R = 0.203f;
		G = 0.285f;
		B = 0.367f;
		break;
	case Building::Indestructable:
		_health = 1;
		Destructable = false;
		R = 0.3f;
		G = 0.3f;
		B = 0.3f;
		break;
	default:
		break;
	}
}

void Building::Display(int tick)
{
	if (_isCollapsing && tick > 1)
		Collapse(tick);

	glPushMatrix();
	glTranslatef(_shakex, 0.0f, _shakez);
	Draw();
	glPopMatrix();
}

void Building::Hit()
{
	if (Destructable)
	{
		_health--;
		if (_health == 0)
		{
			// dead
			_isCollapsing = true;
		}
		else if (_health == 1)
		{
			// red
			R = 0.902f;
			G = 0.296f;
			B = 0.234f;
		}
		else if (_health == 2)
		{
			// orange
			R = 0.972f;
			G = 0.462f;
			B = 0.258f;
		}
	}
}

float Building::Left()
{
	return cx - Width / 2;
}

float Building::Right()
{
	return cx + Width / 2;
}

float Building::Bottom()
{
	return cy - Height / 2;
}

float Building::Top()
{
	return cy + Height / 2;
}

float Building::Back()
{
	return cz - Depth / 2;
}

float Building::Front()
{
	return cz + Depth / 2;
}

bool Building::IsDead()
{
	return _isDead;
}


// private functions
void Building::Draw()
{
	glBegin(GL_QUADS);

	glColor3f(R, G, B);

	// front side
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(Left(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Right(), Top(), Front());
	glVertex3f(Left(), Top(), Front());

	// right side
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Back());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Right(), Top(), Front());

	// Left side
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left(), Bottom(), Front());
	glVertex3f(Left(), Top(), Front());
	glVertex3f(Left(), Top(), Back());

	// Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(Left(), Top(), Front());
	glVertex3f(Right(), Top(), Front());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Left(), Top(), Back());

	// Back side
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left(), Top(), Back());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Right(), Bottom(), Back());

	glEnd();
}

void Building::Collapse(int tick)
{
	// TODO add shaking
	Height -= _ogHeight / (float)tick;

	_shakex += (_shakexValue / tick);
	_shakez += (_shakezValue / tick);

	float shakeMax = 0.1;
	if (_shakex > shakeMax)
	{
		_shakex = shakeMax;
		// begin shaking left
		_shakexValue *= -1;
	}
	else if (_shakex < -shakeMax)
	{
		_shakex = -shakeMax;
		// begin shaking right
		_shakexValue *= -1;
	}
	if (_shakez > shakeMax)
	{
		_shakez = shakeMax;
		// begin shaking forward
		_shakezValue *= -1;
	}
	else if (_shakez < -shakeMax)
	{
		_shakez = -shakeMax;
		// begin shaking backward
		_shakezValue *= -1;
	}

	if (Height <= 0.05)
	{
		Height = 0.05;
		_shakex = 0.0;
		_shakez = 0.0;
		_isCollapsing = false;
		_isDead = true;
	}

	cy = Height / 2 - 0.5;
}
