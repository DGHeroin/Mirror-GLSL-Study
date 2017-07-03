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
#include <stdarg.h>								// 处理可变参数的函数的头文件
#include <string.h>								// 处理字符串的头文件
#include <stdlib.h>								// C++标准库
#include <stdio.h>								// 文件读写库
int		scroll;										// 用来滚动屏幕
int		maxtokens;									// 保存扩展的个数
int		swidth;										// 剪裁宽度
int		sheight;									// 剪裁高度

GLuint		base;									// 字符显示列表的开始值
typedef	struct										// 创建加载TGA图像文件结构
{
	GLubyte	*imageData;								// 图像数据指针
	GLuint	bpp;									// 每个数据所占的位数（必须为24或32）
	GLuint	width;									// 图像宽度
	GLuint	height;									// 图像高度
	GLuint	texID;									// 纹理的ID值
} TextureImage;										// 结构名称

TextureImage	textures[1];						// 保存一个纹理

bool LoadTGA(TextureImage *texture, char *filename)							// 把TGA文件加载入内存
{
	GLubyte		TGAheader[12]={0,0,2,0,0,0,0,0,0,0,0,0};					// 无压缩的TGA文件头
	GLubyte		TGAcompare[12];												// 保存读入的文件头信息
	GLubyte		header[6];													// 保存最有用的图像信息，宽，高，位深
	GLuint		bytesPerPixel;												// 记录每个颜色所占用的字节数
	GLuint		imageSize;													// 记录文件大小
	GLuint		temp;														// 临时变量
	GLuint		type=GL_RGBA;												// 设置默认的格式为GL_RGBA，即32位图像
	FILE *file = fopen(filename, "rb");										// 打开一个TGA文件

	if(	file==NULL ||														// 文件存在么?
		fread(TGAcompare,1,sizeof(TGAcompare),file)!=sizeof(TGAcompare) ||	// 是否包含12个字节的文件头?
		memcmp(TGAheader,TGAcompare,sizeof(TGAheader))!=0		||			// 是否是我们需要的格式?
		fread(header,1,sizeof(header),file)!=sizeof(header))				// 如果是读取下面六个图像信息
	{
		if (file == NULL)													// 文件不存在返回错误
			return false;							
		else
		{
			fclose(file);													// 关闭文件返回错误
			return false;							
		}
	}
	texture->width  = header[1] * 256 + header[0];							// 记录文件高度
	texture->height = header[3] * 256 + header[2];							// 记录文件宽度

 	if(	texture->width	<=0	||												// 宽度是否小于0
		texture->height	<=0	||												// 高度是否小于0
		(header[4]!=24 && header[4]!=32))									// TGA文件是24/32位？
	{
		fclose(file);														// 如果失败关闭文件，返回错误
		return false;								
	}
	texture->bpp	= header[4];											// 记录文件的位深
	bytesPerPixel	= texture->bpp/8;										// 记录每个象素所占的字节数
	imageSize	= texture->width*texture->height*bytesPerPixel;				// 计算TGA文件加载所需要的内存大小
	texture->imageData=(GLubyte *)malloc(imageSize);						// 分配内存去保存TGA数据

	if(	texture->imageData==NULL ||											// 系统是否分配了足够的内存？
		fread(texture->imageData, 1, imageSize, file)!=imageSize)			// 是否成功读入内存?
	{
		if(texture->imageData!=NULL)										// 是否有数据被加载
			free(texture->imageData);										// 如果是，则释放载入的数据

		fclose(file);														// 关闭文件
		return false;														// 返回错误
	}
	for(GLuint i=0; i<int(imageSize); i+=bytesPerPixel)						// 循环所有的像素
	{																		// 交换R和B的值
		temp=texture->imageData[i];						
		texture->imageData[i] = texture->imageData[i + 2];		
		texture->imageData[i + 2] = temp;				
	}

	fclose (file);															// 关闭文件
	// 创建纹理
	glGenTextures(1, &texture[0].texID);									// 创建纹理，并记录纹理ID

	glBindTexture(GL_TEXTURE_2D, texture[0].texID);							// 绑定纹理
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		// 设置过滤器为线性过滤
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		
	if (texture[0].bpp==24)													// 是否为24位图像？
	{
		type=GL_RGB;														// 如果是设置类型为GL_RGB
	}

	glTexImage2D(GL_TEXTURE_2D, 0, type, texture[0].width, texture[0].height, 0, type, GL_UNSIGNED_BYTE, texture[0].imageData);

	return true;															// 纹理绑定完成，成功返回
}

