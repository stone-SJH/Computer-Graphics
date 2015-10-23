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

float r = 0.24;

float xedge = 9.0f, yedge = 3.6f;//桌子

int invalid[14][14];

//母球
float wx = 0, wy = 0;
float wspeed = 0.018f;
float wspeed_delta1 = 0.00001f;
float wspeed_delta2 = 0.0001f;
float wspeed_edge = 0.018f;
float wspeed_edge2 = 0.001f;
int wflag = 0;

//移动球
float mspeed_edge = 0.004f;//最小恒定速度
float mspeed[6];//当前速度
float mspeed_delta = 0.00001f;//衰减/增加速度
float movex[6], movey[6];
int moveflag[6];//是否存在
float moverotate[6];
int movex_edge = 6, movey_edge = 2;

//鬼球
float gspeed[6];
float gspeed_delta = 0.00001f;
float ghostx[6], ghosty[6];
int ghostflag[6];
float ghostrotate[6];


//金球
float gx = 0.0f, gy = 0.0f, gz = -3.5f;


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

void moveball_init(){
	for (int i = 0; i < 6; i++){
		mspeed[i] = mspeed_edge;
		if (i % 2 == 0){
			movex[i] = -1 * rand(movex_edge*10) / 10;
			movey[i] = rand(movey_edge*10) / 10;
		}
		else{
			movex[i] = rand(movex_edge*10) / 10;
			movey[i] = -1 * rand(movey_edge*10) / 10;
		}
		moveflag[i] = 1;
		moverotate[i] = rand(360);
	}
}
void ghostball_init(){
	ghostx[0] = 1.0f; ghosty[0] = -0.4f;
	ghostx[1] = -2.0f; ghosty[1] = 2.2f;
	ghostx[2] = -3.0f; ghosty[2] = 2.4f;
	ghostx[3] = 3.3f; ghosty[3] = 2.3f;
	ghostx[4] = -0.6f; ghosty[4] = -3.3f;
	ghostx[5] = 6.6f; ghosty[5] = -0.3f;
	for (int i = 0; i < 6; i++){
		gspeed[i] = 0;
		ghostflag[i] = 1;
		ghostrotate[i] = 0;
	}
}

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

