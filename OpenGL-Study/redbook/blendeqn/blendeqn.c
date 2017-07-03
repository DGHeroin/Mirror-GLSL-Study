#include <stdio.h>

#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")


void init(void)
{
	glClearColor(1.0, 1.0, 0.0, 0.0);
	glBlendFunc(GL_ONE, GL_ONE);

	
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 1.0);
	glRectf(-0.5, -0.5, 0.5, 0.5);
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if( w <= h){
		glOrtho(-1.5, 1.5, -1.5*(GLfloat)h/(GLfloat)w, 1.5*(GLfloat)h/(GLfloat)w, -10.0, 10.0);
	}else{
		glOrtho( -1.5*(GLfloat)w/(GLfloat)h, 1.5*(GLfloat)w/(GLfloat)h, -1.5, 1.5,-10.0, 10.0);
	}

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void keyboard(unsigned char key , int x, int y)
{
	switch(key){
		case 'a':
		case 'A':
			glBend
			break;
		case 'o':
		case 'O':
			currentGenMode = GL_OBJECT_LINEAR;
			currentPlane = GL_OBJECT_PLANE;
			glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, currentGenMode);
			glTexGenfv(GL_S, currentPlane, currentCoeff);
			glutPostRedisplay();
			break;
		case 's':
		case 'S':
			currentCoeff = slanted;
			glTexGenfv(GL_S, currentPlane, currentCoeff);
			glutPostRedisplay();
			break;
		case 'x':
		case 'X':
			currentCoeff = xequalzero;
			glTexGenfv(GL_S, currentPlane, currentCoeff);
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
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_DEPTH);
	//定义窗口大小
	glutInitWindowSize(256,256);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}