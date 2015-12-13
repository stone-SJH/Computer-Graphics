#include "Flag.h"

GLfloat Flag::getcurve(int pos){
	GLfloat result;
	if (tp == Sin)
		result = range * sin(pos * theta_inc + theta);
	else if (tp == Cos)
		result = range * cos(pos * theta_inc + theta);

	return result;
}

void Flag::setNormal(const GLfloat v1[3], const GLfloat v2[3], const GLfloat v3[3]) {
	const GLfloat s1[] = {
		v2[0] - v1[0], v2[1] - v1[1], v2[2] - v1[2] };
	const GLfloat s2[] = {
		v3[0] - v1[0], v3[1] - v1[1], v3[2] - v1[2] };
	GLfloat n[] = {
		s1[1] * s2[2] - s1[2] * s2[1],
		s1[2] * s2[0] - s1[0] * s2[2],
		s1[0] * s2[1] - s1[1] * s2[0]
	};

	GLfloat abs = sqrt(n[0] * n[0] + n[1] * n[1] + n[2] * n[2]);
	n[0] /= abs;
	n[1] /= abs;
	n[2] /= abs;
	glNormal3fv(n);
}

void Flag::init(){
	x_inc = (max_x - min_x) / segs;
	t_inc = 1.0f / segs;
	theta_inc = 2 * PI * circles / segs;
	tools = new Tools();
	flag_texture = tools->GetBmp(tex_file);
	segs = ((int)((max_x - min_x) * (512 / 2)));
	range = 2.5f;
	circles = 2;
	theta = 0;
	speed = 5.0f;
	tp = Sin;
}

void Flag::setCircles(int c){
	this->circles = c;
}

void Flag::setRange(float r){
	this->range = r;
}

void Flag::setSegs(int s){
	this->segs = s;
}

void Flag::setType(curvetype t){
	tp = t;
}

void Flag::setSpeed(float s){
	speed = s;
}

void Flag::drawFlag(){
	glPolygonMode(GL_BACK, GL_FILL);
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(10.2, 0.0, -3.0);
	glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
	GLUquadricObj *cylinder_obj_1;
	cylinder_obj_1 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_1, 0.1, 0.1, 3.0, 20, 20);
	glPopMatrix();
	glPopMatrix();
	int i;
	glColor3f(1.0f, 1.0f, 1.0f);
	glPushMatrix();
	glTranslatef(10.2, 0.0, -3.0);
	/*static GLuint texid;
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, flag_texture.bmWidth, flag_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, flag_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid);
	glColor4f(1, 1, 1, 1);*/
	glBegin(GL_QUAD_STRIP);
	for (i = 0; i <= segs; ++i){
		const GLfloat z = getcurve(i);
		const GLfloat
			v1[] = { i*x_inc + min_x, max_y, z },
			v2[] = { i*x_inc + min_x, min_y, z },
			v3[] = {
			(i + 1)*x_inc + min_x,
			max_y,
			getcurve(i + 1) };
		//setNormal(v1, v2, v3);
		//glTexCoord2f(i*t_inc, 1.0f);
		glVertex3fv(v1);
		//glTexCoord2f(i*t_inc, 0.0f);
		glVertex3fv(v2);
	}
	glEnd();
	glPopMatrix();
	//glutSwapBuffers();
}

void Flag::idle(){
	theta += speed * PI / 180.0f;
}

Flag::Flag(){
	pos_x = 0;
	pos_y = 0;
	pos_z = 0;
	max_x = 5.0f;
	min_x = -5.0f;
	max_y = 5.0f;
	min_y = -5.0f;
	tex_file = L"D:/pictures/table.bmp";
	init();
}