#if !defined(_3DOBJECT_H_)
#define _3DOBJECT_H_

#include <stdio.h>

//  3D顶点结构
struct sPoint{
	float x, y, z;
};

// 平面方程为: ax + by + cz + d = 0
struct sPlaneEq{
	float a, b, c, d;
};

// 描述一个模型表面的结构
struct sPlane{
	unsigned int p[3];
	sPoint normals[3];
	unsigned int neigh[3];
	sPlaneEq PlaneEq;
	bool visible;
};

// 描述一个产生阴影的物体
struct glObject{
	GLuint nPlanes, nPoints;
	sPoint points[100];
	sPlane planes[200];
};

// 读取模型
inline int ReadObject(char *st, glObject *o){
  FILE *file;
  unsigned int i;

  file = fopen(st, "r");
  if (!file) return FALSE;
  //读取顶点
  fscanf(file, "%d", &(o->nPoints));
  for (i=1;i<=o->nPoints;i++){
    fscanf(file, "%f", &(o->points[i].x));
    fscanf(file, "%f", &(o->points[i].y));
    fscanf(file, "%f", &(o->points[i].z));
  }
  //读取三角形面
  fscanf(file, "%d", &(o->nPlanes));
  for (i=0;i<o->nPlanes;i++){
    fscanf(file, "%d", &(o->planes[i].p[0]));
    fscanf(file, "%d", &(o->planes[i].p[1]));
    fscanf(file, "%d", &(o->planes[i].p[2]));
	//读取每个顶点的法线
    fscanf(file, "%f", &(o->planes[i].normals[0].x));
    fscanf(file, "%f", &(o->planes[i].normals[0].y));
    fscanf(file, "%f", &(o->planes[i].normals[0].z));
    fscanf(file, "%f", &(o->planes[i].normals[1].x));
    fscanf(file, "%f", &(o->planes[i].normals[1].y));
    fscanf(file, "%f", &(o->planes[i].normals[1].z));
    fscanf(file, "%f", &(o->planes[i].normals[2].x));
    fscanf(file, "%f", &(o->planes[i].normals[2].y));
    fscanf(file, "%f", &(o->planes[i].normals[2].z));
  }
  return TRUE;
}

// 设置相邻顶点信息
inline void SetConnectivity(glObject *o){
	unsigned int p1i, p2i, p1j, p2j;
	unsigned int P1i, P2i, P1j, P2j;
	unsigned int i,j,ki,kj;

	//对于模型中的每一个面A
	for(i=0;i<o->nPlanes-1;i++)
	{
		//对于除了此面的其它的面B
		for(j=i+1;j<o->nPlanes;j++)
		{
			//对于面A中的每一个相邻的顶点
			for(ki=0;ki<3;ki++)
			{
				//如果这个相邻的顶点没有被设置
				if(!o->planes[i].neigh[ki])
				{
					for(kj=0;kj<3;kj++)
					{
						p1i=ki;
						p1j=kj;
						p2i=(ki+1)%3;
						p2j=(kj+1)%3;

						p1i=o->planes[i].p[p1i];
						p2i=o->planes[i].p[p2i];
						p1j=o->planes[j].p[p1j];
						p2j=o->planes[j].p[p2j];
				
						//如果面A的边P1i->P1j和面B的边P2i->P2j为同一条边，则又下面的公式的P1i=P1j，并且P2i=P2j
						P1i=((p1i+p2i)-abs(int(p1i-p2i)))/2;
						P2i=((p1i+p2i)+abs(int(p1i-p2i)))/2;
						P1j=((p1j+p2j)-abs(int(p1j-p2j)))/2;
						P2j=((p1j+p2j)+abs(int(p1j-p2j)))/2;

						//记录与这个边相邻的面的索引
						if((P1i==P1j) && (P2i==P2j))
						{
							o->planes[i].neigh[ki] = j+1;	  
							o->planes[j].neigh[kj] = i+1;	  
						}
					}
				}
			}
		}
	}
}

// 计算平面的方程参数
inline void CalcPlane(glObject o, sPlane *plane){
	sPoint v[4];
	int i;

	for (i=0;i<3;i++){
		v[i+1].x = o.points[plane->p[i]].x;
		v[i+1].y = o.points[plane->p[i]].y;
		v[i+1].z = o.points[plane->p[i]].z;
	}
	plane->PlaneEq.a = v[1].y*(v[2].z-v[3].z) + v[2].y*(v[3].z-v[1].z) + v[3].y*(v[1].z-v[2].z);
	plane->PlaneEq.b = v[1].z*(v[2].x-v[3].x) + v[2].z*(v[3].x-v[1].x) + v[3].z*(v[1].x-v[2].x);
	plane->PlaneEq.c = v[1].x*(v[2].y-v[3].y) + v[2].x*(v[3].y-v[1].y) + v[3].x*(v[1].y-v[2].y);
	plane->PlaneEq.d =-( v[1].x*(v[2].y*v[3].z - v[3].y*v[2].z) +
					  v[2].x*(v[3].y*v[1].z - v[1].y*v[3].z) +
					  v[3].x*(v[1].y*v[2].z - v[2].y*v[1].z) );
}

