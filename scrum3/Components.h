 #include <Windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string>
#include <cmath>
#include "macro.h"
#include "Tools.h"
#include "Particle.h"
#include "Terrain.h"

using namespace std;
class CueBall{
public:
	float wx;
	float wy;
	float wz;
	float wspeed = 0.20f;
	float wspeed_edge = 0.20f;//恒定速度
	float wspeed_edge2 = 0.08f;//停止模糊速度
	int wflag;
	float directRotate;
	float Rspeed = 5.0f;
	int score = -50;
	float rot;
	Tools* tools;
	//tga cb_texture;
	BITMAP cb_texture;
	GLuint texid;

	CueBall();
	void draw();
	void setTex();
};

class MoveBalls{
private :
	Particles* particles[MNUM];
public:
	float mspeed_edge = 0.08f;//最小恒定速度
	float mspeed_edge2 = 0.18f;//最大速度
	float mspeed[MNUM];//当前速度
	float movex[MNUM], movey[MNUM];//位置
	float movez[MNUM];
	int moveflag[MNUM];//是否存在
	float moverotate[MNUM];
	int movex_edge = 6, movey_edge = 2;
	int score = -20;

	MoveBalls();
	void draw();
};

class GhostBalls{
public:
	float gspeed[GNUM];
	float ghostx[GNUM], ghosty[GNUM], ghostz[GNUM];
	int ghostflag[GNUM];
	float ghostrotate[GNUM];
	int score = 300;

	GhostBalls();
	void draw();
};

class SnitchBall{
private:
	Particles* particle;
public:
	int timer;
	int timer_max = 500;
	int drop_time = 2;// 1/drop_time是落地的概率
	int drop_max = 150;// 落地持续的时间
	int drop_flag;

	float sspeed = 0.4f;
	float snitchx, snitchy, snitchz;
	int snitchflag;
	int snitchrotate;
	int score = 5000;

	void get_new();
	SnitchBall();
	void draw();
};

class Table{
private:
	Tools* tools;
	BITMAP tb_bmp;
	BITMAP wd_bmp;
	GLuint tb_texid;
	GLuint wd_texid;
	void wd_texture_display();
	void tb_texture_display();

	float a_x;
	//float a_y;
	float b_x;
	//float b_y;

public:
	float xedge = 9.0f;
	float yedge = 3.6f;

	Table();
	bool checkBound();
	void move(float dis);
	void draw();
	void setTex();
};

class Carve{
private:
	float x;
	float y;
	float zmin;
	float zmax;
	BITMAP cv_texture;
	Tools* tools;
	GLuint texid;
public:
	Carve();
	void draw();
	void setTex();
};