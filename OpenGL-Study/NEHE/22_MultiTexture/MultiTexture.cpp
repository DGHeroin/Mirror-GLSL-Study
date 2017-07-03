/*
 *		��Щ��������Jeff Molofee ��2000��д�ģ��ٴ��ұ����лFredric Echols����Ϊ��ʹ��Щ���뿴���������͸�Ч����ʹ������ʹ�á�
 *		����㷢����Щ�����������������������ҵ�վ�� http://nehe.gamedev.net/default.asp ��������֪����
 *********************************************************************************************************************************
 *		����texture.cpp�ļ��޸�
 *		����ĵ�����DancingWind����ģ������ʲô������e-mail:zhouwei02@mails.tsinghua.edu.cn
 *		//old code			//�����˴�����//ɾ��
 *		/********************************************************************************************************************/	/*
 *		new code			//�µĴ���λ������/*************/ /*֮�䣬���ڲ���
 *		/********************************************************************************************************************/	/*
/***********************************����Ĵ���***********************************************************************************/
/********************************************************************************************************************************/

/*********************�������ӵĿ��ļ�*******************************************************************************************/
#pragma comment( lib, "opengl32.lib" )				
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glew32.lib")
#pragma comment( lib, "glaux.lib")
//#pragma comment( lib, "vfw32.lib" )	
/********************************************************************************************************************************/

#include <windows.h>		// Windows��ͷ�ļ�
#include <GL/glew.h>			// �������µ�gl.h,glu.h��
//#include <glut.h>			// ����OpenGLʵ�ÿ�
#include <stdio.h>			// ��׼����/������ͷ�ļ�
#include <math.h>			// ��ѧ�⺯��
#include <GL/glaux.h>			// GLaux���ͷ�ļ�
GLfloat		xrot;			// X ��ת��
GLfloat		yrot;			// Y ��ת��
GLfloat		zrot;			// Z ��ת��
/***********************************����Ĵ���***********************************************************************************/
#include <string.h>													// �ַ���������
#define MAX_EMBOSS (GLfloat)0.01f									// ������ͻ������ֵ
#define __ARB_ENABLE true											// ʹ���������Ƿ�ʹ��ARB��չ
// #define EXT_INFO													// ��ע��ȥ��,����������ʱ�鿴�����չ
#define MAX_EXTENSION_SPACE 10240									// ������չ�ַ�
#define MAX_EXTENSION_LENGTH 256									// ÿ����չ�ַ������ĳ���
bool multitextureSupported=false;									// ��־�Ƿ�֧�ֶ�����Ⱦ
bool useMultitexture=true;											// ���֧��,�Ƿ�ʹ����
GLint maxTexelUnits=1;												// ������Ԫ�ĸ���

PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC	glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB	= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;

GLuint  filter=1;													// �������������
GLuint  texture[3];													// ������������
GLuint  bump[3];													//����������͹����
GLuint  invbump[3];													// ����������ת�˵İ�͹����
GLuint  glLogo;														// glLogo�����־
GLuint  multiLogo;													// multiLogo������������־
GLfloat LightAmbient[]	= { 0.2f, 0.2f, 0.2f};						// ������
GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f};						// �����
GLfloat LightPosition[]	= { 0.0f, 0.0f, 2.0f};						// ��Դλ��
GLfloat Gray[]		= { 0.5f, 0.5f, 0.5f, 1.0f};
bool	emboss=false;												// �Ƿ�ʹ�ð�͹��ͼ
bool    bumps=true;													// �Ƿ���ʵ��͹��ͼЧ��
GLfloat	z=-5.0f;													// ������Ļ5����λ
GLfloat xspeed;														// X ����ת�ٶ�
GLfloat yspeed;														// Y ����ת�ٶ�

// ����������������

GLfloat data[]= {
	// ǰ��
	0.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	1.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, +1.0f,
	0.0f, 1.0f,		-1.0f, +1.0f, +1.0f,
	// ����
	1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
	1.0f, 1.0f,		-1.0f, +1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, -1.0f,
	// ����
	0.0f, 1.0f,		-1.0f, +1.0f, -1.0f,
	0.0f, 0.0f,		-1.0f, +1.0f, +1.0f,
	1.0f, 0.0f,		+1.0f, +1.0f, +1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	// ����
	1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, -1.0f, -1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	1.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	// ����
	1.0f, 0.0f,		+1.0f, -1.0f, -1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, +1.0f, +1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	// ����
	0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
	1.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	1.0f, 1.0f,		-1.0f, +1.0f, +1.0f,
	0.0f, 1.0f,		-1.0f, +1.0f, -1.0f
};

