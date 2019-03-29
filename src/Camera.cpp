#include <Camera.h>
#include <Robot.h>
#include <cmath>

Camera::Camera(Robot* robot)
{
   _robot = robot;
   _eyex = _robot->GetLocation().x;
   _eyey = 0.0f;
   _eyez = 5.0f + _robot->GetLocation().z;

   _atx = _robot->GetLocation().x;
   _aty = 0.0f;
   _atz = _robot->GetLocation().z;
	
   _goalRotx = 0;
   _goalRoty = 0;
   _goalRotz = 0;

   _curRotx = 0;
   _curRoty = 0;
   _curRotz = 0;

   _isRotating = false;
   _isMoving = false;
}

void Camera::Display(int tick)
{
   FollowRobot();

   float rx =_robot->GetLocation().x;
   float ry = _robot->GetLocation().y;
   float rz = _robot->GetLocation().z;
   
   gluLookAt(_eyex+rx, _eyey, _eyez+rz, _atx, _aty, _atz, 0, 1, 0);

   glTranslatef(rx, ry, rz);
   
   glRotatef(_curRotx, 1.0, 0.0, 0.0);
   glRotatef(_curRoty, 0.0, 1.0, 0.0);
   glRotatef(_curRotz, 0.0, 0.0, 1.0);

   glTranslatef(-rx, -ry, -rz);

   if (_isRotating && tick > 1)
      AnimateRotation(tick);

   if (_isMoving && tick > 1)
      AnimateMovement(tick);
}

void Camera::MoveCamera(float eyex, float eyey, float eyez)
{
   _goalEyex = eyex;
   _goalEyey = eyey;
   _goalEyez = eyez;

   _incEyex = _goalEyex - _eyex;
   _incEyey = _goalEyey - _eyey;
   _incEyez = _goalEyez - _eyez;

   _isMoving = true;
}

void Camera::RotateCamera(float rotx, float roty, float rotz)
{
   _goalRotx += rotx;
   _goalRoty += roty;
   _goalRotz += rotz;

   _incRotx = _goalRotx - _curRotx;
   _incRoty = _goalRoty - _curRoty;
   _incRotz = _goalRotz - _curRotz;

   _isRotating = true;
}

bool Camera::IsRotating()
{
   return _isRotating;
}

bool Camera::IsMoving()
{
   return _isMoving;
}

// Private functions

void Camera::AnimateRotation(int tick)
{
   bool doneRotate = true;
   if (_curRotx != _goalRotx)
   {
      _curRotx += (_incRotx / tick) * 2;

      if (_curRotx > _goalRotx && _incRotx > 0 || _curRotx < _goalRotx && _incRotx < 0)
	 _curRotx = _goalRotx;

      doneRotate = false;
   }

   if (_curRoty != _goalRoty)
   {
      _curRoty += (_incRoty / tick) * 2;

      if (_curRoty > _goalRoty && _incRoty > 0 || _curRoty < _goalRoty && _incRoty < 0)
	 _curRoty = _goalRoty;

      doneRotate = false;
   }

   if (_curRotz != _goalRotz)
   {
      _curRotz += (_incRotz / tick) * 2;

      if (_curRotz > _goalRotz && _incRotz > 0 || _curRotz < _goalRotz && _incRotz < 0)
	 _curRotz = _goalRotz;
      doneRotate = false;
   }

   if (doneRotate)
      _isRotating = false;
}

void Camera::AnimateMovement(int tick)
{
   bool doneMove = true;
   if (_eyex != _goalEyex)
   {
      _eyex += (_incEyex / tick) * 2;

      if (_eyex > _goalEyex && _incEyex > 0 || _eyex < _goalEyex && _incEyex < 0)
	 _eyex = _goalEyex;
      doneMove = false;
   }

   if (_eyey != _goalEyey)
   {
      _eyey += (_incEyey / tick) * 2;

      if (_eyey > _goalEyey && _incEyey > 0 || _eyey < _goalEyey && _incEyey < 0)
	 _eyey = _goalEyey;
      doneMove = false;
   }

   if (_eyez != _goalEyez)
   {
      _eyez += (_incEyez / tick) * 2;

      if (_eyez > _goalEyez && _incEyez > 0 || _eyez < _goalEyez && _incEyez < 0)
	 _eyez = _goalEyez;
      doneMove = false;
   }

   if (doneMove)
      _isMoving = false;
}

void Camera::FollowRobot()
{
   _atx = _robot->GetLocation().x;
   _atz = _robot->GetLocation().z;
}
