#include "VTK/include/Surface.h"
#include <Tools/Parameterization.h>

#include <vtkLineSource.h>
#include <vtkPlaneSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>

using namespace std;

Surface::Surface()
{
	isShow = true;
	isTransparent = false;
	property = vtkSmartPointer<vtkProperty>::New();
	property->SetOpacity(1);
	property->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);
}

Surface & Surface::operator=(const Surface & right)
{
	if (&right == this)
		return *this;
	actor = right.actor;
	polyData = right.polyData;
	isShow = right.isShow;
	isTransparent = right.isTransparent;
	varible.clear();
	for (auto x : right.varible)
		varible.push_back(x);
	for (int i = 0; i < right.data.size(); i++)
		data[i] = right.data[i];
	return *this;
}

vtkSmartPointer<vtkActor> Surface::getActor() const
{
	return actor;
}

void Surface::calActor()
{
	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//    mapper->SetInputConnection(contourFilter->GetOutputPort());
	mapper->SetInputData(polyData);
	mapper->ScalarVisibilityOff();

	if(actor == NULL)
		actor = vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
	//actor->GetProperty()->SetColor(180.0 / 255.0, 180.0 / 255.0, 180.0 / 255.0);

}

vtkSmartPointer<vtkPolyData> Surface::getPolyData() const
{
	return polyData;
}

void Surface::setBoolean(Surface * a, Surface * b)
{

}

void Surface::setIsShow(bool ok)
{
	isShow = ok;
	if (!ok)
	{
		property->SetOpacity(0);
	}
	else if(!isTransparent)
	{
		property->SetOpacity(1);
	}
	else
	{
		property->SetOpacity(0.4);
	}
	actor->SetProperty(property);
}

void Surface::setIsTransparent(bool ok)
{
	isTransparent = ok;
	if (!isShow)
	{
		property->SetOpacity(0);
	}
	else if (!isTransparent)
	{
		property->SetOpacity(1);
	}
	else
	{
		property->SetOpacity(0.4);
	}
	actor->SetProperty(property);
}

bool Surface::getIsTransparent() const
{
	return isTransparent;
}

vtkSmartPointer<vtkProperty> Surface::getProperty() const
{
	return property;
}

void Surface::updateProperty()
{
	actor->SetProperty(property);
}

int Surface::getName() const
{
	return Name;
}

vtkSmartPointer<vtkActor> Surface::getLineActor(const Vector3 & p0, const Vector3 & p1)
{
	vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
	line->SetPoint1(p0.x, p0.y, p0.z);
	line->SetPoint2(p1.x, p1.y, p1.z);

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(line->GetOutputPort());
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actorline = vtkSmartPointer<vtkActor>::New();
	actorline->SetMapper(mapper);
	actorline->GetProperty()->SetColor(1, 0, 0);
	return actorline;
}

vtkSmartPointer<vtkActor> Surface::getRectangleActor(const GraphTrans & graphTrans, float w, float d)
{
	w /= 2.0;
	d /= 2.0;
	vtkSmartPointer<vtkPlaneSource> plane = vtkSmartPointer<vtkPlaneSource>::New();
	//plane->SetCenter(0, 0, 0);
	plane->SetOrigin(-w, -d, 0);
	plane->SetPoint1(w, -d, 0);
	plane->SetPoint2(-w, d, 0);
	plane->Update();

	vtkSmartPointer<vtkPolyData> polyData = plane->GetOutput();
	vtkSmartPointer<vtkTransform> transform = vtkSmartPointer<vtkTransform>::New();

	// 用户自定义平移旋转 (先移动后旋转)
	transform->Translate(graphTrans.getTrans_x(), graphTrans.getTrans_y(), graphTrans.getTrans_z());
	transform->RotateWXYZ(graphTrans.getRotate_theta(), graphTrans.getRotate_x(),
		graphTrans.getRotate_y(), graphTrans.getRotate_z());

	vtkSmartPointer<vtkTransformPolyDataFilter> TransFilter =
		vtkSmartPointer<vtkTransformPolyDataFilter>::New();
	TransFilter->SetInputData(polyData);
	TransFilter->SetTransform(transform); //use vtkTransform (or maybe vtkLinearTransform)
	TransFilter->Update();

	vtkSmartPointer<vtkPolyDataMapper>mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	//mapper->SetInputConnection(line->GetOutputPort());
	mapper->SetInputData(TransFilter->GetOutput());
	mapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> actorline = vtkSmartPointer<vtkActor>::New();
	actorline->SetMapper(mapper);
	actorline->GetProperty()->SetColor(1, 0, 1);
	actorline->GetProperty()->SetOpacity(0.4);
	return actorline;
}




