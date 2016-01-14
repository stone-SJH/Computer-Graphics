#include "Flag.h"

GLfloat Flag::getcurve(int pos){
	GLfloat result;
	if (tp == Sin)
		result = range_now * sin(pos * theta_inc + theta);
	else if (tp == Cos)
		result = range_now * cos(pos * theta_inc + theta);
	//cout << result;
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

bool Flag::checkBound(){
	if (pos_x > 10.2 || max_x > 10.2 || min_x > 10.2 ||
		pos_x < -10.2 || max_x < -10.2 || min_x < -10.2)
		return false;
	return true;
}

void Flag::move(float dis){
		this->pos_x += dis;
		this->max_x += dis;
		this->min_x += dis;
}

void Flag::setTex(){
	glGenTextures(1, &texid1);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, flag_texture1.bmWidth, flag_texture1.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, flag_texture1.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	
}

void Flag::init(){
	tools = new Tools();
	flag_texture1 = tools->GetBmp(tex_file);
	segs = ((int)(abs(max_x - min_x) * (512 / 2)));
	range = 0.5f;
	circles = 2;
	theta = 0;
	speed = 5.0f;
	tp = Sin;
	x_inc = abs(max_x - min_x) / segs;
	t_inc = 1.0f / segs;
	theta_inc = 2 * PI * circles / segs;
	range_inc = range / segs;
	range_now = range;
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

void Flag::changeType(int t){
	type = t;
}


void Flag::drawFlag(){
	GLfloat matSpecular[] = { 0.3, 0.0, 0.0, 1.0 };
	GLfloat matShininess[] = { 20.0 };
	GLfloat matEmission1[] = { 1.0, 0.5, 0.0, 1.0 };
	GLfloat matEmission2[] = { 0, 0.1, 0.1, 1.0 };
	GLfloat matEmission[] = { 0.3, 0.2, 0.3, 1 };
	glPolygonMode(GL_BACK, GL_FILL);
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
	GLUquadricObj *cylinder_obj_1;
	cylinder_obj_1 = gluNewQuadric();
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	if (type == 0)
		glMaterialfv(GL_FRONT, GL_EMISSION, matEmission1);
	else if (type == 1)
		glMaterialfv(GL_FRONT, GL_EMISSION, matEmission2);
	gluCylinder(cylinder_obj_1, 0.1, 0.1, 8.0, 20, 20);
	glPopMatrix();
	glPopMatrix();

	int i;	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texid1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	glMaterialfv(GL_FRONT, GL_SPECULAR, matSpecular);
	glMaterialfv(GL_FRONT, GL_SHININESS, matShininess);
	glMaterialfv(GL_FRONT, GL_EMISSION, matEmission);
	if (type == 0) range_now = range;
	else if (type == 1) range_now = 0;
	for (i = 0; i <= segs; ++i){
		GLfloat y = getcurve(i) + 4.7;
		GLfloat x;
		GLfloat xx;
		if (type == 0){
			x = i*x_inc + min_x;
			xx = (i + 1) * x_inc + min_x;
		}
		else if (type == 1){
			x = i*x_inc + max_x;
			xx = (i + 1) * x_inc + max_x;
		}
		if (type == 0) range_now -= range_inc;
		else if (type == 1) range_now += range_inc;
		const GLfloat
			v1[] = { x, y, max_z },
			v2[] = { x, y, min_z },
			v3[] = { xx, getcurve(i+1) + 4.7, min_z };
		setNormal(v1, v2, v3);
		glTexCoord2f(i*t_inc, 1.0f);
		glVertex3f(x, y, min_z);
		glTexCoord2f(i*t_inc, 0.0f);
		glVertex3f(x, y, max_z);
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void Flag::idle(){
	theta += speed * PI / 180.0f;
}

Flag::Flag(){
	pos_x = -1.0f;
	pos_y = 4.7f;
	pos_z = -2.0f;
	max_x = -1.1f;
	min_x = -5.1f;
	max_z = 6.0f;
	min_z = 4.0f;
	type = 0;
	tex_file = L"flag1.bmp";
	//tex_file = L"D:\\pictures\\color1.bmp";
	init();
}

Flag::Flag(float posx, float posy, float posz, float maxx, float maxz, float minx, float minz, LPCTSTR texfile, int tpe){
	pos_x = posx;
	pos_y = posy;
	pos_z = posz;
	max_x = maxx;
	min_x = minx;
	max_z = maxz;
	min_z = minz;
	type = tpe;
	tex_file = texfile;
	init();
}