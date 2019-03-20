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
  
}

void Robot::MoveForward(float Delta)
{
   _antenna->Rotate(GLRotator(0.f, 30.f, 0.f));
   GLVector fv(GetRotation().X, GetRotation().Y, GetRotation().Z);
   Translate(fv * Delta);
}

void Robot::RotateHead(float theta)
{
   _neck->Rotate(theta, GLRotator(0.f, 1.f, 0.f));
}


void Robot::SetMeshes()
{
   _body = new GLMeshComponent();
   if(_body)
   {
      _body->SetMesh(new GLMesh("Meshes/Body.fbx"));
      _body->SetMaterial(GL_Colour::Red);
	  AddChild(_body);
   }
   
   _neck = new GLMeshComponent();
   if(_neck)
   {
      _neck->SetMesh(new GLMesh("Meshes/Neck.fbx"));
      _neck->SetMaterial(GL_Colour::Cyan);
	  AddChild(_neck);
   }
   
   _head = new GLMeshComponent();
   if(_head)
   {
      _head->SetMesh(new GLMesh("Meshes/Head.fbx"));
      _head->SetMaterial(GL_Colour::Blue);
      _neck->AddChild(_head);
   }

   _eyes = new GLMeshComponent();
   if (_eyes)
   {
	   _eyes->SetMesh(new GLMesh("Meshes/Eyes.fbx"));
	   _eyes->SetMaterial(GL_Colour::White);
	   _head->AddChild(_eyes);
   }

   _antenna = new GLMeshComponent();
   if(_antenna)
   {
      _antenna->SetMesh(new GLMesh("Meshes/Antenna.fbx"));
	  _antenna->SetMaterial(GL_Colour::Green);
	  _head->AddChild(_antenna);
   }
   
   _arms = new GLMeshComponent();
   if(_arms)
   {
      _arms->SetMesh(new GLMesh("Meshes/cube.fbx"));
	  AddChild(_arms);
   }
  
   _legs = new GLMeshComponent();
   if(_legs)
   {
      _legs->SetMesh(new GLMesh("Meshes/cube.fbx"));
	  AddChild(_legs);
	  }
   
}
