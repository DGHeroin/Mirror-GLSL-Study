//#include <GL/freeglut.h>
#include <GL/glut.h>

//ȫ�ֱ�������
GLfloat LightPos[] = {5.0f,5.0f,0.0f,1.0f};
GLfloat LightAmbient[] = {0.0f,0.0f,1.0f,1.0f};

void init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_SMOOTH);


}

//��������ĺ���
void drawSphere()
{
	//��Դ����
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHTING);

	glLightfv(GL_LIGHT0,GL_POSITION,LightPos);
	glLightfv(GL_LIGHT0,GL_AMBIENT,LightAmbient);

	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(0,0,2);
	glutSolidSphere(0.5,20,20);
	glPopMatrix();
}

void display()
{
	int i ;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glGetIntegerv(GL_STENCIL_BITS, &i);
	//���ù۲��
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60,1,5,25);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(5,0,6,0,0,0,0,1,0);

	glEnable(GL_DEPTH_TEST);
	glDisable(GL_STENCIL_TEST);
	drawSphere();

#if 1
	glClearStencil(0x0);
	glClear(GL_STENCIL_BUFFER_BIT);
	glStencilFunc(GL_ALWAYS,1,0xFF);
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);  // ����ӿ��Ժ󣬺����ڻ��ƾ��ε����ص�ģ�滺������ֵ��Ϊ1�ˡ�
	glEnable(GL_STENCIL_TEST);
#endif

	glDisable(GL_LIGHTING);
	glColor3f(0.5f,0.5f,0.5f);
	glDepthMask(GL_FALSE);//������Ȼ�����Ϊֻ��
	glRectf(-1.5f,-1.5f,1.5f,1.5f);

	glDepthMask(GL_TRUE);//����������Ȼ�����

#if 1
	glStencilFunc(GL_EQUAL,1,0xFF); // �ھ������������ģ��ֵΪ1�Żᱻ���Ƴ���
	glStencilOp(GL_KEEP,GL_KEEP,GL_REPLACE);
	glScalef(1.0f,1.0f,-1.0f);
	drawSphere();
#endif

	glutSwapBuffers();
}

int main(int argc,char** argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_STENCIL | GLUT_DEPTH);
	glutInitWindowSize(400,400);
	glutInitWindowPosition(100,100);
	glutCreateWindow("Stencil Test");
	init();
	glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	//glutKeyboardFunc(keyboard);
	glutMainLoop();
	return 0;
}