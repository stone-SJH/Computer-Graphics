#include "Viewer.h"

void Viewer::init(){
	fScale = 1.0f;

	mouserdown = GL_FALSE;
	mouseldown = GL_FALSE;
	mousemdown = GL_FALSE;

	mousex = 0; 
	mousey = 0;

	for (int i = 0; i < 3; i++)
		center[i] = 0.0f;

	yrotate = 0.01;
	xrotate = PI / 4;
	celength = 10.0f;

	mSpeed = 0.04f;
	rSpeed = 0.02f;
	lSpeed = 0.04f;
}

void Viewer::cal_eye_pos(){
	if (yrotate > PI / 2.2) yrotate = PI / 2.2;
	if (yrotate < 0)  yrotate = 0.01;
	if (xrotate > 2 * PI)   xrotate = 0.01;
	if (xrotate < 0)   xrotate = 2 * PI;
	if (celength > 50)  celength = 50;
	if (celength < 5)   celength = 5;
	eye[0] = 1 *(center[0] + celength * sin(yrotate) * cos(xrotate));
	eye[2] = 1 * (center[2] + celength * sin(yrotate) * sin(xrotate));
	eye[1] = 1 * (center[1] + celength * cos(yrotate));
}

void Viewer::move_backward()
{
	center[0] += mSpeed * cos(xrotate);
	center[2] += mSpeed * sin(xrotate);
	cal_eye_pos();
}

void Viewer::move_forward()
{
	center[0] -= mSpeed * cos(xrotate);
	center[2] -= mSpeed * sin(xrotate);
	cal_eye_pos();
}

// visual angle rotates
void Viewer::rotate_left()
{
	xrotate -= rSpeed;
	cal_eye_pos();
}

void Viewer::rotate_right()
{
	xrotate += rSpeed;
	cal_eye_pos();
}

void Viewer::rotate_up()
{
	yrotate += rSpeed;
	cal_eye_pos();
}

void Viewer::rotate_down()
{
	yrotate -= rSpeed;
	cal_eye_pos();
}


Viewer::Viewer(){
	init();
}

void Viewer::mouse_func(int button, int state, int x, int y){
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_TRUE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_TRUE;
		if (button == GLUT_MIDDLE_BUTTON) mousemdown = GL_TRUE;
	}
	else
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_FALSE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_FALSE;
		if (button == GLUT_MIDDLE_BUTTON) mousemdown = GL_FALSE;
	}
	mousex = x, mousey = y;
}

void Viewer::mouse_motion(int x, int y){
	if (mouserdown == GL_TRUE)
	{
		xrotate += (x - mousex) / 80.0f;
		yrotate -= (y - mousey) / 120.0f;
	}

	if (mouseldown == GL_TRUE)
	{
		celength += (y - mousey) / 25.0f;
	}
	mousex = x, mousey = y;
	cal_eye_pos();
}

void Viewer::look(){
	cal_eye_pos();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
}