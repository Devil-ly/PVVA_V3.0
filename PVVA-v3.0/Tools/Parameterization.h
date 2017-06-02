#pragma once
/*=========================================================================

Program:   PVVA-V3.0
Module:    Parameterization.h

Copyright (c) LiYun
e-mail 46510258@qq.com

20170512
=========================================================================*/

/*=========================================================================

���岢ʵ����һ�����ַ���ת��Ϊ������ܼ������ѧ���㣬���ʽ�����ò�����ʾ
����R1 = 4;���Խ���Ƕ�� ���� R2 = R1*R1; Ƕ��ʱ��R1�ı䣬R2�Զ�����

=========================================================================*/

#include <unordered_map>
#include <string>
#include <stack>
#include <list>
#include <unordered_set>
#include <VTK/include/Surface.h>
#include "Tools/VaribleBase.h"
using namespace std;
class errorOperator
{
public:
	errorOperator(int _flag)
		:flag(_flag)
	{
	}

	int what() { return flag; }
private:

	/*************************
	* 0 Ϊδ��ʶ��Ĳ�����
	* 1 Ϊ����Ϊ0
	**************************/
	int flag;
};

class Parameterization
{
public:
	Parameterization();
	static double getVal(const string & para);
	
	void setPara(const string & par, double val);
	
	void update(const string & par, double val);

	void addPara(const string & par, const string & val);

	void addParaSurface(const string & par, VaribleBase* _surface);
	void addParaSurface(VaribleBase* a, VaribleBase* b);
	void deleteSurface(VaribleBase* _surface);
	void swapSurface(VaribleBase* a, VaribleBase* b);

	void deletePara(const string & par);

	void clearAll();
	void updateSurface(const string & par);
	
	static unordered_map<string, double> create_map();
	
private:
	static unordered_map<string, double> data;
	unordered_map<string, unordered_set<string>> dataList;
	unordered_map<string, string> RawData; // ԭʼ����û�н��д����
	unordered_map<string, unordered_set<VaribleBase*>> surfaceList; // �����������ݵı�
};