bool isInString(char *string, const char *search) {
	int pos=0;
	int maxpos=strlen(search)-1;
	int len=strlen(string);
	char *other;
	for (int i=0; i<len; i++) {
		if ((i==0) || ((i>1) && string[i-1]=='\n')) {				// �µ���չ����ʼ������
			other=&string[i];
			pos=0;													// ��ʼ�µıȽ�
			while (string[i]!='\n') {								// �Ƚ�������չ��
				if (string[i]==search[pos]) pos++;					// ��һ���ַ�
				if ((pos>maxpos) && string[i+1]=='\n') return true;	// ���������չ����ͬ��ɹ�����
				i++;
			}
		}
	}
	return false;													// û�ҵ�
}

bool initMultitexture(void) {
	char *extensions;
	extensions=strdup((char *) glGetString(GL_EXTENSIONS));			// ������չ���ַ���
	int len=strlen(extensions);
	for (int i=0; i<len; i++)										// ʹ��'\n'�ָ������չ��
		if (extensions[i]==' ') extensions[i]='\n';

#ifdef EXT_INFO
	MessageBox(hWnd,extensions,"supported GL extensions",MB_OK | MB_ICONINFORMATION);
#endif

	if (isInString(extensions,"GL_ARB_multitexture")				// �Ƿ�֧�ֶ���������չ��
		&& __ARB_ENABLE												// �Ƿ�ʹ�ö���������չ��
		&& isInString(extensions,"GL_EXT_texture_env_combine"))		// �Ƿ�֧�����������
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
		glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
		glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");

#ifdef EXT_INFO
		MessageBox(hWnd,"The GL_ARB_multitexture ��չ��ʹ��.","֧�ֶ�������",MB_OK | MB_ICONINFORMATION);
#endif

		return true;
	}
	useMultitexture=false;										// �����֧�ֶ��������򷵻�false
	return false;
}

