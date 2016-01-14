#include "Components.h"

CueBall::CueBall(){
	wx = 0;
	wy = 0;
	wz = -0.6;
	wflag = 0;
	rot = 0;
	directRotate = 0;
	tools = new Tools();
	cb_texture = tools->GetBmp(L"perlin.bmp"); 
}

void CueBall::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid);
	GLfloat matSpecular[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	GLfloat matEmission[] = { 1.0, 1.0, 1.0, 1.0 };
	//»­Ä¸Çò
	glPushMatrix();
	glPushMatrix();
	GLfloat light_position[] = { wx, wy, wz + 1, 1.0 };
	GLfloat ambientLight[] = { 1, 1, 1, 1.0f };
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat spotDir[] = { -1.0f, 0.0f, -1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, ambientLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 20.0f);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2.0);
	glEnable(GL_LIGHTING);
	//glEnable(GL_COLOR_MATERIAL);
	glPopMatrix();
	glTranslatef(wx, wy, wz);
	glRotatef(rot, cos(directRotate / 180 * PI), cos(directRotate / 180 * PI), sin(directRotate / 180 * PI));
	//glutSolidSphere(R, 20, 30);
	GLUquadricObj * sphere = gluNewQuadric();
	gluQuadricOrientation(sphere, GLU_OUTSIDE);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluQuadricTexture(sphere, GL_TRUE);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
	gluSphere(sphere, R, 20, 30);
	gluDeleteQuadric(sphere);
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	//»­¸¨ÖúÏß
	if (!wflag){
		glPushMatrix();
		glTranslatef(wx, wy, wz);
		glColor3f(1.0f, 0.0f, 0.0f);
		glRotatef(directRotate, 0, 0, 1);
		glLineWidth(3);
		glBegin(GL_LINES);
		glVertex3f(0, 0, 1);
		glVertex3f(2, 0, 1);
		glEnd();
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glEnd();
		glTranslatef(2, 0, 1);
		glRotatef(90.0f, 0, 1, 0);
		glutSolidCone(0.1,0.3,10,10);
		glPopMatrix();
	}
}

void CueBall::setTex(){
	/*glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	gluBuild2DMipmaps(GL_TEXTURE_2D, cb_texture.type, cb_texture.imageWidth,
		cb_texture.imageHeight, cb_texture.type, GL_UNSIGNED_BYTE,
		cb_texture.image);*/
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, cb_texture.bmWidth, cb_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, cb_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}

MoveBalls::MoveBalls(){
	for (int i = 0; i < MNUM; i++){
		mspeed[i] = mspeed_edge;
		if (i % 2 == 0){
			movex[i] = -1 * rand(movex_edge * 10) / 10;
			movey[i] = rand(movey_edge * 10) / 10;
		}
		else{
			movex[i] = rand(movex_edge * 10) / 10;
			movey[i] = -1 * rand(movey_edge * 10) / 10;
		}
		moveflag[i] = 1;
		moverotate[i] = rand(360);
		particles[i] = new Particles(movex[i], movey[i], movez[i], 1, 0, 0, moverotate[i]);
	}
}

