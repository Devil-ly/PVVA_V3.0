#pragma once
#include "VTK/include/Surface.h"

class Boolean : public Surface
{
public:
	Boolean();
	~Boolean();

	void updateData();
	void calPolyData(double ds = 0);

	void setBoolean(Surface* a, Surface* b);

private:

	// boolean 布尔运算用的
	Surface* a_surface;
	Surface* b_surface;
};