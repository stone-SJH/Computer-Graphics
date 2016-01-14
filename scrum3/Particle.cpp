#include "Particle.h"

void Particles::init_pars(){
	float rotate;
	float speed;
	for (int i = 0; i < MAXP; i++){
		p[i].active = true;
		p[i].life = 1.0f;
		p[i].fade = float(rand(10)) / 10.0f + 0.1f;
		p[i].r = r;
		p[i].g = g;
		p[i].b = b;
		p[i].x = x;
		p[i].y = y;
		p[i].z = z;
		rotate = protate + rand(PRRANGE) - PRRANGE / 2;
		speed = rand(50) / 100 + 0.1f;
		p[i].xspeed = speed * cos(rotate / 180 * PI);
		p[i].yspeed = speed * sin(rotate / 180 * PI);
		p[i].zspeed = 0;
		/*添加加速度*/
		p[i].xdelta = 0;
		p[i].ydelta = 0;
		p[i].zdelta = 0;
	}
}

void Particles::setPos(float xpos, float ypos, float zpos){
	x = xpos;
	y = ypos;
	z = zpos;
}

void Particles::setRotate(float rotate){
	protate = rotate + 180;
}

void Particles::setColor(float rr, float gg, float bb){
	r = rr;
	g = gg;
	b = bb;
}

void Particles::setActive(){
	for (int i = 0; i < MAXP; i++)
		p[i].active = true;
}

void Particles::setPositive(){
	for (int i = 0; i < MAXP; i++)
		p[i].active = false;
}

void Particles::draw(){
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glDisable(GL_LIGHTING);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	for (int i = 0; i < MAXP; i++){
		float distance = sqrt(pow(p[i].x - x, 2) + pow(p[i].y - y, 2));
		if (distance < 2 * R)
			p[i].active = false;
		else
			p[i].active = true;
		if (p[i].active){
			glColor4f(p[i].r, p[i].g, p[i].b, p[i].life);
			glPushMatrix();
			glTranslatef(p[i].x, p[i].y, p[i].z);
			glBegin(GL_TRIANGLE_STRIP);
			glTexCoord2d(1, 1); glVertex3f(0.13f, 0.15f, 0);
			glTexCoord2d(0, 1); glVertex3f(0, 0.15f, 0);
			glTexCoord2d(1, 0); glVertex3f(0.13f, 0, 0);
			glTexCoord2d(0, 0); glVertex3f(0, 0, 0);
			glEnd();
			glPopMatrix();
			p[i].x += p[i].xspeed;
			p[i].y += p[i].yspeed;
			p[i].z += p[i].zspeed;
			p[i].xspeed += p[i].xdelta;
			p[i].yspeed += p[i].ydelta;
			p[i].zspeed += p[i].zdelta;
			p[i].life -= p[i].fade;
			if (p[i].life <= 0){
				p[i].life = 1.0f;
				p[i].fade = float(rand(10)) / 10.0f + 0.1f;
				p[i].r = r;
				p[i].g = g;
				p[i].b = b;
				p[i].x = x;
				p[i].y = y;
				p[i].z = z;
				float rotate = protate + rand(PRRANGE) - PRRANGE / 2;
				float speed = rand(50) / 100 + 0.1f;
				float ratio = rand(100) / 100;
				p[i].xspeed = speed * cos(rotate / 180 * PI);
				p[i].yspeed = speed * sin(rotate / 180 * PI);
				p[i].zspeed = 0;
				p[i].xdelta = 0;
				p[i].ydelta = 0;
				p[i].zdelta = 0;
			}
		}
	}
	glDisable(GL_BLEND);
	glEnable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
}

