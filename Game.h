#include "Components.h"
#include "macro.h"
#include "Viewer.h"

class Game{
private:
	CueBall* cb;
	MoveBalls* mb;
	GhostBalls* gb;
	SnitchBall* sb;
	Table* tb;
	Viewer* vw;

	int invalid[TOTAL][TOTAL];

	void init();
	void draw_components();
	void edged_move(float& x, float& y, float& speed, float& rotate);
	void ball_collision(float& x1, float& y1, float& speed1, float& rotate1,
		float& x2, float& y2, float& speed2, float& rotate2);
	
	void shoot_idle();
	void collision_check_idle();
	void normal_moveball_idle(float& x, float& y, float& rotate, float& speed);
	void normal_ghostball_idle(float& x, float& y, float& rotate, float& speed);
	//金色飞贼有独特的碰撞检测
	void normal_snitchball_idle();

public:
	void KeyFunc(unsigned char key, int x, int y);
	void MouseFunc(int button, int state, int x, int y);
	void MouseMotion(int x, int y);
	void reshape(int width, int height);
	void redraw();
	void idle();


	Game();
};