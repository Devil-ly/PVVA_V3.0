#include "VaribleBase.h"
#include <Tools/Parameterization.h>

void VaribleBase::addVarible(int index, const std::string& val)
{
	// 如果已经存在则删除后再添加
	deleteVarible(index);
	varible.push_back(std::pair<int, std::string>(index, val));
}

void VaribleBase::deleteVarible(int index)
{
	std::list<std::pair<int, std::string>>::iterator it;
	for (it = varible.begin(); it != varible.end(); it++)
		if (it->first == index)
		{
			varible.erase(it);
			break;
		}
}

void VaribleBase::clearVarible()
{
	varible.clear();
}

std::list<std::pair<int, std::string>> VaribleBase::getVarible() const
{
	return varible;
}

void VaribleBase::setGraphTrans(const GraphTrans & graphtrans)
{
	data[0] = graphtrans.getTrans_x();
	data[1] = graphtrans.getTrans_y();
	data[2] = graphtrans.getTrans_z();
	data[3] = graphtrans.getRotate_x();
	data[4] = graphtrans.getRotate_y();
	data[5] = graphtrans.getRotate_z();
	data[6] = graphtrans.getRotate_theta();
}

GraphTrans VaribleBase::getGraphTrans() const
{
	GraphTrans graphTrans;
	graphTrans.setGraphTransPar(data[0], data[1], data[2],
		data[3], data[4], data[5], data[6]);
	return graphTrans;
}

void VaribleBase::setData(std::vector<double>& _data)
{
	data.resize(_data.size());
	for (int i = 7; i < _data.size(); i++)
		data[i] = _data[i];
}

std::vector<double> VaribleBase::getData() const
{
	vector<double> temp(data.size());
	for (int i = 0; i < data.size(); i++)
		temp[i] = data[i];
	return temp;
}

bool VaribleBase::getIsShow() const
{
	return isShow;
}

void VaribleBase::updateVarible()
{
	for (auto x : varible)
	{
		data[x.first] = Parameterization::getVal(x.second);
	}
}

