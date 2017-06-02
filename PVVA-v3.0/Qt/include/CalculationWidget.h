#ifndef CALCULATIONWIDGET_H
#define CALCULATIONWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QWidget>

#include "Qt/include/GraphTransWidget.h"


class CalculationWidget : public QDialog
{
	Q_OBJECT

public:
	CalculationWidget(QWidget *parent = 0);
	~CalculationWidget();

	void addAllSurface(QStringList _allSurfaceName);
	void addAllSources(QStringList _allSourcesName);

	void getSelSurfaces(int & _NumSel, int *& index); // �õ���Ҫ���㷴�����
	int getSelSources() const; // �õ���Ҫ���㷴�����
	bool getResRectangle(bool & Isdefualt, double * &data);  // �õ�����������
	void setResRectangle(const double &data1, const double &data2);
	int getMeshSize() const; //�õ��ʷ��ܶ���Ϣ
	int getCalMod() const; //�õ�����ģʽ��Ϣ

	double getFre();

signals:
	/*************************
	* 0: ��ʾclose 
	* 1: ��ʾOK
	* 2: ��ʾCheck
	* 3: ��ʾcancel
	**************************/
	void sendData(int);

private slots:
    void buttonOk();
	void buttonCheck();
    void buttonCancel();
	void buttonSave();

	void selectLineEditChange(const QString & text);
	void monitorNumLineEditChange(const QString & text);
	void resultComboboxChange(int Num);
private:

	//page1
	QTabWidget * tabWidget;

	QGroupBox * paraGroupBox;
	QLabel * freLabel;
	QLineEdit * freLineEdit;
	QLabel *  sourceLabel;
	QComboBox * sourceCombobox;
	QLabel *  calModLabel; // ����ģʽ
	QComboBox * calModCombobox;
	QLabel *  MeshLabel;
	QComboBox * MeshCombobox;

	QGroupBox * surfaceNumGroupBox;
	QLabel *  surfaceLabel;
	QLineEdit * surfaceLineEdit;

	QGroupBox * selectGroupBox;
	QLabel *  selectLabel;
	QComboBox * selectCombobox;
	QGridLayout * selectlayout;

	QGroupBox * resultGroupBox;
	QLabel *  resultLabel;
	QComboBox * resultCombobox;
	QLabel *  distanceLabel;
	QLabel * widthLabel;
	QLabel * depthLaebl;
	QLineEdit * distanceLineEdit;
	QLineEdit * widthLineEdit;
	QLineEdit * depthLineEdit;

	QLabel * saveLabel;
	QLineEdit * saveLineEdit;
	QPushButton * saveBtn;

	//page2
	QGroupBox * monitorNumGroupBox;
	QLabel *  monitorNumLabel;
	QLineEdit * monitorNumLineEdit;

	QGroupBox * monitorGroupBox;
	QLabel *  monitorLabel;
	QComboBox * monitorCombobox;
	QGridLayout * monitorlayout;


	QPushButton * okBtn;
	QPushButton * checkBtn;
	QPushButton * cancelBtn;


	int NumSel; // ѡ����Ҫ������������
	int NumMonitor; // ��������������

	QStringList allSurfaceName; // ���������ݵĴ���

	QStringList allRectangleName; // ���о��������ݵĴ���

};

#endif // CALCULATIONWIDGET_H
