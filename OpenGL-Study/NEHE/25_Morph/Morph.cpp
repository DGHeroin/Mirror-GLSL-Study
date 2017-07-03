/*
 *		这些代码是由Jeff Molofee 与2000年写的，再次我必须感谢Fredric Echols，因为他使这些代码看起来更简洁和高效，并使他易于使用。
 *		如果你发现这些代码对你有所帮助，请访问我的站点 http://nehe.gamedev.net/default.asp ，并让我知道。
 *********************************************************************************************************************************
 *		基于base.cpp文件修改
 *		这份文档是由DancingWind翻译的，如果有什么错误请e-mail:zhouwei02@mails.tsinghua.edu.cn
 *		//old code			//不用了代码用//删除
 *		/********************************************************************************************************************/	/*
 *		new code			//新的代码位于两排/*************/ /*之间，便于查找
 *		/********************************************************************************************************************/	/*
/***********************************新添的代码***********************************************************************************/
/********************************************************************************************************************************/

/*********************包含链接的库文件*******************************************************************************************/
#pragma comment( lib, "opengl32.lib" )				
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glew32.lib")
#pragma comment( lib, "glaux.lib")
#pragma comment( lib, "vfw32.lib" )	
/********************************************************************************************************************************/

#include <windows.h>		// Windows的头文件
#include <GL/glew.h>			// 包含最新的gl.h,glu.h库
//#include <glut.h>			// 包含OpenGL实用库
/***********************************新添的代码***********************************************************************************/
#include <math.h>							// 数学库
#include <stdio.h>							// 标准输入输出库
#include <stdlib.h>							// 标准库
GLfloat		xrot,yrot,zrot,								// X, Y & Z 轴的旋转角度
		xspeed,yspeed,zspeed,							// X, Y & Z 轴的旋转速度
		cx,cy,cz=-15;									// 物体的位置

int		key=1;											// 物体的标识符
int		step=0,steps=5000;								// 变换的步数
bool		morph=FALSE;								// 是否使用变形

typedef struct					
{
	float	x, y, z;							
} VERTEX;		

typedef	struct											// 物体结构
{
 int		verts;										// 物体中顶点的个数
 VERTEX		*points;									// 包含顶点数据的指针
} OBJECT;										

int		maxver;											// 最大的顶点数
OBJECT		morph1,morph2,morph3,morph4,				// 我们的四个物体
		helper,*sour,*dest;		 						// 帮助物体,原物体，目标物体

void objallocate(OBJECT *k,int n)
{											
	k->points=(VERTEX*)malloc(sizeof(VERTEX)*n);					// 分配n个顶点的内存空间
}			

void objfree(OBJECT *k)			
{
	free(k->points);								
}

void readstr(FILE *f,char *string)							// 读取一行字符
{
	do										
	{
		fgets(string, 255, f);						// 最多读取255个字符
	} while ((string[0] == '/') || (string[0] == '\n'));				// 遇到回车则停止读取
	return;									// 返回
}

void objload(char *name,OBJECT *k)							// 从文件加载一个模型
{
	int	ver;								// 保存顶点个数
	float	rx,ry,rz;								// 保存模型位置
	FILE	*filein;								// 打开的文件句柄
	char	oneline[255];							// 保存255个字符

	filein = fopen(name, "rt");							// 打开文本文件，供读取
											
	readstr(filein,oneline);							// 读入一行文本
	sscanf(oneline, "Vertices: %d\n", &ver);					// 搜索字符串"Vertices: "，并把其后的顶点数保存在ver变量中
	k->verts=ver;								// 设置模型的顶点个数
	objallocate(k,ver);							// 为模型数据分配内存
	for (int i=0;i<ver;i++)								// 循环所有的顶点
	{
		readstr(filein,oneline);							// 读取一行数据
		sscanf(oneline, "%f %f %f", &rx, &ry, &rz);					// 把顶点数据保存在rx,ry,rz中

		k->points[i].x = rx;							// 保存当前顶点的x坐标
		k->points[i].y = ry;							// 保存当前顶点的y坐标
		k->points[i].z = rz;							// 保存当前顶点的z坐标
	}
	fclose(filein);									// 关闭文件

	if(ver>maxver) maxver=ver;								// 记录最大的顶点数
}			
VERTEX calculate(int i)									// 计算第i个顶点每次变换的位移
{
	VERTEX a;								
	a.x=(sour->points[i].x-dest->points[i].x)/steps;				
	a.y=(sour->points[i].y-dest->points[i].y)/steps;				
	a.z=(sour->points[i].z-dest->points[i].z)/steps;				
	return a;									
}			
/********************************************************************************************************************************/
HDC			hDC=NULL;		// 窗口着色描述表句柄
HGLRC		hRC=NULL;		// OpenGL渲染描述表句柄
HWND		hWnd=NULL;		// 保存我们的窗口句柄
HINSTANCE	hInstance;		// 保存程序的实例

