#pragma once

#include <Object.h>

class Building : public Object
{
  public:
   Building();
   ~Building();

   virtual void Tick();

   bool CanTakeDamage() const;

   void TakeDamage(float Delta);
   void SetMaterial(struct GL_Colour);

  private:
   float _health, _max_health;
   class GLMeshComponent* _mesh;
   GL_Colour* _material;

};
