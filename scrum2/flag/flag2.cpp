/*
#include "GL/glut.h"
#include "math.h"
#define PI 3.141592653589793238462643383279

void Rectangle(void)
{

	glPolygonMode(GL_FRONT, GL_FILL);

	glBegin(GL_POLYGON);
	glColor3f(1.0, 0, 0);
	glVertex2f(0.8, 0.5);
	glVertex2f(-0.8, 0.5);
	glVertex2f(-0.8, -0.5);
	glVertex2f(0.8, -0.5);
	glEnd();

}

void star(float x0, float y0, float d)
{
	float d0 = d*sin(PI / 10) / cos(PI / 5);

	glPolygonMode(GL_FRONT, GL_FILL);

	glColor3f(1.0, 1.0, 0);

	glEnable(GL_POLYGON_SMOOTH);
	glBegin(GL_POLYGON);

	glVertex2f(x0 + d0*cos(3 * PI / 10), y0 + d0*sin(3 * PI / 10));
	glVertex2f(x0 - d*cos(PI / 10), y0 + d*sin(PI / 10));
	glVertex2f(x0 - d0*cos(PI / 10), y0 - d0*sin(PI / 10));
	glVertex2f(x0 - d*sin(PI / 5), y0 - d*cos(PI / 5));
	glVertex2f(x0, y0 - d0);
	glVertex2f(x0 + d*sin(PI / 5), y0 - d*cos(PI / 5));
	glVertex2f(x0 + d0*cos(PI / 10), y0 - d0*sin(PI / 10));
	glVertex2f(x0 + d*cos(PI / 10), y0 + d*sin(PI / 10));
	glVertex2f(x0 + d0*cos(3 * PI / 10), y0 + d0*sin(3 * PI / 10));
	glEnd();

	glBegin(GL_POLYGON);

	glVertex2f(x0, y0 + d);
	glVertex2f(x0 + d0*cos(3 * PI / 10), y0 + d0*sin(3 * PI / 10));
	glVertex2f(x0 - d0*cos(3 * PI / 10), y0 + d0*sin(3 * PI / 10));
	glEnd();
}

void display()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT);

	Rectangle();

	star(-0.58, 0.25, 0.1);

	float x = -0.58;
	float y = 0.25;
	float d = 0.035;
	float l = 0.22;
	float angle = PI / 5;

	for (int i = 1; i<5; i++)
	{
		float x0, y0;
		x0 = x + l*sin(angle*i);
		y0 = y + l*cos(angle*i);
		star(x0, y0, d);
	}

	glFlush();
}

void init()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);

	glColor3f(1.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(200, 150);
	glutCreateWindow("Flag");
	glutDisplayFunc(display);
	init();
	glutMainLoop();
	return 0;

}*/