/*
 *		��Щ��������Jeff Molofee ��2000��д�ģ��ٴ��ұ����лFredric Echols����Ϊ��ʹ��Щ���뿴���������͸�Ч����ʹ������ʹ�á�
 *		����㷢����Щ�����������������������ҵ�վ�� http://nehe.gamedev.net/default.asp ��������֪����
 *********************************************************************************************************************************
 *		����base.cpp�ļ��޸�
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
#include <GL/glew.h>			// �������µ�gl.h,glu.h��
//#include <glut.h>			// ����OpenGLʵ�ÿ�
/***********************************����Ĵ���***********************************************************************************/
#include <math.h>							// ��ѧ��
#include <stdio.h>							// ��׼���������
#include <stdlib.h>							// ��׼��
GLfloat		xrot,yrot,zrot,								// X, Y & Z �����ת�Ƕ�
		xspeed,yspeed,zspeed,							// X, Y & Z �����ת�ٶ�
		cx,cy,cz=-15;									// �����λ��

int		key=1;											// ����ı�ʶ��
int		step=0,steps=5000;								// �任�Ĳ���
bool		morph=FALSE;								// �Ƿ�ʹ�ñ���

typedef struct					
{
	float	x, y, z;							
} VERTEX;		

typedef	struct											// ����ṹ
{
 int		verts;										// �����ж���ĸ���
 VERTEX		*points;									// �����������ݵ�ָ��
} OBJECT;										

int		maxver;											// ���Ķ�����
OBJECT		morph1,morph2,morph3,morph4,				// ���ǵ��ĸ�����
		helper,*sour,*dest;		 						// ��������,ԭ���壬Ŀ������

void objallocate(OBJECT *k,int n)
{											
	k->points=(VERTEX*)malloc(sizeof(VERTEX)*n);					// ����n��������ڴ�ռ�
}			

void objfree(OBJECT *k)			
{
	free(k->points);								
}

void readstr(FILE *f,char *string)							// ��ȡһ���ַ�
{
	do										
	{
		fgets(string, 255, f);						// ����ȡ255���ַ�
	} while ((string[0] == '/') || (string[0] == '\n'));				// �����س���ֹͣ��ȡ
	return;									// ����
}

void objload(char *name,OBJECT *k)							// ���ļ�����һ��ģ��
{
	int	ver;								// ���涥�����
	float	rx,ry,rz;								// ����ģ��λ��
	FILE	*filein;								// �򿪵��ļ����
	char	oneline[255];							// ����255���ַ�

	filein = fopen(name, "rt");							// ���ı��ļ�������ȡ
											
	readstr(filein,oneline);							// ����һ���ı�
	sscanf(oneline, "Vertices: %d\n", &ver);					// �����ַ���"Vertices: "���������Ķ�����������ver������
	k->verts=ver;								// ����ģ�͵Ķ������
	objallocate(k,ver);							// Ϊģ�����ݷ����ڴ�
	for (int i=0;i<ver;i++)								// ѭ�����еĶ���
	{
		readstr(filein,oneline);							// ��ȡһ������
		sscanf(oneline, "%f %f %f", &rx, &ry, &rz);					// �Ѷ������ݱ�����rx,ry,rz��

		k->points[i].x = rx;							// ���浱ǰ�����x����
		k->points[i].y = ry;							// ���浱ǰ�����y����
		k->points[i].z = rz;							// ���浱ǰ�����z����
	}
	fclose(filein);									// �ر��ļ�

	if(ver>maxver) maxver=ver;								// ��¼���Ķ�����
}			
VERTEX calculate(int i)									// �����i������ÿ�α任��λ��
{
	VERTEX a;								
	a.x=(sour->points[i].x-dest->points[i].x)/steps;				
	a.y=(sour->points[i].y-dest->points[i].y)/steps;				
	a.z=(sour->points[i].z-dest->points[i].z)/steps;				
	return a;									
}			
/********************************************************************************************************************************/
HDC			hDC=NULL;		// ������ɫ��������
HGLRC		hRC=NULL;		// OpenGL��Ⱦ��������
HWND		hWnd=NULL;		// �������ǵĴ��ھ��
HINSTANCE	hInstance;		// ��������ʵ��

