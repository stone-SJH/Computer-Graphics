#include "Game.h"
//#include "Tools.h"

Game* game;
//Tools* tools = new Tools();

void KeyFunc(unsigned char key, int x, int y){
	game->KeyFunc(key, x, y);
}

void MouseFunc(int button, int state, int x, int y){
	game->MouseFunc(button, state, x, y);
}

void MouseMotion(int x, int y){
	game->MouseMotion(x, y);
}

void reshape(int width, int height){
	game->reshape(width, height);
}

void redraw(){
	game->redraw();
}

void idle(){
	game->idle();
}

void timer(int value){
	idle();
	glutTimerFunc(40, timer, 1);
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1440, 1024);
	glutCreateWindow("work");
	glEnable(GL_DEPTH_TEST);

	game = new Game();
	glutDisplayFunc(redraw);
	glutReshapeFunc(reshape);
	glutTimerFunc(40, timer, 1);
	glutKeyboardFunc(KeyFunc);
	glutMouseFunc(MouseFunc);
	glutMotionFunc(MouseMotion);

	game->setTex();

	glutMainLoop();
	
	return 0;
}
