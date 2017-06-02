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
	// ����ʱ�ȵ���setData�ٵ���setGraphTrans
	// setData�ᴫ��data��size
	void setData(std::vector<double> &_data);
	std::vector<double> getData() const;

	// �Ƿ���ʾ
	virtual void setIsShow(bool) = 0;
	bool getIsShow() const;

protected:
	void updateVarible();
	// ������Ϣ 0~6Ϊ��ת��ƽ����Ϣ
	std::vector<double> data;

	// �����������Ϣ����һ��int��ʾΪdata�ڵ���һ������
	// �ڶ���int* ָ��������ڴ�
	std::list<std::pair<int, std::string>> varible;

	bool isShow;  // �Ƿ�͸��
};