void MoveBalls::draw(){
	GLfloat matSpecular[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	GLfloat matEmission[] = { 0.0, 0.0, 0.0, 0.0 };
	for (int i = 0; i < MNUM; i++){
		if (moveflag[i]){
			glPushMatrix();
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
			glTranslatef(movex[i], movey[i], movez[i]);
			glutSolidSphere(R, 20, 30);
			glPopMatrix();
			particles[i]->setPos(movex[i], movey[i], movez[i]);
			particles[i]->setRotate(moverotate[i]);
			particles[i]->draw();
			
		}
	}
}

GhostBalls::GhostBalls(){
	ghostx[0] = 5.0f; ghosty[0] = -0.4f;
	ghostx[1] = -3.0f; ghosty[1] = 2.2f;
	ghostx[2] = -4.0f; ghosty[2] = 2.4f;
	ghostx[3] = 1.3f; ghosty[3] = 2.3f;
	ghostx[4] = -2.6f; ghosty[4] = -3.3f;
	ghostx[5] = 2.6f; ghosty[5] = -0.3f;
	for (int i = 0; i < GNUM; i++){
		gspeed[i] = 0;
		ghostflag[i] = 1;
		ghostrotate[i] = 0;
	}
}

void GhostBalls::draw(){
	GLfloat matSpecular[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	GLfloat matEmission[] = { 1, 0.0, 0, 1.0 };
	for (int i = 0; i < GNUM; i++){
		if (ghostflag[i]){
			glPushMatrix();
			glTranslatef(ghostx[i], ghosty[i], ghostz[i]);
			glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
			glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
			glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
			glutSolidSphere(R, 20, 30);
			glPopMatrix();
		}
	}
}

void SnitchBall::get_new(){
	//printf("new!");
	snitchrotate = rand(360);
	if ((snitchrotate % drop_time) == 0){
		//printf("drop!\n");
		drop_flag = 1;
		snitchz = 0.6f;
		snitchx = snitchx * 9 / XEDGE;
		snitchy = snitchy * 3.6 / YEDGE;
	}
	else{
		drop_flag = 0;
		snitchz = 3.5f;
	}
}

SnitchBall::SnitchBall(){
	timer = 0;
	snitchx = 1;
	snitchy = 1;
	snitchflag = 1;
	get_new();
	particle = new Particles(snitchx, snitchy, snitchz, 1, 1, 0, snitchrotate);
}

void SnitchBall::draw(){
	GLfloat matSpecular[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	GLfloat matEmission[] = { 1.0, 1.0, 0.0, 1.0 };
	if (snitchflag){
		glPushMatrix();
		glTranslatef(snitchx, snitchy, snitchz);
		glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
		glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
		glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
		glutSolidSphere(R, 20, 30);
		glPopMatrix();
		particle->setPos(snitchx, snitchy, snitchz);
		particle->setRotate(snitchrotate);
		particle->draw();
	}
}

Carve::Carve(){
	tools = new Tools();
	cv_texture = tools->GetBmp(L"background.bmp");
	x = 15.0f;
	y = 11.0f;
	zmin = -1;
	zmax = 10.0f;
}

void Carve::draw(){
	glDisable(GL_LIGHTING);
	glBindTexture(GL_TEXTURE_2D, texid);
	glEnable(GL_TEXTURE_2D);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(x, y, zmin);
	glTexCoord2f(1, 0); glVertex3f(x, -y, zmin);
	glTexCoord2f(1, 1); glVertex3f(x, -y, zmax);
	glTexCoord2f(0, 1); glVertex3f(x, y, zmax);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-x, y, zmin);
	glTexCoord2f(1, 0); glVertex3f(-x, -y, zmin);
	glTexCoord2f(1, 1); glVertex3f(-x, -y, zmax);
	glTexCoord2f(0, 1); glVertex3f(-x, y, zmax);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(x, y, zmin);
	glTexCoord2f(1, 0); glVertex3f(-x, y, zmin);
	glTexCoord2f(1, 1); glVertex3f(-x, y, zmax);
	glTexCoord2f(0, 1); glVertex3f(x, y, zmax);
	glEnd();
	glPopMatrix();

	glPushMatrix();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(x, -y, zmin);
	glTexCoord2f(1, 0); glVertex3f(-x, -y, zmin);
	glTexCoord2f(1, 1); glVertex3f(-x, -y, zmax);
	glTexCoord2f(0, 1); glVertex3f(x, -y, zmax);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Carve::setTex(){
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, cv_texture.bmWidth, cv_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, cv_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}
Table::Table(){
	tools = new Tools();
	tb_bmp = tools->GetBmp(L"table.bmp");
	wd_bmp = tools->GetBmp(L"wood.bmp");
	a_x = -1.0;
	b_x = 1.0;
}
void Table::setTex(){
	glEnable(GL_TEXTURE_2D);

	glGenTextures(1, &tb_texid);
	glBindTexture(GL_TEXTURE_2D, tb_texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, tb_bmp.bmWidth, tb_bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, tb_bmp.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
	glGenTextures(1, &wd_texid);
	glBindTexture(GL_TEXTURE_2D, wd_texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, wd_bmp.bmWidth, wd_bmp.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, wd_bmp.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

}
void Table::tb_texture_display(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tb_texid);
	glColor4f(1, 1, 1, 1);
	glBegin(GL_QUAD_STRIP);
	glColor3f(0, 0.5, 1);
	glTexCoord2f(1, 0); glVertex3f(10.2, 4.7, -0.5);
	glTexCoord2f(1, 1); glVertex3f(10.2, -4.7, -0.5);
	glTexCoord2f(1 - (10.2 - b_x) / 20.4, 0); glVertex3f(b_x, 4.7, -0.5);
	glTexCoord2f(1 - (10.2 - b_x) / 20.4, 1); glVertex3f(b_x, -4.7, -0.5);
	glColor3f(1, 1, 1);
	glTexCoord2f(1 - (b_x - a_x) / 20.4 - (10.2 - b_x) / 20.4, 0); glVertex3f(a_x, 4.7, -0.5);
	glTexCoord2f(1 - (b_x - a_x) / 20.4 - (10.2 - b_x) / 20.4, 1); glVertex3f(a_x, -4.7, -0.5);
	glColor3f(1, 0.5, 0);
	glTexCoord2f(0, 0); glVertex3f(-10.2, 4.7, -0.5);
	glTexCoord2f(0, 1); glVertex3f(-10.2, -4.7, -0.5);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}
bool Table::checkBound(){
	if (a_x > 10.2 || a_x < -10.2 ||
		b_x > 10.2 || b_x < -10.2)
		return false;
	return true;
}
void Table::move(float dis){
	a_x += dis;
	b_x += dis;
}
void Table::wd_texture_display(){

		glBindTexture(GL_TEXTURE_2D, wd_texid);
		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
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
		//glFlush();
}
void Table::draw(){	
	glColor3f(0.7f, 0.8f, 0.8f);
	//×ÀÍÈ
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
	tb_texture_display();
	glPopMatrix();

	glPushMatrix();
	wd_texture_display();
	glPopMatrix();
}