#pragma once
/*=========================================================================

Program:   PVVA-V3.0
Module:    Surface.h

Copyright (c) LiYun
e-mail 46510258@qq.com

20170511
=========================================================================*/

/*=========================================================================
 
 这是个纯虚类，描述一个surface的公共属性

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

	// cal(计算) and get actor 
	vtkSmartPointer<vtkActor> getActor() const;
	virtual void calActor();

	// cal(计算) and get polyData 
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
	// 设置是否显示
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

	//保存每个模型的显示和剖分数据
	vtkSmartPointer<vtkActor> actor;
	vtkSmartPointer<vtkPolyData> polyData;

	bool isTransparent; // 是否显示
	vtkSmartPointer<vtkProperty> property;

	int Name; // 判断是什么曲面
};