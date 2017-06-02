#include "..\include\RaySource.h"
#include "Tools/GEMS_Memory.h"
#include "Calculation/include/Matrix4D.h"
#include "Calculation/include/Vector3D.h"
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkLine.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <cmath>
#include "Tools/Constant_Var.h"
#include "Calculation/include/RayTrace.h"
#include <Calculation/include/PVVA.h>
#include <Tools/Parameterization.h>

RaySource::RaySource()
{
	n = NULL;
	org = NULL;
	tempN = NULL;
	tempOrg = NULL;
	data.resize(12);
}

RaySource::~RaySource()
{
	if (n != NULL)
	{
		Free_2D(n);
		Free_2D(org);
	}
	if (tempN!= NULL)
	{
		Free_2D(tempN);
		Free_2D(tempOrg);
	}
	clearVector3();
}

RaySource & RaySource::operator=(const RaySource & right)
{
	if(this == &right)
		return *this;
	for (int i = 0; i < 12; i++)
		data[i] = right.data[i];
	type = right.type;
	clearVector3();
	N = right.N;
	M = right.M;
	org = Allocate_2D(org, N, M);
	n = Allocate_2D(n, N, M);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			org[i][j] = right.org[i][j];
			n[i][j] = right.n[i][j];
		}
	RayActor = right.RayActor;
	clearActor();
	for (auto x : right.actors)
		actors.push_back(x); 

	return *this;
}
/*
void RaySource::setGraphTrans(const GraphTrans & graphtrans)
{
	data[0] = graphtrans.getTrans_x();
	data[1] = graphtrans.getTrans_y();
	data[2] = graphtrans.getTrans_z();
	data[3] = graphtrans.getRotate_x();
	data[4] = graphtrans.getRotate_y();
	data[5] = graphtrans.getRotate_z();
	data[6] = graphtrans.getRotate_theta();
}

GraphTrans RaySource::getGraphTrans() const
{
	GraphTrans graphTrans;
	graphTrans.setGraphTransPar(data[0], data[1], data[2],
		data[3], data[4], data[5], data[6]);
	return graphTrans;
}

void RaySource::setData(vector<double> _data)
{
	//data.resize(12);
	//data = new double[n];
	for (int i = 0; i < 12; i++)
		data[i] = _data[i];
}

vector<double> RaySource::getData() const
{
	vector<double> temp(data.size());
	for (int i = 0; i < data.size(); i++)
		temp[i] = data[i];
	return temp;
}
*/
void RaySource::setType(int _type)
{
	type = _type;
}

int RaySource::getType() const
{
	return type;
}

void RaySource::setNM(int _N, int _M)
{
	if (n != NULL)
	{
		Free_2D(n);
		Free_2D(org);
	}
	N = _N;
	M = _M;
}

void RaySource::setLength(float dis)
{
	data[11] = dis;
}

void RaySource::getPlane(Vector3 ** &_org, Vector3 ** &_n)
{
	org = Allocate_2D(org, N, M);
	n = Allocate_2D(n, N, M);
	_org = org;
	_n = n;
}

void RaySource::copyPlane()
{
	if (tempN != NULL)
	{
		Free_2D(tempN);
		Free_2D(tempOrg);
	}
	tempOrg = Allocate_2D(tempOrg, N, M);
	tempN = Allocate_2D(tempN, N, M);
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			tempOrg[i][j] = org[i][j];
			tempN[i][j] = n[i][j];
		}
}

