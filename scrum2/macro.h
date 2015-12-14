#ifndef MACRO
#define MACRO

#include <gl\glut.h>
#include <windows.h> 

#define rand(x) (rand() % x)
#define DELTA 0.0005f
#define MNUM 6
#define GNUM 6
#define TNUM 1
#define TOTAL 14
#define PI 3.14
#define R 0.24
#define XEDGE  12
#define YEDGE  6
#define PAD  0.6

enum curvetype{
	Sin,
	Cos
};
static GLuint wd_texid;
#endif