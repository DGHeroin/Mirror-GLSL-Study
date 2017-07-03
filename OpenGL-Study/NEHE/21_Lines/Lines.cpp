/*
 *		��Щ��������Jeff Molofee ��2000��д�ģ��ٴ��ұ����лFredric Echols����Ϊ��ʹ��Щ���뿴���������͸�Ч����ʹ������ʹ�á�
 *		����㷢����Щ�����������������������ҵ�վ�� http://nehe.gamedev.net/default.asp ��������֪����
 *********************************************************************************************************************************
 *		����ĵ�����DancingWind����ģ������ʲô������e-mail:zhouwei02@mails.tsinghua.edu.cn
 *		//old code			//�����˴�����//ɾ��
 *		/********************************************************************************************************************/	/*
 *		new code			//�µĴ���λ������/*************/ /*֮�䣬���ڲ���
 *		/********************************************************************************************************************/	/*
/***********************************����Ĵ���***********************************************************************************/

#include	<windows.h>							
#include	<stdio.h>							
#include	<stdarg.h>						
#include	 <GL/glew.h>						
#include	<GL/glaux.h>							

HDC			hDC=NULL;							
HGLRC		hRC=NULL;							
HWND		hWnd=NULL;							
HINSTANCE	hInstance;	

bool		keys[256];							
bool		vline[11][10];						// ���洹ֱ�����11�������У�ÿ�������е�10���Ƿ��߹�
bool		hline[10][11];						//����ˮƽ�����11�������У�ÿ�������е�10���Ƿ��߹�
bool		ap;									// A���Ƿ��Ѿ�����
bool		filled;								// �����Ƿ�����?
bool		gameover;							// ��Ϸ�Ƿ����?
bool		anti=TRUE;							// �Ƿ����÷�����?
bool		active=TRUE;							
bool		fullscreen=TRUE;			

int		loop1;									// ͨ��ѭ������
int		loop2;									// ͨ��ѭ������
int		delay;									// ���˵���ͣʱ��
int		adjust=3;								// ������ʾ���ٶ�
int		lives=5;								// ��ҵ�����
int		level=1;								// �ڲ���Ϸ�ĵȼ�
int		level2=level;							// ��ʾ����Ϸ�ĵȼ�
int		stage=1;								// ��Ϸ�Ĺؿ�

struct	object									// ��¼��Ϸ�еĶ���
{
	int	fx, fy;									// ʹ�ƶ����ƽ��
	int	x, y;									// ��ǰ��Ϸ�ߵ�λ��
	float	spin;								// ��ת����
};

struct	object	player;							// �����Ϣ
struct	object	enemy[9];						// ���9�����˵���Ϣ
struct	object	hourglass;						// ������Ϣ

struct			 								// ����ʱ����Ϣ�Ľṹ
{
  __int64       frequency;						// Ƶ��
  float         resolution;						// ʱ����
  unsigned long mm_timer_start;					// ��ý���ʱ���Ŀ�ʼʱ��
  unsigned long mm_timer_elapsed;				// ��ý���ʱ���Ŀ�ʼʱ��
  bool		performance_timer;					// ʹ��Performance Timer?
  __int64       performance_timer_start;		// Performance Timer��ʱ���Ŀ�ʼʱ��
  __int64       performance_timer_elapsed;		// Performance Timer��ʱ���Ŀ�ʼʱ��
} timer;		

int		steps[6]={ 1, 2, 4, 5, 10, 20 };		// ����������ʾ���ٶ�
GLuint		texture[2];							// �ַ�����
GLuint		base;								// �ַ���ʾ�б�Ŀ�ʼֵ

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		

