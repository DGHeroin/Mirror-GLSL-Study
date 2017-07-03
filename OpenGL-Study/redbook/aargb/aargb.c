#include <stdio.h>

#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

static float rotAngle = 0.0;

void init(void)
{
	GLfloat values[2];
	glGetFloatv(GL_LINE_WIDTH_GRANULARITY, values);
	glGetFloatv(GL_LINE_WIDTH_RANGE, values);

	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
	glLineWidth(1.5);

	glClearColor(0.0, 0.0 , 0.0, 0.0);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.0, 1.0, 0.0);
	glPushMatrix();
	glRotatef(-rotAngle, 0.0, 0.0, 0.1);
	glBegin(GL_LINES);
		glVertex2f(-0.5, 0.5);
		glVertex2f(0.5, -0.5);
	glEnd();
	glPopMatrix();

	glColor3f(0.0, 0.0, 1.0);
	glPushMatrix();
	glRotatef(rotAngle, 0.0, 0.0, 0.1);
	glBegin(GL_LINES);
	glVertex2f(0.5, 0.5);
	glVertex2f(-0.5, -0.5);
	glEnd();
	glPopMatrix();

	glFlush();
}


void reshap(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if( w <= h){
		gluOrtho2D(-1.0, 1.0, -1.0*(GLfloat)h/(GLfloat)w, 1.0*(GLfloat)h/(GLfloat)w);
	}else{
		gluOrtho2D( -1.0*(GLfloat)w/(GLfloat)h, 1.0*(GLfloat)w/(GLfloat)h, -1.0, 1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key , int x, int y)
{
	switch(key){
		case 'r':
		case 'R':
			rotAngle += 20;
			if( rotAngle >=360.0) rotAngle = 0.;
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}


int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//��ʾģʽ��ʼ��
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);//|GLUT_DEPTH);
	//���崰�ڴ�С
	glutInitWindowSize(500,500);
	//���崰��λ��
	glutInitWindowPosition(100,100);
	//��������
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshap);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}