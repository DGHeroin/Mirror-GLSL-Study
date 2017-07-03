#include <stdio.h>

#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

static GLfloat spin = 0.0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,1.0);
	glColor3f(0.0, 0.0, 0.0);
#if 0
	glBegin(GL_POLYGON);
		glVertex3d(25, 25, 0);
		glVertex3d(75, 25, 0);
		glVertex3d(75, 75, 0);
		glVertex3d(25, 75, 0);
	glEnd();
#endif
	glPointSize(100);
	glBegin(GL_POINTS);
		glVertex3d(100, 100, 0);
	glEnd();

	glFlush();
}

void init(void)
{	
	int value;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0.0, 300.0, 0.0, 300.0, -1.0, 1.0);
	
	glGetIntegerv(GL_RED_BITS, &value);
	glGetIntegerv(GL_GREEN_BITS, &value);
	glGetIntegerv(GL_BLUE_BITS, &value);
	glGetIntegerv(GL_ALPHA_BITS, &value);
}

int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_RGB);
	//定义窗口大小
	glutInitWindowSize(300,300);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow("double ");
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}