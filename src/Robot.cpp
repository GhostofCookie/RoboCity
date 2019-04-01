#include <Robot.h>

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <cmath>

Robot::Robot()
   : _loc{ new Coord(0.f, 0.f, 0.f) }, _rot{ new Coord(0.f, 0.f, 0.f) }
{
   // Face the robot away from the camera immediately
   Rotate(180.f);
}

Robot::~Robot()
{

}

void Robot::Render(int tick)
{
   glPushMatrix();

   glTranslatef(_loc->x, _loc->y, _loc->z);
   glRotatef(_angle, _rot->x, _rot->y, _rot->z);
   glTranslatef(-_loc->x, -_loc->y, -_loc->z);

   glTranslatef(_loc->x, _loc->y + 0.5, _loc->z);

   if (_isMoving && tick > 1)
      AnimateMovement(tick);

   //////////////////////////////////
   // Antenna
   //////////////////////////////////
   {
      glPushMatrix();

      glColor3f(0.f, 0.1f, 0.5f);
      glRotatef(_antenna_angle, 0.f, 1.f, 0.f);

      glPushMatrix();
      glTranslatef(0.f, 1.25f, 0.f);

      glRotatef(90.f, 1.f, 0.f, 0.f);
      glTranslatef(0.f, -1.25f, 0.f);
      glTranslatef(0.f, 1.25f, 0.f);
      GLUquadric* c = gluNewQuadric();
      gluCylinder(c, 0.05f, 0.05f, 0.2f, 16, 2);
      gluQuadricDrawStyle(c, GL_FILL);
      glPopMatrix();
      glPushMatrix();
      glTranslatef(0.f, 1.25f, 0.f);
      glScalef(2.f, 0.5f, 0.5f);
      
      glutSolidCube(0.1f);
      glPopMatrix();

      glPopMatrix();
   }

   //////////////////////////////////
   // Head
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(0.512f/2, 0.535f/2, 0.586f/2);
      glTranslatef(0.f, 0.8f, 0.f);
      glRotatef(_head_angle, 0.f, 1.f, 0.f);
      glTranslatef(0.f, -1.2f, 0.f);
      glTranslatef(0.f, 1.2f, 0.f);
      glutSolidCube(0.5f);

      //////////////////////////////////
      // Eyes
      //////////////////////////////////
      {
	 // Eye Colour
	 glColor3f(0.8f, 0.f, 0.03f);
	 glPushMatrix();
	 {
	    glTranslatef(-0.15f, 0.f, 0.2f);
	    GLUquadric* c = gluNewQuadric();
	    gluSphere(c, 0.1f, 8, 8);
	    glTranslatef(0.15f, 0.f, -0.2f);
	 }
	 {
	    glTranslatef(0.15f, 0.f, 0.2f);
	    GLUquadric* c = gluNewQuadric();
	    gluSphere(c, 0.1f, 8, 8);
	    glTranslatef(-0.15f, 0.f, -0.2f);
	    glPopMatrix();
	 }
	 glPopMatrix();
      }
   }

   //////////////////////////////////
   // Neck
   //////////////////////////////////
   {
      glPushMatrix();
      glColor3f(0.512f/2, 0.535f/2, 0.586f/2);
      glTranslatef(0.f, 0.6f, 0.f);
      glRotatef(90.f, 1.f, 0.f, 0.f);
      glTranslatef(0.f, -1.f, 0.f);
      glTranslatef(0.f, 1.f, 0.f);
      GLUquadric* c = gluNewQuadric();
      gluCylinder(c, 0.2f, 0.2f, 0.2f, 16, 2);
      gluQuadricDrawStyle(c, GL_FILL);
      glPopMatrix();
   }

   //////////////////////////////////
   // Body
   //////////////////////////////////
   {
      float bw = 0.7f, bh = 1.f;

      glPushMatrix();
      glBegin(GL_QUADS);

      // Front Decal
      {
	 glColor3f(0.195f, 0.996f, 0.492f);
	 glNormal3f(0.f, 0.f, 0.f);
	 glVertex3f(-bw / 2 + 0.1, bh / 2 - 0.1, bw / 2 + 0.01);
	 glVertex3f(-bw / 2 + 0.1, -bh / 2 + 0.1, bw / 2 + 0.01);
	 glVertex3f(bw / 2 - 0.1, -bh / 2 + 0.1, bw / 2 + 0.01);
	 glVertex3f(bw / 2 - 0.1, bh / 2 - 0.1, bw / 2 + 0.01);
      }

      // Front/Back Colour
      glColor3f(0.164f, 0.203f, 0.223f);

      // Forward Face
      glNormal3f(0.f, 0.f, 1.f);
      glVertex3f(-bw / 2, bh / 2, bw / 2);
      glVertex3f(-bw / 2, -bh / 2, bw / 2);
      glVertex3f(bw / 2, -bh / 2, bw / 2);
      glVertex3f(bw / 2, bh / 2, bw / 2);

      // Back Face
      glNormal3f(0.f, 0.f, -1.f);
      glVertex3f(-bw / 2, -bh / 2, -bw / 2);
      glVertex3f(-bw / 2, bh / 2, -bw / 2);
      glVertex3f(bw / 2, bh / 2, -bw / 2);
      glVertex3f(bw / 2, -bh / 2, -bw / 2);

      // Side Colour
      glColor3f(0.164f, 0.203f, 0.223f);

      // Right Face
      glNormal3f(1.f, 0.f, 0.f);
      glVertex3f(-bw / 2, bh / 2, -bw / 2);
      glVertex3f(-bw / 2, -bh / 2, -bw / 2);
      glVertex3f(-bw / 2, -bh / 2, bw / 2);
      glVertex3f(-bw / 2, bh / 2, bw / 2);

      // Left Face
      glNormal3f(-1.f, 0.f, 0.f);
      glVertex3f(bw / 2, bh / 2, bw / 2);
      glVertex3f(bw / 2, -bh / 2, bw / 2);
      glVertex3f(bw / 2, -bh / 2, -bw / 2);
      glVertex3f(bw / 2, bh / 2, -bw / 2);

      // Top and Bottom
      glColor3f(0.164f, 0.203f, 0.223f);
      // Top Face
      glNormal3f(0.f, 1.f, 0.f);
      glVertex3f(-bw / 2, bh / 2, bw / 2);
      glVertex3f(-bw / 2, bh / 2, -bw / 2);
      glVertex3f(bw / 2, bh / 2, -bw / 2);
      glVertex3f(bw / 2, bh / 2, bw / 2);

      // Bottom Face
      glNormal3f(0.f, -1.f, 0.f);
      glVertex3f(-bw / 2, bh / 2, -bw / 2);
      glVertex3f(-bw / 2, bh / 2, bw / 2);
      glVertex3f(bw / 2, bh / 2, bw / 2);
      glVertex3f(bw / 2, bh / 2, -bw / 2);

      glEnd();

      // Triangle Colour
      if(!IsMoving()) glColor3f(0.f, 1.f, 0.f);
      else glColor3f(1.f, 0.f, 0.f);

      // Back Triangles
      glPushMatrix();
      glBegin(GL_TRIANGLES);
      glNormal3f(0.f, -1.f, 0.f);
      glVertex3f(bw / 2 - 0.1f, -bh / 2 + 0.1f, -bw / 2 - 0.01);
      glVertex3f(-bw / 2 + 0.1f, -bh / 2 + 0.1f, -bw / 2 - 0.01);
      glVertex3f(0.f, 0.f, -bw / 2 - 0.01);
      glEnd();

      glBegin(GL_TRIANGLES);
      glNormal3f(0.f, -1.f, 0.f);
      glVertex3f(bw / 2 - 0.1f, 0, -bw / 2 - 0.01f);
      glVertex3f(-bw / 2 + 0.1f, 0, -bw / 2 - 0.01f);
      glVertex3f(0.f, bh / 2 - 0.1f, -bw / 2 - 0.01f);
      glEnd();
      glPopMatrix();

      glPopMatrix();
   }

   //////////////////////////////////
   // Legs
   //////////////////////////////////
   {
      glColor3f(0.512f/2, 0.535f/2, 0.586f/2);

      glPushMatrix();
      glTranslatef(-0.15f, -0.8f, 0.f);
      glScalef(1.f, 3.f, 1.f);
      glRotatef(_legAngle, 1.0, 0.0, 0.0);
      glutSolidCube(0.2f);
      glPopMatrix();

      glPushMatrix();
      glTranslatef(0.15f, -0.8f, 0.f);
      glScalef(1.f, 3.f, 1.f);
      glRotatef(-_legAngle, 1.0, 0.0, 0.0);
      glutSolidCube(0.2f);
      glPopMatrix();
   }

   glPopMatrix();

   _antenna_angle = std::fmod(_antenna_angle + 30.f + 360.f, 360.f);
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
   if (_loc) delete _loc;
   _loc = new Coord(x, y, z);
}

