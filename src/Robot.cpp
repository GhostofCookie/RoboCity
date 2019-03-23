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
   // Face the robot away from the camera immediately
   Rotate(180.f);
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

   glScalef(0.1, 0.1, 0.1);
   
   //////////////////////////////////
   // Antenna
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(0.f, 0.5f, 1.f);
      glTranslatef(0.f, 12.5f, 0.f);
      glRotatef(_antenna_angle, 0.f, 1.f, 0.f);
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
      glTranslatef(0.f, 8.f, 0.f);
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
      glTranslatef(0.f, 6.f, 0.f);
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
      float bw = 7.f, bh = 10.f;
      
      glPushMatrix();
      glBegin(GL_QUADS);
      
      glColor3f(0.f, 0.f, 1.f);
   
      // Forward Face
      glNormal3f(0.f, 0.f, 1.f);
      glVertex3f(-bw/2, bh/2, bw/2);
      glVertex3f(-bw/2, -bh/2, bw/2);
      glVertex3f(bw/2, -bh/2, bw/2);
      glVertex3f(bw/2, bh/2, bw/2);

      // Back Face
      glColor3f(0.f, 1.f, 0.f);
      glNormal3f(0.f, 0.f, 1.f);
      glVertex3f(-bw/2, -bh/2, -bw/2);
      glVertex3f(-bw/2, bh/2, -bw/2);
      glVertex3f(bw/2, bh/2, -bw/2);
      glVertex3f(bw/2, -bh/2, -bw/2);
      
      // Right Face
      glColor3f(1.f, 1.f, 0.f);
      glNormal3f(-1.f, 0.f, 0.f);
      glVertex3f(-bw/2, bh/2, -bw/2);
      glVertex3f(-bw/2, -bh/2, -bw/2);
      glVertex3f(-bw/2, -bh/2, bw/2);
      glVertex3f(-bw/2, bh/2, bw/2);

      // Left Face
      glColor3f(1.f, 1.f, 1.f);
      glNormal3f(1.f, 0.f, 0.f);
      glVertex3f(bw/2, bh/2, bw/2);
      glVertex3f(bw/2, -bh/2, bw/2);
      glVertex3f(bw/2, -bh/2, -bw/2);
      glVertex3f(bw/2, bh/2, -bw/2);

      // Top Face
      glColor3f(1.f, 0.f, 1.f);
      glNormal3f(0.f, 1.f, 0.f);
      glVertex3f(-bw/2, bh/2, bw/2);
      glVertex3f(-bw/2, bh/2, -bw/2);
      glVertex3f(bw/2, bh/2, -bw/2);
      glVertex3f(bw/2, bh/2, bw/2);
      
      // Bottom Face
      glColor3f(1.f, 0.f, 1.f);
      glNormal3f(0.f, -1.f, 0.f);
      glVertex3f(-bw/2, bh/2, -bw/2);
      glVertex3f(-bw/2, bh/2, bw/2);
      glVertex3f(bw/2, bh/2, bw/2);
      glVertex3f(bw/2, bh/2, -bw/2);
      
      glEnd();
      glPopMatrix();
   }
   
   glPopMatrix();
   
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
   _angle = std::fmod(_angle + t + 360.f, 360.f);
   _rot->y = 1.f;
}

void Robot::RotateHead(float t)
{
   _head_angle = std::fmod(_head_angle + t, 360.f);
}

void Robot::MoveForward(float d)
{
   _antenna_angle = std::fmod(_antenna_angle + 30.f + 360.f, 360.f);
   SetLocation(
      _loc->x + ((_angle == 90.f ? 1 : _angle == 270.f ? -1 : 0) * d),
      0.f,
      _loc->z + ((_angle == 0.f ? 1 : _angle == 180.f ? -1 : 0) * d)
      );
}
