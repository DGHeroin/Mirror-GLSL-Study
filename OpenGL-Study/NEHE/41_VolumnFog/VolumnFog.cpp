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

/*********************�������ӵĿ��ļ�*******************************************************************************************/
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
GLfloat	fogColor[4] = {0.6f, 0.3f, 0.0f, 1.0f};					// �����ɫ 
GLfloat camz;													// �������Z��������

// ʹ��FogCoordfEXT����Ҫ�ı���
#define GL_FOG_COORDINATE_SOURCE_EXT	0x8450					// ��GLEXT.H�õ���ֵ
#define GL_FOG_COORDINATE_EXT		0x8451					

typedef void (APIENTRY * PFNGLFOGCOORDFEXTPROC) (GLfloat coord);// ���������꺯����ԭ��

PFNGLFOGCOORDFEXTPROC glFogCoordfEXT = NULL;					// ���������꺯��ָ��ΪNULL

GLuint	texture[1];												// ����

AUX_RGBImageRec *LoadBMP(CHAR *Filename)						// ����λͼͼ��
{
	FILE *File=NULL;											// �ļ����
	if (!Filename)												// ȷ���ļ������ṩ
	{
		return NULL;											// ���û�ṩ������ NULL
	}
	File=fopen(Filename,"r");									// ���Դ��ļ�
	if (File)													// �ļ�����ô?
	{
		fclose(File);											// �رվ��
		return auxDIBImageLoadA(Filename);						// ����λͼ������ָ��
	}
	return NULL;												// �������ʧ�ܣ����� NULL
}

int BuildTexture(char *szPathName, GLuint &texid)							
{
	int Status=FALSE;											// ״ָ̬ʾ��
	AUX_RGBImageRec *TextureImage[1];							// ��������Ĵ洢�ռ�
	memset(TextureImage,0,sizeof(void *)*1);					// ��ָ����Ϊ NULL
	// ����λͼ��������޴������λͼû�ҵ����˳�
	if (TextureImage[0]=LoadBMP(szPathName))
	{
		Status=TRUE;											// �� Status ��Ϊ TRUE
		glGenTextures(1, &texid);								// ��������

		// ʹ������λͼ�������� �ĵ�������
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		// ��������
		glTexImage2D(GL_TEXTURE_2D, 0, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);	// �����˲�
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);	// �����˲�
	}

	if (TextureImage[0])										// �����Ƿ����
	{
		if (TextureImage[0]->data)								// ����ͼ���Ƿ����
		{
			free(TextureImage[0]->data);						// �ͷ�����ͼ��ռ�õ��ڴ�
		}

		free(TextureImage[0]);									// �ͷ�ͼ��ṹ
	}
	return Status;												// ���� Status
}

int Extension_Init()
{
	char Extension_Name[] = "EXT_fog_coord";

	// ������չ�ַ���
	char* glextstring=(char *)malloc(strlen((char *)glGetString(GL_EXTENSIONS))+1);
	strcpy (glextstring,(char *)glGetString(GL_EXTENSIONS));		

	if (!strstr(glextstring,Extension_Name))					// �����Ƿ���������Ҫ����չ
		return FALSE;							

	free(glextstring);											// �ͷŷ�����ڴ�

	// ��ú�����ָ��
	glFogCoordfEXT = (PFNGLFOGCOORDFEXTPROC) wglGetProcAddress("glFogCoordfEXT");

	return TRUE;
}

BOOL Initialize (GL_Window* window, Keys* keys)					//��ʼ��
{
	g_window	= window;						
	g_keys		= keys;							

	// ��ʼ����չ
	if (!Extension_Init())							
		return FALSE;							

	if (!BuildTexture("data/wall.bmp", texture[0]))				// ��������
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
	glFogi(GL_FOG_COORDINATE_SOURCE_EXT, GL_FOG_COORDINATE_EXT);//���������ǻ��ڶ���������

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
	glBegin(GL_QUADS);							//��ǽ
	 	glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
	glEnd();

	glBegin(GL_QUADS);							// ����
	 	glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f,-2.5f, 15.0f);
	glEnd();

	glBegin(GL_QUADS);							// �컨��
		glFogCoordfEXT(0.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f, 15.0f);
	glEnd();

	glBegin(GL_QUADS);							// ��ǽ
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f( 2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f( 2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f( 2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f( 2.5f,-2.5f,-15.0f);
	glEnd();

	glBegin(GL_QUADS);							// ��ǽ
	 	glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 0.0f); glVertex3f(-2.5f,-2.5f, 15.0f);
		glFogCoordfEXT(1.0f); glTexCoord2f(0.0f, 1.0f); glVertex3f(-2.5f, 2.5f, 15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 1.0f); glVertex3f(-2.5f, 2.5f,-15.0f);
		glFogCoordfEXT(0.0f); glTexCoord2f(1.0f, 0.0f); glVertex3f(-2.5f,-2.5f,-15.0f);
	glEnd();

	glFlush ();	                                                                                               
}
/********************************************************************************************************************************/