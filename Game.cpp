#include "Game.h"

void Game::init(){
	cb = new CueBall();
	mb = new MoveBalls();
	gb = new GhostBalls();
	sb = new SnitchBall();
	tb = new Table();
	vw = new Viewer();
	tb_flag = 0;
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 14; j++){
			if (i != j)
				invalid[i][j] = 0;
			else
				invalid[i][j] = 1;
		}
	}
}

void Game::draw_components(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	tb->draw();
	cb->draw();
	mb->draw();
	gb->draw();
	sb->draw();
	glPopMatrix();
}

void Game::edged_move(float& x, float& y, float& speed, float& rotate){
	x += speed * cos(rotate / 180 * PI);
	y += speed * sin(rotate / 180 * PI);
	speed -= DELTA;
	if (speed <= 0)
		speed = 0;
	if (y >= tb->yedge || y <= -1 * tb->yedge){
		rotate = 360 - rotate;
		x += speed * cos(rotate / 180 * PI);
		y += speed * sin(rotate / 180 * PI);
	}
	else if (x >= tb->xedge || x <= -1 * tb->xedge){
		rotate = 540 - rotate;
		x += speed * cos(rotate / 180 * PI);
		y += speed * sin(rotate / 180 * PI);
	}
}

void Game::ball_collision(float& x1, float& y1, float& speed1, float& rotate1,
						  float& x2, float& y2, float& speed2, float& rotate2){
	//Ð±Åö
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

	float arccos1 = rx1 / speed1;
	if (arccos1 > 1)
		arccos1 = 1;
	else if (arccos1 < -1)
		arccos1 = -1;
	float arccos2 = rx2 / speed2;
	if (arccos2 > 1)
		arccos2 = 1;
	else if (arccos2 < -1)
		arccos2 = -1;


	rotate1 = acosf(arccos1) / PI * 180;
	rotate2 = acosf(arccos2) / PI * 180;
	if (y2 < y1){
		rotate1 = rotate1;
		rotate2 = 360 - rotate2;
	}
	else if (x2 < x1){
		rotate1 = 360 - rotate1;
		rotate2 = rotate2;
	}
	int count = 0;//¼ì²é´íÎóÇé¿ö
	float distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	while(distance <= 2 * R){
		float delta = (2 * R - distance) / 20;
		if (cos(rotate1 / 180 * PI) <= 0.1 && cos(rotate1 / 180 * PI) > 0)
			x1 += delta;
		else if (cos(rotate1 / 180 * PI) < 0 && cos(rotate1 / 180 * PI) >= -0.1)
			x1 -= delta;
		else if (sin(rotate1 / 180 * PI) <= 0.1 && sin(rotate1 / 180 * PI) > 0)
			y1 += delta;
		else if (sin(rotate1 / 180 * PI) < 0 && sin(rotate1 / 180 * PI) >= -0.1)
			y1 -= delta;

		if (cos(rotate2 / 180 * PI) <= 0.1 && cos(rotate2 / 180 * PI) > 0)
			x2 += delta;
		else if (cos(rotate2 / 180 * PI) < 0 && cos(rotate2 / 180 * PI) >= -0.1)
			x2 -= delta;
		else if (sin(rotate2 / 180 * PI) <= 0.1 && sin(rotate2 / 180 * PI) > 0)
			y2 += delta;
		else if (sin(rotate2 / 180 * PI) < 0 && sin(rotate2 / 180 * PI) >= -0.1)
			y2 -= delta;

		if (count < 5){
			//collision_check_idle();
			edged_move(x1, y1, speed1, rotate1);
			edged_move(x2, y2, speed2, rotate2);
			distance = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
			count++;
		}
		else {
			count = 0;
			rotate2 += 90;
		}
	}
}

void Game::shoot_idle(){
	if (cb->wflag == 1){
		edged_move(cb->wx, cb->wy, cb->wspeed, cb->directRotate);
	}
	if (cb->wspeed == 0){
		cb->wflag = 0;
		cb->wspeed = cb->wspeed_edge;
	}
}

