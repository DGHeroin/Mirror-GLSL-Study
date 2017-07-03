/*
 *		这些代码是由Jeff Molofee 与2000年写的，再次我必须感谢Fredric Echols，因为他使这些代码看起来更简洁和高效，并使他易于使用。
 *		如果你发现这些代码对你有所帮助，请访问我的站点 http://nehe.gamedev.net/default.asp ，并让我知道。
 *********************************************************************************************************************************
 *		这份文档是由DancingWind翻译的，如果有什么错误请e-mail:zhouwei02@mails.tsinghua.edu.cn
 *		//old code			//不用了代码用//删除
 *		/********************************************************************************************************************/	/*
 *		new code			//新的代码位于两排/*************/ /*之间，便于查找
 *		/********************************************************************************************************************/	/*
/***********************************新添的代码***********************************************************************************/

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
bool		vline[11][10];						// 保存垂直方向的11根线条中，每根线条中的10段是否被走过
bool		hline[10][11];						//保存水平方向的11根线条中，每根线条中的10段是否被走过
bool		ap;									// A键是否已经按下
bool		filled;								// 网格是否被填满?
bool		gameover;							// 游戏是否结束?
bool		anti=TRUE;							// 是否启用反走样?
bool		active=TRUE;							
bool		fullscreen=TRUE;			

int		loop1;									// 通用循环变量
int		loop2;									// 通用循环变量
int		delay;									// 敌人的暂停时间
int		adjust=3;								// 调整显示的速度
int		lives=5;								// 玩家的生命
int		level=1;								// 内部游戏的等级
int		level2=level;							// 显示的游戏的等级
int		stage=1;								// 游戏的关卡

struct	object									// 记录游戏中的对象
{
	int	fx, fy;									// 使移动变得平滑
	int	x, y;									// 当前游戏者的位置
	float	spin;								// 旋转方向
};

struct	object	player;							// 玩家信息
struct	object	enemy[9];						// 最多9个敌人的信息
struct	object	hourglass;						// 宝物信息

struct			 								// 保存时间信息的结构
{
  __int64       frequency;						// 频率
  float         resolution;						// 时间间隔
  unsigned long mm_timer_start;					// 多媒体计时器的开始时间
  unsigned long mm_timer_elapsed;				// 多媒体计时器的开始时间
  bool		performance_timer;					// 使用Performance Timer?
  __int64       performance_timer_start;		// Performance Timer计时器的开始时间
  __int64       performance_timer_elapsed;		// Performance Timer计时器的开始时间
} timer;		

int		steps[6]={ 1, 2, 4, 5, 10, 20 };		// 用来调整显示的速度
GLuint		texture[2];							// 字符纹理
GLuint		base;								// 字符显示列表的开始值

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);		

void TimerInit(void)									// 初始化我们的计时器
{
	memset(&timer, 0, sizeof(timer));					// 清空计时器结构

	// 检测Performance Counter是否可用，可用则创建
	if (!QueryPerformanceFrequency((LARGE_INTEGER *) &timer.frequency))
	{
		// 如果不可用
		timer.performance_timer	= FALSE;				// 设置Performance Timer为false
		timer.mm_timer_start	= timeGetTime();		// 使用普通的计时器
		timer.resolution	= 1.0f/1000.0f;				// 设置单位为毫秒
		timer.frequency		= 1000;						// 设置频率为1000
		timer.mm_timer_elapsed	= timer.mm_timer_start;	// 设置流失的时间为当前的时间
	}
	else
	{
		// 使用Performance Counter计时器
		QueryPerformanceCounter((LARGE_INTEGER *) &timer.performance_timer_start);
		timer.performance_timer		= TRUE;				// 设置Performance Timer为TRUE
		// 计算计时的精确度
		timer.resolution		= (float) (((double)1.0f)/((double)timer.frequency));
		// 设置流失的时间为当前的时间
		timer.performance_timer_elapsed	= timer.performance_timer_start;
	}
}

