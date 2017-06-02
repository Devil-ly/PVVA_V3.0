#ifndef CALCULATETHREAD_H
#define CALCULATETHREAD_H

#include <QThread>

#include "Calculation/include/PVVA.h"
#include "VTK/include/Surface.h"
#include <unordered_map>

class CalculateThread : public QThread
{
	Q_OBJECT

public:
	CalculateThread(double _fre, double _Unit_factor, int *_nameSel,int *_indexSel,
		int _Num, unordered_map<int, unordered_map<int, Surface*>> *_modleDataList, 
		Field* _fieldSource, double _dis,
		double _width, double _depth, int _meshsize, int _calmodel)
	{
		fre = _fre;
		Unit_factor = _Unit_factor;
		nameSel = _nameSel;
		indexSel = _indexSel;
		SurfaceData = _modleDataList;
		Num = _Num;
		fieldSource = _fieldSource;
		dis = _dis;
		width = _width;
		depth = _depth;
		meshsize = _meshsize;
		calmodel = _calmodel;
	}
	//~CalculateThread();
signals:
	void sendField(Field* field);
protected:
	void run();

private:
	int *nameSel;
	int *indexSel;
	unordered_map<int, unordered_map<int, Surface*>> *SurfaceData;
	int Num;
	Field* fieldSource;
	double fre;
	double Unit_factor;
	double dis; //��󴫲�����
	double width; // ���¶����ĳ���
	double depth;
	int meshsize;// �ʷ���Ϣ 
	int calmodel;// ����ģʽ��Ϣ �ʷ����Ȼ��ǽ���������
	
};

#endif // CalculateThread_H