#include <stdio.h>

#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

static int spin = 0;

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
}

void display(void)
{
	GLfloat position[] = {0.0, 0.0, 1.5, 1.0};

	glClear(GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glTranslatef(0.0, 0.0, -5.0);

	glPushMatrix();
	//glLightfv(GL_LIGHT0, GL_POSITION, position);
	glRotated((GLdouble)spin, 1.0, 0.0, 0.0);
	//glTranslatef(0.0, 0.0, 1.5);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glTranslatef(0.0, 0.0, 1.5);
	glDisable(GL_LIGHTING);
	glColor3f(0.0, 1.0, 1.0);
	glutWireCube(0.1);
	glEnable( GL_LIGHTING);
	glPopMatrix();

	glutSolidTorus(0.275, 0.85, 8, 15);
	glPopMatrix();
	glFlush();

	glutSolidSphere(1.0, 20, 16);
	glFlush();
}


void reshap(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void mouse(int button, int state, int x, int y)
{
	switch(button){
		case GLUT_LEFT_BUTTON:
			if( state == GLUT_DOWN){
				spin = (spin+30)%360;
				glutPostRedisplay();
			}
			break;
		default:
			break;
	}
}


int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//定义窗口大小
	glutInitWindowSize(500,500);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshap);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}