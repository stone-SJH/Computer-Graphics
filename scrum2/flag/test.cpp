#include <gl/glut.h>  
#include <math.h>   
#include <Windows.h>
#define GL_PI 3.1415f  
#define MIN_X       (-20.5f)
#define MAX_X       (20.5f)
#define MIN_Y       (-20.5f)
#define MAX_Y       (20.5f)
#define SEGS        ((int)((MAX_X - MIN_X) * (512/2)))
#define RANGE       (2.5f)
#define CIRCLES     (2.0f)
#define SPEED       (5.0f)
#define PI          (3.1415926f)
#define MAX_CHAR       128
#define FONT_SIZE       64
#define TEXTURE_SIZE    FONT_SIZE

GLint SCREEN_WIDTH = 0;
GLint SCREEN_HEIGHT = 0;

GLint windowWidth = 400;
GLint windowHeight = 300;

GLfloat xRotAngle = 45.0f;
GLfloat yRotAngle = -30.0f;
GLfloat theta = 0.0f;

GLint cullFace = 0;
GLint depthTest = 0; 
GLint outLine = 0; 

void processMenu(int value){
	switch (value){
	case 1:
		depthTest = !depthTest;
		break;
	case 2:
		cullFace = !cullFace;
	case 3:
		outLine = !outLine;
		break;
		break;
	default:
		break;
	}  
	glutPostRedisplay();
}

