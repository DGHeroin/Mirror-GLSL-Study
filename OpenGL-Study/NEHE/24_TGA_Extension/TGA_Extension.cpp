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
#include <stdarg.h>								// ����ɱ�����ĺ�����ͷ�ļ�
#include <string.h>								// �����ַ�����ͷ�ļ�
#include <stdlib.h>								// C++��׼��
#include <stdio.h>								// �ļ���д��
int		scroll;										// ����������Ļ
int		maxtokens;									// ������չ�ĸ���
int		swidth;										// ���ÿ��
int		sheight;									// ���ø߶�

GLuint		base;									// �ַ���ʾ�б�Ŀ�ʼֵ
typedef	struct										// ��������TGAͼ���ļ��ṹ
{
	GLubyte	*imageData;								// ͼ������ָ��
	GLuint	bpp;									// ÿ��������ռ��λ��������Ϊ24��32��
	GLuint	width;									// ͼ����
	GLuint	height;									// ͼ��߶�
	GLuint	texID;									// �����IDֵ
} TextureImage;										// �ṹ����

TextureImage	textures[1];						// ����һ������

bool LoadTGA(TextureImage *texture, char *filename)							// ��TGA�ļ��������ڴ�
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// ��ѹ����TGA�ļ�ͷ
	GLubyte		TGAcompare[12];												// ���������ļ�ͷ��Ϣ
	GLubyte		header[6];													// ���������õ�ͼ����Ϣ�����ߣ�λ��
	GLuint		bytesPerPixel;												// ��¼ÿ����ɫ��ռ�õ��ֽ���
	GLuint		imageSize;													// ��¼�ļ���С
	GLuint		temp;														// ��ʱ����
	GLuint		type=GL_RGBA;												// ����Ĭ�ϵĸ�ʽΪGL_RGBA����32λͼ��
	FILE *file = fopen(filename, "rb");										// ��һ��TGA�ļ�

	if(	file==NULL ||														// �ļ�����ô?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// �Ƿ����12���ֽڵ��ļ�ͷ?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0		||			// �Ƿ���������Ҫ�ĸ�ʽ?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// ����Ƕ�ȡ��������ͼ����Ϣ
	{
		if (file == NULL)													// �ļ������ڷ��ش���
			return false;							
		else
		{
			fclose(file);													// �ر��ļ����ش���
			return false;							
		}
	}
	texture->width  = header[1] * 256 + header[0];							// ��¼�ļ��߶�
	texture->height = header[3] * 256 + header[2];							// ��¼�ļ����

 	if(	texture->width	<=0	||												// ����Ƿ�С��0
		texture->height	<=0	||												// �߶��Ƿ�С��0
		(header[4]!=24 && header[4]!=32))									// TGA�ļ���24/32λ��
	{
		fclose(file);														// ���ʧ�ܹر��ļ������ش���
		return false;								
	}
	texture->bpp	= header[4];											// ��¼�ļ���λ��
	bytesPerPixel	= texture->bpp/8;										// ��¼ÿ��������ռ���ֽ���
	imageSize	= texture->width*texture->height*bytesPerPixel;				// ����TGA�ļ���������Ҫ���ڴ��С
	texture->imageData=(GLubyte *)malloc(imageSize);						// �����ڴ�ȥ����TGA����

	if(	texture->imageData==NULL ||											// ϵͳ�Ƿ�������㹻���ڴ棿
		fread(texture->imageData, 1, imageSize, file)!=imageSize)			// �Ƿ�ɹ������ڴ�?
	{
		if(texture->imageData!=NULL)										// �Ƿ������ݱ�����
			free(texture->imageData);										// ����ǣ����ͷ����������

		fclose(file);														// �ر��ļ�
		return false;														// ���ش���
	}
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// ѭ�����е�����
	{																		// ����R��B��ֵ
		temp=texture->imageData[i];						
		texture->imageData[i] = texture->imageData[i + 2];		
		texture->imageData[i + 2] = temp;				
	}

	fclose (file);															// �ر��ļ�
	// ��������
	glGenTextures(1, &texture[0].texID);									// ������������¼����ID

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);							// ������
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// ���ù�����Ϊ���Թ���
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	if (texture[0].bpp==24)													// �Ƿ�Ϊ24λͼ��
	{
		type=GL_RGB;														// �������������ΪGL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;															// �������ɣ��ɹ�����
}

