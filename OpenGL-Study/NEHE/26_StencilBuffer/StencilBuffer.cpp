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
#pragma comment( lib, "vfw32.lib" )	
/********************************************************************************************************************************/

#include <windows.h>		// Windows��ͷ�ļ�
#include  <GL/glew.h>			// �������µ�gl.h,glu.h��
//#include <glut.h>			// ����OpenGLʵ�ÿ�
#include <stdio.h>			// ��׼����/������ͷ�ļ�
#include <math.h>			// ��ѧ�⺯��
#include  <GL/glaux.h>			// GLaux���ͷ�ļ�
/***********************************����Ĵ���***********************************************************************************/
static GLfloat	LightAmb[] = {0.7f, 0.7f, 0.7f, 1.0f};				// ������
static GLfloat	LightDif[] = {1.0f, 1.0f, 1.0f, 1.0f};				// �����
static GLfloat	LightPos[] = {4.0f, 4.0f, 6.0f, 1.0f};				// �ƹ��λ��
GLUquadricObj	*q;													// ʹ�ö��μ����崴����

GLfloat		xrot		=  0.0f;									// X�������ת�Ƕ�
GLfloat		yrot		=  0.0f;									// Y�������ת�ǵ�
GLfloat		xrotspeed	=  0.0f;									// X�������ת�ٶ�
GLfloat		yrotspeed	=  0.0f;									// Y�������ת�ٶ�
GLfloat		zoom		= -7.0f;									// ������Ļ7����λ
GLfloat		height		=  2.0f;									// ���뿪�ذ�ĸ߶�

GLuint		texture[3];												// ʹ����������
/********************************************************************************************************************************/
//GLfloat		xrot;			// X ��ת��
//GLfloat		yrot;			// Y ��ת��
GLfloat		zrot;			// Z ��ת��

//GLuint		texture[1];		// �洢һ������
HDC			hDC=NULL;		// ������ɫ��������
HGLRC		hRC=NULL;		// OpenGL��Ⱦ��������
HWND		hWnd=NULL;		// �������ǵĴ��ھ��
HINSTANCE	hInstance;		// ��������ʵ��

bool	keys[256];			// ������̰���������
bool	active=TRUE;		// ���ڵĻ��־��ȱʡΪTRUE
bool	fullscreen=TRUE;	// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
GLfloat		rtri;			// ���������εĽǶ�
GLfloat		rquad;			// �����ı��εĽǶ�

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
int LoadGLTextures()													// ����*.bmp�ļ�����ת��Ϊ����
{
    int Status=FALSE;								
    AUX_RGBImageRec *TextureImage[3];									// ��������ͼ��
    memset(TextureImage,0,sizeof(void *)*3);					
    if ((TextureImage[0]=LoadBMP("Data/EnvWall.bmp")) &&				// ����ذ�ͼ��
        (TextureImage[1]=LoadBMP("Data/Ball.bmp")) &&					// ������ͼ��
        (TextureImage[2]=LoadBMP("Data/EnvRoll.bmp")))					// ����ǿ��ͼ��
	{
		Status=TRUE;							
		glGenTextures(3, &texture[0]);									// ��������
		for (int loop=0; loop<3; loop++)								// ѭ�����������������
		{
			glBindTexture(GL_TEXTURE_2D, texture[loop]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop]->sizeX, TextureImage[loop]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop]->data);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		}
		for (int loop=0; loop<3; loop++)					
		{
			if (TextureImage[loop])					
			{
				if (TextureImage[loop]->data)			
				{
					free(TextureImage[loop]->data);		
				}
				free(TextureImage[loop]);			
			}
		}
	}
	return Status;														// �ɹ�����
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

//int InitGL(GLvoid)										// �˴���ʼ��OpenGL������������
//{
//	if (!LoadGLTextures())								// ������������������
//	{
//		return FALSE;									// ���δ�����룬����FALSE
//	}
//
//	glEnable(GL_TEXTURE_2D);							// ��������ӳ��
//	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ��ɫ����
//	glClearDepth(1.0f);									// ������Ȼ���
//	glEnable(GL_DEPTH_TEST);							// ������Ȳ���
//	glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ����ϵͳ��͸�ӽ�������
//	return TRUE;										// ��ʼ�� OK
//}

/***********************************����Ĵ���***********************************************************************************/
int InitGL(GLvoid)											// ��ʼ��OpenGL
{	
	if (!LoadGLTextures())									// ��������
	{
		return FALSE;							
	}
	glShadeModel(GL_SMOOTH);								
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);							
	glClearDepth(1.0f);							
	glClearStencil(0);										// �����ɰ�ֵ
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LEQUAL);						
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	glEnable(GL_TEXTURE_2D);								// ʹ��2D����
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);					
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);						
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);					
	glEnable(GL_LIGHT0);	
	glEnable(GL_LIGHTING);
	q = gluNewQuadric();									// ����һ�����μ�����
	gluQuadricNormals(q, GL_SMOOTH);						// ʹ��ƽ������
	gluQuadricTexture(q, GL_TRUE);							// ʹ������

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// ����������ӳ��
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);			

	return TRUE;											// ��ʼ����ɣ��ɹ�����
}

