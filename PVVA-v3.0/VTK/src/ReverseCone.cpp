#include "..\include\ReverseCone.h"

#include <vtkConeSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>

ReverseCone::ReverseCone()
{
	Name = REVERSECONE;
}

ReverseCone::~ReverseCone()
{
}

void ReverseCone::updateData()
{
	updateVarible();
	calPolyData();
	calActor();
}

void ReverseCone::calPolyData(double ds)
{
	vtkSmartPointer<vtkConeSource> cone =
		vtkSmartPointer<vtkConeSource>::New();
	cone->SetAngle(data[7]);
	cone->SetResolution(20);
	cone->Update();

	polyData = cone->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	transform->Translate(0, 0, 0.5);
	transform->RotateWXYZ(90, 0, 1, 0);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();
}
