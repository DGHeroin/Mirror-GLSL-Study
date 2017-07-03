/*
 *		这些代码是由Jeff Molofee 与2000年写的，再次我必须感谢Fredric Echols，因为他使这些代码看起来更简洁和高效，并使他易于使用。
 *		如果你发现这些代码对你有所帮助，请访问我的站点 http://nehe.gamedev.net/default.asp ，并让我知道。
 *********************************************************************************************************************************
 *		基于texture.cpp文件修改
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
#include  <GL/glew.h>			// 包含最新的gl.h,glu.h库
//#include <glut.h>			// 包含OpenGL实用库
#include <stdio.h>			// 标准输入/输出库的头文件
#include <math.h>			// 数学库函数
#include  <GL/glaux.h>			// GLaux库的头文件
/***********************************新添的代码***********************************************************************************/
static GLfloat	LightAmb[] = {0.7f, 0.7f, 0.7f, 1.0f};				// 环境光
static GLfloat	LightDif[] = {1.0f, 1.0f, 1.0f, 1.0f};				// 漫射光
static GLfloat	LightPos[] = {4.0f, 4.0f, 6.0f, 1.0f};				// 灯光的位置
GLUquadricObj	*q;													// 使用二次几何体创建球

GLfloat		xrot		=  0.0f;									// X方向的旋转角度
GLfloat		yrot		=  0.0f;									// Y方向的旋转角的
GLfloat		xrotspeed	=  0.0f;									// X方向的旋转速度
GLfloat		yrotspeed	=  0.0f;									// Y方向的旋转速度
GLfloat		zoom		= -7.0f;									// 移入屏幕7个单位
GLfloat		height		=  2.0f;									// 球离开地板的高度

GLuint		texture[3];												// 使用三个纹理
/********************************************************************************************************************************/
//GLfloat		xrot;			// X 旋转量
//GLfloat		yrot;			// Y 旋转量
GLfloat		zrot;			// Z 旋转量

//GLuint		texture[1];		// 存储一个纹理
HDC			hDC=NULL;		// 窗口着色描述表句柄
HGLRC		hRC=NULL;		// OpenGL渲染描述表句柄
HWND		hWnd=NULL;		// 保存我们的窗口句柄
HINSTANCE	hInstance;		// 保存程序的实例

bool	keys[256];			// 保存键盘按键的数组
bool	active=TRUE;		// 窗口的活动标志，缺省为TRUE
bool	fullscreen=TRUE;	// 全屏标志缺省，缺省设定成全屏模式
GLfloat		rtri;			// 用于三角形的角度
GLfloat		rquad;			// 用于四边形的角度

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);	// WndProc的定义
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

//int LoadGLTextures()									// 载入位图(调用上面的代码)并转换成纹理
//{
//	int Status=FALSE;									// 状态指示器
//	AUX_RGBImageRec *TextureImage[1];					// 创建纹理的存储空间
//	memset(TextureImage,0,sizeof(void *)*1);			// 将指针设为 NULL
//	// 载入位图，检查有无错误，如果位图没找到则退出
//	if (TextureImage[0]=LoadBMP("Data/NeHe.bmp"))
//	{
//		Status=TRUE;									// 将 Status 设为 TRUE
//		glGenTextures(1, &texture[0]);					// 创建纹理
//
//		// 使用来自位图数据生成 的典型纹理
//		glBindTexture(GL_TEXTURE_2D, texture[0]);
//		// 生成纹理
//		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
//
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
//	}
//
//	if (TextureImage[0])								// 纹理是否存在
//	{
//		if (TextureImage[0]->data)						// 纹理图像是否存在
//		{
//			free(TextureImage[0]->data);				// 释放纹理图像占用的内存
//		}
//
//		free(TextureImage[0]);							// 释放图像结构
//	}
//	return Status;										// 返回 Status
//}