void Game::collision_check_idle(){
	//Ä¸ÇòºÍÆäËûÇòÖ®¼äµÄÅö×²¼ì²â
	if (cb->wflag == 1){
		for (int i = 0; i < MNUM; i++){
			if (mb->moveflag[i] == 1){
				float distance = sqrt(pow(cb->wx - mb->movex[i], 2) + pow(cb->wy - mb->movey[i], 2));
				if (distance <= 2 * R && invalid[0][i + 7] == 0){
					invalid[0][i + 7] = 1;
					invalid[i + 7][0] = 1;
					ball_collision(cb->wx, cb->wy, cb->wspeed, cb->directRotate, 
									mb->movex[i], mb->movey[i], mb->mspeed[i], mb->moverotate[i]);
				}
				else{
					//invalid[0][i + 7] = 0;
					//invalid[i + 7][0] = 0;
				}
			}
			if (gb->ghostflag[i] == 1){
				float distance = sqrt(pow(cb->wx - gb->ghostx[i], 2) + pow(cb->wy - gb->ghosty[i], 2));
				if (distance <= 2 * R && invalid[0][i + 1] == 0){
					invalid[0][i + 1] = 1;
					invalid[i + 1][0] = 1;
					ball_collision(cb->wx, cb->wy, cb->wspeed, cb->directRotate, 
									gb->ghostx[i], gb->ghosty[i], gb->gspeed[i], gb->ghostrotate[i]);
				}
				else{
					//invalid[0][i + 1] = 0;
					//invalid[i + 1][0] = 0;
				}
			}
		}
	}
	//ÒÆ¶¯ÇòÖ®¼ä\Óë¹íÇòµÄÅö×²¼ì²â
	for (int i = 0; i < MNUM; i++){
		if (mb->moveflag[i] == 1){
			for (int j = i + 1; j < MNUM; j++){
				if (mb->moveflag[j] == 1){
					float distance = sqrt(pow(mb->movex[i] - mb->movex[j], 2) + pow(mb->movey[i] - mb->movey[j], 2));
					if (distance <= 2 * R && invalid[i + 7][j + 7] == 0){
						invalid[i + 7][j + 7] = 1;
						invalid[j + 7][i + 7] = 1;
						//
						/*int tmp = mb->moverotate[i];
						mb->moverotate[i] = mb->moverotate[j];
						mb->moverotate[j] = tmp;*/
						ball_collision(mb->movex[i], mb->movey[i], mb->mspeed[i], mb->moverotate[i], mb->movex[j], mb->movey[j], mb->mspeed[j], mb->moverotate[j]);
						/*while (distance <= 2 * R){
							edged_move(mb->movex[i], mb->movey[i], mb->mspeed[i], mb->moverotate[i]);
							edged_move(mb->movex[j], mb->movey[j], mb->mspeed[j], mb->moverotate[j]);
							distance = sqrt(pow(mb->movex[i] - mb->movex[j], 2) + pow(mb->movey[i] - mb->movey[j], 2));
						}*/
					}
					else{
						//invalid[i + 7][j + 7] = 0;
						//invalid[j + 7][i + 7] = 0;
					}
				}
			}
			for (int j = 0; j < GNUM; j++){
				if (gb->ghostflag[j] == 1){
					float distance = sqrt(pow(mb->movex[i] - gb->ghostx[j], 2) + pow(mb->movey[i] - gb->ghosty[j], 2));
					if (distance <= 2 * R && invalid[i + 7][j + 1] == 0){
						invalid[i + 7][j + 1] = 1;
						invalid[j + 1][i + 7] = 1;
						ball_collision(mb->movex[i], mb->movey[i], mb->mspeed[i], mb->moverotate[i], gb->ghostx[j], gb->ghosty[j], gb->gspeed[j], gb->ghostrotate[j]);
						/*gb->ghostrotate[j] = mb->moverotate[i];
						mb->moverotate[i] = mb->moverotate[i] + 180;
						gb->gspeed[j] = mb->mspeed[i];*/
						/*while (distance <= 2 * R){
							edged_move(mb->movex[i], mb->movey[i], mb->mspeed[i], mb->moverotate[i]);
							edged_move(gb->ghostx[j], gb->ghosty[j], gb->gspeed[j], gb->ghostrotate[j]);
							distance = sqrt(pow(mb->movex[i] - gb->ghostx[j], 2) + pow(mb->movey[i] - gb->ghosty[j], 2));
						}*/
					}
					else{
						//invalid[i + 7][j + 1] = 0;
						//invalid[j + 1][i + 7] = 0;
					}
				}
			}
		}
	}
	//¹íÇò¼äµÄÅö×²¼ì²â
	for (int i = 0; i < GNUM - 1; i++){
		if (gb->ghostflag[i] == 1){
			for (int j = i + 1; j < GNUM; j++){
				if (gb->ghostflag[j] == 1){
					float distance = sqrt(pow(gb->ghostx[i] - gb->ghostx[j], 2) + pow(gb->ghosty[i] - gb->ghosty[j], 2));
					if (distance <= 2 * R && invalid[i + 1][j + 1] == 0){
						invalid[i + 1][j + 1] = 1;
						invalid[j + 1][i + 1] = 1;
						ball_collision(gb->ghostx[i], gb->ghosty[i], gb->gspeed[i], gb->ghostrotate[i], gb->ghostx[j], gb->ghosty[j], gb->gspeed[j], gb->ghostrotate[j]);
						/*while (distance <= 2 * R){
							edged_move(gb->ghostx[i], gb->ghosty[i], gb->gspeed[i], gb->ghostrotate[i]);
							edged_move(gb->ghostx[j], gb->ghosty[j], gb->gspeed[j], gb->ghostrotate[j]);
							distance = sqrt(pow(gb->ghostx[i] - gb->ghostx[j], 2) + pow(gb->ghosty[i] - gb->ghosty[j], 2));
						}*/
					}
				}
			}
		}
	}
}

