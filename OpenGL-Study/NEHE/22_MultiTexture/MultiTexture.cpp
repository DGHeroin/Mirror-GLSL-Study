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
//#pragma comment( lib, "vfw32.lib" )	
/********************************************************************************************************************************/

#include <windows.h>		// Windows的头文件
#include <GL/glew.h>			// 包含最新的gl.h,glu.h库
//#include <glut.h>			// 包含OpenGL实用库
#include <stdio.h>			// 标准输入/输出库的头文件
#include <math.h>			// 数学库函数
#include <GL/glaux.h>			// GLaux库的头文件
GLfloat		xrot;			// X 旋转量
GLfloat		yrot;			// Y 旋转量
GLfloat		zrot;			// Z 旋转量
/***********************************新添的代码***********************************************************************************/
#include <string.h>													// 字符串处理函数
#define MAX_EMBOSS (GLfloat)0.01f									// 定义了突起的最大值
#define __ARB_ENABLE true											// 使用它设置是否使用ARB扩展
// #define EXT_INFO													// 把注释去掉,可以在启动时查看你的扩展
#define MAX_EXTENSION_SPACE 10240									// 保存扩展字符
#define MAX_EXTENSION_LENGTH 256									// 每个扩展字符串最大的长度
bool multitextureSupported=false;									// 标志是否支持多重渲染
bool useMultitexture=true;											// 如果支持,是否使用它
GLint maxTexelUnits=1;												// 纹理处理单元的个数

PFNGLMULTITEXCOORD1FARBPROC	glMultiTexCoord1fARB	= NULL;
PFNGLMULTITEXCOORD2FARBPROC	glMultiTexCoord2fARB	= NULL;
PFNGLMULTITEXCOORD3FARBPROC	glMultiTexCoord3fARB	= NULL;
PFNGLMULTITEXCOORD4FARBPROC	glMultiTexCoord4fARB	= NULL;
PFNGLACTIVETEXTUREARBPROC	glActiveTextureARB	= NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC	glClientActiveTextureARB= NULL;

GLuint  filter=1;													// 定义过滤器类型
GLuint  texture[3];													// 保存三个纹理
GLuint  bump[3];													//保存三个凹凸纹理
GLuint  invbump[3];													// 保存三个反转了的凹凸纹理
GLuint  glLogo;														// glLogo保存标志
GLuint  multiLogo;													// multiLogo保存多重纹理标志
GLfloat LightAmbient[]	= { 0.2f, 0.2f, 0.2f};						// 环境光
GLfloat LightDiffuse[]	= { 1.0f, 1.0f, 1.0f};						// 漫射光
GLfloat LightPosition[]	= { 0.0f, 0.0f, 2.0f};						// 光源位置
GLfloat Gray[]		= { 0.5f, 0.5f, 0.5f, 1.0f};
bool	emboss=false;												// 是否使用凹凸贴图
bool    bumps=true;													// 是否现实凹凸贴图效果
GLfloat	z=-5.0f;													// 深入屏幕5个单位
GLfloat xspeed;														// X 轴旋转速度
GLfloat yspeed;														// Y 轴旋转速度

// 立方体的纹理和坐标

GLfloat data[]= {
	// 前面
	0.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	1.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, +1.0f,
	0.0f, 1.0f,		-1.0f, +1.0f, +1.0f,
	// 背面
	1.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
	1.0f, 1.0f,		-1.0f, +1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, -1.0f,
	// 上面
	0.0f, 1.0f,		-1.0f, +1.0f, -1.0f,
	0.0f, 0.0f,		-1.0f, +1.0f, +1.0f,
	1.0f, 0.0f,		+1.0f, +1.0f, +1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	// 下面
	1.0f, 1.0f,		-1.0f, -1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, -1.0f, -1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	1.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	// 右面
	1.0f, 0.0f,		+1.0f, -1.0f, -1.0f,
	1.0f, 1.0f,		+1.0f, +1.0f, -1.0f,
	0.0f, 1.0f,		+1.0f, +1.0f, +1.0f,
	0.0f, 0.0f,		+1.0f, -1.0f, +1.0f,
	// 左面
	0.0f, 0.0f,		-1.0f, -1.0f, -1.0f,
	1.0f, 0.0f,		-1.0f, -1.0f, +1.0f,
	1.0f, 1.0f,		-1.0f, +1.0f, +1.0f,
	0.0f, 1.0f,		-1.0f, +1.0f, -1.0f
};

