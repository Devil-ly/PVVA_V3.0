#pragma once
/*=========================================================================

Program:   PVVA-V3.0
Module:    Parameterization.h

Copyright (c) LiYun
e-mail 46510258@qq.com

20170512
=========================================================================*/

/*=========================================================================

定义并实现了一个将字符串转换为计算机能计算的数学运算，表达式可以用参数表示
例：R1 = 4;可以进行嵌套 例： R2 = R1*R1; 嵌套时若R1改变，R2自动更新

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
	* 0 为未能识别的操作符
	* 1 为除数为0
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
	unordered_map<string, string> RawData; // 原始数据没有进行处理的
	unordered_map<string, unordered_set<VaribleBase*>> surfaceList; // 所有引用数据的表
};

