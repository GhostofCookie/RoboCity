#pragma once

class Robot
{
  public:
   Robot();
   ~Robot();

   void Render(int tick);

   struct Coord
   {
   public:
   Coord(float X, float Y, float Z) :x{X}, y{Y}, z{Z} {}
      float x, y, z;
   };
   
   const Coord GetLocation();
   const Coord GetRotation();

   void SetLocation(float, float, float);
   void SetRotation(float, float, float);

   void Rotate(float);
   void RotateHead(float);
   void ResetHead();

   void MoveForward(float);
   const Coord ForwardVector();

   bool IsMoving();

  private:
   Coord* _loc;
   Coord* _rot;
   float _angle, _head_angle, _antenna_angle;
   float _xGoal = 0.0f, _xDelta = 0.0f;
   float _zGoal = 0.0f, _zDelta = 0.0f;
   float _legAngle = 0.0f, _legMove = 3.0f;
   bool _isMoving = false;

   void AnimateMovement(int tick);

};
