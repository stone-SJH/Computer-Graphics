#include "macro.h"

class GameLogic{
public:
	int total_score;
	long time_limit = 1000000;
	long time;
	int win_flag;

	GameLogic();
	void draw_ui();//to be done
};