void Robot::SetRotation(float x, float y, float z)
{
   if (_rot) delete _rot;
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

void Robot::ResetHead()
{
   _head_angle = 0.f;
}

void Robot::MoveForward(float d)
{
   _xGoal = _loc->x + ((_angle == 90.f ? 1 : _angle == 270.f ? -1 : 0) * d);
   _xDelta = _xGoal - _loc->x;
   _zGoal = _loc->z + ((_angle == 0.f ? 1 : _angle == 180.f ? -1 : 0) * d);
   _zDelta = _zGoal - _loc->z;
   _isMoving = true;
}

const Robot::Coord Robot::ForwardVector()
{
   return Coord(
      ((_angle == 90.f ? 1 : _angle == 270.f ? -1 : 0)),
      0.f,
      ((_angle == 0.f ? 1 : _angle == 180.f ? -1 : 0))
      );
}

bool Robot::IsMoving()
{
   return _isMoving;
}

void Robot::AnimateMovement(int tick)
{
   bool moveFinished = true;
   if (_xDelta > 0 || _xDelta < 0)
   {
      float moveDis = _loc->x + (_xDelta / tick) * 2;
      SetLocation(moveDis, 0.0f, _loc->z);
      moveFinished = false;
   }
   else if (_zDelta > 0 || _zDelta < 0)
   {
      float moveDis = _loc->z + (_zDelta / tick) * 2;
      SetLocation(_loc->x, 0.0f, moveDis);
      moveFinished = false;
   }

   if (_loc->x > _xGoal && _xDelta > 0 || _loc->x < _xGoal && _xDelta < 0)
   {
      SetLocation(_xGoal, 0.0f, _loc->z);
      _xDelta = 0;
   }
   else if (_loc->z > _zGoal && _zDelta > 0 || _loc->z < _zGoal && _zDelta < 0)
   {
      SetLocation(_loc->x, 0.0f, _zGoal);
      _zDelta = 0;
   }

   _legAngle += _legMove;

   float lMax = 30.0f;
   if (_legAngle > lMax || _legAngle < -lMax)
      _legMove *= -1;

   if (moveFinished)
   {
      _isMoving = false;
      _legAngle = 0;
   }
}
