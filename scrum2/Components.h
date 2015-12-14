 #include <Windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <string>
#include <cmath>
#include "macro.h"
#include "Tools.h"

using namespace std;
class CueBall{
public:
	float wx;
	float wy;
	float wspeed = 0.20f;
	float wspeed_edge = 0.20f;//�㶨�ٶ�
	float wspeed_edge2 = 0.08f;//ֹͣģ���ٶ�
	int wflag;
	float directRotate;
	float Rspeed = 5.0f;
	int score = -50;

	CueBall();
	void draw();
};

class MoveBalls{
public:
	float mspeed_edge = 0.08f;//��С�㶨�ٶ�
	float mspeed_edge2 = 0.18f;//����ٶ�
	float mspeed[MNUM];//��ǰ�ٶ�
	float movex[MNUM], movey[MNUM];//λ��
	int moveflag[MNUM];//�Ƿ����
	float moverotate[MNUM];
	int movex_edge = 6, movey_edge = 2;
	int score = -20;

	MoveBalls();
	void draw();
};

class GhostBalls{
public:
	float gspeed[GNUM];
	float ghostx[GNUM], ghosty[GNUM];
	int ghostflag[GNUM];
	float ghostrotate[GNUM];
	int score = 300;

	GhostBalls();
	void draw();
};

class SnitchBall{
public:
	int timer;
	int timer_max = 500;
	int drop_time = 2;// 1/drop_time����صĸ���
	int drop_max = 150;// ��س�����ʱ��
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
	float b_y;

public:
	float xedge = 9.0f;
	float yedge = 3.6f;

	Table();
	void draw();
	void setTex();
};