void setNormal(const GLfloat v1[3], const GLfloat v2[3], const GLfloat v3[3]) {
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

void drawString(const char* str) {
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
		// 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

void selectFont(int size, int charset, const char* face) {
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

void drawCNString(const char* str) {
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (i = 0; i<len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

GLuint drawChar_To_Texture(const char* s) {
	wchar_t w;
	HDC hDC = wglGetCurrentDC();

	// 选择字体字号、颜色
	// 不指定字体名字，操作系统提供默认字体
	// 设置颜色为白色
	selectFont(FONT_SIZE, DEFAULT_CHARSET, "");
	glColor3f(1.0f, 1.0f, 1.0f);

	// 转化为宽字符
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, s, 2, &w, 1);

	// 计算绘制的位置
	int width, x, y;
	GetCharWidth32W(hDC, w, w, &width);    // 取得字符的宽度
	x = (TEXTURE_SIZE - width) / 2;
	y = FONT_SIZE / 8;
	//glWindowPos2iARB(x, y); // 一个扩展函数

	GLuint list = glGenLists(1);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_FOG);
	glDisable(GL_TEXTURE_2D);

	wglUseFontBitmaps(hDC, w, 1, list);
	glCallList(list);
	glDeleteLists(list, 1);

	GLuint texID;
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE4,
		0, 0, TEXTURE_SIZE, TEXTURE_SIZE, 0);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texID;
}

void renderScreen(void){
	GLfloat x, y, z, angle;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	glRotatef(xRotAngle, 1.0f, 0.0f, 0.0f);
	glRotatef(yRotAngle, 0.0f, 1.0f, 0.0f);
	if (depthTest)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	if (cullFace)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	if (outLine)
		glPolygonMode(GL_BACK, GL_LINE);
	else
		glPolygonMode(GL_BACK, GL_FILL);
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;  
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH, GL_NICEST);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(-80.0f, 0.0f, 0.0f);
	glVertex3f(80.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, -80.0f, 0.0f);
	glVertex3f(0.0f, 80.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, -80.0f);
	glVertex3f(0.0f, 0.0f, 80.0f);
	glEnd();

	glPushMatrix();
	glTranslatef(80.0f, 0.0f, 0.0f);
	glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
	glutWireCone(3, 6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 80.0f, 0.0f);
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glutWireCone(3, 6, 10, 10);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.0f, 0.0f, 80.0f);
	glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	glutWireCone(3, 6, 10, 10);
	glPopMatrix();

	glColor3f(0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glTranslatef(-23, 20.0, 0.0);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	GLUquadricObj *cylinder_obj_1;
	cylinder_obj_1 = gluNewQuadric();
	glPushMatrix();
	gluCylinder(cylinder_obj_1, 1.5, 1.5, 90.0, 20, 20);
	glPopMatrix();
	glPopMatrix();

	int i;
	const GLfloat x_inc = (MAX_X - MIN_X) / SEGS;
	const GLfloat t_inc = 1.0f / SEGS;
	const GLfloat theta_inc = 2 * PI * CIRCLES / SEGS;
	/*GLuint texid = drawChar_To_Texture("hello, world!");
	GLfloat color[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_BLEND);
	glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, color);
	//glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, texid);*/
	glBegin(GL_QUAD_STRIP);
	glColor3f(1.0f, 0.0f, 1.0f);
	for (i = 0; i <= SEGS; ++i){
	const GLfloat z = RANGE * sin(i*theta_inc + theta);
	const GLfloat
	v1[] = { i*x_inc + MIN_X, MAX_Y, z },
	v2[] = { i*x_inc + MIN_X, MIN_Y, z },
	v3[] = {
	(i + 1)*x_inc + MIN_X,
	MAX_Y,
	RANGE * sin((i + 1)*theta_inc + theta) };
	setNormal(v1, v2, v3);
	glTexCoord2f(0, 1.0f);
	glVertex3fv(v1);
	glTexCoord2f(1, 0.0f);
	glVertex3fv(v2);
	}
	glEnd();
	glPopMatrix(); 
	glutSwapBuffers();
}


void idle(void) {
	theta += (SPEED * PI / 180.0f);
	glutPostRedisplay();
}

//设置Redering State   
void setupRederingState(void){  
	glClearColor(0.0f, 0.0, 0.0, 1.0f);
	//glColor3f(0.0f, 1.0f, 0.0f);
	glShadeModel(GL_FLAT);
}

void changSize(GLint w, GLint h){
	GLfloat ratio;
	GLfloat coordinatesize = 100.0f; 
	if ((w == 0) || (h == 0))
		return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	ratio = (GLfloat)w / (GLfloat)h;
	if (w<h)
		glOrtho(-coordinatesize, coordinatesize, -coordinatesize / ratio, coordinatesize / ratio, -coordinatesize, coordinatesize);
	else
		glOrtho(-coordinatesize*ratio, coordinatesize*ratio, -coordinatesize, coordinatesize, -coordinatesize, coordinatesize);
	glMatrixMode(GL_MODELVIEW);  
	glLoadIdentity();
}
 
void specialKey(int key, int x, int y){

	if (key == GLUT_KEY_UP){
		xRotAngle -= 5.0f;
	}
	else if (key == GLUT_KEY_DOWN){
		xRotAngle += 5.0f;
	}
	else if (key == GLUT_KEY_LEFT){
		yRotAngle -= 5.0f;
	}
	else if (key == GLUT_KEY_RIGHT){
		yRotAngle += 5.0f;
	}
	glutPostRedisplay();
}

int main(int argc, char* argv[]){
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	SCREEN_WIDTH = glutGet(GLUT_SCREEN_WIDTH); 
	SCREEN_HEIGHT = glutGet(GLUT_SCREEN_HEIGHT);
	glutCreateWindow("OpenGL QuadStrip Demo");  
	glutCreateMenu(processMenu);
	glutAddMenuEntry("Toggle depth test", 1);
	glutAddMenuEntry("Toggle cull face", 2);
	glutAddMenuEntry("Toggle back outline", 3);
	glutAttachMenu(GLUT_RIGHT_BUTTON); 
	glutReshapeWindow(windowWidth, windowHeight);
	glutPositionWindow((SCREEN_WIDTH - windowWidth) / 2, (SCREEN_HEIGHT - windowHeight) / 2);
	glutReshapeFunc(changSize);  
	glutDisplayFunc(renderScreen);
	glutIdleFunc(idle);  
	glutSpecialFunc(specialKey);
	setupRederingState();
	glutMainLoop();
	return 0;
}