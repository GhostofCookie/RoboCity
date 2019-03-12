#include <Object.h>

class Robot : public Object
{
  public:
   Robot();
   Robot(float x, float y, float z);
   virtual ~Robot();

   virtual void Tick();

  private:
   void SetMeshes();
   
  private:
   class GLMeshComponent* _body, *_head, *_neck, *antenna, *_arms, *_legs;
   struct GL_Colour* _material;
   
};
