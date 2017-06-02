#include "..\include\ApertureField.h"
#include "Tools/Constant_Var.h"

ApertureField::ApertureField(int _unit)
{
	Name = APERTURE;
	factor = 1;
	isSource = true;
	factor = _unit;
}

ApertureField::~ApertureField()
{
}

void ApertureField::setUnit(double _unit)
{
	factor = _unit;
}

void ApertureField::updateData()
{
	updateVarible();
	data[12] = data[7] / (N_width - 1);
	setDs(data[12]);
	if (is3D)
		calActor3D();
	else
		calActor();
}

void ApertureField::setFileAddress(const string & file)
{
	fileAddress = file;
}

string ApertureField::getFileAddress() const
{
	return fileAddress;
}

void ApertureField::readExEy()
{
	allocateMemory();
	ifstream file(fileAddress);
	int begin = int(data[11]) - 1;
	string temp;
	while (begin--)
		getline(file, temp);
	for (int j = 0; j < M_depth; j++)
	{
		for (int i = 0; i < N_width; i++)
		{
			double a1, p1, a2, p2, a3, p3;
			double temp;
			file >> temp >> temp >> temp >> temp;
			file >> a1 >> p1 >> a2 >> p2 >> a3 >> p3;
			/*Ex[i][j] = complex<double>(a1, p1);
			Ey[i][j] = complex<double>(a2, p2);*/
			Ex[i][j] = complex<double>(a1 * cos((p1 + data[10]) / 180 * Pi),
			a1 * sin((p1 + data[10]) / 180 * Pi));
			Ey[i][j] = complex<double>(a2 * cos((p2 + data[10]) / 180 * Pi),
			a2 * sin((p2 + data[10]) / 180 * Pi));
		}
	}
	file.close();
}
