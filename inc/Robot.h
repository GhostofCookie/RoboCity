#include <Object.h>

class Robot : public Object
{
  public:
   Robot();
   Robot(float x, float y, float z);
   virtual ~Robot();
   virtual void Tick();
   virtual void Translate(GLVector) override;
   virtual void Rotate(GLRotator) override;
   virtual void SetScale(GLScale) override;

  private:
   void SetMeshes();
   
  private:
   class GLMeshComponent* _body, *_head, *_neck, *_antenna, *_arms, *_legs, *_eyes;
   struct GL_Colour* _material;
   
};
