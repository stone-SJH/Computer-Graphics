#ifndef _MACRO_
#define _MACRO_

#include <gl\glut.h>
#include <windows.h> 
#include <vector>

#define fps 72
#define rand(x) (rand() % x)
#define kSpeed    50.0f        
#define DELTA 0.0012f
#define MNUM 6
#define GNUM 6
#define TNUM 1
#define TOTAL 14
#define PI 3.14
#define R 0.24
#define XEDGE  12
#define YEDGE  6
#define PAD  1
#define TAB 0.5
#define MAXP 400
#define PRRANGE 80 //粒子发散角度
#define SECTION 200
#define VORTEXNUM 6
static GLfloat colors[12][3] = // 彩虹颜色
{
	{ 1.0f, 0.5f, 0.5f }, { 1.0f, 0.75f, 0.5f }, { 1.0f, 1.0f, 0.5f }, { 0.75f, 1.0f, 0.5f },
	{ 0.5f, 1.0f, 0.5f }, { 0.5f, 1.0f, 0.75f }, { 0.5f, 1.0f, 1.0f }, { 0.5f, 0.75f, 1.0f },
	{ 0.5f, 0.5f, 1.0f }, { 0.75f, 0.5f, 1.0f }, { 1.0f, 0.5f, 1.0f }, { 1.0f, 0.5f, 0.75f }
};
enum curvetype{
	Sin,
	Cos,
	Custom
};
#endif