#include "Building.h"

Building::Building(float centerx, float centerz, float w, float h, float d, BuildingType bt)
{
	cx = centerx;
	cz = centerz;
	cy = h / 2;

	R = 0.5f;
	G = 0.5f;
	B = 0.5f;

	Width = w;
	Height = h;
	Depth = d;
	_ogHeight = h;

	Destructable = true;
	switch (bt)
	{
	case Building::Weak:
		_health = 1;
		break;
	case Building::Strong:
		_health = 1;
		break;
	case Building::Indestructable:
		_health = 1;
		Destructable = false;
		break;
	default:
		break;
	}
}

void Building::Display(int tick)
{
	if (_isCollapsing)
		Collapse(tick);

	Draw();
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
			R = 0.898;
			G = 0.549;
			B = 0.541;
		}
		else if (_health == 2)
		{
			// orange
			R = 0.972;
			G = 0.462;
			B = 0.258;
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

float Building::Front()
{
	return cz - Depth / 2;
}

float Building::Back()
{
	return cz - Depth / 2;
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
	glVertex3f(Left(), Top(), Front());
	glVertex3f(Right(), Top(), Front());
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Left(), Bottom(), Front());

	// right side
	glVertex3f(Right(), Top(), Front());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Right(), Bottom(), Back());
	glVertex3f(Right(), Bottom(), Front());

	// Left side
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left(), Top(), Back());
	glVertex3f(Left(), Top(), Front());
	glVertex3f(Left(), Bottom(), Front());

	// Top
	glVertex3f(Left(), Top(), Back());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Right(), Top(), Front());
	glVertex3f(Left(), Top(), Front());

	// Back side
	glVertex3f(Right(), Bottom(), Back());
	glVertex3f(Right(), Top(), Back());
	glVertex3f(Left(), Top(), Back());
	glVertex3f(Left(), Bottom(), Back());

	glEnd();
}

void Building::Collapse(int tick)
{
	Height -= _ogHeight / (float)tick;

	if (Height <= 0)
	{
		Height = 0;
		_isCollapsing = false;
		_isDead = true;
	}

	cy = Height / 2;
}
