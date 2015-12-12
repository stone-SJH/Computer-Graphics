#include <GL/glut.h>  
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <cmath>
#include"macro.h"

class Viewer{
private:
	float fScale;

	GLboolean mouserdown;
	GLboolean mouseldown;
	GLboolean mousemdown;

	GLint mousex, mousey;

	GLfloat center[3];
	GLfloat eye[3];

	GLfloat yrotate;
	GLfloat xrotate;
	GLfloat celength;

	GLfloat mSpeed;
	GLfloat rSpeed;
	GLfloat lSpeed;

	void init();
	void cal_eye_pos();
	void move_forward();
	void move_backward();
	void rotate_left();
	void rotate_right();
	void rotate_up();
	void rotate_down();

public:
	Viewer();
	void mouse_func(int button, int state, int x, int y);
	void mouse_motion(int x, int y);
	void look();
};