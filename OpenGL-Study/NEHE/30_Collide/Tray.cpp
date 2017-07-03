#include "tray.h"
#include <math.h>
#include "mathex.h"

//构造函数
TRay::TRay(const TVector &point1, const TVector &point2): _P(point1) 
{
	_V = (point2.isUnit() ? point2 : TVector::unit(point2-point1));
}

//检测两条直线的最近的点，并保存在point1和point2中
bool TRay::adjacentPoints(const TRay &ray, TVector &point1, TVector &point2) const 
{

	if (isValid() && ray.isValid())
	{
		double temp = TVector::dot(_V, ray._V);
		double temp2 = 1.0 - sqr(temp);
		TVector tv;

		//检测直线是否平行
		if (fabs(temp2)<EPSILON) 
		{
			double mu = TVector::dot(_V, _P-ray._P)/temp;
			point1 = _P;
			TVector::add(ray._P, TVector::multiply(ray._V, mu, tv), point2);
		}
		//如果不平行求出最相邻的点
		else 
		{
			double a = TVector::dot(_V, TVector::subtract(ray._P, _P, tv));
			double b = TVector::dot(ray._V, TVector::subtract(_P, ray._P, tv));
			double mu = (b + temp*a)/temp2;
			double lambda = (a + temp*b)/temp2;
			TVector::add(_P, TVector::multiply(_V, lambda, tv), point1);
			TVector::add(ray._P, TVector::multiply(ray._V, mu, tv), point2);
		}
		return true;
	}
	return false;
}

// 返回射线的距离
double TRay::dist(const TRay &ray) const 
{
	TVector point1, point2;
	if (adjacentPoints(ray, point1, point2))
		return point1.dist(point2);
	else
		return 0.0;
}

// 返回射线的距离
double TRay::dist(const TVector &point) const 
{
	if (isValid() && point.isValid()) {
		TVector tv, point2;
		double lambda = TVector::dot(_V, point-_P);
		TVector::add(_P, TVector::multiply(_V, lambda, tv), point2);
		return point.dist(point2);
	}
	return 0.0;
}

// 输入输出流
ostream &TRay::write(ostream &out) const 
{
	return isValid() ? out<<"(P="<<_P<<",V="<<_V<<")" : out<<"Invalid";
}

istream &TRay::read(istream &in) 
{
	char buf[20],ch;
	in >> ch;
	if (ch != 'I')
		in >>ch>>ch>>_P>>ch>>ch>>ch>>_V;
	else {
		*this=TRay();
		in.get(buf,20,'d');
	}
	return in >> ch;
}