bool	keys[256];			// 保存键盘按键的数组
bool	active=TRUE;		// 窗口的活动标志，缺省为TRUE
bool	fullscreen=TRUE;	// 全屏标志缺省，缺省设定成全屏模式
LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc的定义

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// 重置OpenGL窗口大小
{
	if (height==0)										// 防止被零除
	{
		height=1;										// 将Height设为1
	}

	glViewport(0,0,width,height);						// 重置当前的视口

	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
	glLoadIdentity();									// 重置投影矩阵

	// 设置视口的大小
	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵
	glLoadIdentity();									// 重置模型观察矩阵
}

//int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
//{
//	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
//	glClearDepth(1.0f);									// 设置深度缓存
//	glEnable(GL_DEPTH_TEST);							// 启用深度测试
//	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
//	return TRUE;										// 初始化 OK
//}
/***********************************新添的代码***********************************************************************************/
int InitGL(GLvoid)			
{
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);						// 设置半透明混合模式
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// 设置清除色为黑色
	glClearDepth(1.0);										// 设置深度缓存中值为1
	glDepthFunc(GL_LESS);									// 设置深度测试函数
	glEnable(GL_DEPTH_TEST);								// 启用深度测试
	glShadeModel(GL_SMOOTH);								// 设置着色模式为光滑着色
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	maxver=0;												// 初始化最大顶点数为0
	objload("data/sphere.txt",&morph1);						// 加载球模型
	objload("data/torus.txt",&morph2);						// 加载圆环模型
	objload("data/tube.txt",&morph3);						// 加载立方体模型
	objallocate(&morph4,486);								// 为第四个模型分配内存资源
	for(int i=0;i<486;i++)									// 随机设置486个顶点
	{
		morph4.points[i].x=((float)(rand()%14000)/1000)-7;			
		morph4.points[i].y=((float)(rand()%14000)/1000)-7;			 
		morph4.points[i].z=((float)(rand()%14000)/1000)-7;			
	}
	objload("data/sphere.txt",&helper);
	sour=dest=&morph1;								

	return TRUE;											// 初始化完成，成功返回
}
/********************************************************************************************************************************/

int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
	glLoadIdentity();									// 重置当前的模型观察矩阵
/***********************************新添的代码***********************************************************************************/
	glTranslatef(cx,cy,cz);									// 平移和旋转
	glRotatef(xrot,1,0,0);										
	glRotatef(yrot,0,1,0);									
	glRotatef(zrot,0,0,1);										

	xrot+=xspeed; yrot+=yspeed; zrot+=zspeed;				// 根据旋转速度，增加旋转角度

	GLfloat tx,ty,tz;										// 顶点临时变量
	VERTEX q;												// 保存中间计算的临时顶点

	glPointSize(2);
	glEnable(GL_POINT_SMOOTH); 
	glBegin(GL_POINTS);										// 点绘制开始
		for(int i=0;i<morph1.verts;i++)						// 循环绘制模型1中的每一个顶点
		{	
			if(morph) q=calculate(i); else q.x=q.y=q.z=0;	// 如果启用变形，则计算中间模型
			helper.points[i].x-=q.x;					
			helper.points[i].y-=q.y;					
			helper.points[i].z-=q.z;					
			tx=helper.points[i].x;							// 保存计算结果到x,y,z变量中
			ty=helper.points[i].y;						
			tz=helper.points[i].z;					
   
			glColor3f(0,1,1);								// 设置颜色
			glPointSize(3);
			glVertex3f(tx,ty,tz);							// 绘制顶点
			if(morph)
			{
				glColor3f(0,0.5f,1);							// 把颜色变蓝一些
				tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;				// 如果启用变形，则绘制2步后的顶点
				glVertex3f(tx,ty,tz);						

				glColor3f(0,0,1);								// 把颜色变蓝一些
				tx-=2*q.x; ty-=2*q.y; ty-=2*q.y;				// 如果启用变形，则绘制2步后的顶点
				glVertex3f(tx,ty,tz);						
			}
		}		
		glPointSize(1);
	glEnd();		
	// 如果启用变形则把变形步数增加
	if(morph && step<=steps)step++; else { morph=FALSE; sour=dest; step=0;}

/********************************************************************************************************************************/
	return TRUE;										// 一切 OK
}

