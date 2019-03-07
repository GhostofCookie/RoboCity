#include <Object.h>

class Robot : public Object
{
  public:
   Robot();
   Robot(float x, float y, float z);
   virtual ~Robot();

   virtual void Tick();
  private:
   class GLMeshComponent* _mesh;
   struct GL_Colour* _material;
   
};
