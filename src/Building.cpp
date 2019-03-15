#include <Building.h>
#include <Core.h>
#include <Components/MeshComponent.h>

#include <cmath>

Building::Building()
   : _material{ new GL_Colour(GL_Colour::White) },
   _max_health{ 100.f },
   _health{ _max_health }
{
   _mesh = new GLMeshComponent();
   if(_mesh)
   {
      _mesh->SetMesh(new GLMesh("Meshes/Buildings/Building_0.fbx"));
      _mesh->SetMaterial(*_material);
   }
}

Building::~Building()
{
   if(_mesh) delete _mesh;
   if(_material) delete _material;
}

void Building::Tick()
{
   if(_mesh)
   {
      StartRender();
      if(_material) _material->UseColour();
      _mesh->Tick();
      EndRender();
   }
}

bool Building::CanTakeDamage() const
{
   return _health > 0.f || _max_health > -1;
}

void Building::TakeDamage(float Delta)
{
   if (CanTakeDamage())
   {
      _health = std::fmax(_health - Delta, 0.f);
      if(_health < _max_health) SetMaterial(GL_Colour::Yellow);
      else if(_health < _max_health / 2) SetMaterial(GL_Colour::Orange);
      else if(_health < _max_health / 3) SetMaterial(GL_Colour::Red);
      else SetMaterial(GL_Colour::White);
   }
}

void Building::SetMaterial(GL_Colour c)
{
   if(_material) delete _material;
   _material = new GL_Colour(c);
}
