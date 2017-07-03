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

/*********************包含链接的库文件*******************************************************************************************/
#pragma comment( lib, "opengl32.lib" )				
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glew32.lib")
#pragma comment( lib, "glaux.lib")
#pragma comment( lib, "vfw32.lib" )	
#pragma comment( lib, "winmm.lib" )							
/********************************************************************************************************************************/

#include <windows.h>
#include <GL/glew.h>
#include <stdio.h>
#include <GL/glaux.h>
#include <math.h>	

#include "NeHeGL.h"							

#pragma comment( lib, "opengl32.lib" )						
#pragma comment( lib, "glu32.lib" )					

GL_Window*	g_window;						
Keys*		g_keys;								
GLfloat	fogColor[4] = {0.6f, 0.3f, 0.0f, 1.0f};					// 雾的颜色 
GLfloat camz;													// 摄像机在Z方向的深度

// 使用FogCoordfEXT它需要的变量
#define GL_FOG_COORDINATE_SOURCE_EXT	0x8450					// 从GLEXT.H得到的值
#define GL_FOG_COORDINATE_EXT		0x8451					

typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);// 声明雾坐标函数的原形

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;					// 设置雾坐标函数指针为NULL

GLuint	texture[1];												// 纹理

AUX_RGBImageRec *LoadBMP(CHAR *Filename)						// 载入位图图象
{
	FILE *File=NULL;											// 文件句柄
	if (!Filename)												// 确保文件名已提供
	{
		return NULL;											// 如果没提供，返回 NULL
	}
	File=fopen(Filename,"r");									// 尝试打开文件
	if (File)													// 文件存在么?
	{
		fclose(File);											// 关闭句柄
		return auxDIBImageLoadA(Filename);						// 载入位图并返回指针
	}
	return NULL;												// 如果载入失败，返回 NULL
}

int BuildTexture(char *szPathName, GLuint &texid)							
{
	int Status=FALSE;											// 状态指示器
	AUX_RGBImageRec *TextureImage[1];							// 创建纹理的存储空间
	memset(TextureImage,0,sizeof(void *)*1);					// 将指针设为 NULL
	// 载入位图，检查有无错误，如果位图没找到则退出
	if (TextureImage[0]=LoadBMP(szPathName))
	{
		Status=TRUE;											// 将 Status 设为 TRUE
		glGenTextures(1, &texid);								// 创建纹理

		// 使用来自位图数据生成 的典型纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// 生成纹理
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// 线形滤波
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// 线形滤波
	}

	if (TextureImage[0])										// 纹理是否存在
	{
		if (TextureImage[0]->data)								// 纹理图像是否存在
		{
			free(TextureImage[0]->data);						// 释放纹理图像占用的内存
		}

		free(TextureImage[0]);									// 释放图像结构
	}
	return Status;												// 返回 Status
}

int Extension_Init()
{
	char Extension_Name[] = "EXT_fog_coord";

	// 返回扩展字符串
	char* glextstring=(char *)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);
	strcpy (glextstring,(char *)glGetString(GL_EXTENSIONS));		

	if (!strstr(glextstring,Extension_Name))					// 查找是否有我们想要的扩展
		return FALSE;							

	free(glextstring);											// 释放分配的内存

	// 获得函数的指针
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");

	return TRUE;
}

BOOL Initialize (GL_Window* window, Keys* keys)					//初始化
{
	g_window	= window;						
	g_keys		= keys;							

	// 初始化扩展
	if (!Extension_Init())							
		return FALSE;							

	if (!BuildTexture("data/wall.bmp", texture[0]))				// 创建纹理
		return FALSE;							
	
	const GLubyte* version = glGetString(GL_SHADING_LANGUAGE_VERSION);

	glEnable(GL_TEXTURE_2D);						
	glClearColor (0.0f, 0.0f, 0.0f, 0.5f);					
	glClearDepth (1.0f);							
	glDepthFunc (GL_LEQUAL);						
	glEnable (GL_DEPTH_TEST);						
	glShadeModel (GL_SMOOTH);						
	glHint (GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);		
	glEnable(GL_FOG);							
	glFogi(GL_FOG_MODE, GL_LINEAR);						
	glFogfv(GL_FOG_COLOR, fogColor);					
	glFogf(GL_FOG_START,  1.0f);						
	glFogf(GL_FOG_END,    0.0f);						
	glHint(GL_FOG_HINT, GL_NICEST);						
	glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);//设置雾气是基于顶点的坐标的

	camz =	-19.0f;								

	return TRUE;							
}

void Deinitialize (void)												
{
}

void Update (DWORD milliseconds)										
{
	if (g_keys->keyDown [VK_ESCAPE])								
		TerminateApplication (g_window);							

	if (g_keys->keyDown [VK_F1])									
		ToggleFullscreen (g_window);								

	if (g_keys->keyDown [VK_UP] && camz<14.0f)							
		camz+=(float)(milliseconds)/100.0f;							

	if (g_keys->keyDown [VK_DOWN] && camz>-19.0f)						
		camz-=(float)(milliseconds)/100.0f;						
}
void Draw (void)
{
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity ();							

	glTranslatef(0.0f, 0.0f, camz);	
	glBegin(GL_QUADS);							//后墙
	 	glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
	glEnd();

	glBegin(GL_QUADS);							// 地面
	 	glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,-2.5f, 15.0f);
	glEnd();

	glBegin(GL_QUADS);							// 天花板
		glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f, 15.0f);
	glEnd();

	glBegin(GL_QUADS);							// 右墙
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f( 2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
	glEnd();

	glBegin(GL_QUADS);							// 左墙
	 	glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
	glEnd();

	glFlush ();	                                                                                               
}
/********************************************************************************************************************************/