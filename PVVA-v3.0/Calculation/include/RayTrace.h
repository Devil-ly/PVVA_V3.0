#pragma once
/**************************************************************************************
* version 1.0 2017/5/2
* Designer liyun
* E-mail 465120258@qq.com
* Fuction��
* ����׷��
***************************************************************************************/

#ifndef RAYTRACE_H
#define RAYTRACE_H

#include "Tools/Vector3.h"

class RayTrace
{
public:
	virtual ~RayTrace() {};

	void Reflect_Ray(double * a, Vector3 &n, Vector3 org,
		Vector3 &interPoint, double xmin, double xmax,
		double ymin, double ymax, double zmin, double zmax);

	// F(x,y,z) = a0*x^2 + a1*y^2 + a2*z^2 + a3*x*y + a4*y*z +
	// a5*x*z + a6*x + a7*y + a8*z + a9. �� ���� org + t * n �Ľ���
	bool Ray_CurvedSurface(double * a, Vector3 n, Vector3 org,
		double &t, Vector3 &interPoint);

	bool IsIntersect(const Vector3 &orig, const Vector3 &dir, 
		const Vector3 &v0, const Vector3 &v1, const Vector3 &v2,
		Vector3 &intersection, double &t);

	Vector3 ReflectLight(Vector3 a, Vector3 n);

	double CalDistance(const Vector3 &a, const Vector3 &b) const;

	// ����x��y �õ�z��ֵ ����zmin �� zmax ֮ǰ��ֵ �������������򷵻ش��һ��
	bool getZ(double * a, double x, double y, double zmin, double zmax, double &z) const;

};


#endif // !RAYTRACE_H