bool	keys[256];			// ������̰���������
bool	active=TRUE;		// ���ڵĻ��־��ȱʡΪTRUE
bool	fullscreen=TRUE;	// ȫ����־ȱʡ��ȱʡ�趨��ȫ��ģʽ
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc�Ķ���

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
//	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ��ɫ����
//	glClearDepth(1.0f);									// ������Ȼ���
//	glEnable(GL_DEPTH_TEST);							// ������Ȳ���
//	glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ����ϵͳ��͸�ӽ�������
//	return TRUE;										// ��ʼ�� OK
//}
/***********************************����Ĵ���***********************************************************************************/
int InitGL(GLvoid)			
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);						// ���ð�͸�����ģʽ
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// �������ɫΪ��ɫ
	glClearDepth(1.0);										// ������Ȼ�����ֵΪ1
	glDepthFunc(GL_LESS);									// ������Ȳ��Ժ���
	glEnable(GL_DEPTH_TEST);								// ������Ȳ���
	glShadeModel(GL_SMOOTH);								// ������ɫģʽΪ�⻬��ɫ
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	maxver=0;												// ��ʼ����󶥵���Ϊ0
	objload("data/sphere.txt",&morph1);						// ������ģ��
	objload("data/torus.txt",&morph2);						// ����Բ��ģ��
	objload("data/tube.txt",&morph3);						// ����������ģ��
	objallocate(&morph4,486);								// Ϊ���ĸ�ģ�ͷ����ڴ���Դ
	for(int i=0;i<486;i++)									// �������486������
	{
		morph4.points[i].x=((float)(rand()%14000)/1000)-7;			
		morph4.points[i].y=((float)(rand()%14000)/1000)-7;			 
		morph4.points[i].z=((float)(rand()%14000)/1000)-7;			
	}
	objload("data/sphere.txt",&helper);
	sour=dest=&morph1;								

	return TRUE;											// ��ʼ����ɣ��ɹ�����
}
/********************************************************************************************************************************/

int DrawGLScene(GLvoid)									// �����￪ʼ�������еĻ���
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
	glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����
/***********************************����Ĵ���***********************************************************************************/
	glTranslatef(cx,cy,cz);									// ƽ�ƺ���ת
	glRotatef(xrot,1,0,0);										
	glRotatef(yrot,0,1,0);									
	glRotatef(zrot,0,0,1);										

	xrot+=xspeed; yrot+=yspeed; zrot+=zspeed;				// ������ת�ٶȣ�������ת�Ƕ�

	GLfloat tx,ty,tz;										// ������ʱ����
	VERTEX q;												// �����м�������ʱ����

	glPointSize(2);
	glEnable(GL_POINT_SMOOTH); 
	glBegin(GL_POINTS);										// ����ƿ�ʼ
		for(int i=0;i<morph1.verts;i++)						// ѭ������ģ��1�е�ÿһ������
		{	
			if(morph) q=calculate(i); else q.x=q.y=q.z=0;	// ������ñ��Σ�������м�ģ��
			helper.points[i].x-=q.x;					
			helper.points[i].y-=q.y;					
			helper.points[i].z-=q.z;					
			tx=helper.points[i].x;							// �����������x,y,z������
			ty=helper.points[i].y;						
			tz=helper.points[i].z;					
   
			glColor3f(0,1,1);								// ������ɫ
			glPointSize(3);
			glVertex3f(tx,ty,tz);							// ���ƶ���
			if(morph)
			{
				glColor3f(0,0.5f,1);							// ����ɫ����һЩ
				tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;				// ������ñ��Σ������2����Ķ���
				glVertex3f(tx,ty,tz);						

				glColor3f(0,0,1);								// ����ɫ����һЩ
				tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;				// ������ñ��Σ������2����Ķ���
				glVertex3f(tx,ty,tz);						
			}
		}		
		glPointSize(1);
	glEnd();		
	// ������ñ�����ѱ��β�������
	if(morph && step<=steps)step++; else { morph=FALSE; sour=dest; step=0;}

