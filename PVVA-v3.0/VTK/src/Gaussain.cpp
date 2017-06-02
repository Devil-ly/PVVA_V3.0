#include "../include/Gaussain.h"
#include <Tools/Parameterization.h>

Gaussain::Gaussain(int _unit)
{
	Name = GAUSSIAN;
	factor = 1;
	isSource = true;
	factor = _unit;
	//data = nullptr;
}

Gaussain::Gaussain(const Gaussain & _Gaussain)
	:factor(_Gaussain.factor)
{
	data.resize(_Gaussain.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = _Gaussain.data[i];
}

Gaussain & Gaussain::operator=(const Gaussain & right)
{
	if (this == &right)
		return *this;

	factor = right.factor;
	varible.clear();
	for (auto x : right.varible)
		varible.push_back(x);
	data.resize(right.data.size());
	for (int i = 0; i < data.size(); i++)
		data[i] = right.data[i];

	content = right.content;
	N_width = right.N_width;
	M_depth = right.M_depth;
	ds = right.ds;
	isSource = right.isSource;
	actor = right.actor;
	actor3D = right.actor3D;
	isPhs = right.isPhs;
	isLinear = right.isLinear;
	is3D = right.is3D;
	allocateMemory();

	for (int i = 0; i < N_width; i++)
		for (int j = 0; j < M_depth; j++)
		{
			Ex[i][j] = right.Ex[i][j];
			Ey[i][j] = right.Ey[i][j];
			if (!isSource)
			{
				Ez[i][j] = right.Ez[i][j];
				Hx[i][j] = right.Hx[i][j];
				Hy[i][j] = right.Hy[i][j];
				Hz[i][j] = right.Hz[i][j];
			}
		}
	return *this;
}

Gaussain::~Gaussain()
{

}

void Gaussain::setUnit(double _unit)
{
	factor = _unit;
}

void Gaussain::updateData()
{
	updateVarible();

	N_width = int(data[7] / data[12]) + 1;
	M_depth = int(data[8] / data[12]) + 1;

	setDs(data[12]);
	CreateGaussianGaussain();
	if (is3D)
		calActor3D();
	else
		calActor();
}

void Gaussain::CreateGaussianGaussain()
{
	//生成Ex_In, Ey_IN;
	//double z0_back = z0; // 目标距离
	//double w0_back = w0; // 束腰
	//double fre_back = fre; // 频率
	allocateMemory();
	double z0 = data[9];
	double w0 = data[10];
	double fre = data[11];
	double ds = data[12];

	for (int i = 0; i < N_width; i++)
	{
		for (int j = 0; j < M_depth; j++)
		{
			double tempX = (i - (N_width - 1) / 2) * ds * factor;
			double tempY = (j - (M_depth - 1) / 2) * ds * factor;
			double tempr1;
			tempr1 = pow((tempX * tempX + tempY * tempY), 0.5);
			double temper, tempei;
			CreateGaussian(tempr1, w0 * factor, fre * 1e9, z0 * factor, temper, tempei);
			Ex[i][j] = complex<double>(0, 0);
			Ey[i][j] = complex<double>(temper, tempei);
		}
	}
	
}

void Gaussain::CreateGaussian(double r, double w0, double fre,
	double z0, double &Real, double &Imag) const {
	double lamda = C_Speed / fre;
	double k = 2 * Pi * fre / C_Speed;
	double w, Rdl, theta, r2, temp;

	w = w0 * pow((1 + pow(((lamda * z0) / (Pi * w0 * w0)), 2)), 0.5);
	Rdl = z0 / (z0 * z0 + pow((Pi * w0 * w0 / lamda), 2));
	theta = atan((lamda * z0) / (Pi * w0 * w0));
	r2 = r * r;
	temp = pow((2 / w / w / Pi), 0.5) * exp(-r2 / w / w);

	Real = temp * cos(theta - k * z0 - Pi * r2 / lamda * Rdl);
	Imag = temp * sin(theta - k * z0 - Pi * r2 / lamda * Rdl);
}