void Game::normal_moveball_idle(float& x, float& y, float& rotate, float& speed){
	collision_check_idle();
	if (speed > mb->mspeed_edge2)
		speed = mb->mspeed_edge2;
	if (speed > mb->mspeed_edge)
		speed -= DELTA;
	if (speed < mb->mspeed_edge)
		speed += DELTA;
	if (y >= tb->yedge || y <= -1 * tb->yedge){
		rotate = 360 - rotate;
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
	else if (x >= tb->xedge || x <= -1 * tb->xedge){
		rotate = 540 - rotate;
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
	else{
		x += speed * cos((float)rotate / 180 * PI);
		y += speed * sin((float)rotate / 180 * PI);
	}
}

void Game::normal_ghostball_idle(float& x, float& y, float& rotate, float& speed){
	collision_check_idle();	
	if (speed != 0){
		speed -= DELTA;
		if (speed <= 0){
			speed = 0;
			rotate = 0;
		}
		if (y >= tb->yedge || y <= -1 * tb->yedge){
			rotate = 360 - rotate;
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
		else if (x >= tb->xedge || x <= -1 * tb->xedge){
			rotate = 540 - rotate;
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
		else{
			x += speed * cos((float)rotate / 180 * PI);
			y += speed * sin((float)rotate / 180 * PI);
		}
	}
}

void Game::normal_snitchball_idle(){
	if (sb->drop_flag == 1){
		float distance = sqrt(pow(cb->wx - sb->snitchx, 2) + pow(cb->wy - sb->snitchy, 2));
		if (distance <= 2 * R){
			sb->snitchflag = 0;
			return;
		}
		else  if (sb->timer == sb->drop_max){
			sb->get_new();
			sb->timer = 0;
		}
	}
	else{
		if (sb->timer == sb->timer_max){
			sb->get_new();
			sb->timer = 0;
		}
		if (sb->snitchx > XEDGE || sb->snitchx < -1 * XEDGE){
			sb->snitchrotate = 540 - sb->snitchrotate;
		}
		if (sb->snitchy > YEDGE || sb->snitchy < -1 * YEDGE){
			sb->snitchrotate = 360 - sb->snitchrotate;
		}
		sb->snitchx += sb->sspeed * cos((float)sb->snitchrotate / 180 * PI);
		sb->snitchy += sb->sspeed * sin((float)sb->snitchrotate / 180 * PI);
	}
	sb->timer++;
}
void Game::KeyFunc(unsigned char key, int x, int y){
	switch (key)
	{
	case 'a':
		if (cb->wflag == 0){
			cb->directRotate += cb->Rspeed;
			while (cb->directRotate >= 360)
				cb->directRotate -= 360;
		}
		break;
	case 'd':
		if (cb->wflag == 0){
			cb->directRotate -= cb->Rspeed;
			while (cb->directRotate < 0)
				cb->directRotate += 360;
		}
		break;
	case 's':
		if (cb->wflag == 0){
			while (cb->directRotate >= 360)
				cb->directRotate -= 360;
			while (cb->directRotate < 0)
				cb->directRotate += 360;
			cb->wflag = 1;
		}
		break;
	}
	glutPostRedisplay();
}

void Game::MouseFunc(int button, int state, int x, int y)
{
	vw->mouse_func(button, state, x, y);
}

void Game::MouseMotion(int x, int y){
	vw->mouse_motion(x, y);
	glutPostRedisplay();
}

void Game::reshape(int width, int height)
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

void Game::redraw()
{
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	vw->look();
	glPushMatrix();
	glColor3f(1.0f, 1.0f, 1.0f);
	glTranslatef(0.0f, 0.0f, 0.0f);
	glRotatef(80.0f, 1.0f, 0.1f, 0.0f);
	draw_components();
	glPopMatrix();
	//glFlush();
	glutSwapBuffers();
}

void Game::idle(){
	for (int i = 0; i < 14; i++){
		for (int j = 0; j < 14; j++){
			if (i != j)
				invalid[i][j] = 0;
			else
				invalid[i][j] = 1;
		}
	}
	shoot_idle();
	for (int i = 0; i < MNUM; i++){
		normal_moveball_idle(mb->movex[i], mb->movey[i], mb->moverotate[i], mb->mspeed[i]);
		normal_ghostball_idle(gb->ghostx[i], gb->ghosty[i], gb->ghostrotate[i], gb->gspeed[i]);
		normal_snitchball_idle();
	}
	glutPostRedisplay();
}


Game::Game(){
	init();
}
