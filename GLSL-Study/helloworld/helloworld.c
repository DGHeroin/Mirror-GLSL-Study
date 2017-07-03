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
	gluLookAt(0,0,2.0,  0,0,0,  0,1,0);
	//glPushMatrix();
	//glTranslatef(0.0, 0.0, -3.0);
	//glRotatef(90.0, 1.0, 0.0, 0.0);
	//glutWireSphere(1.0, 20, 16);
	glutSolidTeapot(1.0);
	//glPopMatrix();
	


	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
	//定义窗口大小
	glutInitWindowSize(500, 500);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow(argv[0]);
	init();
	if( initShader("v.glsl", "f.glsl") < 0)
	{
		printf("init shader failed");
	}
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}