GLvoid BuildFont(GLvoid)									// 创建字体显示列表
{
	base=glGenLists(256);									// 创建256个显示列表
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);		// 绑定纹理
	for (int loop1=0; loop1<256; loop1++)					// 循环创建256个显示列表
	{
		float cx=float(loop1%16)/16.0f;						// 当前字符的X位置
		float cy=float(loop1/16)/16.0f;						// 当前字符的Y位置

		glNewList(base+loop1,GL_COMPILE);					// 开始创建显示列表
			glBegin(GL_QUADS);								// 创建一个四边形用来包含字符图像
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// 左下方纹理坐标
				glVertex2d(0,16);							// 左下方坐标
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// 右下方纹理坐标
				glVertex2i(16,16);							// 右下方坐标
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);	// 右上方纹理坐标
				glVertex2i(16,0);							// 右上方坐标
				glTexCoord2f(cx,1.0f-cy-0.001f);			// 左上方纹理坐标
				glVertex2i(0,0);							// 左上方坐标
			glEnd();										// 四边形创建完毕
			glTranslated(14,0,0);							// 向右移动14个单位
		glEndList();										// 结束创建显示列表
	}									
}

GLvoid KillFont(GLvoid)
{
	glDeleteLists(base,256);								// 从内存中删除256个显示列表
}

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)
{
	char	text[1024];										// 保存我们的字符
	va_list	ap;												// 指向第一个参数

	if (fmt == NULL)										// 如果要显示的字符为空则返回
		return;									

	va_start(ap, fmt);										// 开始分析参数，并把结果写入到text中
	    vsprintf(text, fmt, ap);							
	va_end(ap);									

	if (set>1)												// 如果字符集大于1则使用第二个字符集
	{
		set=1;									
	}

	glEnable(GL_TEXTURE_2D);								// 使用纹理映射
	glLoadIdentity();										// 重置视口矩阵
	glTranslated(x,y,0);									// 平移到(x,y,0)处
	glListBase(base-32+(128*set));							// 选择字符集

	glScalef(1.0f,2.0f,1.0f);								// 沿Y轴放大一倍

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// 把字符写入到屏幕
	glDisable(GL_TEXTURE_2D);								// 禁止纹理映射 
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

//GLvoid ReSizeGLScene(GLsizei width, GLsizei height)		// 重置OpenGL窗口大小
//{
//	if (height==0)										// 防止被零除
//	{
//		height=1;										// 将Height设为1
//	}
//
//	glViewport(0,0,width,height);						// 重置当前的视口
//
//	glMatrixMode(GL_PROJECTION);						// 选择投影矩阵
//	glLoadIdentity();									// 重置投影矩阵
//
//	// 设置视口的大小
//	gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);
//
//	glMatrixMode(GL_MODELVIEW);							// 选择模型观察矩阵
//	glLoadIdentity();									// 重置模型观察矩阵
//}
/***********************************新添的代码***********************************************************************************/
GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
	swidth=width;										// 设置剪切矩形为窗口大小
	sheight=height;									
	if (height==0)										// 防止高度为0时，被0除
	{
		height=1;								
	}
	glViewport(0,0,width,height);						// 设置窗口可见区
	glMatrixMode(GL_PROJECTION);							
	glLoadIdentity();								
	glOrtho(0.0f,640,480,0.0f,-1.0f,1.0f);				// 设置视口大小为640x480
	glMatrixMode(GL_MODELVIEW);							
	glLoadIdentity();						
}

