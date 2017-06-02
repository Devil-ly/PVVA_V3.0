#include "..\include\Boolean.h"

#include <vtkBooleanOperationPolyDataFilter.h>
#include <vtkPoints.h>
#include <vtkTriangle.h>
#include <vtkCellArray.h>

#include "Calculation/include/Matrix4D.h"
#include "Calculation/include/Vector3D.h"
Boolean::Boolean()
{
	Name = BOOLEANOP;
}

Boolean::~Boolean()
{
}

void Boolean::updateData()
{
	calPolyData();
	calActor();
}

void Boolean::calPolyData(double ds)
{
	int operation = data[7];
	vtkSmartPointer<vtkBooleanOperationPolyDataFilter> booleanOperation =
		vtkSmartPointer<vtkBooleanOperationPolyDataFilter>::New();
	if (operation == 0)
	{
		booleanOperation->SetOperationToUnion();
	}
	else if (operation == 1)
	{
		booleanOperation->SetOperationToIntersection();
	}
	else if (operation == 2)
	{
		booleanOperation->SetOperationToDifference();
	}
	else if (operation == 3)
	{
		booleanOperation->SetOperationToIntersection();

		booleanOperation->SetInputData(0, a_surface->getPolyData()); // set the input data  
		booleanOperation->SetInputData(1, b_surface->getPolyData());
		booleanOperation->Update();

		vtkSmartPointer<vtkPolyData> polyData = booleanOperation->GetOutput();

		// a_surface一定是抛物面
		std::vector<double> surfaceData;
		surfaceData = a_surface->getData();


		float temp = 4 * surfaceData[8]; // data[7] 表示焦距

										 // 世界坐标系转到模型的相对坐标系矩阵（逆矩阵）先旋转后平移
		Vector3D RotateAsix(surfaceData[3], surfaceData[4], surfaceData[5]);
		Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-surfaceData[6], RotateAsix);
		//Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(-surfaceData[0], -surfaceData[1], -surfaceData[2]);

		Vector3D rotatTranslate(surfaceData[0], surfaceData[1], surfaceData[2]);
		rotatTranslate = R_rotatMatrix * rotatTranslate; // 先旋转在平移（把平移的坐标一起旋转）
		Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));

		vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
		vtkSmartPointer<vtkTriangle> p1 = vtkSmartPointer<vtkTriangle>::New();
		vtkSmartPointer<vtkCellArray>pLineCell = vtkCellArray::New();

		int EleNum = polyData->GetNumberOfCells();
		int couter = 0; // 计数器
		for (int i = 0; i < EleNum; i++)
		{
			vtkIdList * p = polyData->GetCell(i)->GetPointIds();
			bool ok = true;
			for (int j = 0; j < 3; j++)
			{
				double * temppoint = polyData->GetPoint(p->GetId(j));
				Vector3 point = R_translateMatrix*R_rotatMatrix*
					Vector3(temppoint[0], temppoint[1], temppoint[2]);
				double tempz = (point.x * point.x + point.y * point.y) / temp;
				tempz = abs(tempz - point.z);
				if (tempz > 0.01)
				{
					ok = false;
					break;
				}
			}
			if (ok)
			{
				points->InsertNextPoint(polyData->GetPoint(p->GetId(0)));
				points->InsertNextPoint(polyData->GetPoint(p->GetId(1)));
				points->InsertNextPoint(polyData->GetPoint(p->GetId(2)));
				p1->GetPointIds()->SetId(0, couter++);
				p1->GetPointIds()->SetId(1, couter++);
				p1->GetPointIds()->SetId(2, couter++);
				pLineCell->InsertNextCell(p1);
			}
		}

		this->polyData = vtkSmartPointer<vtkPolyData>::New();
		this->polyData->SetPoints(points); //获得网格模型中的几何数据：点集  
		this->polyData->SetStrips(pLineCell);

		return;
	}
	booleanOperation->SetInputData(0, a_surface->getPolyData()); // set the input data  
	booleanOperation->SetInputData(1, b_surface->getPolyData());
	booleanOperation->Update();

	this->polyData = booleanOperation->GetOutput();
}

void Boolean::setBoolean(Surface * a, Surface * b)
{
	a_surface = a;
	b_surface = b;
}