void TimerInit(void)									// ��ʼ�����ǵļ�ʱ��
{
	memset(&timer, 0, sizeof(timer));					// ��ռ�ʱ���ṹ

	// ���Performance Counter�Ƿ���ã������򴴽�
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		// ���������
		timer.performance_timer	= FALSE;				// ����Performance TimerΪfalse
		timer.mm_timer_start	= timeGetTime();		// ʹ����ͨ�ļ�ʱ��
		timer.resolution	= 1.0f/1000.0f;				// ���õ�λΪ����
		timer.frequency		= 1000;						// ����Ƶ��Ϊ1000
		timer.mm_timer_elapsed	= timer.mm_timer_start;	// ������ʧ��ʱ��Ϊ��ǰ��ʱ��
	}
	else
	{
		// ʹ��Performance Counter��ʱ��
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer		= TRUE;				// ����Performance TimerΪTRUE
		// �����ʱ�ľ�ȷ��
		timer.resolution		= (float) (((double)1.0f)/((double)timer.frequency));
		// ������ʧ��ʱ��Ϊ��ǰ��ʱ��
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()									// ���ؾ�����ʱ�䣬�Ժ���Ϊ��λ
{
	__int64 time;										// ʹ��64λ������

	if (timer.performance_timer)						// �Ƿ�ʹ��Performance Timer��ʱ��?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);		// ���ص�ǰ��ʱ��
		// ����ʱ���
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		// ʹ����ͨ�ļ�ʱ��������ʱ���
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void ResetObjects(void)								// ������Һ͵���
{
	player.x=0;										// ������������Ͻ�
	player.y=0;								
	player.fx=0;								
	player.fy=0;	
	for (loop1=0; loop1<(stage*level); loop1++)		// ѭ���漴�������еĵ���
	{
		enemy[loop1].x=5+rand()%6;					
		enemy[loop1].y=rand()%11;					
		enemy[loop1].fx=enemy[loop1].x*60;				
		enemy[loop1].fy=enemy[loop1].y*40;				
	}
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

int LoadGLTextures()								
{
	int Status=FALSE;							
	AUX_RGBImageRec *TextureImage[2];					
	memset(TextureImage,0,sizeof(void *)*2);				
	if 	((TextureImage[0]=LoadBMP("Data/Font.bmp")) &&			// ������������
		 (TextureImage[1]=LoadBMP("Data/Image.bmp")))			// ����ͼ������
	{
		Status=TRUE;							

		glGenTextures(2, &texture[0]);					

		for (loop1=0; loop1<2; loop1++)					
		{
			glBindTexture(GL_TEXTURE_2D, texture[loop1]);
			glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[loop1]->sizeX, TextureImage[loop1]->sizeY,
				0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[loop1]->data);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		}

		for (loop1=0; loop1<2; loop1++)					
		{
			if (TextureImage[loop1])				
			{
				if (TextureImage[loop1]->data)			
				{
					free(TextureImage[loop1]->data);	
				}
				free(TextureImage[loop1]);			
			}
		}
	}
	return Status;							
}

GLvoid BuildFont(GLvoid)							
{
	base=glGenLists(256);							
	glBindTexture(GL_TEXTURE_2D, texture[0]);				
	for (loop1=0; loop1<256; loop1++)					
	{
		float cx=float(loop1%16)/16.0f;					
		float cy=float(loop1/16)/16.0f;					

		glNewList(base+loop1,GL_COMPILE);				
			glBegin(GL_QUADS);					
				glTexCoord2f(cx,1.0f-cy-0.0625f);		
				glVertex2d(0,16);				
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	
				glVertex2i(16,16);				
				glTexCoord2f(cx+0.0625f,1.0f-cy);		
				glVertex2i(16,0);				
				glTexCoord2f(cx,1.0f-cy);			
				glVertex2i(0,0);				
			glEnd();						
			glTranslated(15,0,0);					
		glEndList();							
	}									
}

GLvoid KillFont(GLvoid)		
{
	glDeleteLists(base,256);						
}
GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)
{
	char		text[256];						
	va_list		ap;							

	if (fmt == NULL)		
		return;								

	va_start(ap, fmt);							
	    vsprintf(text, fmt, ap);						
	va_end(ap);								

	if (set>1)								
	{
		set=1;							
	}
	glEnable(GL_TEXTURE_2D);						
	glLoadIdentity();							
	glTranslated(x,y,0);							
	glListBase(base-32+(128*set));						

	if (set==0)								
	{
		glScalef(1.5f,2.0f,1.0f);					
	}

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);			
	glDisable(GL_TEXTURE_2D);						
}
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)				 
{
	if (height==0)							
	{
		height=1;							
	}

	glViewport(0,0,width,height);						

	glMatrixMode(GL_PROJECTION);						
	glLoadIdentity();							

	glOrtho(0.0f,width,height,0.0f,-1.0f,1.0f);

	glMatrixMode(GL_MODELVIEW);						
	glLoadIdentity();							
}
int InitGL(GLvoid)
{
	if (!LoadGLTextures())							
	{
		return FALSE;							
	}

	BuildFont();								

	glShadeModel(GL_SMOOTH);						
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);					
	glClearDepth(1.0f);							
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);					
	glEnable(GL_BLEND);							
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	return TRUE;								
}

