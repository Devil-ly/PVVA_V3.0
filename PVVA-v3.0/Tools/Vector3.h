#pragma once
//��������ά������������-�������˵�˺Ͳ��

#ifndef VECTOR3_H
#define VECTOR3_H

#include<iostream>

class Vector3
{
public:
	Vector3(double x = 0, double y = 0, double z = 0)
		:x(x), y(y), z(z)
	{

	};

	~Vector3()
	{
	};

	//����"-"
	Vector3 operator - (const Vector3 &v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	};

	//����"+"
	Vector3 operator + (const Vector3 &v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	};

	//������
	Vector3 operator * (const double &v) const
	{
		return Vector3(x * v, y * v, z * v);
	};

	//���
	double Dot(const Vector3 &v) const
	{
		return x*v.x + y*v.y + z*v.z;
	};

	//���
	Vector3 Cross(const Vector3 &v) const
	{
		return Vector3(
			y * v.z - z * v.y,
			z * v.x - x * v.z,
			x * v.y - y * v.x);
	}

	void Normalization()
	{
		double temp = pow((x*x + y*y + z*z), 0.5);
		x /= temp;
		y /= temp;
		z /= temp;
	}

	double Length() const
	{
		return pow((x*x + y*y + z*z),0.5);
	}

	void display()
	{
		std::cout << x << " " << y << " " << z << std::endl;
	}

	void set(double x1, double y1, double z1)
	{
		x = x1; y = y1; z = z1;
	}
	double x, y, z;
};



#endif // VECTOR3_H