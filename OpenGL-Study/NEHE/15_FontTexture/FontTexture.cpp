/*
 *		��Щ��������Jeff Molofee ��2000��д�ģ��ٴ��ұ����лFredric Echols����Ϊ��ʹ��Щ���뿴���������͸�Ч����ʹ������ʹ�á�
 *		����㷢����Щ�����������������������ҵ�վ�� http://nehe.gamedev.net/default.asp ��������֪����
 *********************************************************************************************************************************
 *		����Font.cpp�ļ��޸�
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
#include <stdio.h>			// ��׼����/������ͷ�ļ�
#include <math.h>			// ��ѧ�⺯��
#include <GL/glaux.h>			// GLaux���ͷ�ļ�
#include <stdarg.h>			// ��������ɱ������ͷ�ļ�
GLuint	base;				// �����������ʾ�б�Ŀ�ʼλ��
GLfloat	rot;				// ��ת����
GLYPHMETRICSFLOAT gmf[256];	// ��¼256���ַ�����Ϣ
GLfloat		xrot;			// X ��ת��
GLfloat		yrot;			// Y ��ת��
GLfloat		zrot;			// Z ��ת��

GLuint		texture[1];		// �洢һ������
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

GLvoid BuildFont(GLvoid)						// ����λͼ����
{
	HFONT	font;								// ������
	HFONT	oldfont;							// �ɵ�������

	base = glGenLists(256);						// ����256����ʾ�б�
	//font = CreateFont(	-24,					// ����߶�
	//			0,								// ������
	//			0,								// �������ת�Ƕ� Angle Of Escapement
	//			0,								// ������ߵ���ת�Ƕ�Orientation Angle
	//			FW_BOLD,						// ���������
	//			FALSE,							// �Ƿ�ʹ��б��
	//			FALSE,							// �Ƿ�ʹ���»���
	//			FALSE,							// �Ƿ�ʹ��ɾ����
	//			ANSI_CHARSET,					// �����ַ���
	//			OUT_TT_PRECIS,					// �������
	//			CLIP_DEFAULT_PRECIS,			// �ü�����
	//			ANTIALIASED_QUALITY,			// �������
	//			FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
	//			"Courier New");					// ��������
/***********************************����Ĵ���***********************************************************************************/
	font = CreateFont(	-12,					// ����߶�
				0,								// ������
				0,								// �������ת�Ƕ� Angle Of Escapement
				0,								// ������ߵ���ת�Ƕ�Orientation Angle
				FW_BOLD,						// ���������
				FALSE,							// �Ƿ�ʹ��б��
				FALSE,							// �Ƿ�ʹ���»���
				FALSE,							// �Ƿ�ʹ��ɾ����
				SYMBOL_CHARSET,					// �����ַ���
				OUT_TT_PRECIS,					// �������
				CLIP_DEFAULT_PRECIS,			// �ü�����
				ANTIALIASED_QUALITY,			// �������
				FF_DONTCARE|DEFAULT_PITCH,		// Family And Pitch
				"Wingdings");					// ��������

/********************************************************************************************************************************/
	oldfont = (HFONT)SelectObject(hDC, font);	// ѡ��������Ҫ������
	wglUseFontOutlines(	hDC,						// ���õ�ǰ�����豸������ľ��
				0,								// ���ڴ�����ʾ�б�����ĵ�һ���ַ���ASCIIֵ
				255,							// �ַ���
				base,							// ��һ����ʾ�б������
				0.1f,							// ����Ĺ⻬�ȣ�ԽСԽ�⻬��0.0Ϊ��⻬��״̬
				0.2f,							// ��z����ͻ���ľ���
				WGL_FONT_POLYGONS,				// ʹ�ö�����������ַ���ÿ��������ж����ķ���
				gmf);							//һ���������ζ������ݵ�����ĵ�ַ��ÿ������Ԫ��������Ӧ����ʾ�б��ַ����������
	SelectObject(hDC, oldfont);					// ѡ��ԭ��������
	DeleteObject(font);							// ɾ������
}

GLvoid KillFont(GLvoid)							// ɾ����ʾ�б�
{
 	glDeleteLists(base, 256);					// ɾ��256����ʾ�б�
}

