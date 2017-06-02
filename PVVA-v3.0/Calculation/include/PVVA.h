/*
* version 1.0 2016/11/29
* Designer liyun
* E-mail 465120258@qq.com
* Fuction��
* ���㷴����
* ���� ԴEx_In,Ey_In, Դλ�á��������� ������������������������
* 
*/
#pragma once
#ifndef PVVA_H
#define PVVA_H

#include <iostream>
#include <vector>
#include <cassert>
#include <fstream>
#include <vector>
#include <list>
#include <complex>

#include "Tools/Vector3.h"
#include "Tools/Vector2.h"
#include "Tools/GEMS_Memory.h"
#include "Tools/Constant_Var.h"
#include "calculation/include/Calculation.h"

#include "VTK/include/Field.h"

#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <VTK/include/Surface.h>

//#include "openGL/include/GraphTrans.h"
//#include "openGL/include/Field.h"
//#include "openGL/include/Surface.h"

// ���ڱ���Ԫ�ص�3���ڵ�
struct Element
{
	int N1, N2, N3;
};
using namespace std;

class PVVA
{
public:
	PVVA();
	~PVVA();
	
	void Initialization(); // ���ݳ�ʼ��
	
	void setUnit(double factor); // ���õ�λ

	void ReadDat(string ProjName);  //��.dat����

	void setSource(const Field* _field); // ����Դ

	// �õ�Դ���е������Լ���������
	void getSourcePoint(Vector3 & interPoint, Vector3 & n_Point) const;
	// Դ���е������Լ���������
	void setSourcePoint(const Vector3 & interPoint, const Vector3 & n_Point);

	static GraphTrans getSourceGraphTrans(const Vector3 & n_Point );

	void setFre(double _fre);

	void setModle(vtkSmartPointer<vtkPolyData> polyData); // ���������ʷ�����
	void setAnalytic_Modle(Surface * modeldata); // ���������ʷ�����


	bool CheckModle(Vector3 & interPoint, Vector3 & n_Point); // check�Ƿ��ܼ���

	void getField(Field * _field); // ���ؽ��

	void CalZ0Theta(); // ����z0 �� theta
	void Analytic_CalZ0Theta();

	void CalPlane(double dis); //����ƽ�洫�� ������ƽ��λ��

	void Reflect(); // ���㷴�� �ı���Exyz_R����λ�ͼ��� �����˷�������
	void Analytic_Reflect(); // �������� ���㷴�� �ı���Exyz_R����λ�ͼ��� �����˷�������
	
	void CalAmplitude(); // ���㷴��ǰ���ͶӰ��� ����Exyz_R���ȱ任

	void InterVal(); // �Է�������Ĳ����в�ֵ��ʹ�������׼������

	void Result(double dis = 1.0); // �õ������

	void Poynting(); //���������Ӧ͢ʸ��

	void getPlane(Vector3 ** &_org, Vector3 ** &_n) const; // �õ�ƽ���͵�ķ���
		
private:

	void AllocateMemory();  //�����ڴ�

	void DatePretreat();  //.dat����Ԥ���� ������Դ�����꣩
			
	void updateSource_n(Vector3 new_n); // ����Դ�ķ������Լ���ת��Ϣ

	/************************************************
	* ����ģ�͵ĵ㷨����������PointNormal�� 
	* ��ʼ������0����setModle������
	**************************************************/
	void calPointNormal();  

	/************************************************
	* ����ģ�͵�����ĳ��interPoint�ķ�����������
	* ���ݾ��뷴�ȣ��ɵ㷨�����ĵó�
	**************************************************/
	Vector3 calSurfaceNormal(const Element &_EleData, Vector3 interPoint);

	//�����Ĺ������ A * B(����)
	complex<double> ConjugateMul(const complex<double> &A, const complex<double> &B) const;
	//��������� A * B
	complex<double> ComMul(const complex<double> &A, const complex<double> &B) const;

	void Split(const string& src, const string& separator, vector<string>& dest);//�ָ�����

	void JumpLine(int N, ifstream& infile);//����

	double CalDistance(const Vector3 &a, const Vector3 &b) const;

	/************************************************
	* ֱ�����������ཻ��� ����false or true
	* orig������� dir ���߷��� v0,v1,v2������������ 
	* intersection ���� ���޽���intersection�����޸�
	**************************************************/
	bool IsIntersect(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0, 
		const Vector3 &v1, const Vector3 &v2, Vector3 &intersection, double &t);