//辅助线
float directRotate = 0;
float Rspeed = 5.0f;
void KeyFunc(unsigned char key, int x, int y)
{
	switch (key)
	{
		case 'a': 
			if (wflag == 0){
				directRotate += Rspeed;
				if (directRotate >= 360)
					directRotate -= 360;
			}
			break;
		case 'd':
			if (wflag == 0){
				directRotate -= Rspeed;
				if (directRotate < 0)
					directRotate += 360;
			}
			break;
		case 's': 
			wflag = 1;
			//printf("------------------\n");
			//printf("%f \n", directRotate);
			break;
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

static BITMAP tb_bmp = GetBmp(L"D:/pictures/table.bmp");
void texture_display(){
	{
		glColor3f(0, 0, 0);
		static GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, tb_bmp.bmWidth, tb_bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tb_bmp.bmBits);
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
static BITMAP wd_bmp = GetBmp(L"D:/pictures/wood.bmp");
void wood_texture_display(){
	{
		glColor3f(0, 0, 0);
		
		static GLuint texid;
		glGenTextures(1, &texid);
		glBindTexture(GL_TEXTURE_2D, texid);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, wd_bmp.bmWidth, wd_bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, wd_bmp.bmBits);
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
	//桌腿
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

	//桌框
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

	//桌面
	glPushMatrix();
	glColor3f(0, 1.0f, 0);
	glScalef(20.0, 9.0, 0.8);
	glutSolidCube(1.0);
	glPopMatrix();

	//桌子纹理
	/*glPolygonMode(GL_FRONT, GL_FILL);
	glPolygonMode(GL_BACK, GL_POINT);

	glPushMatrix();
	glFrontFace(GL_CCW);
	texture_display();
	glPopMatrix();

	glPushMatrix();
	wood_texture_display();
	glPopMatrix();
	*/

	//母球
	glPushMatrix();
	glTranslatef(wx, wy, -0.6);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();

	//鬼球
	for (int i = 0; i < 6; i++){
		if (ghostflag[i]){
			glPushMatrix();
			glTranslatef(ghostx[i], ghosty[i], -0.6);
			glColor3f(0.0f, 0.2f, 0.3f);
			glutSolidSphere(r, 10, 10);
			glPopMatrix();
		}
	}

	//移动球
	for (int i = 0; i < 6; i++){
		if (moveflag[i]){
			glPushMatrix();
			glTranslatef(movex[i], movey[i], -0.6);
			glColor3f(1.0f, 0.3f, 1.0f);
			glutSolidSphere(r, 10, 10);
			glPopMatrix();
		}
	}

	//金球
	glPushMatrix();
	glTranslatef(gx, gy, gz);
	glColor3f(1.0f, 1.0f, 0.0f);
	glutSolidSphere(r, 10, 10);
	glPopMatrix();

	//辅助线
	if (!wflag){
		glPushMatrix();
		glTranslatef(wx, wy, 0);
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

void edged_move(float& x, float& y, float& speed, float& rotate){
	x += speed * cos(rotate / 180 * PI);
	y += speed * sin(rotate / 180 * PI);
	speed -= wspeed_delta1;
	if (speed <= 0)
		speed = 0;
	if (y >= yedge || y <= -1 * yedge){
		rotate = 360 - rotate;
		x += speed * cos(rotate / 180 * PI);
		y += speed * sin(rotate / 180 * PI);
	}
	else if (x >= xedge || x <= -1 * xedge){
		rotate = 540 - rotate;
		x += speed * cos(rotate / 180 * PI);
		y += speed * sin(rotate / 180 * PI);
	}
}
void shoot_idle()
{
	if (wflag == 1){
		/*wx += wspeed * cos(directRotate / 180 * PI);
		wy += wspeed * sin(directRotate / 180 * PI);
		if (wspeed > wspeed_edge2) 
			wspeed -= wspeed_delta1;
		else wspeed -= wspeed_delta2;
		if (wspeed <= 0)
			wspeed = 0;
		if (wy >= yedge || wy <= -1 * yedge){
			directRotate = 360 - directRotate;
			wx += wspeed * cos(directRotate / 180 * PI);
			wy += wspeed * sin(directRotate / 180 * PI);
		}
		else if (wx >= xedge || wx <= -1 * xedge){
			directRotate = 540 - directRotate;
			wx += wspeed * cos(directRotate / 180 * PI);
			wy += wspeed * sin(directRotate / 180 * PI);
		}*/
		edged_move(wx, wy, wspeed, directRotate);
	}
	if (wspeed == 0){
		wflag = 0;
		wspeed = wspeed_edge;
	}
}
//两球碰撞
void ball_collision(float& x1, float& y1, float& speed1, float& rotate1,
					float& x2, float& y2, float& speed2, float& rotate2){
	float krotate = atanf((y2 - y1) / (x2 - x1)) / PI * 180;
	float krotate1 = krotate - rotate1;
	float krotate2 = krotate - rotate2;
	float kx1 = speed1 * cos(krotate1 / 180 * PI);
	float kx2 = speed2 * cos(krotate2 / 180 * PI);
	float ky1 = speed1 * sin(krotate1 / 180 * PI);
	float ky2 = speed2 * sin(krotate2 / 180 * PI);
	float tmp = kx1;
	kx1 = kx2;
	kx2 = tmp;
	speed1 = sqrt(pow(kx1, 2) + pow(ky1, 2));
	speed2 = sqrt(pow(kx2, 2) + pow(ky2, 2));
	float rx1 = kx1 * sin((90 - krotate) / 180 * PI) + ky1 * cos((90 - krotate) / 180 * PI);
	float ry1 = kx1 * cos((90 - krotate) / 180 * PI) + ky1 * sin((90 - krotate) / 180 * PI);
	float rx2 = kx2 * sin((90 - krotate) / 180 * PI) + ky2 * cos((90 - krotate) / 180 * PI);
	float ry2 = kx2 * cos((90 - krotate) / 180 * PI) + ky2 * sin((90 - krotate) / 180 * PI);

	rotate1 = acosf(rx1 / speed1) / PI * 180;
	rotate2 = acosf(rx2 / speed2) / PI * 180;
	if (y2 < y1){
		rotate1 = 360 - rotate1;
		rotate2 = 360 - rotate2;
	}
	for (int c = 0; c < 5; c++){
		edged_move(x1, y1, speed1, rotate1);
		edged_move(x2, y2, speed2, rotate2);
	}
}
//碰撞检测
void collision_check_idle()
{
	//母球和其他球之间的碰撞检测
	if (wflag == 1){
		for (int i = 0; i < 6; i++){
			if (moveflag[i] == 1){
				float distance = sqrt(pow(wx - movex[i], 2) + pow(wy - movey[i], 2));
				if (distance <= 2 * r && invalid[0][i + 7] == 0){
					invalid[0][i + 7] = 1;
					invalid[i + 7][0] = 1;
					ball_collision(wx, wy, wspeed, directRotate, movex[i], movey[i], mspeed[i], moverotate[i]);
				}
				else{
					invalid[0][i + 7] = 0;
					invalid[i + 7][0] = 0;
				}
			}
			if (ghostflag[i] == 1){
				float distance = sqrt(pow(wx - ghostx[i], 2) + pow(wy - ghosty[i], 2));
				if (distance <= 2 * r && invalid[0][i + 1] == 0){
					invalid[0][i + 1] = 1;
					invalid[i + 1][0] = 1;
					ball_collision(wx, wy, wspeed, directRotate, ghostx[i], ghosty[i], gspeed[i], ghostrotate[i]);
				}
				else{
					invalid[0][i + 1] = 0;
					invalid[i + 1][0] = 0;
				}
			}
		}
	}
	//移动球之间\与鬼球的碰撞检测
	for (int i = 0; i < 6; i++){
		if (moveflag[i] == 1){
			for (int j = i + 1; j < 6; j++){
				if (moveflag[j] == 1){
					float distance = sqrt(pow(movex[i] - movex[j], 2) + pow(movey[i] - movey[j], 2));
					if (distance <= 2 * r && invalid[i +7][j + 7] == 0){
						invalid[i + 7][j + 7] = 1;
						invalid[j + 7][i + 7] = 1;
						int tmp = moverotate[i];
						moverotate[i] = moverotate[j];
						moverotate[j] = tmp;
						for (int c = 0; c < 3; c++){
							edged_move(movex[i], movey[i], mspeed[i], moverotate[i]);
							edged_move(movex[j], movey[j], mspeed[j], moverotate[j]);
						}
						/*movex[i] += 5 * mspeed[i] * cos(moverotate[i] / 180 * PI);
						movey[i] += 5 * mspeed[i] * sin(moverotate[i] / 180 * PI);
						movex[j] += 5 * mspeed[j] * cos(moverotate[j] / 180 * PI);
						movey[j] += 5 * mspeed[j] * sin(moverotate[j] / 180 * PI);
						*/
					}
					else{
						invalid[i + 7][j + 7] = 0;
						invalid[j + 7][i + 7] = 0;
					}
				}
			}
			for (int j = 0; j < 6; j++){
				if (ghostflag[j] == 1){
					float distance = sqrt(pow(movex[i]- ghostx[j], 2) + pow(movey[i] - ghosty[j], 2));
					if (distance <= 2 * r && invalid[i + 7][j + 1] == 0){
						invalid[i + 7][j + 1] = 1;
						invalid[j + 1][i + 7] = 1;
						//ball_collision(movex[i], movey[i], mspeed[i], moverotate[i], ghostx[j], ghosty[j], gspeed[j], ghostrotate[j]);
						ghostrotate[j] = moverotate[i];
						moverotate[i] = moverotate[i] + 180;
						gspeed[j] = mspeed[i];
						for (int c = 0; c < 3; c++){
							edged_move(movex[i], movey[i], mspeed[i], moverotate[i]);
							edged_move(ghostx[j], ghosty[j], gspeed[j], ghostrotate[j]);
						}
					}
					else{
						invalid[i + 7][j + 1] = 0;
						invalid[j + 1][i + 7] = 0;
					}
				}
			}
		}
	}
	//鬼球间的碰撞检测
}

void normal_moveball_idle(float& x, float& y, float& rotate, float& speed){
	speed -= mspeed_delta;
	if (speed < mspeed_edge)
		speed += mspeed_edge;
	if (y >= yedge || y <= -1 * yedge){
		rotate = 360 - rotate;
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
	else if (x >= xedge || x <= -1 * xedge){
		rotate = 540 - rotate;
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
	else{
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
	collision_check_idle();
}
void normal_ghostball_idle(float& x, float& y, float& rotate, float& speed){
	if (speed != 0){
		speed -= gspeed_delta;
		if (speed <= 0){
			speed = 0;
			rotate = 0;
		}	
		if (y >= yedge || y <= -1 * yedge){
			rotate = 360 - rotate;
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
		else if (x >= xedge || x <= -1 * xedge){
			rotate = 540 - rotate;
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
		else{
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
	}
	collision_check_idle();
}
int moveflag1 = 0;
int count = 0;
void idle()
{
	shoot_idle();
	//normal_ball_idle(wx, wy, directRotate, wspeed);
	for (int i = 0; i < 6; i++){		
		normal_moveball_idle(movex[i], movey[i], moverotate[i], mspeed[i]);
		normal_ghostball_idle(ghostx[i], ghosty[i], ghostrotate[i], gspeed[i]);
	}
	//collision_check_idle();
	/*
	if (moveflag1 == 0){
		if (gx < 8.0f) gx += 0.2f;
		//if (y2 < 4.0f) y2 += 0.01f;
		count++;
	}
	if (moveflag1 == 1){
		if (gx > -8.0f) gx-= 0.2f;
		//if (y2 > -4.0f) y2 -= 0.01f;
		count++;
	}
	if (gx >= 8.0f)
		moveflag1 = 1;
	if (gx <= -8.0f)
		moveflag1 = 0;
	if (count > 500){
		gz = -0.5f;
	}
	if (count > 550){
		gz = -3.5f;
		count = 0;
	}
	*/
	glutPostRedisplay();
}
GLfloat z = 0.0f;
int tb_flag = 0;
void redraw()
{
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	LookAt();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(80.0f, 1.0f, 0.1f, 0.0f);
	Draw_Table();
	glPopMatrix();
	glFlush();
	glutSwapBuffers();
}
int main(int argc, char *argv[])
{
	moveball_init();
	ghostball_init();
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 14; j++)
			invalid[i][j] = 0;
	}
	glutInit(&argc, argv);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
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