/***********************************新添的代码***********************************************************************************/
int LoadGLTextures()													// 载入*.bmp文件，并转化为纹理
{
    int Status=FALSE;								
    AUX_RGBImageRec *TextureImage[3];									// 创建三个图象
    memset(TextureImage,0,sizeof(void *)*3);					
    if ((TextureImage[0]=LoadBMP("Data/EnvWall.bmp")) &&				// 载入地板图像
        (TextureImage[1]=LoadBMP("Data/Ball.bmp")) &&					// 载入球图像
        (TextureImage[2]=LoadBMP("Data/EnvRoll.bmp")))					// 载入强的图像
	{
		Status=TRUE;							
		glGenTextures(3, &texture[0]);									// 创建纹理
		for (int loop=0; loop<3; loop++)								// 循环设置三个纹理参数
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
	return Status;														// 成功返回
}
/********************************************************************************************************************************/

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
//	if (!LoadGLTextures())								// 调用纹理载入子例程
//	{
//		return FALSE;									// 如果未能载入，返回FALSE
//	}
//
//	glEnable(GL_TEXTURE_2D);							// 启用纹理映射
//	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
//	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
//	glClearDepth(1.0f);									// 设置深度缓存
//	glEnable(GL_DEPTH_TEST);							// 启用深度测试
//	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
//	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
//	return TRUE;										// 初始化 OK
//}

/***********************************新添的代码***********************************************************************************/
int InitGL(GLvoid)											// 初始化OpenGL
{	
	if (!LoadGLTextures())									// 载入纹理
	{
		return FALSE;							
	}
	glShadeModel(GL_SMOOTH);								
	glClearColor(0.2f, 0.5f, 1.0f, 1.0f);							
	glClearDepth(1.0f);							
	glClearStencil(0);										// 设置蒙板值
	glEnable(GL_DEPTH_TEST);						
	glDepthFunc(GL_LEQUAL);						
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	glEnable(GL_TEXTURE_2D);								// 使用2D纹理
	glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);					
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDif);						
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);					
	glEnable(GL_LIGHT0);	
	glEnable(GL_LIGHTING);
	q = gluNewQuadric();									// 创建一个二次几何体
	gluQuadricNormals(q, GL_SMOOTH);						// 使用平滑法线
	gluQuadricTexture(q, GL_TRUE);							// 使用纹理

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);	// 设置球纹理映射
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);			

	return TRUE;											// 初始化完成，成功返回
}

void DrawObject()								// 绘制我们的球
{
#if 1
	glBindTexture(GL_TEXTURE_2D, texture[1]);	// 设置为球的纹理
	gluSphere(q, 0.35f, 32, 16);	
#endif			// 绘制球
#if 1
	glBindTexture(GL_TEXTURE_2D, texture[2]);	// 设置为环境纹理
	glColor4f(1.0f, 1.0f, 1.0f, 0.4f);			// 使用alpha为40%的白色
	// GL_RGB
	glEnable(GL_BLEND);							// 启用混合
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);			// 把原颜色的40%与目标颜色混合
	glEnable(GL_TEXTURE_GEN_S);					// 使用球映射
	glEnable(GL_TEXTURE_GEN_T);						

	gluSphere(q, 0.35f, 32, 16);				// 绘制球体，并混合
#endif
	glColor3f(1.0f, 1.0f, 1.0f);				// 设置为白色

	glDisable(GL_TEXTURE_GEN_S);				// 让OpenGL回到默认的属性
	glDisable(GL_TEXTURE_GEN_T);						
	glDisable(GL_BLEND);		

}

