#pragma once
#include <vector>
#include <list>
#include "Tools/GraphTrans.h"

class VaribleBase
{
public:
	void addVarible(int, const std::string&);
	void deleteVarible(int);
	void clearVarible();
	std::list<std::pair<int, std::string>> getVarible() const;
	virtual void updateData() = 0;

	// set and get GraphTrans
	void setGraphTrans(const GraphTrans & graphtrans);
	GraphTrans getGraphTrans() const;

	// set and get data 
	// 调用时先调用setData再调用setGraphTrans
	// setData会传入data的size
	void setData(std::vector<double> &_data);
	std::vector<double> getData() const;

	// 是否显示
	virtual void setIsShow(bool) = 0;
	bool getIsShow() const;

protected:
	void updateVarible();
	// 数据信息 0~6为旋转和平移信息
	std::vector<double> data;

	// 保存参数化信息，第一个int表示为data内的哪一个数据
	// 第二个int* 指向参数的内存
	std::list<std::pair<int, std::string>> varible;

	bool isShow;  // 是否透明
};