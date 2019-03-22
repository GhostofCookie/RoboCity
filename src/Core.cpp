#include <Core.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <string>

/////////////////////////////////////////////////////////////////////////
// Colours
/////////////////////////////////////////////////////////////////////////
const GL_Colour GL_Colour::Black(0.f, 0.f, 0.f, 1.f);
const GL_Colour GL_Colour::White(1.f, 1.f, 1.f);
const GL_Colour GL_Colour::Grey(0.5f, 0.5f, 0.5f);
const GL_Colour GL_Colour::Red(1.f, 0.f, 0.f);
const GL_Colour GL_Colour::Green(0.f, 1.f, 0.f);
const GL_Colour GL_Colour::Blue(0.f, 0.f, 1.f);
const GL_Colour GL_Colour::Cyan(0.f, 0.8f, 1.f);
const GL_Colour GL_Colour::Emerald(0.f, 1.f, 0.5f);
const GL_Colour GL_Colour::Purple(0.35f, 0.f, 1.f);
const GL_Colour GL_Colour::Yellow(1.f, 1.f, 0.f);
const GL_Colour GL_Colour::Orange(1.f, 0.54f, 0.f);

GL_Colour::GL_Colour(float r, float g, float b)
   : R{ std::fmod(r, 2.f) }, G{ std::fmod(g, 2.f) }, B{ std::fmod(b, 2.f) }, A{ 1.f }, RGBA{r, g, b, 1.f} {}

GL_Colour::GL_Colour(float r, float g, float b, float a)
   : R{ std::fmod(r, 2.f) }, G{ std::fmod(g, 2.f) }, B{ std::fmod(b, 2.f) }, A{ std::fmod(a, 2.f) }, RGBA{r, g, b, a} {}

GL_Colour::GL_Colour(const GL_Colour& c)
   : R{ c.R }, G{ c.G }, B{ c.B }, A{ c.A } {}

void GL_Colour::UseColour()
{
   glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
   glColor4f(R, G, B, A);
}

/////////////////////////////////////////////////////////////////////////
// Fonts
/////////////////////////////////////////////////////////////////////////
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

/////////////////////////////////////////////////////////////////////////
// Vectors
/////////////////////////////////////////////////////////////////////////
GLVector::GLVector() {}

GLVector::GLVector(GLVector * v)
   : X{ v->X }, Y{ v->Y }, Z{ v->Z } {}

GLVector::GLVector(float x, float y)
   : X{ x }, Y{ y }, Z{ 0 } {}

GLVector::GLVector(float x, float y, float z)
   : X{ x }, Y{ y }, Z{ z } {}

const GLVector& GLVector::operator+=(const GLVector& v)
{
   this->X += v.X;
   this->Y += v.Y;
   this->Z += v.Z;
   return *this;
}

const GLVector operator+(const GLVector& v, const GLVector& l)
{
   return GLVector(v.X + l.X, v.Y + l.Y, v.Z + l.Z);
}

const GLVector operator*(const GLVector& v, const float& d)
{
	return GLVector(v.X * d, v.Y * d, v.Z * d);
}

/////////////////////////////////////////////////////////////////////////
// Rotators
/////////////////////////////////////////////////////////////////////////
GLRotator::GLRotator() {}

GLRotator::GLRotator(GLRotator * rot)
   : X{ rot->X }, Y{ rot->Y }, Z{ rot->Z }, Angle{ rot->Angle } {}

GLRotator::GLRotator(float x, float y, float z)
   : X{ x }, Y { y }, Z { z },
   Angle{ std::fmod(std::abs(x) + std::abs(y) + std::abs(z), 360.f) } 
{
}

GLRotator::GLRotator(GLVector v1, GLVector v2)
   : X{ v2.X - v1.X }, Y{ v2.Y - v1.Y }, Z{ v2.Z - v1.Z } {}

const GLRotator operator+(const GLRotator& v, const GLRotator& l)
{
   return GLRotator(std::fmod(v.X + l.X, 360.f),
		    std::fmod(v.Y + l.Y, 360.f),
                    std::fmod(v.Z + l.Z, 360.f));
}

const GLRotator operator*(const GLRotator& v, const float& d)
{
   return GLRotator(v.X * d, v.Y * d, v.Z * d);
}

/////////////////////////////////////////////////////////////////////////
// Scale Vectors
/////////////////////////////////////////////////////////////////////////
GLScale::GLScale() {}

GLScale::GLScale(GLScale* scale)
   : X{ scale->X }, Y{ scale->Y }, Z{ scale->Z } {}

GLScale::GLScale(float s)
   : X{ s }, Y{ s }, Z{ s } {}

GLScale::GLScale(float x, float y, float z)
   : X{ x }, Y{ y }, Z{ z } {}

/////////////////////////////////////////////////////////////////////////
// Transforms
/////////////////////////////////////////////////////////////////////////
GLTransform::GLTransform() {}

GLTransform::GLTransform(GLVector l, GLRotator r, GLScale s)
   : _location{ l }, _rotation{ r }, _scale{ s } {}

const GLVector& GLTransform::GetLocation()
{
   return _location;
}

const GLRotator& GLTransform::GetRotation()
{
   return _rotation;
}

const GLScale& GLTransform::GetScale()
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


