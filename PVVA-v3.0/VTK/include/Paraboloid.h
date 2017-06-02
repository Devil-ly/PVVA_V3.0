#pragma once

#include "VTK/include/Surface.h"

class Paraboloid : public Surface
{
public:
	Paraboloid();
	~Paraboloid();

	void updateData();
	void calPolyData(double ds = 0);

private:

};