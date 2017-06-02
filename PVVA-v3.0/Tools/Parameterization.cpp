#include "Parameterization.h"

Parameterization::Parameterization()
{
}

double Parameterization::getVal(const string & para)
{
	string temp;
	stack<int> operators;
	stack<double> number;
	double tempNum;

	bool isNegative = true; // 判断时减号还是负数

	for (auto x : para)
	{
		if (x == '(')
		{
			isNegative = true;
			if (temp == "sin")
				operators.push(4);
			else if (temp == "cos")
				operators.push(5);
			else if (temp == "tan")
				operators.push(6);
			else if (temp == "sqrt")
				operators.push(7);
			else if (temp == "asin")
				operators.push(8);
			else if (temp == "acos")
				operators.push(9);
			else if (temp == "atan")
				operators.push(10);
			else if (temp == "log")
				operators.push(11);
			else if (temp == "exp")
				operators.push(12);
			else if (temp != "")
			{
				//异常 未能识别的操作符
				throw errorOperator(0);
			}
			temp.clear();
			operators.push(100); // 标志遇到下一个括号后弹出
		}
		else if (x == ')')
		{
			if (!temp.empty())
			{
				if (data.find(temp) == data.end())
				{
					tempNum = atof(temp.c_str());
					if(tempNum == 0 && temp != "0")
						throw errorOperator(2);
					number.push(tempNum);
				}
				else
					number.push(data[temp]);
				temp.clear();
				while (operators.top() != 100)
				{
					if (number.empty())
						throw errorOperator(3);
					double b = number.top();
					number.pop();
					int operators_temp = operators.top();
					operators.pop();
					double res;
					if (operators_temp > 3)
					{
						switch (operators_temp)
						{
						case 4:
							res = sin(b); break;
						case 5:
							res = cos(b); break;
						case 6:
							res = tan(b); break;
						case 7:
							if (b < 0)
								throw errorOperator(1);
							res = sqrt(b); break;
						case 8:
							res = asin(b); break;
						case 9:
							res = acos(b); break;
						case 10:
							res = atan(b); break;
						case 11:
							if (b <= 0)
								throw errorOperator(1);
							res = log(b); break;
						case 12:
							res = exp(b); break;
						default:
							break;
						}
					}
					else
					{
						if (number.empty())
							throw errorOperator(3);
						double a = number.top();
						number.pop();
						switch (operators_temp)
						{
						case 0:
							res = a + b; break;
						case 1:
							res = a - b; break;
						case 2:
							res = a*b; break;
						case 3:
							if (b == 0)
								throw errorOperator(1);
							res = a / b; break;
						default:
							break;
						}

					}
					number.push(res);
				}
				operators.pop();
			}

		}
		else if (x == '+')
		{
			if (temp[temp.length() - 1] == 'e')
			{
				temp += x;
				continue;
			}
			if (!temp.empty())
			{
				if (data.find(temp) == data.end())
				{
					tempNum = atof(temp.c_str());
					if (tempNum == 0 && temp != "0")
						throw errorOperator(2);
					number.push(tempNum);
				}
				else
					number.push(data[temp]);

				if (!operators.empty() && operators.top() > 1)
				{
					if (operators.top() == 2)
					{
						if (number.empty())
							throw errorOperator(3);
						int b = number.top();
						number.pop();
						if (number.empty())
							throw errorOperator(3);
						int a = number.top();
						number.pop();
						number.push(a*b);
						operators.pop();
					}
					else if (operators.top() == 3)
					{
						if (number.empty())
							throw errorOperator(3);
						int b = number.top();
						number.pop();
						if (number.empty())
							throw errorOperator(3);
						int a = number.top();
						number.pop();
						if (b == 0)
							throw errorOperator(1);
						number.push(a / b);
						operators.pop();
					}
				}
			}
			operators.push(0);
			temp.clear();
		}
		else if (x == '-' && !isNegative)
		{
			if (temp[temp.length() - 1] == 'e')
			{
				temp += x;
				continue;
			}
			if (!temp.empty())
			{
				if (data.find(temp) == data.end())
				{
					tempNum = atof(temp.c_str());
					if (tempNum == 0 && temp != "0")
						throw errorOperator(2);
					number.push(tempNum);
				}
				else
					number.push(data[temp]);
				if (!operators.empty() && operators.top() > 1)
				{
					if (operators.top() == 2)
					{
						if (number.empty())
							throw errorOperator(3);
						int b = number.top();
						number.pop();
						if (number.empty())
							throw errorOperator(3);
						int a = number.top();
						number.pop();
						number.push(a*b);
						operators.pop();
					}
					else if (operators.top() == 3)
					{
						if (number.empty())
							throw errorOperator(3);
						int b = number.top();
						number.pop();
						if (number.empty())
							throw errorOperator(3);
						int a = number.top();
						number.pop();
						if (b == 0)
							throw errorOperator(1);
						number.push(a / b);
						operators.pop();
					}
				}
			}
			operators.push(1);
			temp.clear();
		}
		else if (x == '*')
		{
			operators.push(2);
			if (!temp.empty())
			{
				if (data.find(temp) == data.end())
				{
					tempNum = atof(temp.c_str());
					if (tempNum == 0 && temp != "0")
						throw errorOperator(2);
					number.push(tempNum);
				}
				else
					number.push(data[temp]);
			}
			temp.clear();
		}
		else if (x == '/')
		{
			operators.push(3);
			if (!temp.empty())
			{
				if (data.find(temp) == data.end())
				{
					tempNum = atof(temp.c_str());
					if (tempNum == 0 && temp != "0")
						throw errorOperator(2);
					number.push(tempNum);
				}
				else
					number.push(data[temp]);
			}
			temp.clear();
		}
		else if (x != ' ')
		{
			temp += x;
			isNegative = false;
		}

	}
	if (!temp.empty())
	{
		if (data.find(temp) == data.end())
		{
			tempNum = atof(temp.c_str());
			if (tempNum == 0 && temp != "0")
				throw errorOperator(2);
			number.push(tempNum);
		}
		else
			number.push(data[temp]);
		temp.clear();
	}

	while (!operators.empty())
	{
		if(number.empty())
			throw errorOperator(3);
		double b = number.top();
		number.pop();
		int operators_temp = operators.top();
		operators.pop();
		double res;
		if (operators_temp > 3)
		{
			switch (operators_temp)
			{
			case 4:
				res = sin(b); break;
			case 5:
				res = cos(b); break;
			case 6:
				res = tan(b); break;
			case 7:
				if (b < 0)
					throw errorOperator(1);
				res = sqrt(b); break;
			case 8:
				res = asin(b); break;
			case 9:
				res = acos(b); break;
			case 10:
				res = atan(b); break;
			case 11:
				if (b <= 0)
					throw errorOperator(1);
				res = log(b); break;
			case 12:
				res = exp(b); break;
			default:
				break;
			}
		}
		else
		{
			if (number.empty())
				throw errorOperator(3);
			double a = number.top();
			number.pop();
			switch (operators_temp)
			{
			case 0:
				res = a + b; break;
			case 1:
				res = a - b; break;
			case 2:
				res = a*b; break;
			case 3:
				if (b == 0)
					throw errorOperator(1);
				res = a / b; break;
			default:
				break;
			}

		}
		number.push(res);
	}
	if (number.empty())
		throw errorOperator(3);
	return number.top();

}

