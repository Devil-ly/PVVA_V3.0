#include "../include/Field.h"

#include <vtkImageData.h>
#include <vtkInformation.h>
#include <vtkLookupTable.h>
#include <vtkImageMapToColors.h>
#include <vtkTransform.h>
#include <vtkArrowSource.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkDelaunay2D.h>
#include <vtkPoints.h>
#include <vtkFloatArray.h>
#include <vtkPolyData.h>
#include <vtkPointData.h>
#include <vtkProperty.h>

#include <QProgressDialog>
#include <QString>
#include <QCoreApplication>
#include <QTimer>

Field::Field()
{
	init();
	Name = RESFIELD;
}

Field::Field(const Field & _field)
	:content(_field.content), N_width(_field.N_width),
	M_depth(_field.M_depth), ds(_field.ds), isSource(_field.isSource),
	actor(_field.actor), actor3D(_field.actor3D), isPhs(_field.isPhs),
	isLinear(_field.isLinear),is3D(_field.is3D)
{
	Ex = NULL;
	allocateMemory();
	data.resize(_field.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = _field.data[i];
	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = _field.Ex[i][j];
			Ey[i][j] = _field.Ey[i][j];
			//drawData[i][j] = _field.drawData[i][j];
			if (!isSource)
			{
				Ez[i][j] = _field.Ez[i][j];
				Hx[i][j] = _field.Hx[i][j];
				Hy[i][j] = _field.Hy[i][j];
				Hz[i][j] = _field.Hz[i][j];
			}
		}
}

Field & Field::operator=(const Field & _field)
{
	if (&_field == this)
		return *this;

	freeMemory();
	
	content = _field.content;
	N_width = _field.N_width;
	M_depth = _field.M_depth;
	ds = _field.ds;
	isSource = _field.isSource;
	actor = _field.actor;
	actor3D = _field.actor3D;
	isPhs = _field.isPhs;
	isLinear = _field.isLinear;
	is3D = _field.is3D;
	allocateMemory();
	data.clear();
	data.resize(_field.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = _field.data[i];
	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = _field.Ex[i][j];
			Ey[i][j] = _field.Ey[i][j];
			if (!isSource)
			{
				Ez[i][j] = _field.Ez[i][j];
				Hx[i][j] = _field.Hx[i][j];
				Hy[i][j] = _field.Hy[i][j];
				Hz[i][j] = _field.Hz[i][j];
			}
		}

	varible.clear();
	for (auto x : _field.varible)
		varible.push_back(x);

	return *this;
}

Field::~Field()
{
	freeMemory();

}

void Field::init()
{
	Ex = NULL; Ey = NULL; Ez = NULL;	// ������
	Hx = NULL; Hy = NULL; Hz = NULL;


	N_width = 0; M_depth = 0;

	ds = 0;
	isSource = false;
	isPhs = false;
	isLinear = true;
	is3D = false;
	content = 1;
	data.resize(9);
}

void Field::freeMemory()
{
	if (Ex != NULL)
	{
		Free_2D(Ex);
		Free_2D(Ey);

		//Free_2D(drawData);
		if (!isSource)
		{
			Free_2D(Ez);
			Free_2D(Hx);
			Free_2D(Hy);
			Free_2D(Hz);

			Ez = NULL;	// ������
			Hx = NULL;
			Hy = NULL;
			Hz = NULL;
		}

		Ex = NULL; Ey = NULL;
	}
}

void Field::allocateMemory()
{
	freeMemory();
	Ex = Allocate_2D(Ex, N_width, M_depth);
	Ey = Allocate_2D(Ey, N_width, M_depth);
	if (!isSource) // ����Դ�ŷ��������ĳ�����
	{
		Ez = Allocate_2D(Ez, N_width, M_depth);
		Hx = Allocate_2D(Hx, N_width, M_depth);
		Hy = Allocate_2D(Hy, N_width, M_depth);
		Hz = Allocate_2D(Hz, N_width, M_depth);
	}

}

void Field::setNM(int N, int M)
{
	N_width = N;
	M_depth = M;
}

void Field::getNM(int & N, int & M) const
{
	N = N_width;
	M = M_depth;
}

void Field::setPlane(const GraphTrans & _graphTransPara, double _ds)
{
	data[0] = _graphTransPara.getTrans_x();
	data[1] = _graphTransPara.getTrans_y();
	data[2] = _graphTransPara.getTrans_z();
	data[3] = _graphTransPara.getRotate_x();
	data[4] = _graphTransPara.getRotate_y();
	data[5] = _graphTransPara.getRotate_z();
	data[6] = _graphTransPara.getRotate_theta();
	ds = _ds;
	data[7] = N_width * _ds;
	data[8] = M_depth * _ds;

}

