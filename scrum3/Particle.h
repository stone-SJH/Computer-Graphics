#include "macro.h"
#include "Tools.h"
#include <math.h>

typedef struct
{
	bool    active;
	float    life;
	float    fade;

	float r;
	float g;
	float b;

	float x;
	float y;
	float z;
	float xspeed;
	float yspeed;
	float zspeed;
	float xdelta;
	float ydelta;
	float zdelta;
} Pars;

class Particles{
	Pars p[MAXP];
	Tools* tools;
	BITMAP pt_texture;
	GLuint texid1;
	float x;
	float y;
	float z;
	float r;
	float g;
	float b;
	float protate;

	void pt_texture_display();
	void init_pars();
public:
	Particles();
	Particles(float xpos, float ypos, float zpos);
	Particles(float xpos, float ypos, float zpos, float rr, float gg, float bb, float rotate);
	void setPos(float xpos, float ypos, float zpos);
	void setRotate(float rotate);
	void setColor(float rr, float gg, float bb);
	void setTex();
	void setPositive();
	void setActive();
	void draw();
	void drawCollision();
	void drawVortex();

};

