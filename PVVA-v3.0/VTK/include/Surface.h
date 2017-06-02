#pragma once
/*=========================================================================

Program:   PVVA-V3.0
Module:    Surface.h

Copyright (c) LiYun
e-mail 46510258@qq.com

20170511
=========================================================================*/

/*=========================================================================
 
 ���Ǹ������࣬����һ��surface�Ĺ�������

=========================================================================*/


#include <vector>
#include <list>

#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkProperty.h>
#include <Tools/ModelID.h>
#include <Tools/VaribleBase.h>

class Surface: public VaribleBase
{
public:
	Surface();
	virtual ~Surface() = default;
	virtual Surface& operator = (const Surface & right);
	//virtual void draw() = 0;

	// cal(����) and get actor 
	vtkSmartPointer<vtkActor> getActor() const;
	virtual void calActor();

	// cal(����) and get polyData 
	vtkSmartPointer<vtkPolyData> getPolyData() const;
	virtual void calPolyData(double ds = 0) = 0;

	// set
	virtual void setBoolean(Surface* a, Surface* b);
	/*
	void addVarible(int, const std::string&);
	void deleteVarible(int);
	void clearVarible();
	std::list<std::pair<int, std::string>> getVarible() const;
    */
	// �����Ƿ���ʾ
	void setIsShow(bool);
	void setIsTransparent(bool);
	bool getIsTransparent() const;
	vtkSmartPointer<vtkProperty> getProperty() const;
	void updateProperty();

	virtual void updateData() = 0;

	int getName() const;

	static vtkSmartPointer<vtkActor> getLineActor(const Vector3 & p0, 
		const Vector3 & p1);
	static vtkSmartPointer<vtkActor> getRectangleActor(const GraphTrans & 
		graphTrans, float w, float d);

protected:

	//����ÿ��ģ�͵���ʾ���ʷ�����
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkPolyData> polyData;

	bool isTransparent; // �Ƿ���ʾ
	vtkSmartPointer<vtkProperty> property;

	int Name; // �ж���ʲô����
};