/********************************************************************************************************************************/
	return TRUE;										// һ�� OK
}

GLvoid KillGLWindow(GLvoid)								// �������ٴ���
{
/***********************************����Ĵ���***********************************************************************************/
	objfree(&morph1);								// �ͷ�ģ��1�ڴ�
	objfree(&morph2);								// �ͷ�ģ��2�ڴ�
	objfree(&morph3);								// �ͷ�ģ��3�ڴ�
	objfree(&morph4);								// �ͷ�ģ��4�ڴ�
	objfree(&helper);								// �ͷ�ģ��5�ڴ�
/********************************************************************************************************************************/
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
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width,HiWord=Height
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
	//if (!CreateGLWindow("NeHe's OpenGL ������",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
	if (!CreateGLWindow("NeHe's OpenGL ����ʵ��",640,480,16,fullscreen))
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
						if(keys[VK_PRIOR])			// PageUp���Ƿ񱻰���
							zspeed+=0.01f;			// ����������z����ת���ٶ�

						if(keys[VK_NEXT])			// PageDown���Ƿ񱻰���
							zspeed-=0.01f;			// ���¼�����z����ת���ٶ�

						if(keys[VK_DOWN])			// �·�����Ƿ񱻰���
							xspeed+=0.01f;			// ����������x����ת���ٶ�

						if(keys[VK_UP])				// �Ϸ�����Ƿ񱻰���
							xspeed-=0.01f;			// ���¼�����x����ת���ٶ�

						if(keys[VK_RIGHT])			// �ҷ�����Ƿ񱻰���
							yspeed+=0.01f;			// ����������y����ת���ٶ�

						if(keys[VK_LEFT])			// ������Ƿ񱻰���
							yspeed-=0.01f;			// ���¼�����y����ת���ٶ�
						if (keys['Q'])				// Q���Ƿ񱻰���
						cz-=0.01f;					// ��������Ļ���ƶ�

						if (keys['Z'])				// Z���Ƿ񱻰��� 
						cz+=0.01f;					// ��������Ļ���ƶ�

						if (keys['W'])				// W���Ƿ񱻰���
						cy+=0.01f;					// ���������ƶ�

						if (keys['S'])				// S���Ƿ񱻰���
						cy-=0.01f;					// ���������ƶ�

						if (keys['D'])				// D���Ƿ񱻰��� 
						cx+=0.01f;					// ���������ƶ�

						if (keys['A'])				// A���Ƿ񱻰��� 
						cx-=0.01f;					// ���������ƶ�
						if (keys['1'] && (key!=1) && !morph)			// ���1�����£�����ε�ģ��1
						{
							key=1;						
							morph=TRUE;				
							dest=&morph1;					
						}
						if (keys['2'] && (key!=2) && !morph)			// ���2�����£�����ε�ģ��1
						{
							key=2;						
							morph=TRUE;					
							dest=&morph2;					
						}
						if (keys['3'] && (key!=3) && !morph)			// ���3�����£�����ε�ģ��1
						{
							key=3;						
							morph=TRUE;					
							dest=&morph3;					
						}
						if (keys['4'] && (key!=4) && !morph)			// ���4�����£�����ε�ģ��1
						{
							key=4;						
							morph=TRUE;					
							dest=&morph4;					
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
				//if (!CreateGLWindow("NeHe's OpenGL ������",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
				if (!CreateGLWindow("NeHe's OpenGL ����ʵ��",640,480,16,fullscreen))
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