void Particles::drawVortex(){
	glDisable(GL_LIGHTING);
	for (int i = 0; i < MAXP; i++){
		if (p[i].active && p[i].life > 0){
			if (i % 2 == 0)
				glColor4f(0, 0, 0, p[i].life);
			else
				glColor4f(0, 0, 1, p[i].life);
			glPushMatrix();
			glTranslatef(p[i].x, p[i].y, p[i].z);
			glutSolidSphere(R/3, 20, 30);
			glPopMatrix();
			p[i].x += p[i].xspeed;
			p[i].y += p[i].yspeed;
			p[i].z += p[i].zspeed;
			p[i].xspeed += p[i].xdelta;
			p[i].yspeed += p[i].ydelta;
			p[i].zspeed += p[i].zdelta;
			p[i].life -= p[i].fade;
			if (p[i].life <= 0){
				p[i].active = true;
				p[i].life = 1.0f;
				p[i].fade = float(rand(10)) / 10.0f + 0.1f;
				p[i].r = colors[i % 12][0];
				p[i].g = colors[i % 12][1];
				p[i].b = colors[i % 12][2];
				p[i].x = x;
				p[i].y = y;
				p[i].z = z;
				float rotate;
				float speed;
				rotate = rand(360);
				speed = rand(50) / 100 + 0.1f;
				float ratio = 0.5 + rand(100) / 100;
				p[i].xspeed = speed * cos(rotate / 180 * PI) * ratio;
				p[i].yspeed = speed * sin(rotate / 180 * PI) * ratio;
				int escape = rand(25);
				if (escape % 6 == 0)
					p[i].zspeed = 0.2f *ratio;
				else 
					p[i].zspeed = 0.1f * ratio;
				/*添加加速度*/
				p[i].xdelta = DELTA;
				p[i].ydelta = DELTA;
				p[i].zdelta = DELTA;
			}
		}
	}
	glEnable(GL_LIGHTING);
}

void Particles::drawCollision(){
	glDisable(GL_LIGHTING);
	for (int i = 0; i < MAXP; i++){
		if (p[i].active && p[i].life > 0){
			glColor4f(p[i].r, p[i].g, p[i].b, p[i].life);
			glPushMatrix();
			glTranslatef(p[i].x, p[i].y, p[i].z);
			glBegin(GL_LINE_STRIP);
			for (int count = 0; count <= SECTION; count++)
			{
				glVertex3f(0.1*cos(count*PI * 2 / SECTION), 0.1*sin(count*PI * 2 / SECTION), 0);
			}
			glEnd();
			glPopMatrix();
			p[i].x += p[i].xspeed;
			p[i].y += p[i].yspeed;
			p[i].z += p[i].zspeed;
			p[i].xspeed += p[i].xdelta;
			p[i].yspeed += p[i].ydelta;
			p[i].zspeed += p[i].zdelta;
			p[i].life -= p[i].fade;
		}
	}
		glEnable(GL_LIGHTING);
}

Particles::Particles(){
	tools = new Tools();
	pt_texture = tools->GetBmp(L"D:\\pictures\\fire.bmp");
	x = 0;
	y = 0;
	z = 0;
	r = 0;
	g = 0;
	b = 0;
	protate = 0;
	init_pars();
	setTex();
}

Particles::Particles(float xpos, float ypos, float zpos){
	x = xpos;
	y = ypos;
	z = zpos;
	protate = 0;
	float rotate;
	float speed;
	for (int i = 0; i < MAXP; i++){
		p[i].active = true;
		p[i].life = 1.0f;
		p[i].fade = float(rand(10)) / 10.0f + 0.1f;
		p[i].r = colors[i % 12][0];
		p[i].g = colors[i % 12][1];
		p[i].b = colors[i % 12][2];
		p[i].x = x;
		p[i].y = y;
		p[i].z = z;
		rotate = rand(360);
		speed = rand(50) / 100 + 0.1f;
		float ratio = 0.5 + rand(100) / 100;
		p[i].xspeed = speed * cos(rotate / 180 * PI) * ratio;
		p[i].yspeed = speed * sin(rotate / 180 * PI) * ratio;
		p[i].zspeed = 0.2f * ratio;
		/*添加加速度*/
		p[i].xdelta = DELTA;
		p[i].ydelta = DELTA;
		p[i].zdelta = DELTA;
	}
}

Particles::Particles(float xpos, float ypos, float zpos, float rr, float gg, float bb, float rotate){
	tools = new Tools();
	pt_texture = tools->GetBmp(L"D:\\pictures\\fire.bmp");
	x = xpos;
	y = ypos;
	z = zpos;
	r = rr;
	g = gg;
	b = bb;
	protate = rotate + 180;
	init_pars();
	setTex();
}

void Particles::setTex(){
	glGenTextures(1, &texid1);
	glBindTexture(GL_TEXTURE_2D, texid1);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, pt_texture.bmWidth, pt_texture.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, pt_texture.bmBits);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
}