int DrawGLScene(GLvoid)								
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// ѡ���ַ�����
	glColor3f(1.0f,0.5f,1.0f);						
	glPrint(207,24,0,"GRID CRAZY");							// ������Ϸ����"GRID CRAZY"
	glColor3f(1.0f,1.0f,0.0f);						
	glPrint(20,20,1,"Level:%2i",level2);					// ���Ƶ�ǰ�ļ���
	glPrint(20,40,1,"Stage:%2i",stage);						// ���Ƶ�ǰ����Ĺؿ�
	if (gameover)											// ��Ϸ�Ƿ����?
	{
		glColor3ub(rand()%255,rand()%255,rand()%255);		// ���ѡ��һ����ɫ
		glPrint(472,20,1,"GAME OVER");						// ���� GAME OVER �ַ�������Ļ
		glPrint(456,40,1,"PRESS SPACE");					// ��ʾ��Ұ��ո�����¿�ʼ
	}
	for (loop1=0; loop1<lives-1; loop1++)					//ѭ��������ҵ�ʣ������
	{
		glLoadIdentity();						
		glTranslatef(490+(loop1*40.0f),40.0f,0.0f);			// �ƶ�����Ļ���Ͻ�
		glRotatef(-player.spin,0.0f,0.0f,1.0f);				// ��ת���Ƶ�����ͼ��
		glColor3f(0.0f,1.0f,0.0f);							// �����������
		glBegin(GL_LINES);									// �������ͼ��
			glVertex2d(-5,-5);					
			glVertex2d( 5, 5);					
			glVertex2d( 5,-5);					
			glVertex2d(-5, 5);					
		glEnd();							
		glRotatef(-player.spin*0.5f,0.0f,0.0f,1.0f);			
		glColor3f(0.0f,0.75f,0.0f);					
		glBegin(GL_LINES);						
			glVertex2d(-7, 0);					
			glVertex2d( 7, 0);					
			glVertex2d( 0,-7);					
			glVertex2d( 0, 7);					
		glEnd();							
	}
	filled=TRUE;											// �ڲ���ǰ��������������ΪTRUE
	glLineWidth(2.0f);										// �����߿�Ϊ2.0f
	glDisable(GL_LINE_SMOOTH);								// ���÷�����
	glLoadIdentity();							
	for (loop1=0; loop1<11; loop1++)						// ѭ��11����
	{
		for (loop2=0; loop2<11; loop2++)					// ѭ��ÿ���ߵ��߶�
		{
			glColor3f(0.0f,0.5f,1.0f);						// ������Ϊ��ɫ
			if (hline[loop1][loop2])						// �Ƿ��߹���
			{
				glColor3f(1.0f,1.0f,1.0f);					// �ǣ�����Ϊ��ɫ
			}
			if (loop1<10)									// ����ˮƽ��
			{
				if (!hline[loop1][loop2])					// �����ǰ�߶�û���߹��������
				{
					filled=FALSE;				
				}
				glBegin(GL_LINES);							// ���Ƶ�ǰ���߶�
					glVertex2d(20+(loop1*60),70+(loop2*40));	
					glVertex2d(80+(loop1*60),70+(loop2*40));	
				glEnd();					
			}
			glColor3f(0.0f,0.5f,1.0f);						// ������Ϊ��ɫ
			if (vline[loop1][loop2])						// �Ƿ��߹�
			{
				glColor3f(1.0f,1.0f,1.0f);					// �ǣ�����Ϊ��ɫ
			}
			if (loop2<10)									// ���ƴ�ֱ��
			{
				if (!vline[loop1][loop2])					// �����ǰ�߶�û���߹��������
				{
					filled=FALSE;				
				}
				glBegin(GL_LINES);							// ���Ƶ�ǰ���߶�
					glVertex2d(20+(loop1*60),70+(loop2*40));	
					glVertex2d(20+(loop1*60),110+(loop2*40));	
				glEnd();					
			}
			glEnable(GL_TEXTURE_2D);						// ʹ������ӳ��
			glColor3f(1.0f,1.0f,1.0f);						// ����Ϊ��ɫ
			glBindTexture(GL_TEXTURE_2D, texture[1]);		// ������
			if ((loop1<10) && (loop2<10))					// �����߹����ı���
			{
				// ����ı����Ƿ��߹�?
				if (hline[loop1][loop2] && hline[loop1][loop2+1] && vline[loop1][loop2] && vline[loop1+1][loop2])
				{
					glBegin(GL_QUADS);			// �ǣ��������
						glTexCoord2f(float(loop1/10.0f)+0.1f,1.0f-(float(loop2/10.0f)));
						glVertex2d(20+(loop1*60)+59,(70+loop2*40+1));	
						glTexCoord2f(float(loop1/10.0f),1.0f-(float(loop2/10.0f)));
						glVertex2d(20+(loop1*60)+1,(70+loop2*40+1));	
						glTexCoord2f(float(loop1/10.0f),1.0f-(float(loop2/10.0f)+0.1f));
						glVertex2d(20+(loop1*60)+1,(70+loop2*40)+39);	
						glTexCoord2f(float(loop1/10.0f)+0.1f,1.0f-(float(loop2/10.0f)+0.1f));
						glVertex2d(20+(loop1*60)+59,(70+loop2*40)+39);	
					glEnd();				
				}
			}
			glDisable(GL_TEXTURE_2D);				
		}
	}
	glLineWidth(1.0f);	
	if (anti)												// �Ƿ����÷�����?
	{
		glEnable(GL_LINE_SMOOTH);					
	}
	if (hourglass.fx==1)							
	{
		glLoadIdentity();						
		glTranslatef(20.0f+(hourglass.x*60),70.0f+(hourglass.y*40),0.0f);	
		glRotatef(hourglass.spin,0.0f,0.0f,1.0f);			
		glColor3ub(rand()%255,rand()%255,rand()%255);			
		glBegin(GL_LINES);
			glVertex2d(-5,-5);					
			glVertex2d( 5, 5);					
			glVertex2d( 5,-5);					
			glVertex2d(-5, 5);					
			glVertex2d(-5, 5);					
			glVertex2d( 5, 5);					
			glVertex2d(-5,-5);					
			glVertex2d( 5,-5);					
		glEnd();							
	}

	glLoadIdentity();
	glTranslatef(player.fx+20.0f,player.fy+70.0f,0.0f);		// ������ҵ�λ��
	glRotatef(player.spin,0.0f,0.0f,1.0f);					// ��ת����
	glColor3f(0.0f,1.0f,0.0f);						
	glBegin(GL_LINES);							
		glVertex2d(-5,-5);						
		glVertex2d( 5, 5);						
		glVertex2d( 5,-5);						
		glVertex2d(-5, 5);						
	glEnd();								
	glRotatef(player.spin*0.5f,0.0f,0.0f,1.0f);
	glColor3f(0.0f,0.75f,0.0f);						
	glBegin(GL_LINES);							
		glVertex2d(-7, 0);						
		glVertex2d( 7, 0);						
		glVertex2d( 0,-7);						
		glVertex2d( 0, 7);						
	glEnd();		
	for (loop1=0; loop1<(stage*level); loop1++)
	{
		glLoadIdentity();						
		glTranslatef(enemy[loop1].fx+20.0f,enemy[loop1].fy+70.0f,0.0f);
		glColor3f(1.0f,0.5f,0.5f);					
		glBegin(GL_LINES);						
			glVertex2d( 0,-7);					
			glVertex2d(-7, 0);					
			glVertex2d(-7, 0);					
			glVertex2d( 0, 7);					
			glVertex2d( 0, 7);					
			glVertex2d( 7, 0);					
			glVertex2d( 7, 0);					
			glVertex2d( 0,-7);					
		glEnd();		
		glRotatef(enemy[loop1].spin,0.0f,0.0f,1.0f);			
		glColor3f(1.0f,0.0f,0.0f);					
		glBegin(GL_LINES);						
			glVertex2d(-7,-7);					
			glVertex2d( 7, 7);					
			glVertex2d(-7, 7);					
			glVertex2d( 7,-7);					
		glEnd();							
	}
	return TRUE;								
}