void Field::setField(complex<double>** _Ex, complex<double>** _Ey, complex<double>** _Ez, complex<double>** _Hx, complex<double>** _Hy, complex<double>** _Hz)
{
	allocateMemory();
	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = _Ex[i][j];
			Ey[i][j] = _Ey[i][j];
			Ez[i][j] = _Ez[i][j];
			Hx[i][j] = _Hx[i][j];
			Hy[i][j] = _Hy[i][j];
			Hz[i][j] = _Hz[i][j];
		}
}

complex<double>** Field::getEx() const
{
	return Ex;
}

complex<double>** Field::getEy() const
{
	return Ey;
}

void Field::setDs(double _Ds)
{
	ds = _Ds;
}

double Field::getDs() const
{
	return ds;
}

void Field::getSourcePara(GraphTrans & _graphTransPara, int & _N_width, int & _M_depth, double & _ds) const
{
	_graphTransPara.setGraphTransPar(data[0], data[1], data[2],
		data[3], data[4], data[5], data[6]);

	_N_width = N_width;
	_M_depth = M_depth;
	_ds = ds;
}

void Field::setShowPara(int _content, bool _isLinear, bool _isPhs)
{
	content = _content;
	isLinear = _isLinear;
	isPhs = _isPhs;
}

void Field::getShowPara(int & _content, bool & _isLinear, bool & _isPhs)
{
	_content = content;
	_isLinear = isLinear;
	_isPhs = isPhs;
}

void Field::set3D(bool _is3D)
{
	is3D = _is3D;
}

bool Field::get3D() const
{
	return is3D;
}

void Field::calActor3D()
{
	vtkFloatArray *scalars = vtkFloatArray::New();
	vtkSmartPointer<vtkPoints> points =
		vtkSmartPointer<vtkPoints>::New();
	complex<double> ** tempEH;
	switch (content)
	{
	case 0:
		tempEH = Ex;
		break;
	case 1:
		tempEH = Ey;
		break;
	case 2:
		tempEH = Ez;
		break;
	case 3:
		tempEH = Hx;
		break;
	case 4:
		tempEH = Hy;
		break;
	case 5:
		tempEH = Hz;
		break;
	default:
		break;
	}
	complex<double> temp;
	double max = 0;
	for (unsigned int x = 0; x < N_width; x++)
	{
		for (unsigned int y = 0; y < M_depth; y++)
		{
			complex<double> temp;
			temp = tempEH[x][y];
			double tempD = sqrt(temp.real() * temp.real() +
				temp.imag() * temp.imag());
			if (max < tempD)
				max = tempD;
		}
	}

	for (unsigned int x = 0; x < N_width; x++)
	{
		for (unsigned int y = 0; y < M_depth; y++)
		{
			complex<double> temp;
			temp = tempEH[x][y];
			double tempD = sqrt(temp.real() * temp.real() +
				temp.imag() * temp.imag());
			double tempD1 = tempD / max / 2 * data[7];
			tempD = (max - tempD) / max / 2;

			points->InsertNextPoint(x*ds - N_width*ds / 2,
				y*ds - M_depth*ds / 2, tempD1);
			scalars->InsertTuple1(x*N_width + y, tempD);
		}
	}

	vtkSmartPointer<vtkPolyData> polydata =
		vtkSmartPointer<vtkPolyData>::New();
	polydata->SetPoints(points);
	polydata->GetPointData()->SetScalars(scalars);

	double scalarsRange[2];
	scalars->GetRange(scalarsRange);
	scalars->Delete();

	vtkSmartPointer<vtkDelaunay2D> delaunay =
		vtkSmartPointer<vtkDelaunay2D>::New();
	delaunay->SetInputData(polydata);
	delaunay->Update();


	vtkSmartPointer<vtkLookupTable> colorTable =
		vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(scalarsRange);
	colorTable->Build();

	vtkSmartPointer<vtkPolyDataMapper> triangulatedMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	triangulatedMapper->SetInputData(delaunay->GetOutput());
	triangulatedMapper->SetLookupTable(colorTable);
	triangulatedMapper->SetScalarRange(scalarsRange);
	triangulatedMapper->Update();

	vtkSmartPointer<vtkTransform> transform =
		vtkSmartPointer<vtkTransform>::New();
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	//transform->Translate(-para[7] / 2, -para[8] / 2, 0);

	actor3D = vtkSmartPointer<vtkActor>::New();
	actor3D->SetMapper(triangulatedMapper);
	actor3D->SetUserTransform(transform);

}

