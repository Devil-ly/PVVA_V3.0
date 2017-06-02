#pragma once
#ifndef APERTUREFIELD_H
#define APERTUREFIELD_H

#include "vtk/include/Field.h"
class ApertureField : public Field
{
public:
	ApertureField(int _unit);
	~ApertureField();

	void setUnit(double _unit);
	void updateData();
	void setFileAddress(const string & file);
	string getFileAddress() const;
	void readExEy();
private:

	string fileAddress; // ÎÄ¼þµØÖ·
	int factor;
};


#endif // !ApertureField_H
