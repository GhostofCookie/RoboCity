#include <Robot.h>
#include <Core.h>
#include <Components/MeshComponent.h>

Robot::Robot()
   : _material{new GL_Colour(GL_Colour::White)}
{
   SetMeshes();
}

Robot::Robot(float x, float y, float z)
   : _material{new GL_Colour(GL_Colour::White)}
{
   SetMeshes();
}

Robot::~Robot()
{

}

void Robot::Tick()
{
   if (_body && _neck && _head)
   {
	   StartRender();
	   _antenna->Rotate(GLRotator(0.f, 30.f, 0.f));
	   EndRender();
   }
}

void Robot::Translate(GLVector loc)
{
	Object::Translate(loc);
	_body->Translate(loc);
	_head->Translate(loc);
	_eyes->Translate(loc);
	_neck->Translate(loc);
	_arms->Translate(loc);
	_legs->Translate(loc);
	_antenna->Translate(loc);
}

void Robot::Rotate(GLRotator rot)
{
	Object::Rotate(rot);
	_body->Rotate(rot);
	_head->Rotate(rot);
	_eyes->Rotate(rot);
	_neck->Rotate(rot);
	_arms->Rotate(rot);
	_legs->Rotate(rot);
	_antenna->Rotate(rot);
}

void Robot::SetScale(GLScale sca)
{
	Object::SetScale(sca);
	_body->SetScale(sca);
	_head->SetScale(sca);
	_eyes->SetScale(sca);
	_neck->SetScale(sca);
	_arms->SetScale(sca);
	_legs->SetScale(sca);
	_antenna->SetScale(sca);
}

void Robot::SetMeshes()
{
   _body = new GLMeshComponent();
   if(_body)
   {
      _body->SetMesh(new GLMesh("Meshes/Body.fbx"));
      _body->SetMaterial(GL_Colour::Red);
   }
   
   _neck = new GLMeshComponent();
   if(_neck)
   {
      _neck->SetMesh(new GLMesh("Meshes/Neck.fbx"));
      _neck->SetMaterial(GL_Colour::Cyan);
   }
   
   _head = new GLMeshComponent();
   if(_head)
   {
      _head->SetMesh(new GLMesh("Meshes/Head.fbx"));
      _head->SetMaterial(GL_Colour::Blue);
   }

   _eyes = new GLMeshComponent();
   if (_eyes)
   {
	   _eyes->SetMesh(new GLMesh("Meshes/Eyes.fbx"));
	   _eyes->SetMaterial(GL_Colour::White);
   }

   _antenna = new GLMeshComponent();
   if(_antenna)
   {
      _antenna->SetMesh(new GLMesh("Meshes/Antenna.fbx"));
	  _antenna->SetMaterial(GL_Colour::Green);
   }
   
   _arms = new GLMeshComponent();
   if(_arms)
   {
      _arms->SetMesh(new GLMesh("Meshes/cube.fbx"));
   }
  
   _legs = new GLMeshComponent();
   if(_legs)
   {
      _legs->SetMesh(new GLMesh("Meshes/cube.fbx"));
   }
   
}