bool isInString(char *string, const char *search) {
	int pos=0;
	int maxpos=strlen(search)-1;
	int len=strlen(string);
	char *other;
	for (int i=0; i<len; i++) {
		if ((i==0) || ((i>1) && string[i-1]=='\n')) {				// 新的扩展名开始与这里
			other=&string[i];
			pos=0;													// 开始新的比较
			while (string[i]!='\n') {								// 比较整个扩展名
				if (string[i]==search[pos]) pos++;					// 下一个字符
				if ((pos>maxpos) && string[i+1]=='\n') return true;	// 如果整个扩展名相同则成功返回
				i++;
			}
		}
	}
	return false;													// 没找到
}

bool initMultitexture(void) {
	char *extensions;
	extensions=strdup((char *) glGetString(GL_EXTENSIONS));			// 返回扩展名字符串
	int len=strlen(extensions);
	for (int i=0; i<len; i++)										// 使用'\n'分割各个扩展名
		if (extensions[i]==' ') extensions[i]='\n';

#ifdef EXT_INFO
	MessageBox(hWnd,extensions,"supported GL extensions",MB_OK | MB_ICONINFORMATION);
#endif

	if (isInString(extensions,"GL_ARB_multitexture")				// 是否支持多重纹理扩展？
		&& __ARB_ENABLE												// 是否使用多重纹理扩展？
		&& isInString(extensions,"GL_EXT_texture_env_combine"))		// 是否支持纹理环境混合
	{
		glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB,&maxTexelUnits);
		glMultiTexCoord1fARB = (PFNGLMULTITEXCOORD1FARBPROC) wglGetProcAddress("glMultiTexCoord1fARB");
		glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC) wglGetProcAddress("glMultiTexCoord2fARB");
		glMultiTexCoord3fARB = (PFNGLMULTITEXCOORD3FARBPROC) wglGetProcAddress("glMultiTexCoord3fARB");
		glMultiTexCoord4fARB = (PFNGLMULTITEXCOORD4FARBPROC) wglGetProcAddress("glMultiTexCoord4fARB");
		glActiveTextureARB   = (PFNGLACTIVETEXTUREARBPROC) wglGetProcAddress("glActiveTextureARB");
		glClientActiveTextureARB= (PFNGLCLIENTACTIVETEXTUREARBPROC) wglGetProcAddress("glClientActiveTextureARB");

#ifdef EXT_INFO
		MessageBox(hWnd,"The GL_ARB_multitexture 扩展被使用.","支持多重纹理",MB_OK | MB_ICONINFORMATION);
#endif

		return true;
	}
	useMultitexture=false;										// 如果不支持多重纹理则返回false
	return false;
}

void initLights(void) {
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
	glEnable(GL_LIGHT1);
}

