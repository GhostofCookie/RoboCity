#include <Object.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>
#include <stdio.h>
#include <iostream>

#include <App.h>
#include <Core.h>


Object::Object()
   : _transform{ new GLTransform(GLVector(0.f, 0.f, 0.f), GLRotator(0,0,0), GLScale(1.f, 1.f, 1.f)) }
{
   App::AddObjectToPool(this);
}

Object::Object(float x, float y, float z)
   : _transform{ new GLTransform(GLVector(x, y, z), GLRotator(0,0,0), GLScale(1.f, 1.f, 1.f)) }
{
   App::AddObjectToPool(this);
}

Object::~Object() 
{ 
   delete _transform; 
   for (int i = 1; i < _children.size(); i++)
   {
      //if (_children[i]) delete _children[i];
      //_children.pop_back();
   }
   App::RemoveObject(this);

}

void Object::SetLocation(GLVector loc)
{
   _transform->SetLocation(loc);
   for (auto obj : _children)
      if(obj)
	 obj->SetLocation(loc);
}

void Object::SetLocation(float x, float y, float z)
{
   SetLocation(GLVector(x, y, z));
   for (auto obj : _children)
      if(obj) obj->SetLocation(GLVector(x, y, z));
}

void Object::SetRotation(GLRotator rotation)
{
   _transform->SetRotation(rotation);
   for (auto obj : _children)
      if(obj) obj->SetRotation(rotation);
   _angle = _transform->GetRotation().Angle;
}

void Object::SetRotation(float x, float y, float z)
{
   SetRotation(GLRotator(x, y, z));
   for (auto obj : _children)
      if(obj) obj->SetRotation(GLRotator(x, y, z));
}

void Object::SetScale(GLScale scale)
{
   _transform->SetScale(&scale);
   for (auto obj : _children)
      if(obj) obj->SetScale(&scale);
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

   for (auto obj : _children)
      if (obj) obj->SetLocation(
	 _transform->GetRotation().X + v.X,
	 _transform->GetLocation().Y + v.Y,
	 _transform->GetLocation().Z + v.Z);
    std::cout<<"walked"<<std::endl;
}

void Object::Translate(float x, float y, float z)
{
   std::cout<<"walked"<<std::endl;
   GLVector l(
      _transform->GetLocation().X + x,
      _transform->GetLocation().Y + y,
      _transform->GetLocation().Z + z);
   SetLocation(l);
   for (auto obj : _children)
      if(obj)
	 obj->SetLocation(l);

}

void Object::Rotate(GLRotator r)
{
   _transform->SetRotation(r);
   for (auto obj : _children)
      if(obj) obj->Rotate(r);
   _angle = _transform->GetRotation().Angle;
}


void Object::Rotate(float theta, GLRotator v1)
{
   _angle = std::fmod(_angle + theta, 360.f);
   _transform->SetRotation(GLRotator(v1));
   for (auto obj : _children)
      if(obj) obj->Rotate(theta, v1);
}

const GLVector& Object::GetLocation()
{
   return _transform->GetLocation();
}

const GLRotator& Object::GetRotation()
{
   return _transform->GetRotation();
}

const GLScale& Object::GetScale()
{
   return _transform->GetScale();
}

const GLTransform& Object::GetTransform()
{
   return *_transform;
}

void Object::AddChild(Object * obj)
{
   if(obj) _children.push_back(obj);
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

