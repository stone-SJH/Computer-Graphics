#include <GL/glut.h>  
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string>
#include <cmath>
#include "macro.h"
using namespace std;
class CueBall{
public:
	float wx;
	float wy;
	float wspeed = 0.018f;
	float wspeed_edge = 0.018f;//恒定速度
	float wspeed_edge2 = 0.001f;//停止模糊速度
	int wflag;
	float directRotate;
	float Rspeed = 5.0f;

	CueBall();
	void draw();
};

class MoveBalls{
public:
	float mspeed_edge = 0.004f;//最小恒定速度
	float mspeed[MNUM];//当前速度
	float movex[MNUM], movey[MNUM];//位置
	int moveflag[MNUM];//是否存在
	float moverotate[MNUM];
	int movex_edge = 6, movey_edge = 2;

	MoveBalls();
	void draw();
};

class GhostBalls{
public:
	float gspeed[GNUM];
	float ghostx[GNUM], ghosty[GNUM];
	int ghostflag[GNUM];
	float ghostrotate[GNUM];

	GhostBalls();
	void draw();
};

class Table{
private:
	static BITMAP GetBmp(LPCTSTR title);
	void wd_texture_display();
	void tb_texture_display();
public:
	float xedge;
	float yedge;

	Table();
	void draw();
};