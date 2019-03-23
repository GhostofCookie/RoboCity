#include <Robot.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>

Robot::Robot()
   : _loc{new Coord(0.f, 0.f, 0.f)}, _rot{new Coord(0.f, 0.f, 0.f)}
{

}

Robot::~Robot()
{

}

void Robot::Render()
{
   glPushMatrix();

   glTranslatef(_loc->x, _loc->y, _loc->z);
   glRotatef(_angle, _rot->x, _rot->y, _rot->z);
   glTranslatef(-_loc->x, -_loc->y, -_loc->z);
   
   glTranslatef(_loc->x, _loc->y, _loc->z);

   glScalef(.25, .25, .25);

   //////////////////////////////////
   // Antenna
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(0.f, 0.5f, 1.f);
      glTranslatef(0.f, 16.5f, 0.f);
      glRotatef(90.f, 1.f, 0.f, 0.f);
      glTranslatef(0.f, -16.5f, 0.f);
      glTranslatef(0.f, 16.5f, 0.f);
      GLUquadric* c = gluNewQuadric();
      gluCylinder(c, 0.5f, 0.5f, 2.f, 16, 2);
      gluQuadricDrawStyle(c, GL_FILL);
      glPopMatrix();
   }
   
   //////////////////////////////////
   // Head
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(1.f, 0.f, 0.f);
      glTranslatef(0.f, 12.f, 0.f);
      glRotatef(_head_angle, 0.f, 1.f, 0.f);
      glTranslatef(0.f, -12.f, 0.f);
      glTranslatef(0.f, 12.f, 0.f);
      glutSolidCube(5.f);
      glPopMatrix();
   }

   //////////////////////////////////
   // Neck
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(1.f, 0.f, 1.f);
      glTranslatef(0.f, 10.f, 0.f);
      glRotatef(90.f, 1.f, 0.f, 0.f);
      glTranslatef(0.f, -10.f, 0.f);
      glTranslatef(0.f, 10.f, 0.f);
      GLUquadric* c = gluNewQuadric();
      gluCylinder(c, 2.f, 2.f, 2.f, 16, 2);
      gluQuadricDrawStyle(c, GL_FILL);
      glPopMatrix();
   }

   //////////////////////////////////
   // Body
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(0.5f, 0.f, 1.f);
      glutSolidCube(5.f);
      glPopMatrix();
   
      glPopMatrix();
   }
   
}

const Robot::Coord Robot::GetLocation()
{
   return *_loc;
}

const Robot::Coord Robot::GetRotation()
{
   return *_rot;
}

void Robot::SetLocation(float x, float y, float z)
{
   if(_loc) delete _loc;
   _loc = new Coord(x, y, z);
}

void Robot::SetRotation(float x, float y, float z)
{
   if(_rot) delete _rot;
   _rot = new Coord(x, y, z);
}

void Robot::Rotate(float t)
{
   _angle = std::fmod(_angle + t, 360.f);
   _rot->y = 1.f;
}

void Robot::RotateHead(float t)
{
   _head_angle = std::fmod(_head_angle + t, 360.f);
}

void Robot::MoveForward(float d)
{
   SetLocation(
      _loc->x + ((_angle == 90.f ? 1 : _angle == 270.f ? -1 : 0) * d),
      0.f,
      _loc->z + ((_angle == 0.f ? 1 : _angle == 180.f ? -1 : 0) * d)
      );
}
