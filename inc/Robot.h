#pragma once

class Robot
{
  public:
   Robot();
   ~Robot();

   void Render();

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

  private:
   Coord* _loc;
   Coord* _rot;
   float _angle, _head_angle, _antenna_angle;

};
