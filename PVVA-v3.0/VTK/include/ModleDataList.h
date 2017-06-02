#pragma once
/***************************************
��������ģ������
�ṩģ�����ݵ���ɾ�Ĳ鹦��
***************************************/

#ifndef MODLEDATALIST_H
#define MODLEDATALIST_H

#include <list>
#include "VTK/include/ModleData.h"
#include <vtkRenderer.h>
#include <vtkPolyData.h>
#include <vtkActor.h>

using namespace std;

class ModleDataList
{
public:
	ModleDataList();
	~ModleDataList();

	void pushModle(const ModleData &modleData); // ���һ��modle
	void modifyModle(const ModleData &modleData); // �޸�һ��modle
	void removeModle(int name, int index); // ɾ��һ��modle
	void removeModle_Actor(int name, int index, vtkSmartPointer<vtkRenderer> renderer);
	void findModle(int name, int index, ModleData &modleData) const;
	vtkSmartPointer<vtkPolyData> getPolyData(int name, int index, double ds = 0.05) const; // �õ�ģ�ͼ�������
	vtkSmartPointer<vtkActor> getActor(int name, int index);

	void draw(vtkSmartPointer<vtkRenderer> renderer);
	bool getTransparency(int name, int index) const; //�õ�ģ�͵�͸����
	void setTransparency(int name, int index, bool is);

private:
	list<ModleData> data;
};
#endif // !MODLEDATALIST_H
