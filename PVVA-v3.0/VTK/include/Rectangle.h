#pragma once

#include "VTK/include/Surface.h"

class Rectangle : public Surface
{
public:
	Rectangle();
	~Rectangle();

	void updateData();
	void calPolyData(double ds = 0);

private:

};