#pragma once
/***************************************
����ģ������
data[0]~data[6] ����transfer��Ϣ
ģ����Ϣ��data[7]��
***************************************/

#ifndef MODLEDATA_H
#define MODLEDATA_H

#define PARABOLOID 0x00
#define LIGHTSOURCE 0x01
#define RECTANGLE 0x02
#define CYLINDER 0x03
#define BOOLEANOP 0x04
#define STLINPUT 0x05
#define SPHERE 0x06
#define ARROW 0x07 
#define REVERSECONE 0x08 // ���õ�cone
#define ARBITRARYSURFACE 0x09

//#define APERTUREFIELD 0x03
//#define GAUSSIAN 0x04

#include "Tools/GraphTrans.h"
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <string>
#include "Tools/Vector3.h"
#include "Calculation/include/RayTrace.h"

class ModleData: public RayTrace
{
public:
	ModleData();
	ModleData(int _name, int _index, int _dataSize);
	ModleData(const ModleData & _ModleData); // ��д���캯��
	~ModleData();

	ModleData& operator = (const ModleData & _ModleData);

	void setData(float * _data); // ����һ��Ԫ��
	void setNameFile(const string & file);
	void setGraphTrans(const GraphTrans & graphtrans);

	void setBoolean(int _name1, int _index1, int _name2, int _index2,
		ModleData *a, ModleData *b, int operation);

	void setName(int _name); 
	int getName() const;
	int getIndex() const;

	int getDataSize() const;
	float * getData() const;

	void setTransparent(bool is = false); // ����͸��
	bool getTransparent() const; // ����͸��
	
	string getNameFile() const;
	GraphTrans getGraphTrans() const;

	vtkSmartPointer<vtkActor> getActor();
	static vtkSmartPointer<vtkActor> getLineActor(const Vector3 & p0, const Vector3 & p1);
	static vtkSmartPointer<vtkActor> getRectangleActor(const GraphTrans & graphTrans, float w, float d);

	//�õ�ͼ�εļ������� dsΪ�ֱ��� �ڻ�ͼ�������СΪ0.05
	vtkSmartPointer<vtkPolyData> getPolyData(double ds = 0.05) const;
	
private:
	vtkSmartPointer<vtkPolyData> drawParaboloid(double ds) const;
	vtkSmartPointer<vtkPolyData> drawRectangle(double ds) const;
	vtkSmartPointer<vtkPolyData> drawCylinder(double ds) const;
	vtkSmartPointer<vtkPolyData> drawSphere(double ds) const;
	vtkSmartPointer<vtkPolyData> drawArrow() const;
	vtkSmartPointer<vtkPolyData> drawSTLInput() const;
	vtkSmartPointer<vtkPolyData> drawReverseCone() const;
	vtkSmartPointer<vtkPolyData> drawCustom(double ds) const;

	void transformModle(vtkSmartPointer<vtkPolyData> polyData);

	// �������� operation = 0 ��ʾ����
	vtkSmartPointer<vtkPolyData> boolean(ModleData *a, ModleData *b, int operation);

	int name;  // ģ������
	int index; // ģ�ͱ��

	//GraphTrans* graphTrans;

	int dataSize; // ���ݳ���
	float * data;
	string namefile;

	vtkSmartPointer<vtkActor> actor; //  ��ʾ����
	vtkSmartPointer<vtkPolyData> polyData;   // ��������

	bool isNew; // �����new����Ҫ����õ�actor

	bool isTransparent; // �Ƿ�͸��
};

#endif // !MODLEDATA_H