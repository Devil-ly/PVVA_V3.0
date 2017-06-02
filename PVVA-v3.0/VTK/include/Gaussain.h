#pragma once
#ifndef GAUSSIAN_H
#define GAUSSIAN_H

#include <string>
#include <complex>
#include <vector>
#include <QString>
#include "Tools/GEMS_Memory.h"
#include "Tools/Constant_Var.h"
#include "Tools/GraphTrans.h"
#include "vtk/include/Field.h"


using namespace std;

class Gaussain : public Field
{
public:
	Gaussain(int);
	Gaussain(const Gaussain & _Gaussain);
	//Gaussain(int type, float * _data);
	Gaussain & operator =(const Gaussain & right);
	~Gaussain();

	void setUnit(double _unit);

	void updateData();
private:

	void CreateGaussianGaussain();
	void Gaussain::CreateGaussian(double r, double w0, double fre, double z0,
		double &Real, double &Imag) const;

	//double * data; // 保存数据信息
	/****************************************************************************
	*     APERTURE                         GAUSSIAN
	*  0 ~ 6 位置信息                    0 ~ 6 位置信息
	*  7 width 8 depth                   7 width 8 depth
	*  9 scale 10 phase			         9 z0   10 w0
	*  11 StartNum  12 Ds                11 fre   12 Ds
	*  13 datadef       
    *****************************************************************************/
	//vector<double> data;
	
	double factor;
	
};




#endif