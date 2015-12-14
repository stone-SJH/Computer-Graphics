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

void Flag::setTex(){
	glGenTextures(1, &texid);
	glBindTexture(GL_TEXTURE_2D, texid);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, flag_texture.bmWidth, flag_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, flag_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glEnable(GL_TEXTURE_2D);
}

void Flag::init(){
	tools = new Tools();
	flag_texture = tools->GetBmp(tex_file);
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
	glPolygonMode(GL_BACK, GL_FILL);
	if (type == 0) glColor3f(1, 0.5, 0);
	else if (type == 1) glColor3f(0, 0.5, 1);
	glPushMatrix();
	glTranslatef(pos_x, pos_y, pos_z);
	glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
	GLUquadricObj *cylinder_obj_1;
	cylinder_obj_1 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_1, 0.1, 0.1, 3.0, 20, 20);
	glPopMatrix();
	glPopMatrix();
	int i;
	glColor3f(1.0f, 0.0f, 1.0f);
	
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, texid);
	glColor4f(1, 1, 1, 1);
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUAD_STRIP);
	if (type == 0) range_now = range;
	else if (type == 1) range_now = 0;
	for (i = 0; i <= segs; ++i){
		GLfloat y = getcurve(i) + 4.7;
		GLfloat x;
		if (type == 0) x = i*x_inc + min_x;
		else if (type == 1) x = i*x_inc + max_x;
		if (type == 0) range_now -= range_inc;
		else if (type == 1) range_now += range_inc;
		const GLfloat
			v1[] = { i*x_inc + min_x, max_z, y },
			v2[] = { i*x_inc + min_x, min_z, y },
			v3[] = {
			(i + 1)*x_inc + min_x,
			max_z,
			getcurve(i + 1) };
		//setNormal(v1, v2, v3);
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
	pos_z = -3.0f;
	max_x = -1.1f;
	min_x = -3.1f;
	max_z = -2.0f;
	min_z = -3.0f;
	type = 0;
	tex_file = L"D:/pictures/table.bmp";
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