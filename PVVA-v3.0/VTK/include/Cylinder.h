#pragma once

#include "VTK/include/Surface.h"

class Cylinder : public Surface
{
public:
	Cylinder();
	~Cylinder();

	void updateData();
	void calPolyData(double ds = 0);

private:

};