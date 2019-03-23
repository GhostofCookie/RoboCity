#pragma once

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

class Robot;

class Camera {
private:
	float _eyex;
	float _eyey;
	float _eyez;

	float _atx;
	float _aty;
	float _atz;

	// Robot goes here.
	Robot* _robot;

	bool _isRotating;

	float _curRotx;
	float _curRoty;
	float _curRotz;

	float _goalRotx;
	float _goalRoty;
	float _goalRotz;

	float _incRotx;
	float _incRoty;
	float _incRotz;

	bool _isMoving;

	float _goalEyex;
	float _goalEyey;
	float _goalEyez;

	float _incEyex;
	float _incEyey;
	float _incEyez;

	void AnimateRotation(int tick);
	void AnimateMovement(int tick);
	void FollowRobot();
public:
	Camera(Robot* robot);

	void Display(int tick);
	void MoveCamera(float eyex, float eyey, float eyez);
	void RotateCamera(float rotx, float roty, float rotz);
	bool IsRotating();
	bool IsMoving();
};