GLvoid BuildFont(GLvoid)									// ����������ʾ�б�
{
	base=glGenLists(256);									// ����256����ʾ�б�
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);		// ������
	for (int loop1=0; loop1<256; loop1++)					// ѭ������256����ʾ�б�
	{
		float cx=float(loop1%16)/16.0f;						// ��ǰ�ַ���Xλ��
		float cy=float(loop1/16)/16.0f;						// ��ǰ�ַ���Yλ��

		glNewList(base+loop1,GL_COMPILE);					// ��ʼ������ʾ�б�
			glBegin(GL_QUADS);								// ����һ���ı������������ַ�ͼ��
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// ���·���������
				glVertex2d(0,16);							// ���·�����
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// ���·���������
				glVertex2i(16,16);							// ���·�����
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);	// ���Ϸ���������
				glVertex2i(16,0);							// ���Ϸ�����
				glTexCoord2f(cx,1.0f-cy-0.001f);			// ���Ϸ���������
				glVertex2i(0,0);							// ���Ϸ�����
			glEnd();										// �ı��δ������
			glTranslated(14,0,0);							// �����ƶ�14����λ
		glEndList();										// ����������ʾ�б�
	}									
}

GLvoid KillFont(GLvoid)
{
	glDeleteLists(base,256);								// ���ڴ���ɾ��256����ʾ�б�
}

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)
{
	char	text[1024];										// �������ǵ��ַ�
	va_list	ap;												// ָ���һ������

	if (fmt == NULL)										// ���Ҫ��ʾ���ַ�Ϊ���򷵻�
		return;									

	va_start(ap, fmt);										// ��ʼ�������������ѽ��д�뵽text��
	    vsprintf(text, fmt, ap);							
	va_end(ap);									

	if (set>1)												// ����ַ�������1��ʹ�õڶ����ַ���
	{
		set=1;									
	}

	glEnable(GL_TEXTURE_2D);								// ʹ������ӳ��
	glLoadIdentity();										// �����ӿھ���
	glTranslated(x,y,0);									// ƽ�Ƶ�(x,y,0)��
	glListBase(base-32+(128*set));							// ѡ���ַ���

	glScalef(1.0f,2.0f,1.0f);								// ��Y��Ŵ�һ��

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// ���ַ�д�뵽��Ļ
	glDisable(GL_TEXTURE_2D);								// ��ֹ����ӳ�� 
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

//GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// ����OpenGL���ڴ�С
//{
//	if (height==0)										// ��ֹ�����
//	{
//		height=1;										// ��Height��Ϊ1
//	}
//
//	glViewport(0,0,width,height);						// ���õ�ǰ���ӿ�
//
//	glMatrixMode(GL_PROJECTION);						// ѡ��ͶӰ����
//	glLoadIdentity();									// ����ͶӰ����
//
//	// �����ӿڵĴ�С
//	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
//
//	glMatrixMode(GL_MODELVIEW);							// ѡ��ģ�͹۲����
//	glLoadIdentity();									// ����ģ�͹۲����
//}
/***********************************����Ĵ���***********************************************************************************/
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	swidth=width;										// ���ü��о���Ϊ���ڴ�С
	sheight=height;									
	if (height==0)										// ��ֹ�߶�Ϊ0ʱ����0��
	{
		height=1;								
	}
	glViewport(0,0,width,height);						// ���ô��ڿɼ���
	glMatrixMode(GL_PROJECTION);							
	glLoadIdentity();								
	glOrtho(0.0f,640,480,0.0f,-1.0f,1.0f);				// �����ӿڴ�СΪ640x480
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();						
}

/********************************************************************************************************************************/
int InitGL(GLvoid)										// �˴���ʼ��OpenGL������������
{
	glShadeModel(GL_SMOOTH);							// ������Ӱƽ��
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// ��ɫ����
	glClearDepth(1.0f);									// ������Ȼ���
	//glEnable(GL_DEPTH_TEST);							// ������Ȳ���
/***********************************����Ĵ���***********************************************************************************/
	if (!LoadTGA(&textures[0],"Data/Font.TGA"))			// ������������
	{
		return false;									// ����ʧ���򷵻�
	}

	BuildFont();										// ��������
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);	// ����������
/********************************************************************************************************************************/
	//glDepthFunc(GL_LEQUAL);								// ������Ȳ��Ե�����
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// ����ϵͳ��͸�ӽ�������
	return TRUE;										// ��ʼ�� OK
}