void doCube (void) {
	int i;
	glBegin(GL_QUADS);
		// 前面
		glNormal3f( 0.0f, 0.0f, +1.0f);
		for (i=0; i<4; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// 后面
		glNormal3f( 0.0f, 0.0f,-1.0f);
		for (i=4; i<8; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// 上面
		glNormal3f( 0.0f, 1.0f, 0.0f);
		for (i=8; i<12; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// 下面
		glNormal3f( 0.0f,-1.0f, 0.0f);
		for (i=12; i<16; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// 右面
		glNormal3f( 1.0f, 0.0f, 0.0f);
		for (i=16; i<20; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
		// 左面
		glNormal3f(-1.0f, 0.0f, 0.0f);
		for (i=20; i<24; i++) {
			glTexCoord2f(data[5*i],data[5*i+1]);
			glVertex3f(data[5*i+2],data[5*i+3],data[5*i+4]);
		}
	glEnd();
}

// 计算向量v=v*M（左乘）
void VMatMult(GLfloat *M, GLfloat *v) {
	GLfloat res[3];
	res[0]=M[ 0]*v[0]+M[ 1]*v[1]+M[ 2]*v[2]+M[ 3]*v[3];
	res[1]=M[ 4]*v[0]+M[ 5]*v[1]+M[ 6]*v[2]+M[ 7]*v[3];
	res[2]=M[ 8]*v[0]+M[ 9]*v[1]+M[10]*v[2]+M[11]*v[3];
	v[0]=res[0];
	v[1]=res[1];
	v[2]=res[2];
	v[3]=M[15];								
}
// 设置纹理偏移，都为单位长度
// n : 表面的法向量
// c : 当前的顶点纹理坐标，返回纹理坐标的偏移量
// l : 灯光的位置
// s : s方向
// t : t方向
void SetUpBumps(GLfloat *n, GLfloat *c, GLfloat *l, GLfloat *s, GLfloat *t) {
	GLfloat v[3];								// 灯光方向
	GLfloat lenQ;								// 灯光方向向量的长度，使用它来单位化
	// 计算灯光方向
	v[0]=l[0]-c[0];
	v[1]=l[1]-c[1];
	v[2]=l[2]-c[2];
	lenQ=(GLfloat) sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
	v[0]/=lenQ;
	v[1]/=lenQ;
	v[2]/=lenQ;
	// 把方向向量投影到s,t方向 
	c[0]=(s[0]*v[0]+s[1]*v[1]+s[2]*v[2])*MAX_EMBOSS;
	c[1]=(t[0]*v[0]+t[1]*v[1]+t[2]*v[2])*MAX_EMBOSS;
}

void doLogo(void) 
{
	// 必须最后在调用这个函数，以公告板的形式显示两个标志
	glDepthFunc(GL_ALWAYS);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,glLogo);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);	glVertex3f(0.23f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,0.0f);	glVertex3f(0.53f, -0.4f,-1.0f);
		glTexCoord2f(1.0f,1.0f);	glVertex3f(0.53f, -0.25f,-1.0f);
		glTexCoord2f(0.0f,1.0f);	glVertex3f(0.23f, -0.25f,-1.0f);
	glEnd();
	if (useMultitexture) {
		glBindTexture(GL_TEXTURE_2D,multiLogo);
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);	glVertex3f(-0.53f, -0.25f,-1.0f);
			glTexCoord2f(1.0f,0.0f);	glVertex3f(-0.33f, -0.25f,-1.0f);
			glTexCoord2f(1.0f,1.0f);	glVertex3f(-0.33f, -0.15f,-1.0f);
			glTexCoord2f(0.0f,1.0f);	glVertex3f(-0.53f, -0.15f,-1.0f);
		glEnd();
	}
	glDepthFunc(GL_LEQUAL);
}

//使用一个纹理单元处理凹凸贴图
bool doMesh1TexelUnits(void) {
	GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};					// 保存当前的顶点
	GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};					// 保存法线
	GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};					// s纹理坐标方向
	GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};					// t纹理坐标方向
	GLfloat l[4];										// 保存灯光方向
	GLfloat Minv[16];									// 保存模型变换矩阵的逆
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清空背景颜色和深度缓存

	// 创建模型变换矩阵的逆
	glLoadIdentity();
	glRotatef(-yrot,0.0f,1.0f,0.0f);
	glRotatef(-xrot,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,z);
	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	// 设置灯光的位置
	l[0]=LightPosition[0];
	l[1]=LightPosition[1];
	l[2]=LightPosition[2];
	l[3]=1.0f;								
	VMatMult(Minv,l);

	//通道1，绘制凹凸贴图
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	doCube();

	//通道2，绘制反转的凹凸贴图
	glBindTexture(GL_TEXTURE_2D,invbump[filter]);
	glBlendFunc(GL_ONE,GL_ONE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_BLEND);

	glBegin(GL_QUADS);
		// 前面
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=1.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=0; i<4; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			// 设置纹理坐标为偏移后的纹理坐标
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 后面
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=-1.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=4; i<8; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 上面
		n[0]=0.0f;
		n[1]=1.0f;
		n[2]=0.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=8; i<12; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 下面
		n[0]=0.0f;
		n[1]=-1.0f;
		n[2]=0.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=12; i<16; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 右面
		n[0]=1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=-1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=16; i<20; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 左面
		n[0]=-1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=20; i<24; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glTexCoord2f(data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
	glEnd();

	//绘制基本贴图颜色
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	//绘制标志
	doLogo();
	return true;							// 成功返回
}