float TimerGetTime()									// 返回经过的时间，以毫秒为单位
{
	__int64 time;										// 使用64位的整数

	if (timer.performance_timer)						// 是否使用Performance Timer计时器?
	{
		QueryPerformanceCounter((LARGE_INTEGER *) &time);		// 返回当前的时间
		// 返回时间差
		return ( (float) ( time - timer.performance_timer_start) * timer.resolution)*1000.0f;
	}
	else
	{
		// 使用普通的计时器，返回时间差
		return( (float) ( timeGetTime() - timer.mm_timer_start) * timer.resolution)*1000.0f;
	}
}

void ResetObjects(void)								// 重置玩家和敌人
{
	player.x=0;										// 把玩家置于左上角
	player.y=0;								
	player.fx=0;								
	player.fy=0;	
	for (loop1=0; loop1<(stage*level); loop1++)		// 循环随即放置所有的敌人
	{
		enemy[loop1].x=5+rand()%6;					
		enemy[loop1].y=rand()%11;					
		enemy[loop1].fx=enemy[loop1].x*60;				
		enemy[loop1].fy=enemy[loop1].y*40;				
	}
}

AUX_RGBImageRec *LoadBMP(CHAR *Filename)				// 载入位图图象
{
	FILE *File=NULL;									// 文件句柄
	if (!Filename)										// 确保文件名已提供
	{
		return NULL;									// 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");							// 尝试打开文件
	if (File)											// 文件存在么?
	{
		fclose(File);									// 关闭句柄
		return auxDIBImageLoadA(Filename);				// 载入位图并返回指针
	}
	return NULL;										// 如果载入失败，返回 NULL
}

int LoadGLTextures()								
{
	int Status=FALSE;							
	AUX_RGBImageRec *TextureImage[2];					
	memset(TextureImage,0,sizeof(void *)*2);				
	if 	((TextureImage[0]=LoadBMP("Data/Font.bmp")) &&			// 载入字体纹理
		 (TextureImage[1]=LoadBMP("Data/Image.bmp")))			// 载入图像纹理
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
	glBindTexture(GL_TEXTURE_2D, texture[0]);				// 选择字符纹理
	glColor3f(1.0f,0.5f,1.0f);						
	glPrint(207,24,0,"GRID CRAZY");							// 绘制游戏名称"GRID CRAZY"
	glColor3f(1.0f,1.0f,0.0f);						
	glPrint(20,20,1,"Level:%2i",level2);					// 绘制当前的级别
	glPrint(20,40,1,"Stage:%2i",stage);						// 绘制当前级别的关卡
	if (gameover)											// 游戏是否结束?
	{
		glColor3ub(rand()%255,rand()%255,rand()%255);		// 随机选择一种颜色
		glPrint(472,20,1,"GAME OVER");						// 绘制 GAME OVER 字符串到屏幕
		glPrint(456,40,1,"PRESS SPACE");					// 提示玩家按空格键重新开始
	}
	for (loop1=0; loop1<lives-1; loop1++)					//循环绘制玩家的剩余生命
	{
		glLoadIdentity();						
		glTranslatef(490+(loop1*40.0f),40.0f,0.0f);			// 移动到屏幕右上角
		glRotatef(-player.spin,0.0f,0.0f,1.0f);				// 旋转绘制的生命图标
		glColor3f(0.0f,1.0f,0.0f);							// 绘制玩家生命
		glBegin(GL_LINES);									// 绘制玩家图标
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
	filled=TRUE;											// 在测试前，把填充变量设置为TRUE
	glLineWidth(2.0f);										// 设置线宽为2.0f
	glDisable(GL_LINE_SMOOTH);								// 禁用反走样
	glLoadIdentity();							
	for (loop1=0; loop1<11; loop1++)						// 循环11根线
	{
		for (loop2=0; loop2<11; loop2++)					// 循环每根线的线段
		{
			glColor3f(0.0f,0.5f,1.0f);						// 设置线为蓝色
			if (hline[loop1][loop2])						// 是否走过？
			{
				glColor3f(1.0f,1.0f,1.0f);					// 是，设线为白色
			}
			if (loop1<10)									// 绘制水平线
			{
				if (!hline[loop1][loop2])					// 如果当前线段没有走过，则不填充
				{
					filled=FALSE;				
				}
				glBegin(GL_LINES);							// 绘制当前的线段
					glVertex2d(20+(loop1*60),70+(loop2*40));	
					glVertex2d(80+(loop1*60),70+(loop2*40));	
				glEnd();					
			}
			glColor3f(0.0f,0.5f,1.0f);						// 设置线为蓝色
			if (vline[loop1][loop2])						// 是否走过
			{
				glColor3f(1.0f,1.0f,1.0f);					// 是，设线为白色
			}
			if (loop2<10)									// 绘制垂直线
			{
				if (!vline[loop1][loop2])					// 如果当前线段没有走过，则不填充
				{
					filled=FALSE;				
				}
				glBegin(GL_LINES);							// 绘制当前的线段
					glVertex2d(20+(loop1*60),70+(loop2*40));	
					glVertex2d(20+(loop1*60),110+(loop2*40));	
				glEnd();					
			}
			glEnable(GL_TEXTURE_2D);						// 使用纹理映射
			glColor3f(1.0f,1.0f,1.0f);						// 设置为白色
			glBindTexture(GL_TEXTURE_2D, texture[1]);		// 绑定纹理
			if ((loop1<10) && (loop2<10))					// 绘制走过的四边形
			{
				// 这个四边形是否被走过?
				if (hline[loop1][loop2] && hline[loop1][loop2+1] && vline[loop1][loop2] && vline[loop1+1][loop2])
				{
					glBegin(GL_QUADS);			// 是，则绘制它
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
	if (anti)												// 是否启用反走样?
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
	glTranslatef(player.fx+20.0f,player.fy+70.0f,0.0f);		// 设置玩家的位置
	glRotatef(player.spin,0.0f,0.0f,1.0f);					// 旋转动画
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
	if (fullscreen)										// 我们处于全屏模式吗?
	{
		ChangeDisplaySettings(NULL,0);					// 是的话，切换回桌面
		ShowCursor(TRUE);								// 显示鼠标指针
	}

	if (hRC)											//我们拥有OpenGL描述表吗?
	{
		if (!wglMakeCurrent(NULL,NULL))					// 我们能否释放DC和RC描述表?
		{
			MessageBox(NULL,"释放DC或RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}

		if (!wglDeleteContext(hRC))						// 我们能否删除RC?
		{
			MessageBox(NULL,"释放RC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		}
		hRC=NULL;										// 将RC设为 NULL
	}

	if (hDC && !ReleaseDC(hWnd,hDC))					// 我们能否释放 DC?
	{
		MessageBox(NULL,"释放DC失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hDC=NULL;										// 将 DC 设为 NULL
	}

	if (hWnd && !DestroyWindow(hWnd))					// 能否销毁窗口?
	{
		MessageBox(NULL,"释放窗口句柄失败。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hWnd=NULL;										// 将 hWnd 设为 NULL
	}

	if (!UnregisterClass("OpenG",hInstance))			// 能否注销类?
	{
		MessageBox(NULL,"不能注销窗口类。","关闭错误",MB_OK | MB_ICONINFORMATION);
		hInstance=NULL;									// 将 hInstance 设为 NULL
	}

	KillFont();								// 删除创建的字体
}

/*	这个函数创建我们OpenGL窗口，参数为：									*
 *	title			- 窗口标题												*
 *	width			- 窗口宽度												*
 *	height			- 窗口高度												*
 *	bits			- 颜色的位深（8/16/32）									*
 *	fullscreenflag	- 是否使用全屏模式，全屏模式(TRUE)，窗口模式(FALSE)		*/
 
BOOL CreateGLWindow(char* title, int width, int height, int bits, bool fullscreenflag)
{
	GLuint		PixelFormat;			// 保存查找匹配的结果
	WNDCLASS	wc;						// 窗口类结构
	DWORD		dwExStyle;				// 扩展窗口风格
	DWORD		dwStyle;				// 窗口风格
	RECT		WindowRect;				// 取得矩形的左上角和右下角的坐标值
	WindowRect.left=(long)0;			// 将Left   设为 0
	WindowRect.right=(long)width;		// 将Right  设为要求的宽度
	WindowRect.top=(long)0;				// 将Top    设为 0
	WindowRect.bottom=(long)height;		// 将Bottom 设为要求的高度

	fullscreen=fullscreenflag;			// 设置全局全屏标志

	hInstance			= GetModuleHandle(NULL);				// 取得我们窗口的实例
	wc.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 移动时重画，并为窗口取得DC
	wc.lpfnWndProc		= (WNDPROC) WndProc;					// WndProc处理消息
	wc.cbClsExtra		= 0;									// 无额外窗口数据
	wc.cbWndExtra		= 0;									// 无额外窗口数据
	wc.hInstance		= hInstance;							// 设置实例
	wc.hIcon			= LoadIcon(NULL, IDI_WINLOGO);			// 装入缺省图标
	wc.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 装入鼠标指针
	wc.hbrBackground	= NULL;									// GL不需要背景
	wc.lpszMenuName		= NULL;									// 不需要菜单
	wc.lpszClassName	= "OpenG";								// 设定类名字

	if (!RegisterClass(&wc))									// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;											// 退出并返回FALSE
	}
	
	if (fullscreen)												// 要尝试全屏模式吗?
	{
		DEVMODE dmScreenSettings;								// 设备模式
		memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	// 确保内存清空为零
		dmScreenSettings.dmSize=sizeof(dmScreenSettings);		// Devmode 结构的大小
		dmScreenSettings.dmPelsWidth	= width;				// 所选屏幕宽度
		dmScreenSettings.dmPelsHeight	= height;				// 所选屏幕高度
		dmScreenSettings.dmBitsPerPel	= bits;					// 每象素所选的色彩深度
		dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;

		// 尝试设置显示模式并返回结果。注: CDS_FULLSCREEN 移去了状态条
		if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
		{
			// 若模式失败，提供两个选项：退出或在窗口内运行。
			if (MessageBox(NULL,"全屏模式在当前显卡上设置失败！\n使用窗口模式？","NeHe G",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
			{
				//如果用户选择窗口模式，变量fullscreen 的值变为FALSE,程序继续运行
				fullscreen=FALSE;		// 选择窗口模式(Fullscreen=FALSE)
			}
			else
			{
				//如果用户选择退出，弹出消息窗口告知用户程序将结束。并返回FALSE告诉程序窗口未能成功创建。程序退出。
				MessageBox(NULL,"程序将被关闭","错误",MB_OK|MB_ICONSTOP);
				return FALSE;									// 退出并返回 FALSE
			}
		}
	}

	if (fullscreen)												// 仍处于全屏模式吗?
	{
		dwExStyle=WS_EX_APPWINDOW;								// 扩展窗体风格
		dwStyle=WS_POPUP;										// 窗体风格
		ShowCursor(FALSE);										// 隐藏鼠标指针
	}
	else
	{
		dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;			// 扩展窗体风格
		dwStyle=WS_OVERLAPPEDWINDOW;							// 窗体风格
	}

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// 调整窗口达到真正要求的大小

	// 创建窗口
	if (!(hWnd=CreateWindowEx(	dwExStyle,							// 扩展窗体风格
								"OpenG",							// 类名字
								title,								// 窗口标题
								dwStyle |							// 必须的窗体风格属性
								WS_CLIPSIBLINGS |					// 必须的窗体风格属性
								WS_CLIPCHILDREN,					// 必须的窗体风格属性
								0, 0,								// 窗口位置
								WindowRect.right-WindowRect.left,	// 计算调整好的窗口宽度
								WindowRect.bottom-WindowRect.top,	// 计算调整好的窗口高度
								NULL,								// 无父窗口
								NULL,								// 无菜单
								hInstance,							// 实例
								NULL)))								// 不向WM_CREATE传递任何东东
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"窗口创建错误","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	static	PIXELFORMATDESCRIPTOR pfd=				//pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{
		sizeof(PIXELFORMATDESCRIPTOR),				// 上述格式描述符的大小
		1,											// 版本号
		PFD_DRAW_TO_WINDOW |						// 格式支持窗口
		PFD_SUPPORT_OPENGL |						// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,							// 必须支持双缓冲
		PFD_TYPE_RGBA,								// 申请 RGBA 格式
		bits,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,							// 忽略的色彩位
		0,											// 无Alpha缓存
		0,											// 忽略Shift Bit
		0,											// 无累加缓存
		0, 0, 0, 0,									// 忽略聚集位
		16,											// 16位 Z-缓存 (深度缓存) 
		0,											// 无蒙板缓存
		0,											// 无辅助缓存
		PFD_MAIN_PLANE,								// 主绘图层
		0,											// 不使用重叠层
		0, 0, 0										// 忽略层遮罩
	};
	
	if (!(hDC=GetDC(hWnd)))							// 取得设备描述表了么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一个窗口设备描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))	// Windows 找到相应的象素格式了吗?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建一种相匹配的像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if(!SetPixelFormat(hDC,PixelFormat,&pfd))		// 能够设置象素格式么?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能设置像素格式","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if (!(hRC=wglCreateContext(hDC)))				// 能否取得OpenGL渲染描述表?
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能创建OpenGL渲染描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	if(!wglMakeCurrent(hDC,hRC))					// 尝试激活着色描述表
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"不能激活当前的OpenGL渲然描述表","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	ShowWindow(hWnd,SW_SHOW);						// 显示窗口
	SetForegroundWindow(hWnd);						// 略略提高优先级
	SetFocus(hWnd);									// 设置键盘的焦点至此窗口
	ReSizeGLScene(width, height);					// 设置透视 GL 屏幕

	if (!InitGL())									// 初始化新建的GL窗口
	{
		KillGLWindow();								// 重置显示区
		MessageBox(NULL,"初始化失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;								// 返回 FALSE
	}

	return TRUE;									// 成功
}

LRESULT CALLBACK WndProc(	HWND	hWnd,			// 窗口的句柄	
							UINT	uMsg,			// 窗口的消息
							WPARAM	wParam,			// 附加的消息内容
							LPARAM	lParam)			// 附加的消息内容
{
	switch (uMsg)									// 检查Windows消息
	{
		case WM_ACTIVATE:							// 监视窗口激活消息
		{
			if (!HIWORD(wParam))					// 检查最小化状态
			{
				active=TRUE;						// 程序处于激活状态
			}
			else
			{
				active=FALSE;						// 程序不再激活
			}

			return 0;								// 返回消息循环
		}

		case WM_SYSCOMMAND:							// 系统中断命令
		{
			switch (wParam)							// 检查系统调用
			{
				case SC_SCREENSAVE:					// 屏保要运行?
				case SC_MONITORPOWER:				// 显示器要进入节电模式?
				return 0;							// 阻止发生
			}
			break;									// 退出
		}

		case WM_CLOSE:								// 收到Close消息?
		{
			PostQuitMessage(0);						// 发出退出消息
			return 0;								// 返回
		}

		case WM_KEYDOWN:							// 有键按下么?
		{
			keys[wParam] = TRUE;					// 如果是，设为TRUE
			return 0;								// 返回
		}

		case WM_KEYUP:								// 有键放开么?
		{
			keys[wParam] = FALSE;					// 如果是，设为FALSE
			return 0;								// 返回
		}

		case WM_SIZE:								// 调整OpenGL窗口大小
		{
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  // LoWord=Width,HiWord=Height
			return 0;								// 返回
		}
	}

	// 向 DefWindowProc传递所有未处理的消息。
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

	ResetObjects();								// 重置玩家和敌人
	TimerInit();								// 初始化时间计时器

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
			float start=TimerGetTime();				// 返回当前的时间

			if ((active && !DrawGLScene()) || keys[VK_ESCAPE])	
			{
				done=TRUE;					
			}
			else							
			{
				SwapBuffers(hDC);				
			}

			while(TimerGetTime()<start+float(steps[adjust]*2.0f)) {}// 速度快的机器上让其空循环
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
			if (keys['A'] && !ap)					// 如果'A' 键被按下，启用反走样
			{
				ap=TRUE;					
				anti=!anti;					
			}
			if (!keys['A'])						
			{
				ap=FALSE;					
			}
			if (!gameover && active)				// 如果游戏没有结束，则进行游戏循环
			{
				for (loop1=0; loop1<(stage*level); loop1++)	// 循环不同的难度等级
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
					if (delay>(3-level) && (hourglass.fx!=2))		// 如果没有吃到时间停止器
					{
						delay=0;					// 重置时间停止器
						for (loop2=0; loop2<(stage*level); loop2++)	// 循环设置每个敌人的位置
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
					// 敌人的位置和玩家的位置相遇?
					if ((enemy[loop1].fx==player.fx) && (enemy[loop1].fy==player.fy))
					{
						lives--;				// 如果是，生命值减1

						if (lives==0)			// 如果生命值为0，则游戏结束
						{
							gameover=TRUE;		
						}

						ResetObjects();			// 重置所有的游戏变量
						PlaySound("Data/Die.wav", NULL, SND_SYNC);	// 播放死亡的音乐
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
			else								// 如果游戏结束
			{
				if (keys[' '])					// 按下空格？
				{
					gameover=FALSE;				// 开始新的一局
					filled=TRUE;				// 重置所有的变量
					level=1;				
					level2=1;			
					stage=0;				
					lives=5;				
				}
			}
			if (filled)							// 所有网格是否填满
			{
				PlaySound("Data/Complete.wav", NULL, SND_SYNC);	// 播放过关音乐
				stage++;						// 增加游戏难度
				if (stage>3)					// 如果当前的关卡大于3，则进入到下一个大的关卡?
				{
					stage=1;					// 重置当前的关卡
					level++;					// 增加大关卡的值
					level2++;				
					if (level>3)				
					{
						level=3;				// 如果大关卡大于3，则不再增加
						lives++;				// 完成一局给玩家奖励一条生命
						if (lives>5)			// 如果玩家有5条生命，则不再增加
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
				// 播放一段声音
				PlaySound("Data/freeze.wav", NULL, SND_ASYNC | SND_LOOP);
				hourglass.fx=2;					// 设置fx为2，表示吃到时间停止器
				hourglass.fy=0;					// 设置fy为0
			}
			player.spin+=0.5f*steps[adjust];	// 旋转动画
			if (player.spin>360.0f)					
			{
				player.spin-=360;				
			}
			hourglass.spin-=0.25f*steps[adjust];// 旋转动画
			if (hourglass.spin<0.0f)				
			{
				hourglass.spin+=360.0f;				
			}
			hourglass.fy+=steps[adjust];		// 增加fy的值，当他大于一定的时候，产生时间停止计数器
			if ((hourglass.fx==0) && (hourglass.fy>6000/level))	
			{							
				PlaySound("Data/hourglass.wav", NULL, SND_ASYNC);	
				hourglass.x=rand()%10+1;			
				hourglass.y=rand()%11;				
				hourglass.fx=1;					//fx=1表示时间停止器出现					
				hourglass.fy=0;					
			}
			if ((hourglass.fx==1) && (hourglass.fy>6000/level))	
			{				
				hourglass.fx=0;					// 消失后重置时间停止器
				hourglass.fy=0;					
			}
			if ((hourglass.fx==2) && (hourglass.fy>500+(500*level)))
			{							
				PlaySound(NULL, NULL, 0);		// 停止播放音乐
				hourglass.fx=0;					// 重置变量
				hourglass.fy=0;					
			}
			delay++;							// 增加敌人的延迟计数器的值
		}
	}

	// 关闭
	KillGLWindow();								// 删除窗口
	return (msg.wParam);						// 退出程序
}
/*********************************************************************************************************************************/