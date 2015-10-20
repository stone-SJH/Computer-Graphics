#include <GL/glut.h>  
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>  
#include <cmath>
using namespace std;

#define rand(x) (rand() % x)
float fTranslate;
float fRotate;
float fRotateX = 280.0f;
float fRotateY;
float fRotateZ;

float r = 0.2;

float xedge = 9.0f, yedge = 3.6f;//×À×Ó

float wx = 0, wy = 0;//Ä¸Çò
float wspeed = 0.08f;
int wflag = 0;

float x2 = 1.0f, y2 = 3.0f;//ÒÆ¶¯Çò
float x3 = 2.0f, y3 = -2.5f;
int eflag2 = 1;
int eflag3 = 1;
int rotate2 = rand(360);
int rotate3 = rand(360);
float nspeed = 0.04f;

float gx = 0.0f, gy = 0.0f, gz = -3.5f;//½ðÇò
float fScale = 1.0f;

const GLfloat PI = 3.14;

GLboolean mouserdown = GL_FALSE;
GLboolean mouseldown = GL_FALSE;
GLboolean mousemdown = GL_FALSE;

static GLint mousex = 0, mousey = 0;

static GLfloat center[3] = { 0.0f, 0.0f, 0.0f }; 
static GLfloat eye[3]; 

static GLfloat yrotate = 0.01; 
static GLfloat xrotate = PI / 4; 
static GLfloat celength = 10.0f;

static GLfloat mSpeed = 0.04f;
static GLfloat rSpeed = 0.02f;
static GLfloat lSpeed = 0.04f;

// calculate the eye position according to center position and angle,length
void CalEyePostion()
{
	if (yrotate > PI / 2.2) yrotate = PI / 2.2;
	if (yrotate < 0)  yrotate = 0.01;
	if (xrotate > 2 * PI)   xrotate = 0.01;
	if (xrotate < 0)   xrotate = 2 * PI;
	if (celength > 50)  celength = 50;
	if (celength < 5)   celength = 5;
	eye[0] = 0.8*(center[0] + celength * sin(yrotate) * cos(xrotate));
	eye[2] = 1*(center[2] + celength * sin(yrotate) * sin(xrotate));
	eye[1] = 1*(center[1] + celength * cos(yrotate));
}

// center moves
void MoveBackward()
{
	center[0] += mSpeed * cos(xrotate);
	center[2] += mSpeed * sin(xrotate);
	CalEyePostion();
}

void MoveForward()
{
	center[0] -= mSpeed * cos(xrotate);
	center[2] -= mSpeed * sin(xrotate);
	CalEyePostion();
}

// visual angle rotates
void RotateLeft()
{
	xrotate -= rSpeed;
	CalEyePostion();
}

void RotateRight()
{
	xrotate += rSpeed;
	CalEyePostion();
}

void RotateUp()
{
	yrotate += rSpeed;
	CalEyePostion();
}

void RotateDown()
{
	yrotate -= rSpeed;
	CalEyePostion();
}

//¸¨ÖúÏß
float directRotate = 0;
float Rspeed = 5.0;
void KeyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a': directRotate += Rspeed; break;
		case 'd': directRotate -= Rspeed; break;
		case 's': wflag = 1; break;
	}
	glutPostRedisplay();
}

void MouseFunc(int button, int state, int x, int y)
{
	if (state == GLUT_DOWN)
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_TRUE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_TRUE;
		if (button == GLUT_MIDDLE_BUTTON)mousemdown = GL_TRUE;
	}
	else
	{
		if (button == GLUT_RIGHT_BUTTON) mouserdown = GL_FALSE;
		if (button == GLUT_LEFT_BUTTON) mouseldown = GL_FALSE;
		if (button == GLUT_MIDDLE_BUTTON)mousemdown = GL_FALSE;
	}
	mousex = x, mousey = y;
}

void MouseMotion(int x, int y)
{
	if (mouserdown == GL_TRUE)
	{
		xrotate += (x - mousex) / 80.0f;
		yrotate -= (y - mousey) / 120.0f;
	}

	if (mouseldown == GL_TRUE)
	{
		celength += (y - mousey) / 25.0f;
	}
	mousex = x, mousey = y;
	CalEyePostion();
	glutPostRedisplay();
}

void LookAt()
{
	CalEyePostion();
	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], 0, 1, 0);
}

BITMAP GetBmp(LPCTSTR title)
{
	static HBITMAP hbmp = 0;
	hbmp = (HBITMAP)::LoadImage(GetModuleHandle(0), title, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	static BITMAP bmp = { 0 };
	GetObject(hbmp, sizeof(BITMAP), &bmp);
	return bmp;
}


void texture_display(){
	{
		glColor3f(0, 0, 0);
		static BITMAP bmp = GetBmp(L"D:/pictures/table.bmp");
		static GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid);
		glColor4f(1, 1, 1, 1);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-10.2, 4.7, -0.5);
		glTexCoord2f(1, 0); glVertex3f(10.2, 4.7, -0.5);
		glTexCoord2f(1, 1); glVertex3f(10.2, -4.7, -0.5);
		glTexCoord2f(0, 1); glVertex3f(-10.2, -4.7, -0.5);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		glFlush();
	};
}

