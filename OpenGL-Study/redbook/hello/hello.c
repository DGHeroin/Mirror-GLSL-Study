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
	glPushMatrix();

	glBegin(GL_POLYGON);
	glVertex3d(0, 0, 0);
	glVertex3d(75, 0, 0);
	glVertex3d(75, 75, 0);
	glVertex3d(0, 75, 0);
	glEnd();
	glTranslatef(20, 0, 0);
	glPopMatrix();
#endif 
	//glLoadIdentity();

	gluLookAt(5,5,5, 0, 0, 0, 0, 1, 0);
	glLoadIdentity();
	glColor3ub(255, 0, 0);
	glBegin(GL_LINES); // x : R

	glVertex3i(-10,0,0);
	glVertex3i(10,0,0);
	glEnd();
	glColor3ub(0, 0 ,0);
	glBegin(GL_LINES); // y : G
	glVertex3i(0,10,0);
	glVertex3i(0,-10,0);
	glEnd();
	glColor3ub(0,0,255);
	glBegin(GL_LINES); // z: B
	glVertex3i(0,0,-10);
	glVertex3i(0,0,10);
	glEnd();					
	glutWireCube(1.0);

	glFlush();
}

void init(void)
{	
	int value;
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0,1.0,1.0,1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glOrtho(-20.0, 20.0, -20.0, 20.0, -20.0, 20.0);

	glGetIntegerv(GL_RED_BITS, &value);
	glGetIntegerv(GL_RED_BITS, &value);
	glGetIntegerv(GL_RED_BITS, &value);
	glGetIntegerv(GL_RED_BITS, &value);
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