	/************************************************
	* ֱ����ƽ���ཻ��� origֱ����� dir ���߷��� 
	* ����t t>0 �ཻ��ֱ��ǰ�� t<0 �ཻ��ֱ�ߺ�
	* ƽ��Ϊ�㷨ʽ n*(P-P0) = 0
	* P0Ϊƽ���ϵ�һ�� nΪƽ�淨��
	***************************************************/
	double IntersectPlane(const Vector3 &orig, const Vector3 &dir, const Vector3 &Plane_org,
		const Vector3 &Plane_n, Vector3 &intersection);

	double IntersectPoint(const Vector3 &orig, const Vector3 &dir, const Vector3 &v0,
		const Vector3 &E1, const Vector3 &E2, Vector3 &intersection);

	//���㷴����� ������������ ������ ���ط�������
	Vector3 ReflectLight(Vector3 a, Vector3 n);

	// ���㷴��糡 ���뷨���� ����糡ExEyEz 
	void CalReflectExyz(const Vector3 &n, const complex<double> &Ex, const complex<double> &Ey,
		const complex<double> &Ez, complex<double> &Ex_out, complex<double> &Ey_out, complex<double> &Ez_out);

	// �����ĸ���Χ�ǵ����  ��Ч�ڼ���������ABC �� ������ADC�������
	double CalSquare(const Vector3 &A, const Vector3 &B, const Vector3 &C, const Vector3 &D) const;
	double CalSquare(const Vector3 &A, const Vector3 &B, const Vector3 &C) const;
	
	// �����������������潻�� ����ֱ�ߵ����ͷ����� �������
	bool Analytic_Paraboloid_InterPoint(const Vector3 &O, const Vector3 &n, double Focus, double r,
		double &t, Vector3 & InterPoint);

	// ��ֵ�õġ����ж��������߶��Ƿ��ཻ
	bool InterVal_IsInsect(const Vector2 &A, const Vector2 &B, const Vector2 &C, const Vector2 &D);

	// ���ɸ�˹����TEST��
	void CreateGaussian(double r, double z, double w0, double f, double &Real, double &Imag);

private:
	int N, M; // ������εĳ��� N*M
	double f; // Ƶ�� Ĭ�� 10.65e9
	double lamda; // ����
	double k;
	double theta; // ���������Դ�����z����ļн�
	double ds;
	double w0; // ����
	double z0; //Ŀ�����

	double unit_factor; // ����ģ�͵ĵ�λת��Ϊ�׵�ϵ��

	//ģ�Ͳ��� (����ʽ��)
	//vector<float> ModlePara; // ģ�Ͳ���
	//GraphTrans ModleGraphTrans; // ��ת����
	//int ModleIndex;

	GraphTrans SourceGraphTrans;// Դ����ת����

	complex<double> ** Px, ** Py, ** Pz;	// ��Ӧ͢ʸ�� 
	complex<double> ** Ex1, ** Ey1, ** Ez1;	// ����calculation�����ĵ��ĵ�ų�
	complex<double> ** Hx1, ** Hy1, ** Hz1;	// 

	complex<double> ** Ex_In, ** Ey_In;	// Դ 
	
	complex<double> ** Ex_R, ** Ey_R, ** Ez_R;	// ������������ĵ��ĵ糡
	complex<double> ** Ex_R_Inter, ** Ey_R_Inter, ** Ez_R_Inter;	// ��ֵ��ĵ糡


	string ProjName;   // ���ݵĵ�ַ
	int NodesNum, EleNum;  // .dat��Ԫ�غͽڵ�ĸ���
	Element *EleData;  // ����.dat�ڵ�Ԫ������
	Vector3 *NodesXYZ; // ����.dat�ڵĽڵ�����
	Vector3 *PointNormal; // ��������ĵ㷨����

	Vector3 Org_Source, n_Source; //Org_Sourceƽ���е� n_Sourceƽ�淨����
	double theta_Source;   // Դ��-Z��ļн�

	Vector3 **Plane;  // ƽ���������꣨�������꣩
	Vector3 **n_Plane;  // ƽ�洫�������ķ�����
	Vector3 **R_Plane; // �����ĸ��������

	Vector3 **InterVal_Plane; // ��ֵƽ��
	struct coordinate_ij
	{
		int i, j;
	};
	vector<coordinate_ij> **Interpolation; //��ֵ�õ��м����
    // test�� ɾ
	Vector3 InterPoint_test; //
	Vector3 **Plane_org;   // ����������
	Vector3 **test_Plane;

	vector<double> Analytic_data; // �������õ���������0~6Ϊģ�͵�λ������
};


#endif
