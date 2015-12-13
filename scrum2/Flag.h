#include <gl/glut.h>  
#include <math.h>   
#include <Windows.h>
#include "Tools.h"
#include "macro.h"
#include <iostream>
using namespace std;

class Flag{
private:
	float pos_x;
	float pos_y;
	float pos_z;
	float max_x;
	float max_y;
	float min_x;
	float min_y;
	LPCTSTR tex_file;
	int segs;
	int circles;
	float range;
	float theta;
	float speed;
	curvetype tp;

	GLfloat x_inc;
	GLfloat t_inc;
	GLfloat theta_inc;
	Tools* tools;
	BITMAP flag_texture;

	void setNormal(const GLfloat v1[3], const GLfloat v2[3], const GLfloat v3[3]);
	GLfloat getcurve(int pos);
	void init();
public:
	Flag();
	//Flag(float posx, float posy, float posz, float maxx, float maxy, float minx, float miny, string texfile);
	void setSegs(int segs);
	void setCircles(int circles);
	void setRange(float range);
	void setType(curvetype type);
	void setSpeed(float speed);

	void drawFlag();
	void idle();
};