void DrawFloor()			
{
	glBindTexture(GL_TEXTURE_2D, texture[0]);	// 选择地板纹理，地板由一个长方形组成
	glBegin(GL_QUADS);							
		glNormal3f(0.0, 1.0, 0.0);				
		glTexCoord2f(0.0f, 1.0f);				// 左下
		glVertex3f(-2.0, 0.0, 2.0);				

		glTexCoord2f(0.0f, 0.0f);				// 左上
		glVertex3f(-2.0, 0.0,-2.0);				

		glTexCoord2f(1.0f, 0.0f);				// 右上
		glVertex3f( 2.0, 0.0,-2.0);				

		glTexCoord2f(1.0f, 1.0f);				// 右下
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

int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
{

	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
	//glLoadIdentity();									// 重置当前的模型观察矩阵
/***********************************新添的代码***********************************************************************************/
	// 清除缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// 设置剪切平面
	double eqr[] = {0.0f,-1.0f, 0.0f, 0.0f};	
	glLoadIdentity();							
	glTranslatef(0.0f, -1.6f, zoom);					// 平移和缩放地面
	glColorMask(0,0,0,0);
	glEnable(GL_STENCIL_TEST);							// 启用蒙板缓存
	glStencilFunc(GL_ALWAYS, 1, 1);						// 设置蒙板测试总是通过，参考值设为1，掩码值也设为1
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			// 设置当深度测试不通过时，保留蒙板中的值不变。如果通过则使用参考值替换蒙板值
	glDisable(GL_DEPTH_TEST);							// 禁用深度测试
	DrawFloor();										// 绘制地面
	glEnable(GL_DEPTH_TEST);							//启用深度测试
	glColorMask(1,1,1,1);								// 可以绘制颜色
	glStencilFunc(GL_EQUAL, 1, 1);						//下面的设置指定当我们绘制时，不改变蒙板缓存区的值
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);		
	glEnable(GL_CLIP_PLANE0);							// 使用剪切平面
	glClipPlane(GL_CLIP_PLANE0, eqr);					// 设置剪切平面为地面，并设置它的法线为向下
	glPushMatrix();										// 保存当前的矩阵
		glScalef(1.0f, -1.0f, 1.0f);					// 沿Y轴反转
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos);	// 设置灯光0
		glTranslatef(0.0f, height, 0.0f);				
		glRotatef(xrot, 1.0f, 0.0f, 0.0f);				
		glRotatef(yrot, 0.0f, 1.0f, 0.0f);				
		DrawObject();									// 绘制反射的球
	glPopMatrix();										// 弹出保存的矩阵
	glDisable(GL_CLIP_PLANE0);							// 禁用剪切平面
	glDisable(GL_STENCIL_TEST);							// 关闭蒙板
	glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
	glEnable(GL_BLEND);									// 启用混合
	glDisable(GL_LIGHTING);								// 关闭光照
	glColor4f(1.0f, 1.0f, 1.0f, 0.8f);					// 设置颜色为白色
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	// 设置混合系数
	DrawFloor();										// 绘制地面
	glEnable(GL_LIGHTING);								// 使用光照
	glDisable(GL_BLEND);								// 禁用混合
	glTranslatef(0.0f, height, 0.0f);					// 移动高位height的位置
	glRotatef(xrot, 1.0f, 0.0f, 0.0f);					// 设置球旋转的角度
	glRotatef(yrot, 0.0f, 1.0f, 0.0f);					
	DrawObject();										// 绘制球
	xrot += xrotspeed;									// 更新X轴旋转速度
	yrot += yrotspeed;									// 更新Y轴旋转速度
	glFlush();											// 强制OpenGL执行所有命令
/********************************************************************************************************************************/	
	return TRUE;										// 一切 OK
}

GLvoid KillGLWindow(GLvoid)								// 正常销毁窗口
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
		//0,											// 无蒙板缓存
/***********************************新添的代码***********************************************************************************/
		1,											// 启用蒙板缓存
/********************************************************************************************************************************/
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
			ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));  
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
//	if (!CreateGLWindow("NeHe's 纹理实例",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
	if (!CreateGLWindow("NeHe's 显示列表",640,480,16,fullscreen))
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
					ProcessKeyboard();			// 处理按键相应
/********************************************************************************************************************************/
				}
			}

			if (keys[VK_F1])						// F1键按下了么?
			{
				keys[VK_F1]=FALSE;					// 若是，使对应的Key数组中的值为 FALSE
				KillGLWindow();						// 销毁当前的窗口
				fullscreen=!fullscreen;				// 切换 全屏 / 窗口 模式
				// 重建 OpenGL 窗口
//				if (!CreateGLWindow("NeHe's 纹理实例",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
				if (!CreateGLWindow("NeHe's 显示列表",640,480,16,fullscreen))
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
