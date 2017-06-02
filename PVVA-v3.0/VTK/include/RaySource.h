#pragma once
#ifndef RAYSOURCE_H
#define RAYSOURCE_H

#include <vector>
#include <list>
#include <vtkActor.h>
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>

#include "Tools/GraphTrans.h"
#include "Tools/Vector3.h"
#include <Calculation/include/RayTrace.h>
#include "VTK/include/Surface.h"
#include "Tools/VaribleBase.h"

using namespace std;

class RaySource: public RayTrace, public VaribleBase
{
public:
	RaySource();
	~RaySource();
	RaySource & operator = (const RaySource & right);

	//void setGraphTrans(const GraphTrans & graphtrans);
	//GraphTrans getGraphTrans() const;

	//void setData(vector<double> _data);
	//vector<double> getData() const;

	void setType(int _type);
	int getType() const;

	void setNM(int _N, int _M);
	void setLength(float dis); // ���ó���

	void getPlane(Vector3 **&_org, Vector3 **&_n);
	void copyPlane(); // ��������ݷ������ѭ����
	void calRayTrace();
	void setSurface(Surface * modleData, vtkActor*); // ��������Ķ�������
	void setSurface(vtkSmartPointer<vtkPolyData> polyData, 
		vtkActor*); // ����������Դ

	void calRayActor(); // ����Ray
	vtkSmartPointer<vtkActor> getRayActor();
	list<vtkActor*> getActors();

	void setSelSurface(list<Surface*>);
	void updateData();

	void clearActor();
	void clearVector3(); // �������delete
	
	void setIsShow(bool);
private:

	/****************************************************************************                       
	*  0 ~ 6 λ����Ϣ 
	*  7 width    8 depth(���κ͵���ʱ)        7 angle    8 ������ (��Դʱ��                 
	*  9 widthGap 10 depthGap(���κ͵���ʱ)	   9 thetaGap 10 phiGap(��Դʱ)        
	*  11 length                               11 length                
	*****************************************************************************/
	//vector<double> data;
	int type;// 0 ���� 2 ��Դ 1 ����  3 Field ����

	Vector3 **org; // ��ʼ��
	Vector3 **n; // ��ʼ����

	//������̵���ʱ
	Vector3 **tempOrg; // ��ʼ��
	Vector3 **tempN; // ��ʼ����

	vtkSmartPointer<vtkActor> RayActor; 
	list<vtkActor*> actors; // ����׷�ٺ�Ĺ���
	int N, M;

	list<Surface*> SelSurface; // ����ѡ�е���

};

#endif