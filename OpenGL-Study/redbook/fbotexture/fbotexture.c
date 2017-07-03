#include <stdio.h>

#include <Windows.h>
#include <GL/glew.h>
#include <GL/freegult.h>

#pragma  comment(lib, "glew32.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

GLuint framebuffer, texture;


void display(void)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, 256, 256);

	glClearColor(1.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glViewport(0,0, 512, 512);
	glClearColor(0.0, 0.0, 1.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glBlitFramebuffer(0, 0, 255, 255, 0, 0, 255, 255, GL_COLOR_BUFFER_BIT, GL_NEAREST);
	glutSwapBuffers();
}

void init(void)
{	
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}
	
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, TexWidth)
}

int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_RGB);
	//定义窗口大小
	glutInitWindowSize(600,800);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow("double ");
	init();
	glutDisplayFunc(display);
	glutMainLoop();

	return 0;
}