void Parameterization::setPara(const string & par, double val)
{
	data[par] = val;
	if (dataList.find(par) != dataList.end())
	{
		for (auto x : dataList[par])
		{
			update(x, getVal(RawData[x])); // 更新所有与par相连的参数
		}
	}
}

void Parameterization::update(const string & par, double val)
{
	data[par] = val;
}

void Parameterization::addPara(const string & par, const string & val)
{
	// 首先检测val中是否有data内的参数
	string temp;
	for (auto x : val)
	{
		if (x == '*' || x == '/' || x == '-' || x == '+'
			|| x == '(' || x == ')')
		{
			if (x == '+' || x == '-')
				if (temp[temp.length() - 1] == 'e')
				{
					temp += x;
					continue;
				}
			// 加入变换表中 若引用的参数变换 自动变换
			if (data.find(temp) != data.end())
			{
				dataList[temp].insert(par);
				for (auto y : dataList)
				{
					//如果有父类 父类上也加上
					//避免递归查找
					if (y.second.find(temp) != y.second.end())
						dataList[y.first].insert(par);
				}
			}
			temp.clear();
		}
		else if (x != ' ')
			temp += x;
	}
	if (data.find(temp) != data.end())
	{
		dataList[temp].insert(par);
		for (auto y : dataList)
		{
			//如果有父类 父类上也加上
			if (y.second.find(temp) != y.second.end())
				dataList[y.first].insert(par);
		}
	}

	// 将surface*传给他的父类
	if (!surfaceList[par].empty())
	{
		for (auto x : dataList[par])
		{
			for (auto y : surfaceList[par])
				surfaceList[x].insert(y);
		}
	}
	setPara(par, getVal(val));
	RawData[par] = val;
}