GLvoid KillGLWindow(GLvoid)							
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

	KillFont();								// ɾ������������
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
	wc.lpszClassName	= "OpenG";								// �趨������

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

int WINAPI WinMain(	HINSTANCE	hInstance,				
			HINSTANCE	hPrevInstance,				
			LPSTR		lpCmdLine,				
			int		nCmdShow)				
{
	MSG	msg;								
	BOOL	done=FALSE;							

	
	if (MessageBox(NULL,"Would You Like To Run In Fullscreen Mode?", "Start FullScreen?",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;						
	}

	if (!CreateGLWindow("NeHe's Line Tutorial",640,480,16,fullscreen))	
	{
		return 0;							
	}

	ResetObjects();								// ������Һ͵���
	TimerInit();								// ��ʼ��ʱ���ʱ��

	while(!done)								
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))			
		{
			if (msg.message==WM_QUIT)				
			{
				done=TRUE;					
			}
			else							
			{
				TranslateMessage(&msg);				
				DispatchMessage(&msg);				
			}
		}
		else								
		{
			float start=TimerGetTime();				// ���ص�ǰ��ʱ��

			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	
			{
				done=TRUE;					
			}
			else							
			{
				SwapBuffers(hDC);				
			}

			while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}// �ٶȿ�Ļ����������ѭ��
			if (keys[VK_F1])
			{
				keys[VK_F1]=FALSE;				
				KillGLWindow();					
				fullscreen=!fullscreen;				
				if (!CreateGLWindow("NeHe's Line Tutorial",640,480,16,fullscreen))
				{
					return 0;				
				}
			}
			if (keys['A'] && !ap)					// ���'A' �������£����÷�����
			{
				ap=TRUE;					
				anti=!anti;					
			}
			if (!keys['A'])						
			{
				ap=FALSE;					
			}
			if (!gameover && active)				// �����Ϸû�н������������Ϸѭ��
			{
				for (loop1=0; loop1<(stage*level); loop1++)	// ѭ����ͬ���Ѷȵȼ�
				{
					if ((enemy[loop1].x<player.x) && (enemy[loop1].fy==enemy[loop1].y*40))
					{
						enemy[loop1].x++;		
					}

					if ((enemy[loop1].x>player.x) && (enemy[loop1].fy==enemy[loop1].y*40))
					{
						enemy[loop1].x--;		
					}

					if ((enemy[loop1].y<player.y) && (enemy[loop1].fx==enemy[loop1].x*60))
					{
						enemy[loop1].y++;		
					}

					if ((enemy[loop1].y>player.y) && (enemy[loop1].fx==enemy[loop1].x*60))
					{
						enemy[loop1].y--;		
					}
					if (delay>(3-level) && (hourglass.fx!=2))		// ���û�гԵ�ʱ��ֹͣ��
					{
						delay=0;					// ����ʱ��ֹͣ��
						for (loop2=0; loop2<(stage*level); loop2++)	// ѭ������ÿ�����˵�λ��
						{
							if (enemy[loop2].fx<enemy[loop2].x*60)	
							{
								enemy[loop2].fx+=steps[adjust];	
								enemy[loop2].spin+=steps[adjust];	
							}
							if (enemy[loop2].fx>enemy[loop2].x*60)	
							{
								enemy[loop2].fx-=steps[adjust];	
								enemy[loop2].spin-=steps[adjust];	
							}
							if (enemy[loop2].fy<enemy[loop2].y*40)	
							{
								enemy[loop2].fy+=steps[adjust];	
								enemy[loop2].spin+=steps[adjust];	
							}
							if (enemy[loop2].fy>enemy[loop2].y*40)	
							{
								enemy[loop2].fy-=steps[adjust];	
								enemy[loop2].spin-=steps[adjust];	
							}
						}
					}
					// ���˵�λ�ú���ҵ�λ������?
					if ((enemy[loop1].fx==player.fx) && (enemy[loop1].fy==player.fy))
					{
						lives--;				// ����ǣ�����ֵ��1

						if (lives==0)			// �������ֵΪ0������Ϸ����
						{
							gameover=TRUE;		
						}

						ResetObjects();			// �������е���Ϸ����
						PlaySound("Data/Die.wav", NULL, SND_SYNC);	// ��������������
					}
				}
				if (keys[VK_RIGHT] && (player.x<10) && (player.fx==player.x*60) && (player.fy==player.y*40))
				{
					hline[player.x][player.y]=TRUE;		
					player.x++;				
				}
				if (keys[VK_LEFT] && (player.x>0) && (player.fx==player.x*60) && (player.fy==player.y*40))
				{
					player.x--;				
					hline[player.x][player.y]=TRUE;		
				}
				if (keys[VK_DOWN] && (player.y<10) && (player.fx==player.x*60) && (player.fy==player.y*40))
				{
					vline[player.x][player.y]=TRUE;		
					player.y++;				
				}
				if (keys[VK_UP] && (player.y>0) && (player.fx==player.x*60) && (player.fy==player.y*40))
				{
					player.y--;				
					vline[player.x][player.y]=TRUE;		
				}
				if (player.fx<player.x*60)			
				{
					player.fx+=steps[adjust];		
				}
				if (player.fx>player.x*60)			
				{
					player.fx-=steps[adjust];		
				}
				if (player.fy<player.y*40)			
				{
					player.fy+=steps[adjust];		
				}
				if (player.fy>player.y*40)			
				{
					player.fy-=steps[adjust];		
				}
			}
			else								// �����Ϸ����
			{
				if (keys[' '])					// ���¿ո�
				{
					gameover=FALSE;				// ��ʼ�µ�һ��
					filled=TRUE;				// �������еı���
					level=1;				
					level2=1;			
					stage=0;				
					lives=5;				
				}
			}
			if (filled)							// ���������Ƿ�����
			{
				PlaySound("Data/Complete.wav", NULL, SND_SYNC);	// ���Ź�������
				stage++;						// ������Ϸ�Ѷ�
				if (stage>3)					// �����ǰ�Ĺؿ�����3������뵽��һ����Ĺؿ�?
				{
					stage=1;					// ���õ�ǰ�Ĺؿ�
					level++;					// ���Ӵ�ؿ���ֵ
					level2++;				
					if (level>3)				
					{
						level=3;				// �����ؿ�����3����������
						lives++;				// ���һ�ָ���ҽ���һ������
						if (lives>5)			// ��������5����������������
						{
							lives=5;		
						}
					}
				}
				ResetObjects();					

				for (loop1=0; loop1<11; loop1++)
				{
					for (loop2=0; loop2<11; loop2++)	
					{
						if (loop1<10)			
						{
							hline[loop1][loop2]=FALSE;	
						}
						if (loop2<10)			
						{
							vline[loop1][loop2]=FALSE;	
						}
					}
				}
			}

			if ((player.fx==hourglass.x*60) && (player.fy==hourglass.y*40) && (hourglass.fx==1))
			{
				// ����һ������
				PlaySound("Data/freeze.wav", NULL, SND_ASYNC | SND_LOOP);
				hourglass.fx=2;					// ����fxΪ2����ʾ�Ե�ʱ��ֹͣ��
				hourglass.fy=0;					// ����fyΪ0
			}
			player.spin+=0.5f*steps[adjust];	// ��ת����
			if (player.spin>360.0f)					
			{
				player.spin-=360;				
			}
			hourglass.spin-=0.25f*steps[adjust];// ��ת����
			if (hourglass.spin<0.0f)				
			{
				hourglass.spin+=360.0f;				
			}
			hourglass.fy+=steps[adjust];		// ����fy��ֵ����������һ����ʱ�򣬲���ʱ��ֹͣ������
			if ((hourglass.fx==0) && (hourglass.fy>6000/level))	
			{							
				PlaySound("Data/hourglass.wav", NULL, SND_ASYNC);	
				hourglass.x=rand()%10+1;			
				hourglass.y=rand()%11;				
				hourglass.fx=1;					//fx=1��ʾʱ��ֹͣ������					
				hourglass.fy=0;					
			}
			if ((hourglass.fx==1) && (hourglass.fy>6000/level))	
			{				
				hourglass.fx=0;					// ��ʧ������ʱ��ֹͣ��
				hourglass.fy=0;					
			}
			if ((hourglass.fx==2) && (hourglass.fy>500+(500*level)))
			{							
				PlaySound(NULL, NULL, 0);		// ֹͣ��������
				hourglass.fx=0;					// ���ñ���
				hourglass.fy=0;					
			}
			delay++;							// ���ӵ��˵��ӳټ�������ֵ
		}
	}

	// �ر�
	KillGLWindow();								// ɾ������
	return (msg.wParam);						// �˳�����
}
/*********************************************************************************************************************************/