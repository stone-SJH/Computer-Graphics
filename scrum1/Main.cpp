#include "Game.h"

Game* game = new Game();

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

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
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
