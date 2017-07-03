#include <stdio.h>

#include <Windows.h>
#include <GL/glut.h>

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "freeglut.lib")


#define checkImageWidth 64
#define checkImageHeight 64
#define subImageWidth 16
#define subImageHeight 16

static GLubyte checkImage[checkImageHeight][checkImageWidth][4];
static GLubyte subImage[subImageHeight][subImageWidth][4];

static GLuint texName;

void makeCheckImage(void)
{
	int i, j, c;
	for( i = 0; i < checkImageHeight; i++){
		for(j = 0; j < checkImageWidth; j++){
			c = (((i&0x8)==0)^((j&0x8)==0))*255;
			checkImage[i][j][0] =  (GLubyte)c;
			checkImage[i][j][1] =  (GLubyte)c;
			checkImage[i][j][2] =  (GLubyte)c;
			checkImage[i][j][3] =  (GLubyte)255;
		}
	}

	for( i = 0; i < subImageHeight; i++){
		for(j = 0; j < subImageWidth; j++){
			c = (((i&0x4)==0)^((j&0x4)==0))*255;
			subImage[i][j][0] =  (GLubyte)c;
			subImage[i][j][1] =  (GLubyte)0;
			subImage[i][j][2] =  (GLubyte)0;
			subImage[i][j][3] =  (GLubyte)255;
		}
	}
}

void keyboard(unsigned char key , int x, int y)
{
	switch(key){
		case 's':
		case 'S':
			glBindTexture(GL_TEXTURE_2D, texName);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 12, 44,
				subImageWidth, subImageHeight, GL_RGBA, GL_UNSIGNED_BYTE, subImage);
			glutPostRedisplay();
			break;
		case 'r':
		case 'R':
			glBindTexture(GL_TEXTURE_2D, texName);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight, 0,GL_RGBA, GL_UNSIGNED_BYTE, checkImage);
			glutPostRedisplay();
			break;
		case 27:
			exit(0);
			break;
		default:
			break;
	}
}

void init(void)
{
	glClearColor(1.0,1.0,1.0,1.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);	 //  这个非常关键
	makeCheckImage();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, checkImageWidth, checkImageHeight,
		0, GL_RGBA, GL_UNSIGNED_BYTE, checkImage);

	//glLoadIdentity();
	//glOrtho(0.0, 300.0, 0.0, 300.0, -1.0, 1.0);
}

void display(void)
{
	GLint width;

	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texName);
#if 1
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(0.0,0.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3f(1.0,0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3f(1.0,1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(0.0,1.0, 0.0);
	glEnd();


	glPushMatrix();
	glTranslatef(2.0, 0.0, 0.0);

	glBegin(GL_QUADS);

	glTexCoord2f(0.0, 0.0); glVertex3f(0.0,0.0, 0.0);
	glTexCoord2f(0.5, 0.0); glVertex3f(1.0,0.0, 0.0);
	glTexCoord2f(0.5, 0.5); glVertex3f(1.0,1.0, 0.0);
	glTexCoord2f(0.0, 0.5); glVertex3f(0.0,1.0, 0.0);
	glEnd();

	glPopMatrix();

	glPushMatrix();
	glTranslatef(4.0, 0.0, 0.0);

	glBegin(GL_QUADS);
	glTexCoord2f(0.5, 0.0); glVertex3f(0.0,0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3f(1.0,0.0, 0.0);
	glTexCoord2f(1.0, 0.5); glVertex3f(1.0,1.0, 0.0);
	glTexCoord2f(0.5, 0.5); glVertex3f(0.0,1.0, 0.0);
	glEnd();

	glPopMatrix();

#endif

#if 0
	glDisable(GL_TEXTURE_2D);
	glColor3b(255,255,255);
	glBegin(GL_QUADS);

	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(2.0,0.0, 0.0);
	glVertex3f(2.0,2.0, 0.0);
	glVertex3f(0.0,2.0, 0.0);

	glEnd();
#endif

	glFlush();

	glDisable(GL_TEXTURE_2D);
}


void reshap(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(60.0, (GLfloat)w/(GLfloat)h, 1.0, 30.0);
	gluOrtho2D(0.0, 8.0, 8.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//glTranslatef(0.0, 0.0, -3.6);
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
	glutKeyboardFunc(keyboard);
	glutMainLoop();

	return 0;
}