//int DrawGLScene(GLvoid)									// �����￪ʼ�������еĻ���
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����Ļ����Ȼ���
//	glLoadIdentity();									// ���õ�ǰ��ģ�͹۲����
//	return TRUE;										// һ�� OK
//}
/***********************************����Ĵ���***********************************************************************************/
int DrawGLScene(GLvoid)			
{
	char	*token;										// ������չ�ַ���
	int	cnt=0;											// ��¼��չ�ַ����ĸ���

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �����������Ȼ���

	glColor3f(1.0f,0.5f,0.5f);							// ����Ϊ��ɫ
	glPrint(50,16,1,"Renderer");							
	glPrint(80,48,1,"Vendor");						
	glPrint(66,80,1,"Version");		
	glColor3f(1.0f,0.7f,0.4f);							// ����Ϊ�ٻ�ɫ
	glPrint(200,16,1,(char *)glGetString(GL_RENDERER));	// ��ʾOpenGL��ʵ����֯
	glPrint(200,48,1,(char *)glGetString(GL_VENDOR));	// ��ʾ������
	glPrint(200,80,1,(char *)glGetString(GL_VERSION));	// ��ʾ��ǰ�汾

	glColor3f(0.5f,0.5f,1.0f);							// ����Ϊ��ɫ
	glPrint(192,432,1,"NeHe Productions");				// ����Ļ�ĵ׶�д��NeHe Productions�ַ���
	glLoadIdentity();									// ����ģ�ͱ任����
	glColor3f(1.0f,1.0f,1.0f);							// ����Ϊ��ɫ
	glBegin(GL_LINE_STRIP);					
		glVertex2d(639,417);							
		glVertex2d(  0,417);							
		glVertex2d(  0,480);							
		glVertex2d(639,480);						
		glVertex2d(639,128);					
	glEnd();								
	glBegin(GL_LINE_STRIP);					
		glVertex2d(  0,128);						
		glVertex2d(639,128);							
		glVertex2d(639,  1);						
		glVertex2d(  0,  1);							
		glVertex2d(  0,417);						
	glEnd();		
	glScissor(1	,int(0.135416f*sheight),swidth-2,int(0.597916f*sheight));	// �����������
	glEnable(GL_SCISSOR_TEST);												// ʹ�ü��ò���

	char* text=(char*)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);	// Ϊ����OpenGL��չ���ַ��������ڴ�ռ�
	strcpy (text,(char *)glGetString(GL_EXTENSIONS));						// ����OpenGL��չ�ַ���
	token=strtok(text," ");													// ���ո�ָ�text�ַ��������ѷָ����ַ���������token��
	while(token!=NULL)														// ���token��ΪNULL
	{
		cnt++;																// ���Ӽ�����
		if (cnt>maxtokens)													// ��¼������չ������
		{
			maxtokens=cnt;							
		}
		glColor3f(0.5f,1.0f,0.5f);											// ������ɫΪ��ɫ
		glPrint(0,96+(cnt*32)-scroll,0,"%i",cnt);							// ���Ƶڼ�����չ��

		glColor3f(1.0f,1.0f,0.5f);											// ������ɫΪ��ɫ
		glPrint(50,96+(cnt*32)-scroll,0,token);								// �����i����չ��

		token=strtok(NULL," ");												// ������һ����չ��
	}
	glDisable(GL_SCISSOR_TEST);												// ���ü��ò���

	free (text);															// �ͷŷ�����ڴ�
	glFlush();																// ִ�����е���Ⱦ����
	return TRUE;															// �ɹ�����
}

/********************************************************************************************************************************/
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
/***********************************����Ĵ���***********************************************************************************/
	KillFont();									// ɾ������
/********************************************************************************************************************************/
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
//	if (!CreateGLWindow("NeHe's OpenGL ������",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
	if (!CreateGLWindow("NeHe's OpenGL TGA�ļ�����չ����ʾ",640,480,16,fullscreen))
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
					if (keys[VK_UP] && (scroll>0))						// ���ϵļ�ͷ�Ƿ񱻰���?
					{
						scroll-=2;										// ����ǣ�����scroll��ֵ
					}
					if (keys[VK_DOWN] && (scroll<32*(maxtokens-9)))		// ���µļ�ͷ�Ƿ񱻰�ס
					{
						scroll+=2;										// ����ǣ�����scroll��ֵ
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
//				if (!CreateGLWindow("NeHe's OpenGL ������",640,480,16,fullscreen))
/***********************************����Ĵ���***********************************************************************************/
				if (!CreateGLWindow("NeHe's OpenGL TGA�ļ�����չ����ʾ",640,480,16,fullscreen))
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