void RaySource::calRayTrace()
{
	Vector3D RotateAsix(data[3], data[4], data[5]);
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(data[6], RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(data[0], data[1], data[2]);
	

	if (type == 0) // ����
	{
		N = int(data[7] / data[9] + 1.0);
		M = int(data[8] / data[10] + 1.0);
		clearVector3();
		if (org != NULL)
		{
			Free_2D(org);
			Free_2D(n);
		}
		org = Allocate_2D(org, N, M);
		n = Allocate_2D(n, N, M);
		for(int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
			{
				Vector3 temp(i * data[9] - data[7] / 2, j * data[10] - data[8] / 2, 0);
				// ������任����������
				org[i][j] = translateMatrix * rotatMatrix * temp;
				n[i][j] = rotatMatrix * Vector3(0, 0, 1);
				n[i][j].Normalization();
			}
	}
	else if (type == 2)
	{
		N = int(360.0 / data[9]);
		M = 1;
		clearVector3();
		if (org != NULL)
		{
			Free_2D(org);
			Free_2D(n);
		}
		org = Allocate_2D(org, N, M);
		n = Allocate_2D(n, N, M);
		double z = cos(data[7] / 180 * Pi);
		double r = sin(data[7] / 180 * Pi);
		for (int i = 0; i < N; i++)
			for (int j = 0; j < M; j++)
			{
				// ������任����������
				org[i][j] = translateMatrix * rotatMatrix * Vector3(0, 0, 0);
				n[i][j] = rotatMatrix * Vector3(r*sin(data[9] * i / 180 * Pi),
					r*cos(data[9] * i / 180 * Pi), z);
				n[i][j].Normalization();
			}
	}
}

void RaySource::setSurface(Surface * modleData, vtkActor* actor)
{
	vector<double> surfaceData = modleData->getData();
	// ��������ϵת��ģ�͵��������ϵ�������������ת��ƽ��
	Vector3D RotateAsix(surfaceData[3], surfaceData[4], surfaceData[5]);
	Matrix4D R_rotatMatrix = Matrix4D::getRotateMatrix(-surfaceData[6], RotateAsix);
	Vector3D rotatTranslate(surfaceData[0], surfaceData[1], surfaceData[2]);
	rotatTranslate = R_rotatMatrix * rotatTranslate; // ����ת��ƽ�ƣ���ƽ�Ƶ�����һ����ת��
	Matrix4D R_translateMatrix = Matrix4D::getTranslateMatrix(rotatTranslate * (-1));


	// ģ�͵��������ϵת�������������
	Matrix4D rotatMatrix = Matrix4D::getRotateMatrix(surfaceData[6], RotateAsix);
	Matrix4D translateMatrix = Matrix4D::getTranslateMatrix(surfaceData[0], surfaceData[1], surfaceData[2]);

	// ���߲���
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();

	double *a = new double[10]();
	double xmin, xmax, ymin, ymax, zmin, zmax;
	if (modleData->getName() == PARABOLOID)  // ������Ϊһ���������
	{
		float temp = -4 * surfaceData[8];
		a[0] = 1; a[1] = 1; a[8] = temp;
		double radius = surfaceData[7];
		xmin = -radius * 2; xmax = radius * 2; ymin = -radius * 2;
		ymax = radius * 2; zmin = 1.0f / temp; zmax = -radius / temp;
	}
	int cout = 0;// ������
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			// ����������ϵת��ģ�͵��������ϵ
			Vector3 temp_org = R_translateMatrix*R_rotatMatrix*tempOrg[i][j];
			Vector3 temp_n = R_rotatMatrix*tempN[i][j]; // ����ֻ������ת
			// ������䷨�ߺͽ���
			Vector3  interPoint;
			Reflect_Ray(a, temp_n, temp_org, interPoint, xmin, xmax, ymin,
				ymax, zmin, zmax);

			// ��ģ�͵��������ϵת����������ϵ
			interPoint = translateMatrix * rotatMatrix * interPoint;
			tempN[i][j] = rotatMatrix * temp_n; // ���·���

			// ���õ��ĵ㱣����points�в����ü��ι�ϵ��line��
			points->InsertNextPoint(tempOrg[i][j].x, tempOrg[i][j].y, tempOrg[i][j].z);
			points->InsertNextPoint(interPoint.x, interPoint.y, interPoint.z);
			p1->GetPointIds()->SetId(0, cout++);
			p1->GetPointIds()->SetId(1, cout++);
			pLineCell->InsertNextCell(p1);

			// �������
			tempOrg[i][j] = interPoint;
		}

	// ����actor
	vtkSmartPointer<vtkPolyData>pointsData = vtkSmartPointer<vtkPolyData>::New();
	pointsData->SetPoints(points); //�������ģ���еļ������ݣ��㼯  
	pointsData->SetLines(pLineCell);

	vtkSmartPointer<vtkPolyDataMapper> pointMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	pointMapper->SetInputData(pointsData);
	pointMapper->Update();

	//vtkActor* actor = vtkActor::New(); // ����delete
	actor->SetMapper(pointMapper);

	actor->GetProperty()->SetColor(1, 0, 0);
	// ������õ�actor������list��
	actors.push_back(actor);
}

void RaySource::setSurface(vtkSmartPointer<vtkPolyData> polyData,
	vtkActor* actor)
{
	int NodesNum = polyData->GetNumberOfPoints();
	int EleNum = polyData->GetNumberOfCells();

	Element *EleData;  // ����.dat�ڵ�Ԫ������
	Vector3 *NodesXYZ; // ����.dat�ڵĽڵ�����
	Vector3 *PointNormal;

	NodesXYZ = new Vector3[NodesNum];
	PointNormal = new Vector3[NodesNum];
	EleData = new Element[EleNum];

	for (int i = 0; i < NodesNum; i++)
	{
		double * p = polyData->GetPoint(i);
		NodesXYZ[i].x = p[0];
		NodesXYZ[i].y = p[1];
		NodesXYZ[i].z = p[2];
		PointNormal[i].set(0, 0, 0);
	}

	for (int i = 0; i < EleNum; i++)
	{
		vtkIdList * p = polyData->GetCell(i)->GetPointIds();
		EleData[i].N1 = p->GetId(0);
		EleData[i].N2 = p->GetId(1);
		EleData[i].N3 = p->GetId(2);
	}
	for (int i = 0; i < EleNum; i++)
	{
		Vector3 tempa = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N2];
		Vector3 tempb = NodesXYZ[EleData[i].N1] - NodesXYZ[EleData[i].N3];
		//Vector3 tempb = NodesXYZ[EleData[i].N3] - NodesXYZ[EleData[i].N1];
		Vector3 n_light = tempa.Cross(tempb);  //������
		n_light.Normalization();
		PointNormal[EleData[i].N1] = PointNormal[EleData[i].N1] + n_light;
		PointNormal[EleData[i].N2] = PointNormal[EleData[i].N2] + n_light;
		PointNormal[EleData[i].N3] = PointNormal[EleData[i].N3] + n_light;
	}

	for (int i = 0; i < NodesNum; i++)
	{
		PointNormal[i].Normalization();
	}

	// ���߲���
	vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell = vtkSmartPointer<vtkCellArray>::New();
	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();
	int cout = 0;// ������
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			for (int k = 0; k < EleNum; k++)  //���뷴����Ľ���
			{
				Vector3  interPoint;
				double t;
				if (IsIntersect(tempOrg[i][j], tempN[i][j], NodesXYZ[EleData[k].N1],
					NodesXYZ[EleData[k].N2], NodesXYZ[EleData[k].N3], interPoint, t))
				{
					if (t > 0)
					{
						// ����						
						double a = CalDistance(NodesXYZ[EleData[k].N1], interPoint);
						double b = CalDistance(NodesXYZ[EleData[k].N2], interPoint);
						double c = CalDistance(NodesXYZ[EleData[k].N3], interPoint);
						a *= a; b *= b; c *= c;
						double sum = 1 / (a*b + b*c + a*c);
						Vector3 n_light = (PointNormal[EleData[k].N1] * b*c + PointNormal[EleData[k].N2] * a*c +
							PointNormal[EleData[k].N3] * b*a) * sum;

						n_light.Normalization();

						tempN[i][j] = ReflectLight(tempN[i][j], n_light);   // �������

					}
				}
				else
					interPoint = tempOrg[i][j];

				
				// ���õ��ĵ㱣����points�в����ü��ι�ϵ��line��
				points->InsertNextPoint(tempOrg[i][j].x, tempOrg[i][j].y, tempOrg[i][j].z);
				points->InsertNextPoint(interPoint.x, interPoint.y, interPoint.z);
				p1->GetPointIds()->SetId(0, cout++);
				p1->GetPointIds()->SetId(1, cout++);
				pLineCell->InsertNextCell(p1);

				// �������
				tempOrg[i][j] = interPoint;
				//
			}
		}

	// ����actor
	vtkSmartPointer<vtkPolyData>pointsData = vtkSmartPointer<vtkPolyData>::New();
	pointsData->SetPoints(points); //�������ģ���еļ������ݣ��㼯  
	pointsData->SetLines(pLineCell);

	vtkSmartPointer<vtkPolyDataMapper> pointMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	pointMapper->SetInputData(pointsData);
	pointMapper->Update();

	//vtkActor* actor = vtkActor::New(); // ����delete
	actor->SetMapper(pointMapper);

	actor->GetProperty()->SetColor(1, 0, 0);
	// ������õ�actor������list��
	actors.push_back(actor);


	delete[] NodesXYZ; 
	delete[] PointNormal;
	delete[] EleData;
}

