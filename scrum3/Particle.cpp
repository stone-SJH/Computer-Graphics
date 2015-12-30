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
			glutSolidSphere(0.04, 10, 10);
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
				p[i].xspeed = speed * cos(rotate / 180 * PI);
				p[i].yspeed = speed * sin(rotate / 180 * PI);
				p[i].zspeed = 0;
				p[i].xdelta = 0;
				p[i].ydelta = 0;
				p[i].zdelta = 0;
			}
		}
	}
}

void Particles::drawCollision(){
	for (int i = 0; i < MAXP; i++){
		if (p[i].active && p[i].life > 0){
			glColor4f(p[i].r, p[i].g, p[i].b, p[i].life);
			glPushMatrix();
			glTranslatef(p[i].x, p[i].y, p[i].z);
			glutSolidSphere(0.04, 10, 10);
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
}

Particles::Particles(){
	x = 0;
	y = 0;
	z = 0;
	r = 0;
	g = 0;
	b = 0;
	protate = 0;
	init_pars();
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
		p[i].xspeed = speed * cos(rotate / 180 * PI);
		p[i].yspeed = speed * sin(rotate / 180 * PI);
		p[i].zspeed = 0;
		/*添加加速度*/
		p[i].xdelta = 0;
		p[i].ydelta = 0;
		p[i].zdelta = 0;
	}
}

Particles::Particles(float xpos, float ypos, float zpos, float rr, float gg, float bb, float rotate){
	x = xpos;
	y = ypos;
	z = zpos;
	r = rr;
	g = gg;
	b = bb;
	protate = rotate + 180;
	init_pars();
}