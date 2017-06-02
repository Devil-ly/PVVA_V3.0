#include "qt/include/CalculateThread.h"

void CalculateThread::run()
{
	double Mesh;
	if (meshsize == 0)
		Mesh = 0.005;
	else if (meshsize == 2)
		Mesh = 0.02;
	else
		Mesh = 0.01;
	//workField->getActor3D();
	// **********开始计算***********
	PVVA pvva;
	// 设置单位
	pvva.setUnit(Unit_factor);
	// 设置频率
	pvva.setFre(fre);
	// 读入源并分配内存
	pvva.setSource(fieldSource);
	for (int i = 0; i < Num; i++)
	{
		if (calmodel == 1 && nameSel[i] == PARABOLOID)// 解析解优先
		{
			pvva.setAnalytic_Modle((*SurfaceData)[nameSel[i]][indexSel[i]]);
			pvva.Analytic_CalZ0Theta();
			pvva.Analytic_Reflect();
		}	
		else
		{
			(*SurfaceData)[nameSel[i]][indexSel[i]]->calPolyData(Mesh);
			pvva.setModle((*SurfaceData)[nameSel[i]][indexSel[i]]->getPolyData());
			pvva.CalZ0Theta();
			pvva.Reflect();
		}			
		pvva.InterVal();
	}
	pvva.Result(dis);
	Field *tempField = new Field;
	pvva.getField(tempField);
	emit sendField(tempField);
	delete[] nameSel;
	delete[] indexSel;
}