void Parameterization::addParaSurface(const string & par, VaribleBase * _surface)
{
	string temp;
	for (auto x : par)
	{
		if (x == '*' || x == '/' || x == '-' || x == '+'
			|| x == '(' || x == ')')
		{
			if (x == '+' || x == '-')
				if (temp[temp.length() - 1] == 'e')
				{
					temp += x;
					continue;
				}
			// 加入变换表中 若引用的参数变换 自动变换
			if (data.find(temp) != data.end())
			{
				surfaceList[temp].insert(_surface);
				for (auto y : dataList)
				{
					//如果有父类 父类上也加上
					//避免递归查找
					if (y.second.find(temp) != y.second.end())
						surfaceList[y.first].insert(_surface);
				}
			}
			temp.clear();
		}
		else if (x != ' ')
			temp += x;
	}
	if (data.find(temp) != data.end())
	{
		surfaceList[temp].insert(_surface);
		for (auto y : dataList)
		{
			//如果有父类 父类上也加上
			//避免递归查找
			if (y.second.find(temp) != y.second.end())
				surfaceList[y.first].insert(_surface);
		}
	}
}

void Parameterization::addParaSurface(VaribleBase * a, VaribleBase * b)
{
	unordered_map<string, unordered_set<VaribleBase*>>::iterator it;
	for (it = surfaceList.begin(); it != surfaceList.end(); it++)
	{
		if (it->second.find(a) != it->second.end())
		{
			it->second.insert(b);
		}
	}
}

void Parameterization::deleteSurface(VaribleBase * _surface)
{
	//for (auto x : surfaceList)  auto x 是拷贝 不是引用
	unordered_map<string, unordered_set<VaribleBase*>>::iterator it;
	for(it = surfaceList.begin(); it != surfaceList.end(); it++)
	{
		it->second.erase(_surface);
	}
}

void Parameterization::swapSurface(VaribleBase * a, VaribleBase * b)
{
	deleteSurface(b);
	unordered_map<string, unordered_set<VaribleBase*>>::iterator it;
	for (it = surfaceList.begin(); it != surfaceList.end(); it++)
	{
		if (it->second.find(a) != it->second.end())
		{
			it->second.erase(a);
			it->second.insert(b);
		}
	}
}

void Parameterization::deletePara(const string & par)
{
	data.erase(par);
	dataList.erase(par);
	RawData.erase(par);
	unordered_map<string, unordered_set<string>>::iterator it;
	for (it = dataList.begin(); it != dataList.end(); it++)
	{
		//如果有父类 父类上也删除
		if (it->second.find(par) != it->second.end())
			it->second.erase(par);
	}
}

void Parameterization::clearAll()
{
	data.clear();
	dataList.clear();
	RawData.clear();
}

void Parameterization::updateSurface(const string & par)
{
	for (auto x : surfaceList[par])
	{
		x->updateData();
	}
}

unordered_map<string, double> Parameterization::create_map()
{
	unordered_map<string, double> m;
	m["Pi"] = 3.1415926;
	//m[3] = 4;
	//m[5] = 6;
	return m;
}


unordered_map<string, double> Parameterization::data
= Parameterization::create_map();