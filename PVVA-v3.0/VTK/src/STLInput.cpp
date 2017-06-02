#include "..\include\STLInput.h"

#include <vtkSTLReader.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

STLInput::STLInput(const string & file)
{
	fileName = file;
	data.resize(7);
	Name = STLINPUT;
}

STLInput::STLInput(const STLInput & stl)
{
	fileName = stl.fileName;
	data.resize(stl.data.size());
	actor = stl.actor;
	polyData = stl.polyData;
	isShow = stl.isShow;
	isTransparent = stl.isTransparent;
	varible.clear();
	for (auto x : stl.varible)
		varible.push_back(x);
	for (int i = 0; i < stl.data.size(); i++)
		data[i] = stl.data[i];
	property = stl.property;
	polyDataSTL = stl.polyDataSTL;
	Name = STLINPUT;
}

STLInput::~STLInput()
{
}

void STLInput::updateData()
{
	updateVarible();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyDataSTL);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();

	polyData = TransFilter->GetOutput();

	calActor();
}

void STLInput::calPolyData(double ds)
{
	vtkSmartPointer<vtkSTLReader> reader =
		vtkSmartPointer<vtkSTLReader>::New();
	reader->SetFileName(fileName.c_str());
	reader->Update();
	polyDataSTL = reader->GetOutput();
	/*
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();
	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyDataSTL);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();
    */
	polyData = reader->GetOutput();
}

void STLInput::setNameFile(const string & file)
{
	fileName = file;
}
