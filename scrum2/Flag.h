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
	float max_z;
	float min_x;
	float min_z;
	LPCTSTR tex_file;
	int segs;
	int circles;
	float range;
	float theta;
	float speed;
	int type;
	curvetype tp;

	GLfloat range_inc;
	GLfloat range_now;
	GLfloat x_inc;
	GLfloat t_inc;
	GLfloat theta_inc;
	Tools* tools;
	BITMAP flag_texture;
	GLuint texid;

	
	void setNormal(const GLfloat v1[3], const GLfloat v2[3], const GLfloat v3[3]);
	GLfloat getcurve(int pos);
	void init();
public:
	Flag();
	Flag(float posx, float posy, float posz, float maxx, float maxy, float minx, float miny, LPCTSTR texfile, int type);
	void changeType(int type);
	void setSegs(int segs);
	void setCircles(int circles);
	void setRange(float range);
	void setType(curvetype type);
	void setSpeed(float speed);

	void move(float posx, float posy);
	void setTex();
	void drawFlag();//[TO DO]
	void idle();
};