/********************************************************************************************************************************/
int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
{
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
	glClearDepth(1.0f);									// 设置深度缓存
	//glEnable(GL_DEPTH_TEST);							// 启用深度测试
/***********************************新添的代码***********************************************************************************/
	if (!LoadTGA(&textures[0],"Data/Font.TGA"))			// 载入字体纹理
	{
		return false;									// 载入失败则返回
	}

	BuildFont();										// 创建字体
	glBindTexture(GL_TEXTURE_2D, textures[0].texID);	// 绑定字体纹理
/********************************************************************************************************************************/
	//glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
	return TRUE;										// 初始化 OK
}

//int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
//{
//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
//	glLoadIdentity();									// 重置当前的模型观察矩阵
//	return TRUE;										// 一切 OK
//}
/***********************************新添的代码***********************************************************************************/
int DrawGLScene(GLvoid)			
{
	char	*token;										// 保存扩展字符串
	int	cnt=0;											// 纪录扩展字符串的个数

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清楚背景和深度缓存

	glColor3f(1.0f,0.5f,0.5f);							// 设置为红色
	glPrint(50,16,1,"Renderer");							
	glPrint(80,48,1,"Vendor");						
	glPrint(66,80,1,"Version");		
	glColor3f(1.0f,0.7f,0.4f);							// 设置为橘黄色
	glPrint(200,16,1,(char *)glGetString(GL_RENDERER));	// 显示OpenGL的实现组织
	glPrint(200,48,1,(char *)glGetString(GL_VENDOR));	// 显示销售商
	glPrint(200,80,1,(char *)glGetString(GL_VERSION));	// 显示当前版本

	glColor3f(0.5f,0.5f,1.0f);							// 设置为蓝色
	glPrint(192,432,1,"NeHe Productions");				// 在屏幕的底端写上NeHe Productions字符串
	glLoadIdentity();									// 重置模型变换矩阵
	glColor3f(1.0f,1.0f,1.0f);							// 设置为白色
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
	glScissor(1	,int(0.135416f*sheight),swidth-2,int(0.597916f*sheight));	// 定义剪裁区域
	glEnable(GL_SCISSOR_TEST);												// 使用剪裁测试

	char* text=(char*)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);	// 为保存OpenGL扩展的字符串分配内存空间
	strcpy (text,(char *)glGetString(GL_EXTENSIONS));						// 返回OpenGL扩展字符串
	token=strtok(text," ");													// 按空格分割text字符串，并把分割后的字符串保存在token中
	while(token!=NULL)														// 如果token不为NULL
	{
		cnt++;																// 增加计数器
		if (cnt>maxtokens)													// 纪录最大的扩展名数量
		{
			maxtokens=cnt;							
		}
		glColor3f(0.5f,1.0f,0.5f);											// 设置颜色为绿色
		glPrint(0,96+(cnt*32)-scroll,0,"%i",cnt);							// 绘制第几个扩展名

		glColor3f(1.0f,1.0f,0.5f);											// 设置颜色为黄色
		glPrint(50,96+(cnt*32)-scroll,0,token);								// 输出第i个扩展名

		token=strtok(NULL," ");												// 查找下一个扩展名
	}
	glDisable(GL_SCISSOR_TEST);												// 禁用剪裁测试

	free (text);															// 释放分配的内存
	glFlush();																// 执行所有的渲染命令
	return TRUE;															// 成功返回
}

/********************************************************************************************************************************/
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
/***********************************新添的代码***********************************************************************************/
	KillFont();									// 删除字体
/********************************************************************************************************************************/
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
//	if (!CreateGLWindow("NeHe's OpenGL 程序框架",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
	if (!CreateGLWindow("NeHe's OpenGL TGA文件和扩展名显示",640,480,16,fullscreen))
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
					if (keys[VK_UP] && (scroll>0))						// 向上的箭头是否被按下?
					{
						scroll-=2;										// 如果是，减少scroll的值
					}
					if (keys[VK_DOWN] && (scroll<32*(maxtokens-9)))		// 向下的箭头是否被按住
					{
						scroll+=2;										// 如果是，增加scroll的值
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
//				if (!CreateGLWindow("NeHe's OpenGL 程序框架",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
				if (!CreateGLWindow("NeHe's OpenGL TGA文件和扩展名显示",640,480,16,fullscreen))
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
