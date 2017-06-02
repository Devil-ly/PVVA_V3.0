#pragma once
#include "VTK/include/Surface.h"

class STLInput : public Surface
{
public:
	STLInput(const string & file);
	STLInput(const STLInput & stl);
	~STLInput();


	void updateData();
	void calPolyData(double ds = 0);
	void setNameFile(const string & file);
private:
	vtkSmartPointer<vtkPolyData> polyDataSTL;
	string fileName;
};