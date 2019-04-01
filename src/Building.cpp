#include "Building.h"

Building::Building(float centerx, float centerz, float w, float h, float d, BuildingType bt)
{
	cx = centerx;
	cz = centerz;
	// -0.5 for shifting the world down
	cy = h / 2 - 0.5;


	Width = w;
	Height = h;
	Depth = d;
	_ogHeight = h;

	Destructible = true;
	Type = bt;
	switch (Type)
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
		Destructible = false;
		R = 0.101f;
		G = 0.142f;
		B = 0.183f;
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
	if (Destructible && !_isDead)
	{
		_health--;
		if (_health == 0)
		{
			// dead
			R = 1.0f;
			G = .2f;
			B = .2f;
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
	glColor3f(R, G, B);

	switch (Type)
	{
	case Building::Weak:
		DrawWeak();
		break;
	case Building::Strong:
		DrawStrong();
		break;
	case Building::Indestructable:
		DrawIndestructable();
		break;
	default:
		break;
	}

}

//Pyramid
void Building::DrawWeak()
{
	glBegin(GL_TRIANGLES);

	// back
	CalculateNormal(cx, Bottom(), cz, Right(), Bottom(), Back(), Left(), Top(), Back());
	glVertex3f(cx, Top(), cz); // peak
	glVertex3f(Right(), Bottom(), Back());
	glVertex3f(Left(), Bottom(), Back());
	
	// left
	CalculateNormal(cx, Top(), cz, Left(), Bottom(), Back(), Left(), Bottom(), Front());
	glVertex3f(cx, Top(), cz); // peak
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left(), Bottom(), Front());
	
	// right
	CalculateNormal(cx, Top(), cz, Right(), Bottom(), Front(), Right(), Bottom(), Back());
	glVertex3f(cx, Top(), cz); // peak
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Back());

	// front
	CalculateNormal(cx, Top(), cz, Right(), Bottom(), Front(), Left(), Bottom(), Front());
	glVertex3f(cx, Top(), cz);
	glVertex3f(Left(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Front());
	glEnd();
}

// Rectangles
void Building::DrawStrong()
{
	glBegin(GL_QUADS);

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

void Building::DrawIndestructable()
{
	float offset = 0.5;
	float heightOff = 2.0;
	glBegin(GL_QUADS);
	// front side
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(Left(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Right() - offset, Top()/heightOff, Front() - offset);
	glVertex3f(Left() + offset, Top()/heightOff, Front() - offset);

	// right side
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(Right(), Bottom(), Front());
	glVertex3f(Right(), Bottom(), Back());
	glVertex3f(Right() - offset, Top()/heightOff, Back() + offset);
	glVertex3f(Right() - offset, Top()/heightOff, Front() - offset);

	// Left side
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left(), Bottom(), Front());
	glVertex3f(Left() + offset, Top()/heightOff, Front() - offset);
	glVertex3f(Left() + offset, Top()/heightOff, Back() + offset);

	// Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(Left() + offset, Top()/heightOff, Front() - offset);
	glVertex3f(Right() - offset, Top()/heightOff, Front() - offset);
	glVertex3f(Right() - offset, Top()/heightOff, Back() + offset);
	glVertex3f(Left() + offset, Top()/heightOff, Back() + offset);

	// Back side
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(Left(), Bottom(), Back());
	glVertex3f(Left() + offset, Top()/heightOff, Back() + offset);
	glVertex3f(Right() - offset, Top()/heightOff, Back() + offset);
	glVertex3f(Right(), Bottom(), Back());

	// second block
	// front side
	glNormal3f(0.0f, 0.0f, 1.0f);
	glVertex3f(Left() + offset, Bottom(), Front() - offset);
	glVertex3f(Right() - offset, Bottom(), Front() - offset);
	glVertex3f(Right(), Top() / 3, Front());
	glVertex3f(Left(), Top() / 3, Front());

	// right side
	glNormal3f(1.0f, 0.0f, 0.0f);
	glVertex3f(Right() - offset, Bottom(), Front() - offset);
	glVertex3f(Right() - offset, Bottom(), Back() + offset);
	glVertex3f(Right(), Top() / 3, Back());
	glVertex3f(Right(), Top() / 3, Front());

	// Left side
	glNormal3f(-1.0f, 0.0f, 0.0f);
	glVertex3f(Left() + offset, Bottom(), Back() + offset);
	glVertex3f(Left() + offset, Bottom(), Front() - offset);
	glVertex3f(Left(), Top() / 3, Front());
	glVertex3f(Left(), Top() / 3, Back());

	// Top
	glNormal3f(0.0f, 1.0f, 0.0f);
	glVertex3f(Left(), Top() / 3, Front());
	glVertex3f(Right(), Top() / 3, Front());
	glVertex3f(Right(), Top() / 3, Back());
	glVertex3f(Left(), Top() / 3, Back());

	// Back side
	glNormal3f(0.0f, 0.0f, -1.0f);
	glVertex3f(Left() + offset, Bottom(), Back() + offset);
	glVertex3f(Left(), Top() / 3, Back());
	glVertex3f(Right(), Top() / 3, Back());
	glVertex3f(Right() - offset, Bottom(), Back() + offset);

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


void Building::CalculateNormal(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
{
	float nx = (y1 - y0)*(z2 - z0) - (z1 - z0)*(y2 - y0);
	float ny = -((x1 - x0)*(z2 - z0) - (z1 - z0)*(x2 - x0));
	float nz = (x1 - x0)*(y2 - y0) - (y1 - y0)*(x2 - x0);
	glNormal3f(nx, ny, nz);
}