void RaySource::calRayActor()
{
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	vtkSmartPointer<vtkCellArray> pLineCell =
		vtkSmartPointer<vtkCellArray>::New();

	vtkSmartPointer<vtkLine> p1 = vtkSmartPointer<vtkLine>::New();
	int cout = 0;
	for (int i = 0; i < N; i++)
		for (int j = 0; j < M; j++)
		{
			points->InsertNextPoint(tempOrg[i][j].x, tempOrg[i][j].y, tempOrg[i][j].z);
			points->InsertNextPoint(tempOrg[i][j].x + tempN[i][j].x * data[11],
				tempOrg[i][j].y + tempN[i][j].y * data[11],
				tempOrg[i][j].z + tempN[i][j].z * data[11]);
			p1->GetPointIds()->SetId(0, cout++);
			p1->GetPointIds()->SetId(1, cout++);
			pLineCell->InsertNextCell(p1);
		}

	vtkSmartPointer<vtkPolyData>pointsData = vtkSmartPointer<vtkPolyData>::New();
	pointsData->SetPoints(points); //�������ģ���еļ������ݣ��㼯  
	pointsData->SetLines(pLineCell);

	vtkSmartPointer<vtkPolyDataMapper> pointMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	pointMapper->SetInputData(pointsData);
	pointMapper->Update();

	if(RayActor == NULL)
		RayActor = vtkSmartPointer<vtkActor>::New();
	RayActor->SetMapper(pointMapper);

	RayActor->GetProperty()->SetColor(1, 0, 0);
}

