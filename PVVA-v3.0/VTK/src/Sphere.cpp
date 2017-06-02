#include "..\include\Sphere.h"

#include <vtkSphereSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkTriangleFilter.h>

Sphere::Sphere()
{
	Name = SPHERE;
}

Sphere::~Sphere()
{
}

void Sphere::updateData()
{
	updateVarible();
	calPolyData();
	calActor();
}

void Sphere::calPolyData(double ds)
{
	float r = data[7];
	vtkSmartPointer<vtkSphereSource> sphere = vtkSmartPointer<vtkSphereSource>::New();
	//plane->SetCenter(0, 0, 0);

	sphere->SetRadius(r);
	sphere->SetCenter(data[0], data[1], data[2]);
	sphere->SetThetaResolution(40);
	sphere->SetPhiResolution(40);
	sphere->Update();

	polyData = sphere->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)

	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

	vtkSmartPointer<vtkTriangleFilter> triangleFilter =
		vtkSmartPointer<vtkTriangleFilter>::New();
	triangleFilter->SetInputData(polyData);
	//triangleFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	triangleFilter->Update();
	polyData = triangleFilter->GetOutput();
}
