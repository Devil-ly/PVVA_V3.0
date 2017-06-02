#include "Qt/include/STLInputWidget.h"
#include <QFileDialog>

STLInputWidget::STLInputWidget(QWidget *parent, int wayButton)
{
	setWindowTitle(tr("Input STL"));

	// page1
	//defGroupBox
	defGroupBox = new QGroupBox;
	addDefGroupBox(defGroupBox, "Qt/images/rectangle_coor.png");


	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);

	
	//dimGroupBox
	STLLineEdit = new QLineEdit;
	STLBtn = new QPushButton(tr("Browse..."));
	connect(STLBtn, SIGNAL(clicked()), this, SLOT(buttonSTL()));

	QGridLayout * layoutSTL = new QGridLayout;
	layoutSTL->addWidget(STLLineEdit, 0, 0);
	layoutSTL->addWidget(STLBtn, 0, 1);


	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("STL address"));
	dimGroupBox->setLayout(layoutSTL);
	dimGroupBox->hide();
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

STLInputWidget::~STLInputWidget()
{
	
}

void STLInputWidget::setWidgetData(Surface const* _modleData)
{
	//STLLineEdit->setText(QString::fromStdString(_modleData.getNameFile()));
	setConnect(false);
	
	setGraphTransData(_modleData->getGraphTrans());
	list<pair<int, string>>varible = _modleData->getVarible();
	for (auto x : varible)
		changeText(x.first, x.second);

	setConnect(true);
}

bool STLInputWidget::getWidgetData(Surface * _modleData,
	Parameterization* parameterization) const
{
	bool ok;
	vector<double> para(7);
	
	GraphTrans graphTransPara = getGraphTransData(_modleData,
		parameterization, ok);

	_modleData->setData(para);
	//_modleData->setNameFile(STLLineEdit->text().toStdString());
	_modleData->setGraphTrans(graphTransPara);

	return ok;
}

QString STLInputWidget::getName() const
{
	return getGraphTransName();
}

void STLInputWidget::buttonSTL()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open the STL"),
		"",
		tr("*.stl"));
	if (!filename.isEmpty())
	{
		STLLineEdit->setText(filename);
	}
	emit sendData(5);
}

void STLInputWidget::changeText(int index, const string& t)
{
	if (index < 7)
		changeTransText(index, t);
}