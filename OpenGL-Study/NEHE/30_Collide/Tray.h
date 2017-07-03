#ifndef tray_h
#define tray_h

#include <iostream>
#include "mathex.h"
#include "tvector.h"
///////////////////////////////////////////////////////////////////////
//  射线类
///////////////////////////////////////////////////////////////////////
class TRay
{
	private:
		TVector _P; // 射线的端点
		TVector _V; // 射线的方向
		// 输入和输出
		ostream &write(ostream &out) const;
		istream &read(istream &in);
	public:
		// 构造函数
		TRay() {}
		TRay(const TVector &point1, const TVector &point2);
		// 检测两条直线的最近的点，并保存在point1和point2中
		bool adjacentPoints(const TRay &ray, TVector &point1, TVector &point2) const;
		// 反向射线的方向，原点不变
		static TRay &invert(const TRay &r, TRay &result) { result._P = r._P; TVector::invert(r._V, result._V); return result; }
		TRay operator-() const { return invert(*this, TRay()); }
		// 返回成员变量
		TVector P() const { return _P; }
		TVector V() const { return _V; }
		int isValid() const { return V().isUnit() && P().isValid(); }
		// 返回射线的距离
		double dist(const TRay &ray) const;
		double dist(const TVector &point) const;
		// 输入输出流
		friend ostream &operator<<(ostream &out, const TRay &o) { return o.write(out); }
		friend istream &operator>>(istream &in, TRay &o) { return o.read(in); }
};
#endif