//使用多重纹理处理凹凸贴图
bool doMesh2TexelUnits(void) 
{
	GLfloat c[4]={0.0f,0.0f,0.0f,1.0f};					// 保存当前的顶点
	GLfloat n[4]={0.0f,0.0f,0.0f,1.0f};					// 保存法线
	GLfloat s[4]={0.0f,0.0f,0.0f,1.0f};					// s纹理坐标方向
	GLfloat t[4]={0.0f,0.0f,0.0f,1.0f};					// t纹理坐标方向
	GLfloat l[4];							// 保存灯光方向
	GLfloat Minv[16];							// 保存模型变换矩阵的逆
	int i;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			// 清空背景颜色和深度缓存

	// 创建模型变换矩阵的逆
	glLoadIdentity();
	glRotatef(-yrot,0.0f,1.0f,0.0f);
	glRotatef(-xrot,1.0f,0.0f,0.0f);
	glTranslatef(0.0f,0.0f,-z);
	glGetFloatv(GL_MODELVIEW_MATRIX,Minv);
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	// 设置灯光的位置
	l[0]=LightPosition[0];
	l[1]=LightPosition[1];
	l[2]=LightPosition[2];
	l[3]=1.0f;								
	VMatMult(Minv,l);
	// 纹理单元 #0
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, bump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

	// 纹理单元 #1
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, invbump[filter]);
	glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);
	glTexEnvf (GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_ADD);

	// 禁用混合和光照
	glDisable(GL_BLEND);
	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
		// 前面
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=1.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=0; i<4; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 后面
		n[0]=0.0f;
		n[1]=0.0f;
		n[2]=-1.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=4; i<8; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 上面
		n[0]=0.0f;
		n[1]=1.0f;
		n[2]=0.0f;
		s[0]=1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=8; i<12; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 下面
		n[0]=0.0f;
		n[1]=-1.0f;
		n[2]=0.0f;
		s[0]=-1.0f;
		s[1]=0.0f;
		s[2]=0.0f;
		t[0]=0.0f;
		t[1]=0.0f;
		t[2]=-1.0f;
		for (i=12; i<16; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 右面
		n[0]=1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=-1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=16; i<20; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
		// 左面
		n[0]=-1.0f;
		n[1]=0.0f;
		n[2]=0.0f;
		s[0]=0.0f;
		s[1]=0.0f;
		s[2]=1.0f;
		t[0]=0.0f;
		t[1]=1.0f;
		t[2]=0.0f;
		for (i=20; i<24; i++) {
			c[0]=data[5*i+2];
			c[1]=data[5*i+3];
			c[2]=data[5*i+4];
			SetUpBumps(n,c,l,s,t);
			glMultiTexCoord2fARB(GL_TEXTURE0_ARB,data[5*i], data[5*i+1]);
			glMultiTexCoord2fARB(GL_TEXTURE1_ARB,data[5*i]+c[0], data[5*i+1]+c[1]);
			glVertex3f(data[5*i+2], data[5*i+3], data[5*i+4]);
		}
	glEnd();
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	if (!emboss) {
		glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glBindTexture(GL_TEXTURE_2D,texture[filter]);
		glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
		glEnable(GL_BLEND);
		glEnable(GL_LIGHTING);
		doCube();
	}
	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	doLogo();
	return true;								
}

//绘制一个无凹凸贴图的立方体 
bool doMeshNoBumps(void) 
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();							
	glTranslatef(0.0f,0.0f,z);

	glRotatef(xrot,1.0f,0.0f,0.0f);
	glRotatef(yrot,0.0f,1.0f,0.0f);

	if (useMultitexture) {
		glActiveTextureARB(GL_TEXTURE1_ARB);
		glDisable(GL_TEXTURE_2D);
		glActiveTextureARB(GL_TEXTURE0_ARB);
	}

	glDisable(GL_BLEND);
	glBindTexture(GL_TEXTURE_2D,texture[filter]);
	glBlendFunc(GL_DST_COLOR,GL_SRC_COLOR);
	glEnable(GL_LIGHTING);
	doCube();

	xrot+=xspeed;
	yrot+=yspeed;
	if (xrot>360.0f) xrot-=360.0f;
	if (xrot<0.0f) xrot+=360.0f;
	if (yrot>360.0f) yrot-=360.0f;
	if (yrot<0.0f) yrot+=360.0f;

	doLogo();
	return true;								
}

