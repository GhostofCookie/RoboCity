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
   _mesh->SetMesh(new GLMesh("Meshes/cube.fbx"));
   SetLocation(GLVector(x, y, z));
}

Robot::~Robot()
{

}

void Robot::Tick()
{
	if (_mesh)
	{
		StartRender();

		if (_material) _material->UseColour();
		_mesh->Tick();
		
		EndRender();
	}
}
