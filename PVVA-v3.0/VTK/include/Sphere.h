#pragma once
#include "VTK/include/Surface.h"

class Sphere : public Surface
{
public:
	Sphere();
	~Sphere();

	void updateData();
	void calPolyData(double ds = 0);

private:

};