vtkSmartPointer<vtkActor> RaySource::getRayActor()
{
	return RayActor;
}

list<vtkActor*> RaySource::getActors()
{
	return actors;
}

void RaySource::setSelSurface(list<Surface*> t)
{
	SelSurface.clear();
	clearActor();
	list<Surface*>::iterator it;
	for (it = t.begin(); it != t.end(); it++)
	{
		SelSurface.push_back(*it);
		vtkActor* actor = vtkActor::New();
		actors.push_back(actor);
	}


}

void RaySource::updateData()
{
	updateVarible();
	calRayTrace();
	copyPlane();
	list<Surface*>::iterator it;
	list<vtkActor*>::iterator itactor;

	for (it = SelSurface.begin(), itactor = actors.begin();
		it != SelSurface.end(); it++, itactor++)
	{
		//(*it)->getName()
		if ((*it)->getName() == PARABOLOID)
		{
			setSurface(*it, *itactor);
		}
		else
		{
			setSurface((*it)->getPolyData(), *itactor);
		}
	}

	calRayActor();
}

void RaySource::clearActor()
{
	//for (auto x : actors) // ��֪��ʲô����
	//	x->Delete();
	actors.clear();
}

void RaySource::clearVector3()
{
	if (n != NULL)
	{
		Free_2D(n);
		Free_2D(org);
	}
	n = NULL;
	org = NULL;
}

void RaySource::setIsShow(bool ok)
{
	isShow = ok;
	if (!ok)
	{
		RayActor->GetProperty()->SetOpacity(0);
		for (auto x : actors)
			x->GetProperty()->SetOpacity(0);
	}
	else
	{
		RayActor->GetProperty()->SetOpacity(1);
		for (auto x : actors)
			x->GetProperty()->SetOpacity(1);
	}
}

