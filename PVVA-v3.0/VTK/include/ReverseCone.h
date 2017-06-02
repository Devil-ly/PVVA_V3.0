#pragma once

#include "VTK/include/Surface.h"

class ReverseCone : public Surface
{
public:
	ReverseCone();
	~ReverseCone();

	void updateData();
	void calPolyData(double ds = 0);

private:

};