GLvoid KillGLWindow(GLvoid)								// 正常销毁窗口
{
/***********************************新添的代码***********************************************************************************/
	objfree(&morph1);								// 释放模型1内存
	objfree(&morph2);								// 释放模型2内存
	objfree(&morph3);								// 释放模型3内存
	objfree(&morph4);								// 释放模型4内存
	objfree(&helper);								// 释放模型5内存
/********************************************************************************************************************************/
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
	wc.lpszClassName	= "OpenG";							// 设定类名字

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

int WINAPI WinMain(HINSTANCE	hInstance,			// 当前窗口实例
					HINSTANCE	hPrevInstance,		// 前一个窗口实例
					LPSTR		lpCmdLine,			// 命令行参数
					int			nCmdShow)			// 窗口显示状态
{
	MSG		msg;									// Windowsx消息结构
	BOOL	done=FALSE;								// 用来退出循环的Bool 变量

	// 提示用户选择运行模式
	if (MessageBox(NULL,"你想在全屏模式下运行么？", "设置全屏模式",MB_YESNO|MB_ICONQUESTION)==IDNO)
	{
		fullscreen=FALSE;							// FALSE为窗口模式
	}

	// 创建OpenGL窗口
	//if (!CreateGLWindow("NeHe's OpenGL 程序框架",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
	if (!CreateGLWindow("NeHe's OpenGL 变形实例",640,480,16,fullscreen))
/********************************************************************************************************************************/

	{
		return 0;									// 失败退出
	}

	while(!done)									// 保持循环直到 done=TRUE
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))	// 有消息在等待吗?
		{
			if (msg.message==WM_QUIT)				// 收到退出消息?
			{
				done=TRUE;							// 是，则done=TRUE
			}
			else									// 不是，处理窗口消息
			{
				TranslateMessage(&msg);				// 翻译消息
				DispatchMessage(&msg);				// 发送消息
			}
		}
		else										// 如果没有消息
		{
			// 绘制场景。监视ESC键和来自DrawGLScene()的退出消息
			if (active)								// 程序激活的么?
			{
				if (keys[VK_ESCAPE])				// ESC 按下了么?
				{
					done=TRUE;						// ESC 发出退出信号
				}
				else								// 不是退出的时候，刷新屏幕
				{
					DrawGLScene();					// 绘制场景
					SwapBuffers(hDC);				// 交换缓存 (双缓存)
/***********************************新添的代码***********************************************************************************/
						if(keys[VK_PRIOR])			// PageUp键是否被按下
							zspeed+=0.01f;			// 按下增加绕z轴旋转的速度

						if(keys[VK_NEXT])			// PageDown键是否被按下
							zspeed-=0.01f;			// 按下减少绕z轴旋转的速度

						if(keys[VK_DOWN])			// 下方向键是否被按下
							xspeed+=0.01f;			// 按下增加绕x轴旋转的速度

						if(keys[VK_UP])				// 上方向键是否被按下
							xspeed-=0.01f;			// 按下减少绕x轴旋转的速度

						if(keys[VK_RIGHT])			// 右方向键是否被按下
							yspeed+=0.01f;			// 按下增加沿y轴旋转的速度

						if(keys[VK_LEFT])			// 左方向键是否被按下
							yspeed-=0.01f;			// 按下减少沿y轴旋转的速度
						if (keys['Q'])				// Q键是否被按下
						cz-=0.01f;					// 是则向屏幕里移动

						if (keys['Z'])				// Z键是否被按下 
						cz+=0.01f;					// 是则向屏幕外移动

						if (keys['W'])				// W键是否被按下
						cy+=0.01f;					// 是则向上移动

						if (keys['S'])				// S键是否被按下
						cy-=0.01f;					// 是则向下移动

						if (keys['D'])				// D键是否被按下 
						cx+=0.01f;					// 是则向右移动

						if (keys['A'])				// A键是否被按下 
						cx-=0.01f;					// 是则向左移动
						if (keys['1'] && (key!=1) && !morph)			// 如果1被按下，则变形到模型1
						{
							key=1;						
							morph=TRUE;				
							dest=&morph1;					
						}
						if (keys['2'] && (key!=2) && !morph)			// 如果2被按下，则变形到模型1
						{
							key=2;						
							morph=TRUE;					
							dest=&morph2;					
						}
						if (keys['3'] && (key!=3) && !morph)			// 如果3被按下，则变形到模型1
						{
							key=3;						
							morph=TRUE;					
							dest=&morph3;					
						}
						if (keys['4'] && (key!=4) && !morph)			// 如果4被按下，则变形到模型1
						{
							key=4;						
							morph=TRUE;					
							dest=&morph4;					
						}
/********************************************************************************************************************************/
				}
			}

			if (keys[VK_F1])						// F1键按下了么?
			{
				keys[VK_F1]=FALSE;					// 若是，使对应的Key数组中的值为 FALSE
				KillGLWindow();						// 销毁当前的窗口
				fullscreen=!fullscreen;				// 切换 全屏 / 窗口 模式
				// 重建 OpenGL 窗口
				//if (!CreateGLWindow("NeHe's OpenGL 程序框架",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
				if (!CreateGLWindow("NeHe's OpenGL 变形实例",640,480,16,fullscreen))
/********************************************************************************************************************************/
				{
					return 0;						// 如果窗口未能创建，程序退出
				}
			}
		}
	}

	// 关闭程序
	KillGLWindow();									// 销毁窗口
	return (msg.wParam);							// 退出程序
}
