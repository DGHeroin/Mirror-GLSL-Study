#include <windows.h>													// Windows头文件
#include <GL/glew.h>														// OpenGL头文件
#include "NeHeGL.h"														// NeheGL头文件

// 定义一个用户自定义消息
#define WM_TOGGLEFULLSCREEN (WM_USER+1)									
																		
static BOOL g_isProgramLooping;											// 是否进行程序循环							
static BOOL g_createFullScreen;											// 如果TRUE，则创建全屏窗口					                       

void TerminateApplication (GL_Window* window)							// 关闭程序
{
	PostMessage (window->hWnd, WM_QUIT, 0, 0);							// 发送退出命令
	g_isProgramLooping = FALSE;											// 停止程序循环
}

void ToggleFullscreen (GL_Window* window)								// 切换全屏/窗口模式
{
	PostMessage (window->hWnd, WM_TOGGLEFULLSCREEN, 0, 0);				// 发送切换命令
}

void ReshapeGL (int width, int height)									// 根据窗口大小，调整OpenGL视口大小
{
	glViewport (0, 0, (GLsizei)(width), (GLsizei)(height));				// 设置窗口可见区为窗口大小
	glMatrixMode (GL_PROJECTION);										// 设置投影矩阵
	glLoadIdentity ();													// 重置它
	gluPerspective (45.0f, (GLfloat)(width)/(GLfloat)(height),			// 设置可视冷苔体
					1.0f, 100.0f);		
	glMatrixMode (GL_MODELVIEW);										// 设置为模型变换矩阵
	glLoadIdentity ();													// 重置它
}