void wood_texture_display(){
	{
		glColor3f(0, 0, 0);
		static BITMAP bmp = GetBmp(L"D:/pictures/wood.bmp");
		static GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, bmp.bmWidth, bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, bmp.bmBits);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texid);
		glColor4f(1, 1, 1, 1);		

		glFrontFace(GL_CCW);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-10.2, 4.7, 0.5);
		glTexCoord2f(1, 0); glVertex3f(10.2, 4.7, 0.5);
		glTexCoord2f(1, 1); glVertex3f(10.2, 4.7, -0.5);
		glTexCoord2f(0, 1); glVertex3f(-10.2, 4.7, -0.5);
		glEnd();

		glFrontFace(GL_CW);
		glBegin(GL_QUADS);		
		glTexCoord2f(0, 0); glVertex3f(-10.2, -4.7, 0.5);
		glTexCoord2f(1, 0); glVertex3f(10.2, -4.7, 0.5);
		glTexCoord2f(1, 1); glVertex3f(10.2, -4.7, -0.5);
		glTexCoord2f(0, 1); glVertex3f(-10.2, -4.7, -0.5);
		glEnd();

		glFrontFace(GL_CW);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(10.2, -4.7, 0.5);
		glTexCoord2f(1, 0); glVertex3f(10.2, 4.7, 0.5);
		glTexCoord2f(1, 1); glVertex3f(10.2, 4.7, -0.5);
		glTexCoord2f(0, 1); glVertex3f(10.2, -4.7, -0.5);
		glEnd();

		glFrontFace(GL_CCW);
		glBegin(GL_QUADS);
		glTexCoord2f(0, 0); glVertex3f(-10.2, -4.7, 0.5);
		glTexCoord2f(1, 0); glVertex3f(-10.2, 4.7, 0.5);
		glTexCoord2f(1, 1); glVertex3f(-10.2, 4.7, -0.5);
		glTexCoord2f(0, 1); glVertex3f(-10.2, -4.7, -0.5);

		glEnd();

		glDisable(GL_TEXTURE_2D);
		glFlush();
	};
}

