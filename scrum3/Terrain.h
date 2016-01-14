#ifndef _TERRAIN_
#define _TERRAIN_
#include "Tools.h"
#include "Perlin.h"
#include "Vec.h"

struct Ter{
	float x, y, z;
	float s, t;//ÎÆÀí×ø±ê
	float norx, nory, norz;
};

class Terrain{
private: 
	float xpos;
	float ypos;
	//int zpos;
	int scope;
	float xscale = 0.12f;
	float yscale = 0.088f;
	Ter terrain[250][250];
	Perlin* p;
	LPCTSTR tex_file;
	Tools* tools;
	BITMAP ter_texture;
	GLuint texid1;

	void init();
public:
	Terrain();
	void setTex();
	float getHeight(float x, float y);
	void setPos(float x, float y);
	void draw();
};
#endif