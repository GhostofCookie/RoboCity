#include <Camera.h>
#include <Robot.h>

Camera::Camera(Robot* robot)
{

   float robotx = robot->GetLocation().x;
   float robotz = robot->GetLocation().z;
   
	_eyex = robotx;
	_eyey = 0.0f;
	_eyez = 5.0f+robotz;

	_atx = robotx;
	_aty = 0.0f;
	
	// Get the z value of the robot
	_atz = robotz;
	_robot = robot;

	_goalRotx = 0;
	_goalRoty = 0;
	_goalRotz = 0;

	_curRotx = 0;
	_curRoty = 0;
	_curRotz = 0;

	_isRotating = false;
}

void Camera::Display(int tick)
{

   float robotx = _robot->GetLocation().x;
   float robotz = _robot->GetLocation().z;
   
	gluLookAt(robotx, _eyey, _eyez+robotz, robotx, _aty, robotz, 0, 1, 0);

	glRotatef(_curRotx, 1.0, 0.0, 0.0);
	glRotatef(_curRoty, 0.0, 1.0, 0.0);
	glRotatef(_curRotz, 0.0, 0.0, 1.0);

	if (_isRotating)
		AnimateRotation(tick);
}

void Camera::MoveCamera(float atx, float aty, float atz)
{
}

void Camera::RotateCamera(float rotx, float roty, float rotz)
{
	_goalRotx = rotx;
	_goalRoty += roty;
	_goalRotz = rotz;

	_incRotx = _goalRotx - _curRotx;
	_incRoty = _goalRoty - _curRoty;
	_incRotz = _goalRotz - _curRotz;

	_isRotating = true;
}

bool Camera::IsRotating()
{
	return _isRotating;
}

// Private functions

void Camera::AnimateRotation(int tick)
{
	bool doneRotate = true;
	if (_curRotx != _goalRotx)
	{
		_curRotx += (_incRotx / tick) * 2;

		if (_curRotx > _goalRotx)
			_curRotx = _goalRotx;
		doneRotate = false;
	}

	if (_curRoty != _goalRoty)
	{
		_curRoty += (_incRoty / tick) * 2;

		if (_curRoty > _goalRoty)
			_curRoty = _goalRoty;
		doneRotate = false;
	}

	if (_curRotz != _goalRotz)
	{
		_curRotz += (_incRotz / tick) * 2;

		if (_curRotz > _goalRotz)
			_curRotz = _goalRotz;
		doneRotate = false;
	}

	if (doneRotate)
		_isRotating = false;
}
