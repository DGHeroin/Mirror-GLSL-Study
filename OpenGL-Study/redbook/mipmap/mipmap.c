#include <stdio.h>

#include <Windows.h>
#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")

GLubyte mipmapImage32[32][32][4];
GLubyte mipmapImage16[16][16][4];
GLubyte mipmapImage8[8][8][4];
GLubyte mipmapImage4[4][4][4];
GLubyte mipmapImage2[2][2][4];
GLubyte mipmapImage1[1][1][4];

static GLuint texName;

void makeCheckImage(void)
{
	int i, j, c;
	for( i = 0; i < 32; i++){
		for(j = 0; j < 32; j++){
			mipmapImage32[i][j][0] =  (GLubyte)255;
			mipmapImage32[i][j][1] =  (GLubyte)255;
			mipmapImage32[i][j][2] =  (GLubyte)0;
			mipmapImage32[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			mipmapImage16[i][j][0] =  (GLubyte)255;
			mipmapImage16[i][j][1] =  (GLubyte)0;
			mipmapImage16[i][j][2] =  (GLubyte)255;
			mipmapImage16[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < 8; i++){
		for(j = 0; j < 8; j++){
			mipmapImage8[i][j][0] =  (GLubyte)255;
			mipmapImage8[i][j][1] =  (GLubyte)0;
			mipmapImage8[i][j][2] =  (GLubyte)0;
			mipmapImage8[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < 4; i++){
		for(j = 0; j < 4; j++){
			mipmapImage4[i][j][0] =  (GLubyte)0;
			mipmapImage4[i][j][1] =  (GLubyte)255;
			mipmapImage4[i][j][2] =  (GLubyte)0;
			mipmapImage4[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < 2; i++){
		for(j = 0; j < 2; j++){
			mipmapImage2[i][j][0] =  (GLubyte)0;
			mipmapImage2[i][j][1] =  (GLubyte)0;
			mipmapImage2[i][j][2] =  (GLubyte)255;
			mipmapImage2[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < 1; i++){
		for(j = 0; j < 1; j++){
			mipmapImage1[i][j][0] =  (GLubyte)255;
			mipmapImage1[i][j][1] =  (GLubyte)255;
			mipmapImage1[i][j][2] =  (GLubyte)255;
			mipmapImage1[i][j][3] =  (GLubyte)255;
		}
	}
	
}


void init(void)
{
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);

	glTranslatef(0.0, 0.0, -3.6);
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 32, 32,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage32);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 16, 16,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage16);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage8);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 4, 4,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage4);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage2);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1,
		0, GL_RGBA, GL_UNSIGNED_BYTE, mipmapImage1);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glEnable(GL_TEXTURE_2D);
}


void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);

	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-2.0,-1.0, 0.0);
	glTexCoord2f(0.0, 8.0); glVertex3f(-2.0,1.0, 0.0);
	glTexCoord2f(8.0, 8.0); glVertex3f(2000.0,1.0, -6000.0);
	glTexCoord2f(8.0, 0.0); glVertex3f(2000.0,1.0, -6000.0);
	glEnd();
	glFlush();
}


void reshap(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 30.0);
	//gluOrtho2D(0.0, 8.0, 8.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -3.6);
}



int main(int argc,  char** argv)
{
	glutInit(&argc,argv);
	//显示模式初始化
	glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB|GLUT_RGB);
	//定义窗口大小
	glutInitWindowSize(600,600);
	//定义窗口位置
	glutInitWindowPosition(100,100);
	//创建窗口
	glutCreateWindow(argv[0]);
	init();
	glutDisplayFunc(display);
	glutReshapeFunc(reshap);
	glutMainLoop();

	return 0;
}