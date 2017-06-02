#include "Qt/include/CylinderWidget.h"

CylinderWidget::CylinderWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Create cylinder"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/cylinder_coor.png");

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);
	
	//dimGroupBox
	widthlabel = new QLabel(tr("Radius(R)"));
	depthlabel = new QLabel(tr("Height(H)"));

	widthLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	widthLineEidt->setText(tr("0.5"));
	depthLineEidt->setText(tr("1"));

	connect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(widthlabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	QGridLayout * layout4 = new QGridLayout;
	addNameLabel(layout4);

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addLayout(layout4);

	QWidget * widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	QWidget * widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");


	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));


	QGridLayout * layout5 = new QGridLayout;
	addBtn(layout5, wayButton);

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layout5);

}

CylinderWidget::~CylinderWidget()
{
	
}

void CylinderWidget::setWidgetData(Surface const* _modleData)
{
	setConnect(false);
	disconnect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	disconnect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	vector<double> para = _modleData->getData();
	widthLineEidt->setText(QString::number(para[7]));
	depthLineEidt->setText(QString::number(para[8]));
	setGraphTransData(_modleData->getGraphTrans());
	list<pair<int, string>>varible = _modleData->getVarible();
	for (auto x : varible)
		changeText(x.first, x.second);
	connect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	setConnect(true);
}

bool CylinderWidget::getWidgetData(Surface * _modleData,
	Parameterization* parameterization) const
{
	bool ok, ok_back;
	vector<double> para(9);
	string temp;
	para[7] = widthLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = widthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[7]))
		{
			parameterization->addParaSurface(temp, _modleData);
			_modleData->addVarible(7, temp);
		}
		else
		{
			widthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}

	}
	else
		_modleData->deleteVarible(7);

	para[8] = depthLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = depthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[8]))
		{
			parameterization->addParaSurface(temp, _modleData);
			_modleData->addVarible(8, temp);
		}
		else
		{
			depthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_modleData->deleteVarible(8);

	ok = true;
	GraphTrans graphTransPara = getGraphTransData(_modleData,
		parameterization, ok_back);
	ok = ok && ok_back;

	_modleData->setData(para);
	_modleData->setGraphTrans(graphTransPara);
	return ok;
}

QString CylinderWidget::getName() const
{
	return getGraphTransName();
}

void CylinderWidget::changeText(int index, const string& t)
{
	if (index < 7)
		changeTransText(index, t);
	else if (index == 7)
		widthLineEidt->setText(QString::fromStdString(t));
	else if (index == 8)
		depthLineEidt->setText(QString::fromStdString(t));
}

void CylinderWidget::on_DefChange(QString t)
{
	bool ok = false;
	t.toFloat(&ok);
	string temp;
	if (!ok)
	{
		temp = t.toStdString();
		try
		{
			Parameterization::getVal(temp);
		}
		catch (errorOperator &e)
		{
			return;
		}

	}
	widthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	depthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	emit sendData(5);
}
