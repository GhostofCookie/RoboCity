#include <Core.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <string>



const GL_Colour GL_Colour::Black(0.f, 0.f, 0.f, 1.f);
const GL_Colour GL_Colour::White(1.f, 1.f, 1.f);
const GL_Colour GL_Colour::Grey(0.08f, 0.08f, 0.08f);
const GL_Colour GL_Colour::Red(1.f, 0.f, 0.f);
const GL_Colour GL_Colour::Green(0.f, 1.f, 0.f);
const GL_Colour GL_Colour::Blue(0.f, 0.f, 1.f);
const GL_Colour GL_Colour::Cyan(0.f, 0.8f, 1.f);
const GL_Colour GL_Colour::Emerald(0.f, 1.f, 0.5f);
const GL_Colour GL_Colour::Purple(0.35f, 0.f, 1.f);

GL_Colour::GL_Colour(float r, float g, float b)
	: R{ std::fmod(r, 2.f) }, G{ std::fmod(g, 2.f) }, B{ std::fmod(b, 2.f) }, A{ 1.f } {}

GL_Colour::GL_Colour(float r, float g, float b, float a)
	: R{ std::fmod(r, 2.f) }, G{ std::fmod(g, 2.f) }, B{ std::fmod(b, 2.f) }, A{ std::fmod(a, 2.f) } {}

GL_Colour::GL_Colour(const GL_Colour& c)
	: R{ c.R }, G{ c.G }, B{ c.B }, A{ c.A } {}

void GL_Colour::UseColour()
{
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glColor4f(R, G, B, A);
}

const GL_Font GL_Font::HELVETICA(0);
const GL_Font GL_Font::TIMES(1);

GL_Font::GL_Font(int i)
	: index{ i }
{}

void* GL_Font::GetFont()
{
	switch (index)
	{
	case 0:
		return GLUT_BITMAP_HELVETICA_12;
	case 1:
		return GLUT_BITMAP_TIMES_ROMAN_10;
	}
	return GLUT_BITMAP_HELVETICA_12;
}

GLVector::GLVector() {}

GLVector::GLVector(float x, float y)
	: X{ x }, Y{ y }, Z{ 0 } {}

GLVector::GLVector(float x, float y, float z)
	: X{ x }, Y{ y }, Z{ z } {}

GLRotator::GLRotator() {}

GLRotator::GLRotator(GLRotator * rot)
	: X{ rot->X }, Y{ rot->Y }, Z{ rot->Z }, Angle{ rot->Angle } {}

GLRotator::GLRotator(float d)
	: X{ d }, Y{ d }, Z{ d } {}

GLRotator::GLRotator(float x, float y, float z)
	: X{ x > 1 ? x / x : x }, Y{ y > 1 ? y / y : y }, Z{ z > 1 ? z / z : z }
	, Angle{ std::fmod(std::abs(x) + std::abs(y) + std::abs(z), 360.f) } {}

GLRotator::GLRotator(float x1, float y1, float z1, float x2, float y2, float z2)
	: X{ (x2 - x1) }, Y{ (y2 - y1) }, Z{ (z2 - z1) } {}

GLRotator::GLRotator(GLVector v1, GLVector v2)
	: X{ v2.X - v1.X }, Y{ v2.Y - v1.Y }, Z{ v2.Z - v1.Z } {}

GLScale::GLScale() {}

GLScale::GLScale(GLScale* scale)
	: X{ scale->X }, Y{ scale->Y }, Z{ scale->Z } {}

GLScale::GLScale(float x, float y, float z)
	: X{ x }, Y{ y }, Z{ z } {}

GLTransform::GLTransform() {}

GLTransform::GLTransform(GLVector l, GLRotator r, GLScale s)
	: _location{ l }, _rotation{ r }, _scale{ s } {}

GLVector GLTransform::GetLocation() const
{
	return _location;
}

GLRotator GLTransform::GetRotation() const
{
	return _rotation;
}

GLScale GLTransform::GetScale() const
{
	return _scale;
}

void GLTransform::SetLocation(GLVector loc)
{
	_location = loc;
}

void GLTransform::SetRotation(GLRotator rot)
{
	_rotation = rot;
}

void GLTransform::SetScale(GLScale scl)
{
	_scale = scl;
}


