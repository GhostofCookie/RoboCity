#include <Robot.h>
#include <Core.h>
#include <Components/MeshComponent.h>

Robot::Robot()
   : _material{new GL_Colour(GL_Colour::White)}
{
   _mesh = new GLMeshComponent();
   _mesh->SetMesh(new GLMesh("Meshes/sphere.fbx"));
}

Robot::Robot(float x, float y, float z)
   : _material{new GL_Colour(GL_Colour::White)}
{
   _mesh = new GLMeshComponent();
   _mesh->SetMesh(new GLMesh("Meshes/sphere.fbx"));
}

Robot::~Robot()
{

}

void Robot::Tick()
{
   StartRender();
   if(_material) _material->UseColour();
   if(_mesh) _mesh->Tick();
   EndRender();
}
