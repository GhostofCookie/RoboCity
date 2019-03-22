#include <Object.h>

class Robot : public Object
{
  public:
   Robot();
   Robot(float x, float y, float z);
   virtual ~Robot();
   virtual void Tick();

   void MoveForward(float);
   void RotateHead(float);
   void RotateBody(float);

  private:
   void SetMeshes();
   
  private:
   class GLMeshComponent* _body, *_head, *_neck, *_antenna, *_arms, *_legs, *_eyes;
   struct GL_Colour* _material;
   
};
