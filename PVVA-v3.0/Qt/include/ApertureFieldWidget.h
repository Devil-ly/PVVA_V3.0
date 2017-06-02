#ifndef APERTUREFIELDWIDGET_H
#define APERTUREFIELDWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include "Qt/include/GraphTransWidget.h"
#include "VTK/include/ApertureField.h"
#include "VTK/include/Surface.h"
#include "Tools/Parameterization.h"

class ApertureFieldWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	ApertureFieldWidget(QWidget *parent = 0, int wayButton = 0);
	~ApertureFieldWidget();

	void setWidgetData(ApertureField const * _ApertureFieldPtr);
	bool getWidgetData(ApertureField * _Source,
		Parameterization* parameterization) const;
	bool getRectData(Surface * _modleData) const;
	QString ApertureFieldWidget::getName() const;


private slots:
    void EfileEeven();
	void HfileEeven();
	void setDataDef(int Num);
	void on_DefChange(QString);
private:
	void changeText(int, const string&);
	//page1
	QTabWidget * tabWidget;
	QGroupBox * defGroupBox;
	QGroupBox * baseGroupBox;
	QGroupBox * dimGroupBox;


	QLabel * widthlabel;
	QLabel * depthlabel;

	QLineEdit * widthLineEidt;
	QLineEdit * depthLineEidt;

	QLabel * label;
	QLineEdit * RecLineEidt;

	//page3
	QGroupBox * imgGroupBox;

	QLabel * scalelabel;
	QLabel * phaselabel;
	QLabel * datadeflabel;

	QLineEdit * scaleLineEidt;
	QLineEdit * phaseLineEidt;
	QComboBox * datadefComboBox;

	QGroupBox * sourceGroupBox;
	QLabel * Efieldlabel;
	QLabel * Hfieldlabel;
	QLabel * StartNumlabel;

	QPushButton * Efieldbtn;
	QPushButton * Hfieldbtn;

	QLineEdit * EfieldLineEidt;
	QLineEdit * HfieldLineEidt;
	QLineEdit * StartNumLineEidt;

	QGroupBox * destinationGroupBox;
	QLabel * UNumlabel;
	QLabel * VNumlabel;

	QLineEdit * UNumLineEidt;
	QLineEdit * VNumLineEidt;
};

#endif // RECTANGLEWIDGET_H
