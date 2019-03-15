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
   ~GL_Colour() {}
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
   static const GL_Colour Yellow;
   static const GL_Colour Orange;

public:
   float R, G, B, A;
   float RGBA[4];
};

/**
 * Creates a font recognizable by GLUT but more Human Readable.
 */
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

/**
 * Creates a vector which is used to define locational information about an Object.
 */
struct GLVector
{
public:
   GLVector();
   GLVector(GLVector*);
   GLVector(float, float);
   GLVector(float, float, float);
   ~GLVector() {}

   const GLVector& operator+=(const GLVector&);
   friend const GLVector operator+(const GLVector&, const GLVector&);
		
   float X, Y, Z;
};


/**
 * Creates a vector which is used to define rotational information about an Object.
 */
struct GLRotator
{
public:
   GLRotator();
   GLRotator(GLRotator*);
   GLRotator(float);
   GLRotator(float, float, float);
   GLRotator(float, float, float, float, float, float);
   GLRotator(GLVector v1, GLVector v2);
   ~GLRotator() {}
		
   float X, Y, Z;
   float Angle;
};


/**
 * Creates a vector which is used to define Object scale.
 */
struct GLScale
{
public:
   GLScale();
   GLScale(GLScale*);
   GLScale(float);
   GLScale(float, float, float);
   ~GLScale() {}

   float X, Y, Z;
};


/**
 * Creates a structure which contains location, rotation, and scale data.
 */
struct GLTransform
{
public:
   GLTransform();
   GLTransform(GLVector, GLRotator, GLScale);
   ~GLTransform() {}

   const GLVector& GetLocation();
   const GLRotator& GetRotation();
   const GLScale& GetScale();

   void SetLocation(GLVector);
   void SetRotation(GLRotator);
   void SetScale(GLScale);

private:
   GLVector	_location;
   GLRotator	_rotation;
   GLScale	_scale;
};