/********************************************************************************************************************************/
//GLuint		texture[1];		// 存储一个纹理
HDC			hDC=NULL;		// 窗口着色描述表句柄
HGLRC		hRC=NULL;		// OpenGL渲染描述表句柄
HWND		hWnd=NULL;		// 保存我们的窗口句柄
HINSTANCE	hInstance;		// 保存程序的实例

bool	keys[256];			// 保存键盘按键的数组
bool	active=TRUE;		// 窗口的活动标志，缺省为TRUE
bool	fullscreen=TRUE;	// 全屏标志缺省，缺省设定成全屏模式
//GLfloat		rtri;			// 用于三角形的角度
//GLfloat		rquad;			// 用于四边形的角度

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
int LoadGLTextures()									// 载入*.bmp图像，并转换为纹理
{								
	bool status=true;		
	AUX_RGBImageRec *Image=NULL;						
	char *alpha=NULL;

	// 加载基础纹理
	if (Image=auxDIBImageLoad("Data/Base.bmp")) {
		glGenTextures(3, texture);						// 创建3个纹理

		// 创建使用临近过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, texture[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// 创建使用线形过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, texture[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// 创建使用线形Mipmap过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, texture[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
	}
	else status=false;

	if (Image) {										// 如果图像句柄存在，则释放图像回收资源
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}

	// 载入凹凸贴图
	if (Image=auxDIBImageLoad("Data/Bump.bmp")) {
		glPixelTransferf(GL_RED_SCALE,0.5f);							// 把颜色值变为原来的50%
		glPixelTransferf(GL_GREEN_SCALE,0.5f);				
		glPixelTransferf(GL_BLUE_SCALE,0.5f);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);		//不使用重复贴图
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glGenTextures(3, bump);	
		// 创建使用临近过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, bump[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		// 创建使用线形过滤器过滤得纹理e
		glBindTexture(GL_TEXTURE_2D, bump[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		//创建使用线形Mipmap过滤器过滤得纹理

		glBindTexture(GL_TEXTURE_2D, bump[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		for (int i=0; i<3*Image->sizeX*Image->sizeY; i++)		// 反转凹凸贴图数据
			Image->data[i]=255-Image->data[i];

		glGenTextures(3, invbump);								// 创建三个反转了凹凸贴图

		// 创建使用临近过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, invbump[0]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		// 创建使用线形过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, invbump[1]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, Image->sizeX, Image->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, Image->data);

		// 创建使用线形Mipmap过滤器过滤得纹理
		glBindTexture(GL_TEXTURE_2D, invbump[2]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB8, Image->sizeX, Image->sizeY, GL_RGB, GL_UNSIGNED_BYTE, Image->data);
		
		glPixelTransferf(GL_RED_SCALE,1.0f);					
		glPixelTransferf(GL_GREEN_SCALE,1.0f);			
		glPixelTransferf(GL_BLUE_SCALE,1.0f);
	}
	else status=false;
	if (Image) {											// 如果图像存在，则删除
		if (Image->data) delete Image->data;				
		delete Image;
	}

	if (Image=auxDIBImageLoad("Data/OpenGL_ALPHA.bmp")) {
		alpha=new char[4*Image->sizeX*Image->sizeY];
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
			alpha[4*a+3]=Image->data[a*3];				
		if (!(Image=auxDIBImageLoad("Data/OpenGL.bmp"))) status=false;
		for (int a=0; a<Image->sizeX*Image->sizeY; a++) {
			alpha[4*a]=Image->data[a*3];				
			alpha[4*a+1]=Image->data[a*3+1];		
			alpha[4*a+2]=Image->data[a*3+2];			
		}

		glGenTextures(1, &glLogo);						// 创建标志纹理

		// 使用线形过滤器
		glBindTexture(GL_TEXTURE_2D, glLogo);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);
		delete alpha;
	}
	else status=false;

	if (Image) {										// 如果图像存在，则删除
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}

	// 载入扩展标志纹理
	if (Image=auxDIBImageLoad("Data/multi_on_alpha.bmp")) {
		alpha=new char[4*Image->sizeX*Image->sizeY];			
		for (int a=0; a<Image->sizeX*Image->sizeY; a++)
			alpha[4*a+3]=Image->data[a*3];					
		if (!(Image=auxDIBImageLoad("Data/multi_on.bmp"))) status=false;
		for (int a=0; a<Image->sizeX*Image->sizeY; a++) {		
			alpha[4*a]=Image->data[a*3];					// R
			alpha[4*a+1]=Image->data[a*3+1];				// G
			alpha[4*a+2]=Image->data[a*3+2];				// B
		}
		glGenTextures(1, &multiLogo);					
		glBindTexture(GL_TEXTURE_2D, multiLogo);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Image->sizeX, Image->sizeY, 0, GL_RGBA, GL_UNSIGNED_BYTE, alpha);

		delete alpha;
	}
	else status=false;

	if (Image) {										// 如果图像存在，则删除
		if (Image->data) delete Image->data;				
		delete Image;
		Image=NULL;
	}
	return status;								
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

int InitGL(GLvoid)										// 此处开始对OpenGL进行所有设置
{
/***********************************新添的代码***********************************************************************************/
	multitextureSupported=initMultitexture();
	initLights();	
/********************************************************************************************************************************/
	if (!LoadGLTextures())								// 调用纹理载入子例程
	{
		return FALSE;									// 如果未能载入，返回FALSE
	}
	glEnable(GL_TEXTURE_2D);							// 启用纹理映射
	glShadeModel(GL_SMOOTH);							// 启用阴影平滑
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);				// 黑色背景
	glClearDepth(1.0f);									// 设置深度缓存
	glEnable(GL_DEPTH_TEST);							// 启用深度测试
	glDepthFunc(GL_LEQUAL);								// 所作深度测试的类型
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// 告诉系统对透视进行修正
	return TRUE;										// 初始化 OK
}

int DrawGLScene(GLvoid)									// 从这里开始进行所有的绘制
{

//	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// 清除屏幕和深度缓存
//	glLoadIdentity();									// 重置当前的模型观察矩阵
/***********************************新添的代码***********************************************************************************/
	if (bumps) 
	{
		if (useMultitexture && maxTexelUnits>1)
		{
			return doMesh2TexelUnits();
		}
		else 
		{	
			return doMesh1TexelUnits(); 	
		}
	}
	else 
		return doMeshNoBumps();

/********************************************************************************************************************************/	
//	return TRUE;										// 一切 OK
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
	if (!CreateGLWindow("NeHe's 多重纹理",640,480,16,fullscreen))
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
					if (keys['E'])
					{
						keys['E']=false;
						emboss=!emboss;
					}

					if (keys['M'])
					{
						keys['M']=false;
						useMultitexture=((!useMultitexture) && multitextureSupported);
					}

					if (keys['B'])
					{
						keys['B']=false;
						bumps=!bumps;
					}

					if (keys['F'])
					{
						keys['F']=false;
						filter++;
						filter%=3;
					}

					if (keys[VK_PRIOR])
					{
						z-=0.02f;
					}

					if (keys[VK_NEXT])
					{
						z+=0.02f;
					}

					if (keys[VK_UP])
					{
						xspeed-=0.01f;
					}

					if (keys[VK_DOWN])
					{
						xspeed+=0.01f;
					}

					if (keys[VK_RIGHT])
					{
						yspeed+=0.01f;
					}

					if (keys[VK_LEFT])
					{
						yspeed-=0.01f;
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
//				if (!CreateGLWindow("NeHe's 纹理实例",640,480,16,fullscreen))
/***********************************新添的代码***********************************************************************************/
				if (!CreateGLWindow("NeHe's 多重纹理",640,480,16,fullscreen))
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