GLvoid glPrint(const char *fmt, ...)			// �Զ���GL������庯��
{
	char		text[256];						// �������ִ�
	va_list		ap;								// ָ��һ�������б��ָ��
	if (fmt == NULL)							// ����������򷵻�
		return;	
	va_start(ap, fmt);							// �����ɱ����
	    vsprintf(text, fmt, ap);				// �Ѳ���ֵд���ַ���
	va_end(ap);									// ��������
	float		length=0;						// ��ѯ�ַ����ĳ���
	for (unsigned int loop=0;loop<(strlen(text));loop++)	// ���������ַ����ĳ���
	{
		length+=gmf[text[loop]].gmfCellIncX;		
	}
	glTranslatef(-length/2,0.0f,0.0f);			// ���ַ������������
	glPushAttrib(GL_LIST_BIT);					// ����ʾ�б�����ѹ�����Զ�ջ
	glListBase(base);							// ������ʾ�б�Ļ���ֵΪ0
	glCallLists(strlen(text), GL_UNSIGNED_BYTE, text);		// ������ʾ�б�����ַ���
	glPopAttrib();								// �������Զ�ջ
}


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

int LoadGLTextures()									// ����λͼ(��������Ĵ���)��ת��������
{
	int Status=FALSE;									// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];					// ��������Ĵ洢�ռ�
	memset(TextureImage,0,sizeof(void *)*1);			// ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
//	if (TextureImage[0]=LoadBMP("Data/NeHe.bmp"))
/***********************************����Ĵ���***********************************************************************************/
	if (TextureImage[0]=LoadBMP("Data/Lights.bmp"))			// ����λͼ
/********************************************************************************************************************************/
	{
		Status=TRUE;									// �� Status ��Ϊ TRUE
		glGenTextures(1, &texture[0]);					// ��������

		//// ʹ������λͼ�������� �ĵ�������
		//glBindTexture(GL_TEXTURE_2D, texture[0]);
		//// ��������
		//glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		//glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
/***********************************����Ĵ���***********************************************************************************/
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP  );
		glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP  );
		glEnable(GL_TEXTURE_GEN_S);
		glEnable(GL_TEXTURE_GEN_T);
/********************************************************************************************************************************/	
	}

	if (TextureImage[0])								// �����Ƿ����
	{
		if (TextureImage[0]->data)						// ����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);				// �ͷ�����ͼ��ռ�õ��ڴ�
		}

		free(TextureImage[0]);							// �ͷ�ͼ��ṹ
	}
	return Status;										// ���� Status
}

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
	if (!LoadGLTextures())					// ��������
	{
		return FALSE;					// ʧ���򷵻�
	}
	BuildFont();						// ����������ʾ�б�
	glEnable(GL_LIGHT0);					// ʹ�õ�0�ŵ�
	glEnable(GL_LIGHTING);					// ʹ�ù���
	glBindTexture(GL_TEXTURE_2D, texture[0]);			// ѡ��ʹ�õ�����
/********************************************************************************************************************************/
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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
	glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����

	//glTranslatef(0.0f,0.0f,-10.0f);						// ������Ļһ����λ
	//glRotatef(rot,1.0f,0.0f,0.0f);						// ��X����ת
	//glRotatef(rot*1.5f,0.0f,1.0f,0.0f);					// ��Y����ת
	//glRotatef(rot*1.4f,0.0f,0.0f,1.0f);					// ��Z����ת
	//// ��������λ��������ɫ
	//glColor3f(1.0f*float(cos(rot/20.0f)),1.0f*float(sin(rot/25.0f)),1.0f-0.5f*float(cos(rot/17.0f)));
	//	glPrint("NeHe - %3.2f",rot/50);						// ������ֵ���Ļ
	//rot+=0.5f;											// ������ת����
/***********************************����Ĵ���***********************************************************************************/
	// ���������λ��
	glTranslatef(1.1f*float(cos(rot/16.0f)),0.8f*float(sin(rot/20.0f)),-3.0f);
	glRotatef(rot,1.0f,0.0f,0.0f);							// ��X����ת
	glRotatef(rot*1.2f,0.0f,1.0f,0.0f);						// ��Y����ת
	glRotatef(rot*1.4f,0.0f,0.0f,1.0f);						// ��Z����ת
	glTranslatef(-0.35f,-0.35f,0.1f);						// �ƶ���������ʾ��λ��
	glPrint("N");											// ���ƺ��������
	rot+=0.1f;												// ������ת����
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

	KillFont();							// ɾ������
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
//	if (!CreateGLWindow("NeHe's 3D����ʵ��",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
	if (!CreateGLWindow("NeHe's �����ַ�ʵ��",640,480,16,fullscreen))
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
				}
			}

			if (keys[VK_F1])						// F1��������ô?
			{
				keys[VK_F1]=FALSE;					// ���ǣ�ʹ��Ӧ��Key�����е�ֵΪ FALSE
				KillGLWindow();						// ���ٵ�ǰ�Ĵ���
				fullscreen=!fullscreen;				// �л� ȫ�� / ���� ģʽ
				// �ؽ� OpenGL ����
//				if (!CreateGLWindow("NeHe's 3D����ʵ��",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
				if (!CreateGLWindow("NeHe's �����ַ�ʵ��",640,480,16,fullscreen))
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
