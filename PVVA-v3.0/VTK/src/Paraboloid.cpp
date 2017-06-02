#include "VTK/include/Paraboloid.h"

#include <vtkQuadric.h>
#include <vtkSampleFunction.h>
#include <vtkContourFilter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

Paraboloid::Paraboloid()
{
	Name = PARABOLOID;
}

Paraboloid::~Paraboloid()
{
}

void Paraboloid::updateData()
{
	updateVarible();
	calPolyData();
	calActor();
}

void Paraboloid::calPolyData(double ds)
{
	double radius = data[7];
	double temp = -4 * data[8]; // data[7] ��ʾ����
	vtkSmartPointer<vtkQuadric>quadric = vtkSmartPointer<vtkQuadric>::New();
	quadric->SetCoefficients(1, 1, 0, 0, 0, 0, 0, 0, temp, 0);

	//���κ��������ֱ���
	vtkSmartPointer<vtkSampleFunction>sample = vtkSmartPointer<vtkSampleFunction>::New();
	if (ds == 0)
		sample->SetSampleDimensions(40, 40, 20);
	else
		sample->SetSampleDimensions(int(radius / ds) * 2,
			int(radius / ds) * 2, int(-temp / ds) * 2); // �������ds�й�
	sample->SetImplicitFunction(quadric);
	double xmin = -radius * 2, xmax = radius * 2, ymin = -radius * 2,
		ymax = radius * 2, zmin = 1.0f / temp, zmax = -radius / temp + 1.0f / temp;
	sample->SetModelBounds(xmin, xmax, ymin, ymax, zmin, zmax);
	vtkSmartPointer<vtkContourFilter> contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	contourFilter->SetInputConnection(sample->GetOutputPort());
	contourFilter->GenerateValues(1, 1, 1);
	contourFilter->Update();

	polyData = contourFilter->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// �û��Զ���ƽ����ת (���ƶ�����ת)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	transform->Translate(0, 0, -zmin);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
	polyData = TransFilter->GetOutput();

}