// 绘制模型
void DrawGLObject(glObject o)
{
	unsigned int i, j;

	glBegin(GL_TRIANGLES);
	for (i=0; i<o.nPlanes; i++){
		for (j=0; j<3; j++){
			glNormal3f(o.planes[i].normals[j].x,
					o.planes[i].normals[j].y,
					o.planes[i].normals[j].z);
			glVertex3f(o.points[o.planes[i].p[j]].x,
					o.points[o.planes[i].p[j]].y,
					o.points[o.planes[i].p[j]].z);
		}
	}
	glEnd();
}

void doShadowPass(glObject *o, float *lp)
{
	unsigned int	i, j, k, jj;
	unsigned int	p1, p2;
	sPoint			v1, v2;

	//对模型中的每一个面
	for (i=0; i<o->nPlanes;i++)
	{	
		//如果面在灯光的前面
		if (o->planes[i].visible)
		{
			//对于被灯光照射的面的每一个相邻的面
			for (j=0;j<3;j++)
			{
				k = o->planes[i].neigh[j];
				//如果面不存在，或不被灯光照射，那么这个边是边界
				if ((!k) || (!o->planes[k-1].visible))
				{
					// 获得面的两个顶点
					p1 = o->planes[i].p[j];
					jj = (j+1)%3;
					p2 = o->planes[i].p[jj];

					//计算边的顶点到灯光的方向，并放大100倍
					v1.x = (o->points[p1].x - lp[0])*100;
					v1.y = (o->points[p1].y - lp[1])*100;
					v1.z = (o->points[p1].z - lp[2])*100;

					v2.x = (o->points[p2].x - lp[0])*100;
					v2.y = (o->points[p2].y - lp[1])*100;
					v2.z = (o->points[p2].z - lp[2])*100;
					
					//绘制构成阴影体边界的面
					glBegin(GL_TRIANGLE_STRIP);
						glVertex3f(o->points[p1].x,
									o->points[p1].y,
									o->points[p1].z);
						glVertex3f(o->points[p1].x + v1.x,
									o->points[p1].y + v1.y,
									o->points[p1].z + v1.z);

						glVertex3f(o->points[p2].x,
									o->points[p2].y,
									o->points[p2].z);
						glVertex3f(o->points[p2].x + v2.x,
									o->points[p2].y + v2.y,
									o->points[p2].z + v2.z);
					glEnd();
				}
			}
		}
	}

}

//绘制阴影
void  CastShadow(glObject *o, float *lp)
{
	float			side;

	//根据平面方程，设置那些在灯光的前方的平面为可见
	for (int i=0;i<o->nPlanes;i++)
	{
		side =	o->planes[i].PlaneEq.a*lp[0]+
				o->planes[i].PlaneEq.b*lp[1]+
				o->planes[i].PlaneEq.c*lp[2]+
				o->planes[i].PlaneEq.d*lp[3];
		if (side >0) o->planes[i].visible = TRUE;
				else o->planes[i].visible = FALSE;
	}

	glDisable( GL_LIGHTING );					// 关闭灯光
	
	glColorMask( GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE );		// 关闭颜色缓存的写入	
	
	glDepthFunc( GL_LEQUAL );					// 设置深度比较函数
	glDepthMask( GL_FALSE );					// 关闭深度缓存的写入	
	
	glEnable( GL_STENCIL_TEST );				// 使用蒙板缓存
	glStencilFunc( GL_ALWAYS, 1, 0xFFFFFFFFL );	// 设置蒙板函数


	// 如果是逆时针（即面向视点）的多边形，通过了蒙板和深度测试，则把蒙板的值增加1
	glFrontFace(GL_CCW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_INCR);
	doShadowPass(o,lp);

	// 如果是顺时针（即背向视点）的多边形，通过了蒙板和深度测试，则把蒙板的值减少1
	glFrontFace(GL_CW);
	glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
	doShadowPass(o,lp);

	glFrontFace(GL_CCW);
	glColorMask(1, 1, 1, 1);

	//把阴影绘制上颜色
	glColor4f(0.0f, 0.0f, 0.0f, 0.4f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glStencilFunc(GL_NOTEQUAL, 0, 0xffffffff);
	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	glPushMatrix();
	glLoadIdentity();
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(-0.1f, 0.1f,-0.10f);
		glVertex3f(-0.1f,-0.1f,-0.10f);
		glVertex3f( 0.1f, 0.1f,-0.10f);
		glVertex3f( 0.1f,-0.1f,-0.10f);
	glEnd();
	glPopMatrix();
	glDisable(GL_BLEND);

	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glEnable(GL_LIGHTING);
	glDisable(GL_STENCIL_TEST);
	glShadeModel(GL_SMOOTH);
}

#endif // _3DOBJECT_H_