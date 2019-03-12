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
      _body->Translate(GetLocation());
      _head->Translate(GetLocation());
      _neck->Translate(GetLocation());
      
      _body->Rotate(GetRotation());
      _head->Rotate(GetRotation());
      _neck->Rotate(GetRotation());
      EndRender();
   }
}

void Robot::SetMeshes()
{
   _body = new GLMeshComponent();
   if(_body)
   {
      _body->SetMesh(new GLMesh("Meshes/cube.fbx"));
      _body->SetLocation(GLVector(0, 0, 0));
      _body->SetScale(GLScale(10.f, 20.f, 10.f));
      _body->SetMaterial(GL_Colour::Red);
   }

   
   _neck = new GLMeshComponent();
   if(_neck)
   {
      _neck->SetMesh(new GLMesh("Meshes/cube.fbx"));
      _neck->SetLocation(GLVector(0, 22.f, 0));
      _neck->SetScale(GLScale(5.f, 2.f, 5.f));
      _neck->SetMaterial(GL_Colour::Cyan);
   }

   
   _head = new GLMeshComponent();
   if(_head)
   {
      _head->SetMesh(new GLMesh("Meshes/sphere.fbx"));
      _head->SetLocation(GLVector(0, 34, 0));
      _head->SetScale(GLScale(15.f));
      _head->SetMaterial(GL_Colour::Blue);
   }

   
   /*
   _arms = new GLMeshComponent();
   _arms->SetMesh(new GLMesh("Meshes/cube.fbx"));
   _arms->SetLocation(GLVector(0, 0, 0));
   _arms->SetScale(GLScale(0.f));
   */
}