void Draw_Table()
{

	//glTranslatef(0, 0, -0.8);
	//glRotated(280.0f, 1.0f, 0.0f, 0.0f);
	//glRotated(40.0f, 0, 1.0f, 0);
	//glRotated(fRotate, 0, 0, 1.0f);


	//glTranslatef(0.0, 0.0, -3.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//×ÀÍÈ
	glColor3f(0.7f, 0.8f, 0.8f);

	glPushMatrix();
	glTranslatef(8, 4.0, 0.0);
	GLUquadricObj *cylinder_obj_1;
	cylinder_obj_1 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_1, 0.5, 0.5, 6.0, 20, 20);
	glPopMatrix();
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-8.0, 4.0, 0.0);
	GLUquadricObj *cylinder_obj_2;
	cylinder_obj_2 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_2, 0.5, 0.5, 6.0, 20, 20);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-8.0, -4.0, 0.0);
	GLUquadricObj *cylinder_obj_3;
	cylinder_obj_3 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_3, 0.5, 0.5, 6.0, 20, 20);
	glPopMatrix();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(8.0, -4.0, 0.0);
	GLUquadricObj *cylinder_obj_4;
	cylinder_obj_4 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_4, 0.5, 0.5, 6.0, 20, 20);
	glPopMatrix();
	glPopMatrix();

	//×À¿ò
	glPushMatrix();
	glTranslatef(0, 4.6, 0);
	glColor3f(0.6, 0.2, 0.0);
	glScalef(20.2, 0.2, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0, -4.6, 0);
	glColor3f(0.6, 0.2, 0.0);
	glScalef(20.2, 0.2, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(10.1, 0, 0);
	glColor3f(0.6, 0.2, 0.0);
	glScalef(0.2, 9.4, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-10.1, 0, 0);
	glColor3f(0.6, 0.2, 0.0);
	glScalef(0.2, 9.4, 1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	//×ÀÃæ
	glPushMatrix();
	glColor3f(0, 1.0f, 0);
	glScalef(20.0, 9.0, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	//×À×ÓÎÆÀí
	glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_POINT);

	glPushMatrix();
	glFrontFace(GL_CCW);
	texture_display();
	glPopMatrix();

	glPushMatrix();
	wood_texture_display();
	glPopMatrix();


	//Ä¸Çò
	glPushMatrix();
	glTranslatef(wx, wy, -0.6);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(r, 16, 16);
	glPopMatrix();

	//¹íÇòs
	glPushMatrix();
	glTranslatef(-3.0, 2.0, -0.6);
	glColor3f(0.0f, 0.3f, 0.3f);
	glutSolidSphere(r, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, -3.0, -0.6);
	glColor3f(0.0f, 0.3f, 0.3f);
	glutSolidSphere(r, 16, 16);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(3.0, 1.0, -0.6);
	glColor3f(0.0f, 0.3f, 0.3f);
	glutSolidSphere(r, 16, 16);
	glPopMatrix();

	//ÒÆ¶¯Çò
	if (eflag2){
		glPushMatrix();
		glTranslatef(x2, y2, -0.6);
		glColor3f(1.0f, 0.3f, 1.0f);
		glutSolidSphere(r, 16, 16);
		glPopMatrix();
	}
	if (eflag3){
		glPushMatrix();
		glTranslatef(x3, y3, -0.6);
		glColor3f(1.0f, 0.3f, 1.0f);
		glutSolidSphere(r, 16, 16);
		glPopMatrix();
	}

	//½ðÇò
	glPushMatrix();
	glTranslatef(gx, gy, gz);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(r, 16, 16);
	glPopMatrix();

	//¸¨ÖúÏß
	if (!wflag){
		glPushMatrix();
		glColor3f(1.0f, 1.0f, 1.0f);
		glRotatef(directRotate, 0, 0, 1);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex3f(0, 0, -0.6);
		glVertex3f(2, 0, -0.6);
		glEnd();
		glPopMatrix();
	}
	glPopMatrix();

}

void reshape(int width, int height)
{
	if (height == 0)
	{
		height = 1;
	}
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(120.0f, (GLfloat)width / (GLfloat)height, 0.1f, 20.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void shoot_idle()
{
	if (wflag == 1){
		wx += wspeed * cos(directRotate / 180 * PI);
		wy += wspeed * sin(directRotate / 180 * PI);
	}
	if (wx >= xedge || wx <= -1*xedge || wy >= yedge || wy <= -1*yedge){
		wx = 0;
		wy = 0;
		wflag = 0;
	}
}
//½öÅö×²¼ì²â
void collision_check_idle(float x, float y, int& flag)
{
	float distance = sqrt(pow(wx - x, 2) + pow(wy - y, 2));
	if (distance <= 2 * r){
		flag = 0;
		wx = 0;
		wy = 0;
		wflag = 0;
	}
}

void normal_moveball_idle(float& x, float& y, int& rotate){
	if (x >= xedge || y >= yedge || x <= -1 * xedge || y <= -1 * yedge){
		rotate = rand(360);
		float xx;
		float yy;
		xx = x + 3 * nspeed * cos((float)rotate / 180 * PI);
		yy = y + 3 * nspeed * sin((float)rotate / 180 * PI);
		if (xx >= xedge || yy >= yedge || xx <= -1 * xedge || yy <= -1 * yedge);
		else{
			x = xx;
			y = yy;
		}
	}
	else{
		x += nspeed * cos((float)rotate / 180 * PI);
		y += nspeed * sin((float)rotate / 180 * PI);
	}
}

int moveflag1 = 0;
int moveflag = 0;
int count = 0;
void idle()
{
	shoot_idle();
	collision_check_idle(x2, y2, eflag2);
	normal_moveball_idle(x2, y2, rotate2);
	collision_check_idle(x3, y3, eflag3);
	normal_moveball_idle(x3, y3, rotate3);

	if (moveflag == 0){
		if (gx < 8.0f) gx += 0.2f;
		//if (y2 < 4.0f) y2 += 0.01f;
		count++;
	}
	if (moveflag == 1){
		if (gx > -8.0f) gx-= 0.2f;
		//if (y2 > -4.0f) y2 -= 0.01f;
		count++;
	}
	if (gx >= 8.0f)
		moveflag = 1;
	if (gx <= -8.0f)
		moveflag = 0;
	if (count > 500){
		gz = -0.5f;
	}
	if (count > 550){
		gz = -3.5f;
		count = 0;
	}

	glutPostRedisplay();
}
GLfloat z = 0.0f;
void redraw()
{
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	LookAt();
	glPushMatrix();
	glColor3f(0, 1.0f, 1.0f);
	//glTranslatef(4.0f, 0.0f, -3.0f);
	glutSolidSphere(0.4, 10, 10);
	glPopMatrix();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(80.0f, 1.0f, 0.1f, 0.0f);
	Draw_Table();
	glPopMatrix();

	glutSwapBuffers();
}
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutInitWindowSize(840, 480);
	glutCreateWindow("work");
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutIdleFunc(idle);
	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMotion);

	glutMainLoop();

	return 0;
}
