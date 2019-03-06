#include <Object.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>

#include <App.h>
#include <Core.h>


Object::Object()
	: _transform{ new GLTransform(GLVector(0.f, 0.f, 0.f), GLRotator(0.f), GLScale(1.f, 1.f, 1.f)) }
{
	App::AddObjectToPool(this);
}

Object::Object(float x, float y, float z)
	: _transform{ new GLTransform(GLVector(x, y, z), GLRotator(0.f), GLScale(1.f, 1.f, 1.f)) }
{
	App::AddObjectToPool(this);
}

Object::~Object() { delete _transform; }

void Object::SetLocation(GLVector loc)
{
	_transform->SetLocation(loc);
}

void Object::SetLocation(float x, float y, float z)
{
	SetLocation(GLVector(x, y, z));
}

void Object::SetRotation(GLRotator rotation)
{
	_transform->SetRotation(rotation);
	_angle = _transform->GetRotation().Angle;
}

void Object::SetRotation(float x, float y, float z)
{
	SetRotation(GLRotator(x, y, z));
}

void Object::SetScale(GLScale scale)
{
	_transform->SetScale(&scale);
}

void Object::SetScale(float x, float y, float z)
{
	SetScale(GLScale(x, y, z));
}

void Object::Translate(GLVector v)
{
	SetLocation(
		_transform->GetRotation().X + v.X,
		_transform->GetLocation().Y + v.Y,
		_transform->GetLocation().Z + v.Z);
}

void Object::Translate(float x, float y, float z)
{
	SetLocation(GLVector(
		_transform->GetLocation().X + x,
		_transform->GetLocation().Y + y,
		_transform->GetLocation().Z + z));
}

void Object::Rotate(GLRotator r)
{
	_transform->SetRotation(r);
	_angle = std::fmod(_angle + _transform->GetRotation().Angle, 360.f);
}


void Object::Rotate(float theta, GLVector v1, GLVector v2)
{
	_angle = std::fmod(_angle + theta, 360.f);
	_transform->SetRotation(GLRotator(v1, v2));
}

GLVector Object::GetLocation()
{
	return _transform->GetLocation();
}

GLRotator Object::GetRotation()
{
	return _transform->GetRotation();
}

GLScale Object::GetScale()
{
	return _transform->GetScale();
}

GLTransform Object::GetTransform()
{
	return *_transform;
}

void Object::StartRender()
{
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(_transform->GetLocation().X, _transform->GetLocation().Y, _transform->GetLocation().Z);
	glRotatef(_angle, _transform->GetRotation().X, _transform->GetRotation().Y, _transform->GetRotation().Z);
	glTranslatef(-_transform->GetLocation().X, -_transform->GetLocation().Y, -_transform->GetLocation().Z);

	glTranslatef(_transform->GetLocation().X, _transform->GetLocation().Y, _transform->GetLocation().Z);

	glScalef(_transform->GetScale().X, _transform->GetScale().Y, _transform->GetScale().Z);
}

void Object::EndRender()
{
	glPopMatrix();
}

