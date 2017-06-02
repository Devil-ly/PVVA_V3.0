#ifndef RECTANGLEWIDGET_H
#define RECTANGLEWIDGET_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include "Qt/include/GraphTransWidget.h"
#include <VTK/include/Surface.h>


class RectangleWidget : public GraphTransWidget
{
	Q_OBJECT

public:
	RectangleWidget(QWidget *parent = 0, int wayButton = 0);
	~RectangleWidget();

	void setWidgetData(Surface const* _modleData);
	bool getWidgetData(Surface * _modleData, 
		Parameterization* parameterization) const;
	QString getName() const;

private slots:
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

};

#endif // RECTANGLEWIDGET_H
