#pragma once

#ifdef _WIN32
#include <direct.h>
#define GetCurrentDir _getcwd
#else
#include <unistd.h>
#define GetCurrentDir getcwd
#endif

#define GET_OBJECT_NAME(n) #n

/** 
* Defines a colour based on RGB or RGBA values.
*/
struct GL_Colour
{
public:
	explicit GL_Colour(float, float, float);
	explicit GL_Colour(float, float, float, float);
	GL_Colour(const GL_Colour& c);
	void UseColour();

	static const GL_Colour Black;
	static const GL_Colour White;
	static const GL_Colour Grey;
	static const GL_Colour Red;
	static const GL_Colour Green;
	static const GL_Colour Blue;
	static const GL_Colour Cyan;
	static const GL_Colour Emerald;
	static const GL_Colour Purple;

private:
	float R, G, B, A;
};

struct GL_Font
{
public:
	explicit GL_Font(int);
	void* GetFont();
	
	static const GL_Font HELVETICA;
	static const GL_Font TIMES;
private:
	int index;
};

struct GLVector
{
public:
	GLVector();
	GLVector(float, float);
	GLVector(float, float, float);
		
	float X, Y, Z;
};

struct GLRotator
{
public:
	GLRotator();
	GLRotator(GLRotator*);
	GLRotator(float);
	GLRotator(float, float, float);
	GLRotator(float, float, float, float, float, float);
	GLRotator(GLVector v1, GLVector v2);
		
	float X, Y, Z;
	float Angle;
};

struct GLScale
{
	GLScale();
	GLScale(GLScale*);
	GLScale(float, float, float);

	float X, Y, Z;
};

struct GLTransform
{
public:
	GLTransform();
	GLTransform(GLVector, GLRotator, GLScale);
	~GLTransform() {}

	GLVector GetLocation() const;
	GLRotator GetRotation() const;
	GLScale GetScale() const;

	void SetLocation(GLVector);
	void SetRotation(GLRotator);
	void SetScale(GLScale);

private:
	GLVector	_location;
	GLRotator	_rotation;
	GLScale		_scale;

};

