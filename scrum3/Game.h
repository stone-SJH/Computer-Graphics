#ifndef _GAME_
#define _GAME_
#include "Components.h"
#include "macro.h"
#include "Viewer.h"
#include "Mode.h"
#include "Flag.h"
#include "Terrain.h"

struct Point{
	float x;
	float y;
	float z;
};

class Game{
private:
	//int parindex;
	vector<Point*> pts;
	vector<Particles*> ps;
	Particles* ps2[VORTEXNUM];
	CueBall* cb;
	MoveBalls* mb;
	GhostBalls* gb;
	SnitchBall* sb;
	Table* tb;
	Flag* fl0;
	Flag* fl1;
	Viewer* vw;
	GameLogic* gl;
	Terrain* tr;
	Carve* cv;

	bool LightController1;
	bool LightController2;
	bool LightController3;
	bool LightController4;
	bool LightController0;
	bool VortexController;
	bool BackgroundController;

	bool tb_flag;
	int invalid[TOTAL][TOTAL];

	void init();
	void draw_components();
	void edged_move(float& x, float& y, float& speed, float& rotate);
	void ball_collision(float& x1, float& y1, float& speed1, float& rotate1,
		float& x2, float& y2, float& speed2, float& rotate2);
	void ball_drop(float& x, float& y, float& speed, float& rotate, int& flag, int refresh);
	
	void shoot_idle();
	void collision_check_idle();
	void normal_moveball_idle(float& x, float& y, float& rotate, float& speed);
	void normal_ghostball_idle(float& x, float& y, float& rotate, float& speed);
	void normal_snitchball_idle();
	void pocket_check_idle();

public:
	void KeyFunc(unsigned char key, int x, int y);
	void MouseFunc(int button, int state, int x, int y);
	void MouseMotion(int x, int y);
	void reshape(int width, int height);
	void redraw();
	void idle();
	void moveFlag(float dis);
	void setTex();

	Game();
};
#endif