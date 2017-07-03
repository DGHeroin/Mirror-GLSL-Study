#include <stdio.h>

#include <Windows.h>
#include <GL/glew.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")
#pragma comment(lib, "glew32.lib")

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);


	glewInit();

	if (GL_TRUE != glewGetExtension((const GLubyte*) "GL_ARB_fragment_shader"))
	{
		printf("GL_ARB_fragment_shader extension is not available!");
	}
	if (GL_TRUE != glewGetExtension((const GLubyte*) "GL_ARB_vertex_shader"))
	{
		printf("GL_ARB_vertex_shader extension is not available!");
	}
	if (GL_TRUE != glewGetExtension((const GLubyte*) "GL_ARB_shader_objects"))
	{
		printf("GL_ARB_shader_objects extension is not available!");
	}
}

void display(void)
{
	GLdouble eqn[] = {0.0, 1.0, 0.0, 0.0};
	GLdouble eqn2[] = {1.0, 0.0, 0.0, 0.0};

	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0, 1.0, 1.0);
	glPushMatrix();
	glTranslatef(0.0, 0.0, -3.0);
	glRotatef(90.0, 1.0, 0.0, 0.0);
	glutWireSphere(1.0, 20, 16);
	glPopMatrix();
	glClipPlane(GL_CLIP_PLANE0, eqn);
	glEnable(GL_CLIP_PLANE0);
	glClipPlane(GL_CLIP_PLANE1, eqn2);
	glEnable(GL_CLIP_PLANE1);


	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}


int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//��ʾģʽ��ʼ��
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	//���崰�ڴ�С
	glutInitWindowSize(500, 500);
	//���崰��λ��
	glutInitWindowPosition(100,100);
	//��������
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}