BOOL ChangeScreenResolution (int width, int height, int bitsPerPixel)	// 改变屏幕的大小和位深
{
	DEVMODE dmScreenSettings;											// 设备模式
	ZeroMemory (&dmScreenSettings, sizeof (DEVMODE));					// 清0
	dmScreenSettings.dmSize				= sizeof (DEVMODE);				// 设置为该结构的大小
	dmScreenSettings.dmPelsWidth		= width;						// 设置宽度
	dmScreenSettings.dmPelsHeight		= height;						// 设置高度
	dmScreenSettings.dmBitsPerPel		= bitsPerPixel;					// 设置位深
	dmScreenSettings.dmFields			= DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;			// 设置需要改变的地方
	if (ChangeDisplaySettings (&dmScreenSettings, CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL)	// 改变显示模式
	{
		return FALSE;													// 出错则返回
	}
	return TRUE;														// 成功返回
}

BOOL CreateWindowGL (GL_Window* window)									
{
	DWORD windowStyle = WS_OVERLAPPEDWINDOW;							// 设置窗口类型为一般的弹出式窗口
	DWORD windowExtendedStyle = WS_EX_APPWINDOW;						// 设置窗口的扩展属性

	PIXELFORMATDESCRIPTOR pfd =											//pfd 告诉窗口我们所希望的东东，即窗口使用的像素格式
	{																	
		sizeof (PIXELFORMATDESCRIPTOR),									// 上述格式描述符的大小
		1,																// 版本号
		PFD_DRAW_TO_WINDOW |											// 格式支持窗口
		PFD_SUPPORT_OPENGL |											// 格式必须支持OpenGL
		PFD_DOUBLEBUFFER,												// 必须支持双缓冲
		PFD_TYPE_RGBA,													// 申请 RGBA 格式
		window->init.bitsPerPixel,										// 选定色彩深度
		0, 0, 0, 0, 0, 0,												// 忽略的色彩位
		0,																// 无Alpha缓存
		0,																// 忽略Shift Bit
		0,																// 无累加缓存
		0, 0, 0, 0,														// 忽略聚集位
		16,																// 16位 Z-缓存 (深度缓存) 
		0,																// 无蒙板缓存
		0,																// 无辅助缓存
		PFD_MAIN_PLANE,													// 主绘图层
		0,																// 不使用重叠层
		0, 0, 0															// 忽略层遮罩
	};

	RECT windowRect = {0, 0, window->init.width, window->init.height};	// 记录窗口的大小

	GLuint PixelFormat;													// 将保存的像素格式

	if (window->init.isFullScreen == TRUE)								// 如果为全屏模式
	{
		if (ChangeScreenResolution (window->init.width, window->init.height, window->init.bitsPerPixel) == FALSE)
		{
			//全屏模式失败，弹出对话框，告知在窗口模式下运行
			MessageBox (HWND_DESKTOP, "模式切换失败\n在窗口模式下运行", "错误", MB_OK | MB_ICONEXCLAMATION);
			window->init.isFullScreen = FALSE;							// 设置isFullscreen为False
		}
		//在全屏模式下运行
		else															
		{
			ShowCursor(FALSE);											// 不显示光标
			windowStyle = WS_POPUP;										// 设置窗口模式为弹出式窗口
			windowExtendedStyle |= WS_EX_TOPMOST;						// 设置窗口的扩展模式为最顶层窗口
		}																
	}
	else																//如果不为全屏模式
	{
		//调整窗口大小，以容纳边框等窗口部件
		AdjustWindowRectEx (&windowRect, windowStyle, 0, windowExtendedStyle);
	}

	// 创建OpenGL窗口
	window->hWnd = CreateWindowEx (windowExtendedStyle,					// 扩展窗体风格
								   window->init.application->className,	// 类名字
								   window->init.title,					// 窗口标题
								   windowStyle,							// 必须的窗体风格属性
								   0, 0,								// 窗口位置
								   windowRect.right - windowRect.left,	// 计算调整好的窗口宽度
								   windowRect.bottom - windowRect.top,	// 计算调整好的窗口高度
								   HWND_DESKTOP,						// 无父窗口
								   0,									// 无菜单
								   window->init.application->hInstance, // 实例
								   window);								// 向window的WM_CREATE传递window结构
																		
	if (window->hWnd == 0)												// 窗口创建是否成功	
	{
		return FALSE;													// 不成功则返回
	}

	window->hDC = GetDC (window->hWnd);									// 返回设备描述表
	if (window->hDC == 0)												// 是否成功获得设备描述表
	{
		// 失败
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回失败
	}

	PixelFormat = ChoosePixelFormat (window->hDC, &pfd);				// 选择一种相容的像素格式
	if (PixelFormat == 0)												// 是否获得相容的像素格式
	{
		// 失败
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	if (SetPixelFormat (window->hDC, PixelFormat, &pfd) == FALSE)		// 设置像素格式
	{
		// 错误
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	window->hRC = wglCreateContext (window->hDC);						// 设置OpenGL渲染描述表
	if (window->hRC == 0)												// 如果没有成功创建
	{
		ReleaseDC (window->hWnd, window->hDC);							// 释放设备描述表
		window->hDC = 0;												// 设置窗口设备描述表为0
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												// 设置窗口句柄为0
		return FALSE;													// 返回错误
	}

	if (wglMakeCurrent (window->hDC, window->hRC) == FALSE)				// 设置RC为当前的OpenGL的渲染描述表
	{
		wglDeleteContext (window->hRC);									// 删除当前的OpenGL渲染描述表
		window->hRC = 0;												// 释放设备描述表
		ReleaseDC (window->hWnd, window->hDC);							// 设置窗口设备描述表为0
		window->hDC = 0;												// 删除窗口
		DestroyWindow (window->hWnd);									// 设置窗口句柄为0
		window->hWnd = 0;												// 返回错误
		return FALSE;													
	}

	ShowWindow (window->hWnd, SW_NORMAL);								// 显示窗口
	window->isVisible = TRUE;											// 设置窗口为可见

	ReshapeGL (window->init.width, window->init.height);				// 根据窗口大小，调整OpenGL对应的窗口大小

	ZeroMemory (window->keys, sizeof (Keys));							// 初始化按键记录表

	window->lastTickCount = GetTickCount ();							// 记录当前的时间数

	return TRUE;														// 窗口创建完毕，成功返回
																		
}

BOOL DestroyWindowGL (GL_Window* window)								// 删除窗口
{
	if (window->hWnd != 0)												// 如果窗口句柄不为0
	{	
		if (window->hDC != 0)											// 如果窗口设备描述表不为0
		{
			wglMakeCurrent (window->hDC, 0);							// 释放当前的OpenGL渲染描述表
			if (window->hRC != 0)										
			{
				wglDeleteContext (window->hRC);						
				window->hRC = 0;										

			}
			ReleaseDC (window->hWnd, window->hDC);						// 释放窗口设备描述表	
			window->hDC = 0;											
		}
		DestroyWindow (window->hWnd);									// 删除窗口
		window->hWnd = 0;												
	}

	if (window->init.isFullScreen)										// 如果是全屏模式，变换为初始模式
	{
		ChangeDisplaySettings (NULL,0);									
	}
	ShowCursor(TRUE);													// 显示鼠标
	return TRUE;														// 删除窗口完毕，成功返回
}

// 窗口消息处理函数
LRESULT CALLBACK WindowProc (HWND hWnd,									// 窗口的句柄	
							 UINT uMsg,									// 窗口的消息
							 WPARAM wParam,								// 附加的消息内容
							 LPARAM lParam)								// 附加的消息内容
{
	// 返回创建信息
	GL_Window* window = (GL_Window*)(GetWindowLong (hWnd, GWL_USERDATA));

	//循环处理每个消息
	switch (uMsg)														
	{
		case WM_SYSCOMMAND:												// 捕获系统消息
		{
			switch (wParam)												// 检测系统调用
			{
				case SC_SCREENSAVE:										// 如果是要运行屏保程序
				case SC_MONITORPOWER:									// 如果是要关闭显示器
				return 0;												// 截获上面的消息，不做处理
			}
			break;														
		}
		return 0;														

		case WM_CREATE:													// 窗口创建
		{
			CREATESTRUCT* creation = (CREATESTRUCT*)(lParam);			// 获得创建的窗口信息
			window = (GL_Window*)(creation->lpCreateParams);			
			SetWindowLong (hWnd, GWL_USERDATA, (LONG)(window));
		}
		return 0;														

		case WM_CLOSE:													// 关闭窗口
			TerminateApplication(window);								// 结束程序
		return 0;														

		case WM_SIZE:													// 改变大小
			switch (wParam)												
			{
				case SIZE_MINIMIZED:									// 最小化
					window->isVisible = FALSE;							// 如果是，设窗口不可见
				return 0;												

				case SIZE_MAXIMIZED:									// 最大化
					window->isVisible = TRUE;							// 设置窗口可见
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// 更改OpenGL窗口的大小
				return 0;												

				case SIZE_RESTORED:										// 创口是否被重新载入了
					window->isVisible = TRUE;							// 设置窗口可见
					ReshapeGL (LOWORD (lParam), HIWORD (lParam));		// 更改OpenGL窗口的大小
				return 0;												
			}
		break;															

		case WM_KEYDOWN:												// 更新按键控制
			if ((wParam >= 0) && (wParam <= 255))						// 如果为键盘支持的256个字符
			{
				window->keys->keyDown [wParam] = TRUE;					// 记录那个字符被按下
				return 0;												
			}
		break;															

		case WM_KEYUP:													// 更新按键控制
			if ((wParam >= 0) && (wParam <= 255))						
			{
				window->keys->keyDown [wParam] = FALSE;					// 记录那个字符被释放
				return 0;												
			}
		break;															

		case WM_TOGGLEFULLSCREEN:										// 切换全拼/窗口模式
			g_createFullScreen = (g_createFullScreen == TRUE) ? FALSE : TRUE;
			PostMessage (hWnd, WM_QUIT, 0, 0);
		break;															
	}

	return DefWindowProc (hWnd, uMsg, wParam, lParam);					
}

BOOL RegisterWindowClass (Application* application)						// 注册一个窗口			
{																		
	WNDCLASSEX windowClass;												// 窗口类
	ZeroMemory (&windowClass, sizeof (WNDCLASSEX));						// 清空结构为0
	windowClass.cbSize			= sizeof (WNDCLASSEX);					// 窗口结构的大小
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;	// 设置窗口类型为，移动时重画，并为窗口取得DC
	windowClass.lpfnWndProc		= (WNDPROC)(WindowProc);				// WndProc处理消息
	windowClass.hInstance		= application->hInstance;				// 设置实例
	windowClass.hbrBackground	= (HBRUSH)(COLOR_APPWORKSPACE);			// 设置背景
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);			// 载入光标
	windowClass.lpszClassName	= application->className;				// 设置类名



	if (RegisterClassEx (&windowClass) == 0)							// 尝试注册窗口类
	{
		MessageBox(NULL,"注册窗口失败","错误",MB_OK|MB_ICONEXCLAMATION);
		return FALSE;													// 退出并返回FALSE									
	}
	return TRUE;														
}

// 程序入口，Windows主循环
int WINAPI WinMain (HINSTANCE hInstance,			// 当前窗口实例
					HINSTANCE hPrevInstance,		// 前一个窗口实例
					LPSTR lpCmdLine,				// 命令行参数
					int nCmdShow)					// 窗口显示状态
{
	Application			application;				// 程序结构					
	GL_Window			window;						// 窗口结构			
	Keys				keys;						// 按键结构			
	BOOL				isMessagePumpActive;		// 					
	MSG					msg;						// 消息结构					
	DWORD				tickCount;					// 用来计数			

	application.className = "OpenGL";				// 程序名为OpenGL					
	application.hInstance = hInstance;				// 保留窗口实例	

	// 设置窗口信息
	ZeroMemory (&window, sizeof (GL_Window));							// 把这个结构清0
	window.keys					= &keys;								// 设置保存按键的数组
	window.init.application		= &application;							// 保存程序实例
	window.init.title			= "NeHe's Tutorial";					// 设置标题
	window.init.width			= 640;									// 设置窗口宽度
	window.init.height			= 480;									// 设置窗口高度
	window.init.bitsPerPixel	= 32;									// 设置颜色深度为32									
	window.init.isFullScreen	= TRUE;									// 设置去全屏幕状态

	ZeroMemory (&keys, sizeof (Keys));									//清空保存按键的数组

	// 询问是否在全屏状态下运行
	if (MessageBox (HWND_DESKTOP, "你想在全屏状态下运行么?", "开始全屏运行", MB_YESNO | MB_ICONQUESTION) == IDNO)
	{
		window.init.isFullScreen = FALSE;								
	}

	// 注册窗口
	if (RegisterWindowClass (&application) == FALSE)					
	{
		MessageBox (HWND_DESKTOP, "注册窗口失败!", "错误", MB_OK | MB_ICONEXCLAMATION);
		return -1;														
	}

	g_isProgramLooping = TRUE;											// 设置程序循环为TRUE
	g_createFullScreen = window.init.isFullScreen;						// 记录是否创建全屏窗口
	while (g_isProgramLooping)											// 如果程序循环为TRUE
	{
		//创建一个窗口
		window.init.isFullScreen = g_createFullScreen;					
		if (CreateWindowGL (&window) == TRUE)							
		{
			// 创建功，初始化OpenGL
			if (Initialize (&window, &keys) == FALSE)					
			{
				// 如果失败，则退出
				TerminateApplication(&window);							
			}
			//初始化成功
			else														
			{
				// 如果消息队列中有消息，处理它
				isMessagePumpActive = TRUE;								
				while (isMessagePumpActive == TRUE)						
				{
					// 获得一条消息
					if (PeekMessage (&msg, window.hWnd, 0, 0, PM_REMOVE) != 0)
					{
						// 如果不是退出消息，把消息发送到处理函数
						if (msg.message != WM_QUIT)						
						{
							DispatchMessage (&msg);						
						}
						// 否则结束退出
						else											
						{
							isMessagePumpActive = FALSE;				
						}
					}
					//如果没有消息
					else												
					{
						//窗口不可见
						if (window.isVisible == FALSE)					
						{
							//等待下一条消息
							WaitMessage ();								
						}
						//窗口可见
						else											
						{
							//获得当前时间计数器值
							tickCount = GetTickCount ();				
							//根据时间，更新场景
							Update (tickCount - window.lastTickCount);	
							//记录当前的时间计数器值
							window.lastTickCount = tickCount;			
							//绘制场景
							Draw ();									
							//交换帧缓存
							SwapBuffers (window.hDC);					
						}
					}
				}														
			}															

			//准备卸载
			Deinitialize ();											

			//删除当前窗口
			DestroyWindowGL (&window);									
		}
		//否则退出程序
		else															
		{
			MessageBox (HWND_DESKTOP, "创建OpenGL窗口错误", "错误", MB_OK | MB_ICONEXCLAMATION);
			g_isProgramLooping = FALSE;									
		}
	}																	

	// 取消注册的窗口
	UnregisterClass (application.className, application.hInstance);		
	return 0;
}																		