void initLights(void) {
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void doCube (void) {
	int i;
	glBegin(GL_QUADS);
		// ǰ��
		glNormal3f( 0.0f, 0.0f, +1.0f);
		for (i=0; i<4; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// ����
		glNormal3f( 0.0f, 0.0f,-1.0f);
		for (i=4; i<8; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// ����
		glNormal3f( 0.0f, 1.0f, 0.0f);
		for (i=8; i<12; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// ����
		glNormal3f( 0.0f,-1.0f, 0.0f);
		for (i=12; i<16; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// ����
		glNormal3f( 1.0f, 0.0f, 0.0f);
		for (i=16; i<20; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// ����
		glNormal3f(-1.0f, 0.0f, 0.0f);
		for (i=20; i<24; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
	glEnd();
}

// ��������v=v*M����ˣ�
void VMatMult(GLfloat *M, GLfloat *v) {
	GLfloat res[3];
	res[0]=M[ 0]*v[0]+M[ 1]*v[1]+M[ 2]*v[2]+M[ 3]*v[3];
	res[1]=M[ 4]*v[0]+M[ 5]*v[1]+M[ 6]*v[2]+M[ 7]*v[3];
	res[2]=M[ 8]*v[0]+M[ 9]*v[1]+M[10]*v[2]+M[11]*v[3];
	v[0]=res[0];
	v[1]=res[1];
	v[2]=res[2];
	v[3]=M[15];								
}
// ��������ƫ�ƣ���Ϊ��λ����
// n : ����ķ�����
// c : ��ǰ�Ķ����������꣬�������������ƫ����
// l : �ƹ��λ��
// s : s����
// t : t����
void SetUpBumps(GLfloat *n, GLfloat *c, GLfloat *l, GLfloat *s, GLfloat *t) {
	GLfloat v[3];								// �ƹⷽ��
	GLfloat lenQ;								// �ƹⷽ�������ĳ��ȣ�ʹ��������λ��
	// ����ƹⷽ��
	v[0]=l[0]-c[0];
	v[1]=l[1]-c[1];
	v[2]=l[2]-c[2];
	lenQ=(GLfloat) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0]/=lenQ;
	v[1]/=lenQ;
	v[2]/=lenQ;
	// �ѷ�������ͶӰ��s,t���� 
	c[0]=(s[0]*v[0]+s[1]*v[1]+s[2]*v[2])*MAX_EMBOSS;
	c[1]=(t[0]*v[0]+t[1]*v[1]+t[2]*v[2])*MAX_EMBOSS;
}

void doLogo(void) 
{
	// ��������ڵ�������������Թ�������ʽ��ʾ������־
	glDepthFunc(GL_ALWAYS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,glLogo);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0.23f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(0.53f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(0.53f, -0.25f,-1.0f);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0.23f, -0.25f,-1.0f);
	glEnd();
	if (useMultitexture) {
		glBindTexture(GL_TEXTURE_2D,multiLogo);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(-0.53f, -0.25f,-1.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(-0.33f, -0.25f,-1.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(-0.33f, -0.15f,-1.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(-0.53f, -0.15f,-1.0f);
		glEnd();
	}
	glDepthFunc(GL_LEQUAL);
}

//ʹ��һ������Ԫ����͹��ͼ
bool doMesh1TexelUnits(void) {
	GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};					// ���浱ǰ�Ķ���
	GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};					// ���淨��
	GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};					// s�������귽��
	GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};					// t�������귽��
	GLfloat l[4];										// ����ƹⷽ��
	GLfloat Minv[16];									// ����ģ�ͱ任�������
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// ��ձ�����ɫ����Ȼ���

	// ����ģ�ͱ任�������
	glLoadIdentity();
	glRotatef(-yrot,0.0f,1.0f,0.0f);
	glRotatef(-xrot,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,z);
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	// ���õƹ��λ��
	l[0]=LightPosition[0];
	l[1]=LightPosition[1];
	l[2]=LightPosition[2];
	l[3]=1.0f;								
	VMatMult(Minv,l);

	//ͨ��1�����ư�͹��ͼ
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	doCube();

	//ͨ��2�����Ʒ�ת�İ�͹��ͼ
	glBindTexture(GL_TEXTURE_2D,invbump[filter]);
	glBlendFunc(GL_ONE,GL_ONE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	glBegin(GL_QUADS);
		// ǰ��
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=1.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=0; i<4; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			// ������������Ϊƫ�ƺ����������
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=-1.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=4; i<8; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=1.0f;
		n[2]=0.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=8; i<12; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=-1.0f;
		n[2]=0.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=12; i<16; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=-1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=16; i<20; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=-1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=20; i<24; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
	glEnd();

	//���ƻ�����ͼ��ɫ
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	//���Ʊ�־
	doLogo();
	return true;							// �ɹ�����
}

//ʹ�ö���������͹��ͼ
bool doMesh2TexelUnits(void) 
{
	GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};					// ���浱ǰ�Ķ���
	GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};					// ���淨��
	GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};					// s�������귽��
	GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};					// t�������귽��
	GLfloat l[4];							// ����ƹⷽ��
	GLfloat Minv[16];							// ����ģ�ͱ任�������
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// ��ձ�����ɫ����Ȼ���

	// ����ģ�ͱ任�������
	glLoadIdentity();
	glRotatef(-yrot,0.0f,1.0f,0.0f);
	glRotatef(-xrot,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	// ���õƹ��λ��
	l[0]=LightPosition[0];
	l[1]=LightPosition[1];
	l[2]=LightPosition[2];
	l[3]=1.0f;								
	VMatMult(Minv,l);
	// ����Ԫ #0
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

	// ����Ԫ #1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, invbump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);

	// ���û�Ϻ͹���
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
		// ǰ��
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=1.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=0; i<4; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=-1.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=4; i<8; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=1.0f;
		n[2]=0.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=8; i<12; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=0.0f;
		n[1]=-1.0f;
		n[2]=0.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=12; i<16; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=-1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=16; i<20; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// ����
		n[0]=-1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=20; i<24; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
	glEnd();
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	doLogo();
	return true;								
}

//����һ���ް�͹��ͼ�������� 
bool doMeshNoBumps(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();							
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	if (useMultitexture) {
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture[filter]);
	glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
	glEnable(GL_LIGHTING);
	doCube();

	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	doLogo();
	return true;								
}

/********************************************************************************************************************************/
//GLuint		texture[1];		// �洢һ������
HDC			hDC=NULL;		// ������ɫ��������
HGLRC		hRC=NULL;		// OpenGL��Ⱦ��������
HWND		hWnd=NULL;		// �������ǵĴ��ھ��
HINSTANCE	hInstance;		// ��������ʵ��

bool	keys[256];			// ������̰���������
bool	active=TRUE;		// ���ڵĻ��־��ȱʡΪTRUE
bool	fullscreen=TRUE;	// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
//GLfloat		rtri;			// ���������εĽǶ�
//GLfloat		rquad;			// �����ı��εĽǶ�

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc�Ķ���
AUX_RGBImageRec *LoadBMP(CHAR *Filename)				// ����λͼͼ��
{
	FILE *File=NULL;									// �ļ����
	if (!Filename)										// ȷ���ļ������ṩ
	{
		return NULL;									// ���û�ṩ������ NULL
	}
	File=fopen(Filename,"r");							// ���Դ��ļ�
	if (File)											// �ļ�����ô?
	{
		fclose(File);									// �رվ��
		return auxDIBImageLoadA(Filename);				// ����λͼ������ָ��
	}
	return NULL;										// �������ʧ�ܣ����� NULL
}

//int LoadGLTextures()									// ����λͼ(��������Ĵ���)��ת��������
//{
//	int Status=FALSE;									// ״ָ̬ʾ��
//	AUX_RGBImageRec *TextureImage[1];					// ��������Ĵ洢�ռ�
//	memset(TextureImage,0,sizeof(void *)*1);			// ��ָ����Ϊ NULL
//	// ����λͼ��������޴������λͼû�ҵ����˳�
//	if (TextureImage[0]=LoadBMP("Data/NeHe.bmp"))
//	{
//		Status=TRUE;									// �� Status ��Ϊ TRUE
//		glGenTextures(1, &texture[0]);					// ��������
//
//		// ʹ������λͼ�������� �ĵ�������
//		glBindTexture(GL_TEXTURE_2D, texture[0]);
//		// ��������
//		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
//
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
//	}
//
//	if (TextureImage[0])								// �����Ƿ����
//	{
//		if (TextureImage[0]->data)						// ����ͼ���Ƿ����
//		{
//			free(TextureImage[0]->data);				// �ͷ�����ͼ��ռ�õ��ڴ�
//		}
//
//		free(TextureImage[0]);							// �ͷ�ͼ��ṹ
//	}
//	return Status;										// ���� Status
//}
/***********************************����Ĵ���***********************************************************************************/
int LoadGLTextures()									// ����*.bmpͼ�񣬲�ת��Ϊ����
{								
	bool status=true;		
	AUX_RGBImageRec *Image=NULL;						
	char *alpha=NULL;

	// ���ػ�������
	if (Image=auxDIBImageLoad("Data/Base.bmp")) {
		glGenTextures(3, texture);						// ����3������

		// ����ʹ���ٽ����������˵�����
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// ����ʹ�����ι��������˵�����
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// ����ʹ������Mipmap���������˵�����
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
	}
	else status=false;

	if (Image) {										// ���ͼ�������ڣ����ͷ�ͼ�������Դ
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}

	// ���밼͹��ͼ
	if (Image=auxDIBImageLoad("Data/Bump.bmp")) {
		glPixelTransferf(GL_RED_SCALE,0.5f);							// ����ɫֵ��Ϊԭ����50%
		glPixelTransferf(GL_GREEN_SCALE,0.5f);				
		glPixelTransferf(GL_BLUE_SCALE,0.5f);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);		//��ʹ���ظ���ͼ
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glGenTextures(3, bump);	
		// ����ʹ���ٽ����������˵�����
		glBindTexture(GL_TEXTURE_2D, bump[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		// ����ʹ�����ι��������˵�����e
		glBindTexture(GL_TEXTURE_2D, bump[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		//����ʹ������Mipmap���������˵�����

		glBindTexture(GL_TEXTURE_2D, bump[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		for (int i=0; i<3*Image->sizeX*Image->sizeY; i++)		// ��ת��͹��ͼ����
			Image->data[i]=255-Image->data[i];

		glGenTextures(3, invbump);								// ����������ת�˰�͹��ͼ

		// ����ʹ���ٽ����������˵�����
		glBindTexture(GL_TEXTURE_2D, invbump[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		// ����ʹ�����ι��������˵�����
		glBindTexture(GL_TEXTURE_2D, invbump[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// ����ʹ������Mipmap���������˵�����
		glBindTexture(GL_TEXTURE_2D, invbump[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		glPixelTransferf(GL_RED_SCALE,1.0f);					
		glPixelTransferf(GL_GREEN_SCALE,1.0f);			
		glPixelTransferf(GL_BLUE_SCALE,1.0f);
	}
	else status=false;
	if (Image) {											// ���ͼ����ڣ���ɾ��
		if (Image->data) delete Image->data;				
		delete Image;
	}

	if (Image=auxDIBImageLoad("Data/OpenGL_ALPHA.bmp")) {
		alpha=new char[4*Image->sizeX*Image->sizeY];
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
			alpha[4*a+3]=Image->data[a*3];				
		if (!(Image=auxDIBImageLoad("Data/OpenGL.bmp"))) status=false;
		for (int a=0; a<Image->sizeX*Image->sizeY; a++) {
			alpha[4*a]=Image->data[a*3];				
			alpha[4*a+1]=Image->data[a*3+1];		
			alpha[4*a+2]=Image->data[a*3+2];			
		}

		glGenTextures(1, &glLogo);						// ������־����

		// ʹ�����ι�����
		glBindTexture(GL_TEXTURE_2D, glLogo);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);
		delete alpha;
	}
	else status=false;

	if (Image) {										// ���ͼ����ڣ���ɾ��
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}

	// ������չ��־����
	if (Image=auxDIBImageLoad("Data/multi_on_alpha.bmp")) {
		alpha=new char[4*Image->sizeX*Image->sizeY];			
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
			alpha[4*a+3]=Image->data[a*3];					
		if (!(Image=auxDIBImageLoad("Data/multi_on.bmp"))) status=false;
		for (int a=0; a<Image->sizeX*Image->sizeY; a++) {		
			alpha[4*a]=Image->data[a*3];					// R
			alpha[4*a+1]=Image->data[a*3+1];				// G
			alpha[4*a+2]=Image->data[a*3+2];				// B
		}
		glGenTextures(1, &multiLogo);					
		glBindTexture(GL_TEXTURE_2D, multiLogo);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

		delete alpha;
	}
	else status=false;

	if (Image) {										// ���ͼ����ڣ���ɾ��
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}
	return status;								
}

/********************************************************************************************************************************/
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// ����OpenGL���ڴ�С
{
	if (height==0)										// ��ֹ�����
	{
		height=1;										// ��Height��Ϊ1
	}

	glViewport(0,0,width,height);						// ���õ�ǰ���ӿ�

	glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
	glLoadIdentity();									// ����ͶӰ����

	// �����ӿڵĴ�С
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// ѡ��ģ�͹۲����
	glLoadIdentity();									// ����ģ�͹۲����
}

int InitGL(GLvoid)										// �˴���ʼ��OpenGL������������
{
/***********************************����Ĵ���***********************************************************************************/
	multitextureSupported=initMultitexture();
	initLights();	
/********************************************************************************************************************************/
	if (!LoadGLTextures())								// ������������������
	{
		return FALSE;									// ���δ�����룬����FALSE
	}
	glEnable(GL_TEXTURE_2D);							// ��������ӳ��
	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ��ɫ����
	glClearDepth(1.0f);									// ������Ȼ���
	glEnable(GL_DEPTH_TEST);							// ������Ȳ���
	glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ����ϵͳ��͸�ӽ�������
	return TRUE;										// ��ʼ�� OK
}

int DrawGLScene(GLvoid)									// �����￪ʼ�������еĻ���
{

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
//	glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����
/***********************************����Ĵ���***********************************************************************************/
	if (bumps) 
	{
		if (useMultitexture && maxTexelUnits>1)
		{
			return doMesh2TexelUnits();
		}
		else 
		{	
			return doMesh1TexelUnits(); 	
		}
	}
	else 
		return doMeshNoBumps();

/********************************************************************************************************************************/	
//	return TRUE;										// һ�� OK
}

GLvoid KillGLWindow(GLvoid)								// �������ٴ���
{
	if (fullscreen)										// ���Ǵ���ȫ��ģʽ��?
	{
		ChangeDisplaySettings(NULL,0);					// �ǵĻ����л�������
		ShowCursor(TRUE);								// ��ʾ���ָ��
	}

	if (hRC)											//����ӵ��OpenGL��������?
	{
		if (!wglMakeCurrent(NULL,NULL))					// �����ܷ��ͷ�DC��RC������?
		{
			MessageBox(NULL,"�ͷ�DC��RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// �����ܷ�ɾ��RC?
		{
			MessageBox(NULL,"�ͷ�RCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// ��RC��Ϊ NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// �����ܷ��ͷ� DC?
	{
		MessageBox(NULL,"�ͷ�DCʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// �� DC ��Ϊ NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// �ܷ����ٴ���?
	{
		MessageBox(NULL,"�ͷŴ��ھ��ʧ�ܡ�","�رմ���",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// �� hWnd ��Ϊ NULL
	}

	if (!UnregisterClass("OpenG",hInstance))			// �ܷ�ע����?
	{
		MessageBox(NULL,"����ע�������ࡣ","�رմ���",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// �� hInstance ��Ϊ NULL
	}
}

/*	���������������OpenGL���ڣ�����Ϊ��									*
 *	title			- ���ڱ���												*
 *	width			- ���ڿ��												*
 *	height			- ���ڸ߶�												*
 *	bits			- ��ɫ��λ�8/16/32��									*
 *	fullscreenflag	- �Ƿ�ʹ��ȫ��ģʽ��ȫ��ģʽ(TRUE)������ģʽ(FALSE)		*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// �������ƥ��Ľ��
	WNDCLASS	wc;						// ������ṹ
	DWORD		dwExStyle;				// ��չ���ڷ��
	DWORD		dwStyle;				// ���ڷ��
	RECT		WindowRect;				// ȡ�þ��ε����ϽǺ����½ǵ�����ֵ
	WindowRect.left=(long)0;			// ��Left   ��Ϊ 0
	WindowRect.right=(long)width;		// ��Right  ��ΪҪ��Ŀ��
	WindowRect.top=(long)0;				// ��Top    ��Ϊ 0
	WindowRect.bottom=(long)height;		// ��Bottom ��ΪҪ��ĸ߶�

	fullscreen=fullscreenflag;			// ����ȫ��ȫ����־

	hInstance			= GetModuleHandle(NULL);				// ȡ�����Ǵ��ڵ�ʵ��
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// �ƶ�ʱ�ػ�����Ϊ����ȡ��DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc������Ϣ
	wc.cbClsExtra		= 0;									// �޶��ⴰ������
	wc.cbWndExtra		= 0;									// �޶��ⴰ������
	wc.hInstance		= hInstance;							// ����ʵ��
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// װ��ȱʡͼ��
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// װ�����ָ��
	wc.hbrBackground	= NULL;									// GL����Ҫ����
	wc.lpszMenuName		= NULL;									// ����Ҫ�˵�
	wc.lpszClassName	= "OpenG";							// �趨������

	if (!RegisterClass(&wc))									// ����ע�ᴰ����
	{
		MessageBox(NULL,"ע�ᴰ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// �˳�������FALSE
	}
	
	if (fullscreen)												// Ҫ����ȫ��ģʽ��?
	{
		DEVMODE dmScreenSettings;								// �豸ģʽ
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// ȷ���ڴ����Ϊ��
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode �ṹ�Ĵ�С
		dmScreenSettings.dmPelsWidth	= width;				// ��ѡ��Ļ���
		dmScreenSettings.dmPelsHeight	= height;				// ��ѡ��Ļ�߶�
		dmScreenSettings.dmBitsPerPel	= bits;					// ÿ������ѡ��ɫ�����
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// ����������ʾģʽ�����ؽ����ע: CDS_FULLSCREEN ��ȥ��״̬��
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// ��ģʽʧ�ܣ��ṩ����ѡ��˳����ڴ��������С�
			if (MessageBox(NULL,"ȫ��ģʽ�ڵ�ǰ�Կ�������ʧ�ܣ�\nʹ�ô���ģʽ��","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				//����û�ѡ�񴰿�ģʽ������fullscreen ��ֵ��ΪFALSE,�����������
				fullscreen=FALSE;		// ѡ�񴰿�ģʽ(Fullscreen=FALSE)
			}
			else
			{
				//����û�ѡ���˳���������Ϣ���ڸ�֪�û����򽫽�����������FALSE���߳��򴰿�δ�ܳɹ������������˳���
				MessageBox(NULL,"���򽫱��ر�","����",MB_OK|MB_ICONSTOP);
				return FALSE;									// �˳������� FALSE
			}
		}
	}

	if (fullscreen)												// �Դ���ȫ��ģʽ��?
	{
		dwExStyle=WS_EX_APPWINDOW;								// ��չ������
		dwStyle=WS_POPUP;										// ������
		ShowCursor(FALSE);										// �������ָ��
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// ��չ������
		dwStyle=WS_OVERLAPPEDWINDOW;							// ������
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڴﵽ����Ҫ��Ĵ�С

	// ��������
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// ��չ������
								"OpenG",							// ������
								title,								// ���ڱ���
								dwStyle |							// ����Ĵ���������
								WS_CLIPSIBLINGS |					// ����Ĵ���������
								WS_CLIPCHILDREN,					// ����Ĵ���������
								0, 0,								// ����λ��
								WindowRect.right-WindowRect.left,	// ��������õĴ��ڿ��
								WindowRect.bottom-WindowRect.top,	// ��������õĴ��ڸ߶�
								NULL,								// �޸�����
								NULL,								// �޲˵�
								hInstance,							// ʵ��
								NULL)))								// ����WM_CREATE�����κζ���
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"���ڴ�������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				//pfd ���ߴ���������ϣ���Ķ�����������ʹ�õ����ظ�ʽ
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// ������ʽ�������Ĵ�С
		1,											// �汾��
		PFD_DRAW_TO_WINDOW |						// ��ʽ֧�ִ���
		PFD_SUPPORT_OPENGL |						// ��ʽ����֧��OpenGL
		PFD_DOUBLEBUFFER,							// ����֧��˫����
		PFD_TYPE_RGBA,								// ���� RGBA ��ʽ
		bits,										// ѡ��ɫ�����
		0, 0, 0, 0, 0, 0,							// ���Ե�ɫ��λ
		0,											// ��Alpha����
		0,											// ����Shift Bit
		0,											// ���ۼӻ���
		0, 0, 0, 0,									// ���Ծۼ�λ
		16,											// 16λ Z-���� (��Ȼ���) 
		0,											// ���ɰ建��
		0,											// �޸�������
		PFD_MAIN_PLANE,								// ����ͼ��
		0,											// ��ʹ���ص���
		0, 0, 0										// ���Բ�����
	};
	
	if (!(hDC=GetDC(hWnd)))							// ȡ���豸��������ô?
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"���ܴ���һ�������豸������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows �ҵ���Ӧ�����ظ�ʽ����?
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"���ܴ���һ����ƥ������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// �ܹ��������ظ�ʽô?
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"�����������ظ�ʽ","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// �ܷ�ȡ��OpenGL��Ⱦ������?
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"���ܴ���OpenGL��Ⱦ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// ���Լ�����ɫ������
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"���ܼ��ǰ��OpenGL��Ȼ������","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// ��ʾ����
	SetForegroundWindow(hWnd);						// ����������ȼ�
	SetFocus(hWnd);									// ���ü��̵Ľ������˴���
	ReSizeGLScene(width, height);					// ����͸�� GL ��Ļ

	if (!InitGL())									// ��ʼ���½���GL����
	{
		KillGLWindow();								// ������ʾ��
		MessageBox(NULL,"��ʼ��ʧ��","����",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// ���� FALSE
	}

	return TRUE;									// �ɹ�
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// ���ڵľ��	
							UINT	uMsg,			// ���ڵ���Ϣ
							WPARAM	wParam,			// ���ӵ���Ϣ����
							LPARAM	lParam)			// ���ӵ���Ϣ����
{
	switch (uMsg)									// ���Windows��Ϣ
	{
		case WM_ACTIVATE:							// ���Ӵ��ڼ�����Ϣ
		{
			if (!HIWORD(wParam))					// �����С��״̬
			{
				active=TRUE;						// �����ڼ���״̬
			}
			else
			{
				active=FALSE;						// �����ټ���
			}

			return 0;								// ������Ϣѭ��
		}

		case WM_SYSCOMMAND:							// ϵͳ�ж�����
		{
			switch (wParam)							// ���ϵͳ����
			{
				case SC_SCREENSAVE:					// ����Ҫ����?
				case SC_MONITORPOWER:				// ��ʾ��Ҫ����ڵ�ģʽ?
				return 0;							// ��ֹ����
			}
			break;									// �˳�
		}

		case WM_CLOSE:								// �յ�Close��Ϣ?
		{
			PostQuitMessage(0);						// �����˳���Ϣ
			return 0;								// ����
		}

		case WM_KEYDOWN:							// �м�����ô?
		{
			keys[wParam] = TRUE;					// ����ǣ���ΪTRUE
			return 0;								// ����
		}

		case WM_KEYUP:								// �м��ſ�ô?
		{
			keys[wParam] = FALSE;					// ����ǣ���ΪFALSE
			return 0;								// ����
		}

		case WM_SIZE:								// ����OpenGL���ڴ�С
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
			return 0;								// ����
		}
	}

	// �� DefWindowProc��������δ�������Ϣ��
	return DefWindowProc(hWnd,uMsg,wParam,lParam);
}

int WINAPI WinMain(HINSTANCE	hInstance,			// ��ǰ����ʵ��
					HINSTANCE	hPrevInstance,		// ǰһ������ʵ��
					LPSTR		lpCmdLine,			// �����в���
					int			nCmdShow)			// ������ʾ״̬
{
	MSG		msg;									// Windowsx��Ϣ�ṹ
	BOOL	done=FALSE;								// �����˳�ѭ����Bool ����

	// ��ʾ�û�ѡ������ģʽ
	if (MessageBox(NULL,"������ȫ��ģʽ������ô��", "����ȫ��ģʽ",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// FALSEΪ����ģʽ
	}

	// ����OpenGL����
//	if (!CreateGLWindow("NeHe's ����ʵ��",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
	if (!CreateGLWindow("NeHe's ��������",640,480,16,fullscreen))
/********************************************************************************************************************************/
	{
		return 0;									// ʧ���˳�
	}

	while(!done)									// ����ѭ��ֱ�� done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// ����Ϣ�ڵȴ���?
		{
			if (msg.message==WM_QUIT)				// �յ��˳���Ϣ?
			{
				done=TRUE;							// �ǣ���done=TRUE
			}
			else									// ���ǣ���������Ϣ
			{
				TranslateMessage(&msg);				// ������Ϣ
				DispatchMessage(&msg);				// ������Ϣ
			}
		}
		else										// ���û����Ϣ
		{
			// ���Ƴ���������ESC��������DrawGLScene()���˳���Ϣ
			if (active)								// ���򼤻��ô?
			{
				if (keys[VK_ESCAPE])				// ESC ������ô?
				{
					done=TRUE;						// ESC �����˳��ź�
				}
				else								// �����˳���ʱ��ˢ����Ļ
				{
					DrawGLScene();					// ���Ƴ���
					SwapBuffers(hDC);				// �������� (˫����)
/***********************************����Ĵ���***********************************************************************************/
					if (keys['E'])
					{
						keys['E']=false;
						emboss=!emboss;
					}

					if (keys['M'])
					{
						keys['M']=false;
						useMultitexture=((!useMultitexture) && multitextureSupported);
					}

					if (keys['B'])
					{
						keys['B']=false;
						bumps=!bumps;
					}

					if (keys['F'])
					{
						keys['F']=false;
						filter++;
						filter%=3;
					}

					if (keys[VK_PRIOR])
					{
						z-=0.02f;
					}

					if (keys[VK_NEXT])
					{
						z+=0.02f;
					}

					if (keys[VK_UP])
					{
						xspeed-=0.01f;
					}

					if (keys[VK_DOWN])
					{
						xspeed+=0.01f;
					}

					if (keys[VK_RIGHT])
					{
						yspeed+=0.01f;
					}

					if (keys[VK_LEFT])
					{
						yspeed-=0.01f;
					}
/********************************************************************************************************************************/
				}
			}

			if (keys[VK_F1])						// F1��������ô?
			{
				keys[VK_F1]=FALSE;					// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE
				KillGLWindow();						// ���ٵ�ǰ�Ĵ���
				fullscreen=!fullscreen;				// �л� ȫ�� / ���� ģʽ
				// �ؽ� OpenGL ����
//				if (!CreateGLWindow("NeHe's ����ʵ��",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
				if (!CreateGLWindow("NeHe's ��������",640,480,16,fullscreen))
/********************************************************************************************************************************/
				{
					return 0;						// �������δ�ܴ����������˳�
				}
			}
		}
	}

	// �رճ���
	KillGLWindow();									// ���ٴ���
	return (msg.wParam);							// �˳�����
}