void Field::calActor()
{
	auto info = vtkSmartPointer<vtkInformation>::New();
	vtkSmartPointer<vtkImageData> img = vtkSmartPointer<vtkImageData>::New();
	img->SetDimensions(N_width, M_depth, 1);
	//img->SetSpacing(0.005, 0.005, 1);
	img->SetSpacing(0.01*ds / 0.01, 0.01*ds / 0.01, 1);
	img->SetScalarType(VTK_DOUBLE, info);
	img->SetNumberOfScalarComponents(1, info);
	img->AllocateScalars(info);

	double *ptr = (double*)img->GetScalarPointer();
	double max = 0, min = 0;
	complex<double> ** tempEH;
	switch (content)
	{
	case 0:
		tempEH = Ex;
		break;
	case 1:
		tempEH = Ey;
		break;
	case 2:
		tempEH = Ez;
		break;
	case 3:
		tempEH = Hx;
		break;
	case 4:
		tempEH = Hy;
		break;
	case 5:
		tempEH = Hz;
		break;
	default:
		break;
	}
	complex<double> test = tempEH[27][27];
	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			double tempD;
			complex<double> temp;
			temp = tempEH[j][i];

			if (isPhs)
			{
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
			}
			else
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
			if (!isLinear)
				tempD = log(tempD);

			*ptr++ = tempD;
			if (max < tempD)
				max = tempD;
			if (min > tempD)
				min = tempD;
		}
	ptr = (double*)img->GetScalarPointer();
	if (!isPhs)
		for (int i = 0; i<N_width * M_depth * 1; i++, ptr++)
			*ptr = max - *ptr;

	vtkSmartPointer<vtkLookupTable> colorTable =
		vtkSmartPointer<vtkLookupTable>::New();
	colorTable->SetRange(min, max);

	colorTable->Build();

	vtkSmartPointer<vtkImageMapToColors> colorMap =
		vtkSmartPointer<vtkImageMapToColors>::New();
	colorMap->SetInputData(img);
	colorMap->SetLookupTable(colorTable);
	colorMap->Update();

	vtkSmartPointer<vtkTransform> transform =
		vtkSmartPointer<vtkTransform>::New();
	transform->Translate(data[0], data[1], data[2]);
	transform->RotateWXYZ(data[6], data[3], data[4], data[5]);
	transform->Translate(-data[7] / 2, -data[8] / 2, 0);
	actor = vtkSmartPointer<vtkImageActor>::New();
	actor->SetInputData(colorMap->GetOutput());
	actor->SetUserTransform(transform);
}

vtkSmartPointer<vtkImageActor> Field::getActor() const
{
	return actor;
}

vtkSmartPointer<vtkActor> Field::getActor3D() const
{
	return actor3D;
}

void Field::save(const std::string & fileName) const
{
	if (isSource)
	{
		ofstream outfile(fileName + "_ExEy.txt");
		outfile << "Ex   Ey   Ez" << endl;
		for (int i = 0; i < N_width; i++)
		{
			for (int j = 0; j < M_depth; j++)
			{
				complex<double> temp = Ex[i][j];
				double tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile << tempD << " ";

				temp = Ey[i][j];
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile << tempD << " ";
				outfile << "0 0" << "\n";
			}
		}
		outfile.close();

	}
	else 
	{
		ofstream outfile(fileName + "_ExEyEz.txt");
		outfile << "Ex   Ey   Ez" << endl;
		for (int i = 0; i < N_width; i++)
		{
			for (int j = 0; j < M_depth; j++)
			{
				//Ex
				complex<double> temp = Ex[i][j];
				double tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile << tempD << " ";

				//Ey
				temp = Ey[i][j];
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile << tempD << " ";

				//Ez
				temp = Ez[i][j];
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile << tempD << " ";

				outfile << "\n";
			}
		}
		outfile.close();

		ofstream outfile1(fileName + "_HxHyHz.txt");
		outfile1 << "Hx   Hy   Hz" << endl;
		for (int i = 0; i < N_width; i++)
		{
			for (int j = 0; j < M_depth; j++)
			{
				//Hx
				complex<double> temp = Hx[i][j];
				double tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile1 << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile1 << tempD << " ";

				//Hy
				temp = Hy[i][j];
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile1 << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile1 << tempD << " ";

				//Hz
				temp = Hz[i][j];
				tempD = pow((temp.real() * temp.real() +
					temp.imag() * temp.imag()), 0.5);
				outfile1 << tempD << " ";
				if (temp.real() != 0)
					tempD = atan(temp.imag() / temp.real());
				else
					tempD = 0;
				outfile1 << tempD << " ";

				outfile << "\n";
			}
		}
		outfile.close();
	}
}

void Field::updateData()
{
	if (is3D)
		calActor3D();
	else
		calActor();
}

void Field::setIsShow(bool ok)
{
	isShow = ok;
	if (!ok)
	{
		if (actor3D != NULL)
			actor3D->GetProperty()->SetOpacity(0);		
		actor->SetOpacity(0);
	}
	else
	{
		if (actor3D != NULL)
			actor3D->GetProperty()->SetOpacity(1);
		actor->SetOpacity(1);
	}
}