void DrawObject()								// �������ǵ���
{
#if 1
	glBindTexture(GL_TEXTURE_2D, texture[1]);	// ����Ϊ�������
	gluSphere(q, 0.35f, 32, 16);	
#endif			// ������
#if 1
	glBindTexture(GL_TEXTURE_2D, texture[2]);	// ����Ϊ��������
	glColor4f(1.0f, 1.0f, 1.0f, 0.4f);			// ʹ��alphaΪ40%�İ�ɫ
	// GL_RGB
	glEnable(GL_BLEND);							// ���û��
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);			// ��ԭ��ɫ��40%��Ŀ����ɫ���
	glEnable(GL_TEXTURE_GEN_S);					// ʹ����ӳ��
	glEnable(GL_TEXTURE_GEN_T);						

	gluSphere(q, 0.35f, 32, 16);				// �������壬�����
#endif
	glColor3f(1.0f, 1.0f, 1.0f);				// ����Ϊ��ɫ

	glDisable(GL_TEXTURE_GEN_S);				// ��OpenGL�ص�Ĭ�ϵ�����
	glDisable(GL_TEXTURE_GEN_T);						
	glDisable(GL_BLEND);		

}

void DrawFloor()			
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);	// ѡ��ذ������ذ���һ�����������
	glBegin(GL_QUADS);							
		glNormal3f(0.0, 1.0, 0.0);				
		glTexCoord2f(0.0f, 1.0f);				// ����
		glVertex3f(-2.0, 0.0, 2.0);				

		glTexCoord2f(0.0f, 0.0f);				// ����
		glVertex3f(-2.0, 0.0,-2.0);				

		glTexCoord2f(1.0f, 0.0f);				// ����
		glVertex3f( 2.0, 0.0,-2.0);				

		glTexCoord2f(1.0f, 1.0f);				// ����
		glVertex3f( 2.0, 0.0, 2.0);				
	glEnd();								
}

void ProcessKeyboard()								
{
	if (keys[VK_RIGHT])	yrotspeed += 0.08f;				
	if (keys[VK_LEFT])	yrotspeed -= 0.08f;				
	if (keys[VK_DOWN])	xrotspeed += 0.08f;				
	if (keys[VK_UP])	xrotspeed -= 0.08f;			

	if (keys['A'])		zoom +=0.05f;					
	if (keys['Z'])		zoom -=0.05f;					

	if (keys['W'])	height +=0.03f;					
	if (keys['S'])	height -=0.03f;					
}

/********************************************************************************************************************************/

int DrawGLScene(GLvoid)									// �����￪ʼ�������еĻ���
{

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
	//glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����
/***********************************����Ĵ���***********************************************************************************/
	// �������
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// ���ü���ƽ��
	double eqr[] = {0.0f,-1.0f, 0.0f, 0.0f};	
	glLoadIdentity();							
	glTranslatef(0.0f, -1.6f, zoom);					// ƽ�ƺ����ŵ���
	glColorMask(0,0,0,0);
	glEnable(GL_STENCIL_TEST);							// �����ɰ建��
	glStencilFunc(GL_ALWAYS, 1, 1);						// �����ɰ��������ͨ�����ο�ֵ��Ϊ1������ֵҲ��Ϊ1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			// ���õ���Ȳ��Բ�ͨ��ʱ�������ɰ��е�ֵ���䡣���ͨ����ʹ�òο�ֵ�滻�ɰ�ֵ
	glDisable(GL_DEPTH_TEST);							// ������Ȳ���
	DrawFloor();										// ���Ƶ���
	glEnable(GL_DEPTH_TEST);							//������Ȳ���
	glColorMask(1,1,1,1);								// ���Ի�����ɫ
	glStencilFunc(GL_EQUAL, 1, 1);						//���������ָ�������ǻ���ʱ�����ı��ɰ建������ֵ
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		
	glEnable(GL_CLIP_PLANE0);							// ʹ�ü���ƽ��
	glClipPlane(GL_CLIP_PLANE0, eqr);					// ���ü���ƽ��Ϊ���棬���������ķ���Ϊ����
	glPushMatrix();										// ���浱ǰ�ľ���
		glScalef(1.0f, -1.0f, 1.0f);					// ��Y�ᷴת
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);	// ���õƹ�0
		glTranslatef(0.0f, height, 0.0f);				
		glRotatef(xrot, 1.0f, 0.0f, 0.0f);				
		glRotatef(yrot, 0.0f, 1.0f, 0.0f);				
		DrawObject();									// ���Ʒ������
	glPopMatrix();										// ��������ľ���
	glDisable(GL_CLIP_PLANE0);							// ���ü���ƽ��
	glDisable(GL_STENCIL_TEST);							// �ر��ɰ�
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glEnable(GL_BLEND);									// ���û��
	glDisable(GL_LIGHTING);								// �رչ���
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);					// ������ɫΪ��ɫ
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// ���û��ϵ��
	DrawFloor();										// ���Ƶ���
	glEnable(GL_LIGHTING);								// ʹ�ù���
	glDisable(GL_BLEND);								// ���û��
	glTranslatef(0.0f, height, 0.0f);					// �ƶ���λheight��λ��
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// ��������ת�ĽǶ�
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);					
	DrawObject();										// ������
	xrot += xrotspeed;									// ����X����ת�ٶ�
	yrot += yrotspeed;									// ����Y����ת�ٶ�
	glFlush();											// ǿ��OpenGLִ����������
/********************************************************************************************************************************/	
	return TRUE;										// һ�� OK
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
		//0,											// ���ɰ建��
/***********************************����Ĵ���***********************************************************************************/
		1,											// �����ɰ建��
/********************************************************************************************************************************/
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
	if (!CreateGLWindow("NeHe's ��ʾ�б�",640,480,16,fullscreen))
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
					ProcessKeyboard();			// ��������Ӧ
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
				if (!CreateGLWindow("NeHe's ��ʾ�б�",640,480,16,fullscreen))
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
