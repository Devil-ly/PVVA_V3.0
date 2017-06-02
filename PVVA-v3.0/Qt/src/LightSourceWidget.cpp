#include "Qt/include/LightSourceWidget.h"
#include <QMessageBox>

LightSourceWidget::LightSourceWidget(QWidget *parent, int wayButton)

{
	setWindowTitle(tr("Create LightSource"));

	// page1
	//defGroupBox
	combobox = new QComboBox;
	combobox->addItem(tr("Rectangle"));
	combobox->addItem(tr("Custom"));
	combobox->addItem(tr("Point"));
	//combobox->addItem(tr("Select"));
	
	connect(combobox, SIGNAL(currentIndexChanged(int)), this, SLOT(comboChange(int)));

	imgLlabel = new QLabel;
	imgLlabel->setPixmap(QPixmap("Qt/images/rectangle_coor.png"));

	QGridLayout * layout1 = new QGridLayout;
	layout1->addWidget(combobox);
	layout1->addWidget(imgLlabel);

	defGroupBox = new QGroupBox;
	defGroupBox->setLayout(layout1);
	defGroupBox->setTitle(tr("Definition methods"));

	//baseGroupBox
	baseGroupBox = new QGroupBox;
	addBaseGroupBox(baseGroupBox);
	
	//dimGroupBox
	radiuslabel = new QLabel(tr("Width(W)"));
	depthlabel = new QLabel(tr("Depth(D)"));

	widthLineEidt = new QLineEdit;
	depthLineEidt = new QLineEdit;

	widthLineEidt->setText(tr("1"));
	depthLineEidt->setText(tr("1"));
	connect(widthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));
	connect(depthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(on_DefChange(QString)));

	QGridLayout * layout3= new QGridLayout;
	layout3->addWidget(radiuslabel, 0, 0);
	layout3->addWidget(depthlabel, 1, 0);
	layout3->addWidget(widthLineEidt, 0, 1);
	layout3->addWidget(depthLineEidt, 1, 1);

	dimGroupBox = new QGroupBox;
	dimGroupBox->setTitle(tr("Dimensions"));
	dimGroupBox->setLayout(layout3);

	// label
	layoutName = new QGridLayout;
	addNameLabel(layoutName);

	sourceCombobox = new QComboBox;
	QGridLayout * layoutsource = new QGridLayout;
	layoutsource->addWidget(sourceCombobox);
	sourceGroupBox = new QGroupBox;
	sourceGroupBox->setTitle(tr("Select Source"));
	sourceGroupBox->setLayout(layoutsource);
	sourceGroupBox->hide();

	//tabLayout1
	QVBoxLayout * tabLayout1; // page1
	tabLayout1 = new QVBoxLayout;
	tabLayout1->addWidget(defGroupBox);
	tabLayout1->addWidget(baseGroupBox);
	tabLayout1->addWidget(dimGroupBox);
	tabLayout1->addLayout(layoutName);
	tabLayout1->addWidget(sourceGroupBox);

	widget1 = new QWidget;
	widget1->setLayout(tabLayout1);

	// page2
	//imgGroupBox
	//QWidget * widget2;
	widget2 = new QWidget;
	addRotateWidget(widget2, "Qt/images/RotateWidget.png");

	//page3
	//img2GroupBox
	img2GroupBox = new QGroupBox;
	addDefGroupBox(img2GroupBox, "Qt/images/rectangle_coor.png");

	//LightNumGroupBox
	var1label = new QLabel(tr("Width interval"));
	var2label = new QLabel(tr("Depth interval"));

	var1LineEidt = new QLineEdit;
	var2LineEidt = new QLineEdit;

	var1LineEidt->setText(tr("0.1"));
	var2LineEidt->setText(tr("0.1"));

	QGridLayout * layout9 = new QGridLayout;
	layout9->addWidget(var1label, 0, 0);
	layout9->addWidget(var2label, 1, 0);
	layout9->addWidget(var1LineEidt, 0, 1);
	layout9->addWidget(var2LineEidt, 1, 1);

	LightNumGroupBox = new QGroupBox;
	LightNumGroupBox->setTitle(tr("Ray numbers"));
	LightNumGroupBox->setLayout(layout9);

	// lengthGroupBox
	lengthlabel = new QLabel(tr("length"));

	lengthLineEidt = new QLineEdit;
	lengthLineEidt->setText(tr("1"));
	connect(var1LineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));
	connect(var2LineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));
	connect(lengthLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(Page3Change(QString)));

	QGridLayout * layoutlength = new QGridLayout;
	layoutlength->addWidget(lengthlabel, 0, 0);
	layoutlength->addWidget(lengthLineEidt, 0, 1);
	
	lengthGroupBox = new QGroupBox;
	lengthGroupBox->setTitle(tr("Length of ray after reflection"));
	lengthGroupBox->setLayout(layoutlength);

	QVBoxLayout * tabLayout3;
	tabLayout3 = new QVBoxLayout;
	tabLayout3->addWidget(img2GroupBox);
	tabLayout3->addWidget(LightNumGroupBox);
	tabLayout3->addWidget(lengthGroupBox);


	widget3 = new QWidget;
	widget3->setLayout(tabLayout3);

	//page4
	//img3GroupBox
	img3GroupBox = new QGroupBox;
	addDefGroupBox(img3GroupBox, "Qt/images/LightSource_SelSurface.png");

	//surfaceNumGroupBox
	surfaceNumlabel = new QLabel(tr("Num of Surfaces"));
	selectLineEidt = new QLineEdit;
	selectLineEidt->setText(tr("3"));

	connect(selectLineEidt, SIGNAL(textChanged(QString)),
		this, SLOT(selectLineEidtChange(QString)));

	QGridLayout * layout10 = new QGridLayout;
	layout10->addWidget(surfaceNumlabel, 0, 0);
	layout10->addWidget(selectLineEidt, 0, 1);

	surfaceNumGroupBox = new QGroupBox;
	surfaceNumGroupBox->setTitle(tr("Add surfaces to trace"));
	surfaceNumGroupBox->setLayout(layout10);
	
	//selectGroupBox
	NumSel = 3;
	selectlayout = new QGridLayout;
	selectcombobox = new QComboBox[NumSel];
	selectlabel = new QLabel[NumSel];
	for (int i = 0; i < NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
	
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}

	selectGroupBox = new QGroupBox;
	selectGroupBox->setTitle(tr("Select surfaces"));
	selectGroupBox->setLayout(selectlayout);

	selectTabLayout = new QVBoxLayout;
	selectTabLayout->addWidget(img3GroupBox);
	selectTabLayout->addWidget(surfaceNumGroupBox);
	selectTabLayout->addWidget(selectGroupBox);

	selectWidget = new QWidget;
	selectWidget->setLayout(selectTabLayout);

	tabWidget = new QTabWidget;
	tabWidget->addTab(widget1, tr("Geometry"));
	tabWidget->addTab(widget2, tr("Workplane"));
	tabWidget->addTab(widget3, tr("Rays"));
	tabWidget->addTab(selectWidget, tr("Surfaces"));
	connect(tabWidget, SIGNAL(currentChanged(int)), this, SLOT(tabWidgetChange(int)));


	QGridLayout * layoutbt = new QGridLayout;
	addBtn(layoutbt, wayButton);
	setAddBtn();

	QVBoxLayout * mainlayout = new QVBoxLayout(this);
	mainlayout->addWidget(tabWidget);
	mainlayout->addLayout(layoutbt); 

}

LightSourceWidget::~LightSourceWidget()
{
	delete[]selectcombobox;
	delete[]selectlabel;
}

void LightSourceWidget::comboChange(int index)
{
	tabWidget->removeTab(1);
	tabWidget->removeTab(2);
	tabWidget->removeTab(3);

	if (index == 1)
	{
		radiuslabel->setText(tr("Width(W)"));
		depthlabel->setText(tr("Depth(D)"));
		depthlabel->show();
		depthLineEidt->show();
		var1label->setText(tr("Width interval"));
		var2label->setText(tr("Depth interval"));
		var2label->show();
		var2LineEidt->show();
		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
	}
	else if (index == 0)
	{
		radiuslabel->setText(tr("Width(W)"));
		depthlabel->setText(tr("Depth(D)"));
		depthlabel->show();
		depthLineEidt->show();
		var1label->setText(tr("Width interval"));
		var2label->setText(tr("Depth interval"));
		var2label->show();
		var2LineEidt->show();

		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
	}
	else if(index == 2)
	{
		radiuslabel->setText(tr("Angle(A)"));
		widthLineEidt->setText(tr("30"));
		depthlabel->hide();
		depthLineEidt->hide();
		var1label->setText(tr("Theta interval"));
		var2label->hide();
		var2LineEidt->hide();

		tabWidget->addTab(widget2, tr("Workplane"));
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
		//setText(tr("Phi interval"));
	}
	else
	{
		tabWidget->addTab(widget3, tr("Rays"));
		tabWidget->addTab(selectWidget, tr("Surfaces"));
		LightNumGroupBox->hide();
		baseGroupBox->hide();
		dimGroupBox->hide();
		setNameHide(true);
		sourceGroupBox->show();
		emit sendData(5);
		return;
	}

	LightNumGroupBox->show();
	baseGroupBox->show();
	dimGroupBox->show();
	setNameHide(false);
	sourceGroupBox->hide();

	emit sendData(5);
}

void LightSourceWidget::setWidgetData(const RaySource * _LightSource)
{
	vector<double> para = _LightSource->getData();
	widthLineEidt->setText(QString::number(para[7]));
	depthLineEidt->setText(QString::number(para[8]));
	var1LineEidt->setText(QString::number(para[9]));
	var2LineEidt->setText(QString::number(para[10]));
	lengthLineEidt->setText(QString::number(para[11]));
	setGraphTransData(_LightSource->getGraphTrans());

	list<pair<int, string>>varible = _LightSource->getVarible();
	for (auto x : varible)
		changeText(x.first, x.second);

	combobox->setCurrentIndex(_LightSource->getType());
}

bool LightSourceWidget::getWidgetData(RaySource * _LightSource) const
{
	vector<double> para(12);
	int type;
	GraphTrans graphTransPara;

	bool ok, ok_back;
	string temp;
	para[7] = widthLineEidt->text().toDouble(&ok);
	if (!ok)
	{
		temp = widthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[7]))
		{
			//parameterization->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(7, temp);
		}
		else
		{
			widthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}

	}
	else
		_LightSource->deleteVarible(7);

	para[8] = depthLineEidt->text().toFloat(&ok);
	if (!ok)
	{
		temp = depthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[8]))
		{
			//parameterization->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(8, temp);
		}
		else
		{
			depthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_LightSource->deleteVarible(8);
	
	para[9] = var1LineEidt->text().toFloat(&ok);
	if (!ok)
	{
		temp = var1LineEidt->text().toStdString();
		if (isLegitimate(temp, para[9]))
		{
			//parameterization->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(9, temp);
		}
		else
		{
			var1LineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_LightSource->deleteVarible(9);

	para[10] = var2LineEidt->text().toFloat(&ok);
	if (!ok)
	{
		temp = var2LineEidt->text().toStdString();
		if (isLegitimate(temp, para[10]))
		{
			//parameterization->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(10, temp);
		}
		else
		{
			var2LineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_LightSource->deleteVarible(10);

	type = combobox->currentIndex();

	para[11] = lengthLineEidt->text().toFloat(&ok);
	if (!ok)
	{
		temp = lengthLineEidt->text().toStdString();
		if (isLegitimate(temp, para[11]))
		{
			//parameterization->addParaSurface(temp, _LightSource);
			_LightSource->addVarible(11, temp);
		}
		else
		{
			lengthLineEidt->setStyleSheet("background-color:rgba(255,0,0,255)");
			return false;
		}
	}
	else
		_LightSource->deleteVarible(11);


	ok = true;
	graphTransPara = getGraphTransData(_LightSource, ok_back);
	ok = ok && ok_back;

	_LightSource->setData(para);
	_LightSource->setGraphTrans(graphTransPara);
	_LightSource->setType(type);

	return ok;
}

void LightSourceWidget::addAllSurface(QStringList _allSurfaceName)
{
	allSurfaceName = _allSurfaceName;
	for (int i = 0; i < NumSel; i++)
	{
		selectcombobox[i].clear();
		selectcombobox[i].addItems(allSurfaceName);
	}
}

void LightSourceWidget::addAllSources(QStringList _allSourcesName)
{
	sourceCombobox->clear();
	sourceCombobox->addItems(_allSourcesName);
}

int LightSourceWidget::getSelSources() const
{
	return sourceCombobox->currentIndex();
}

float LightSourceWidget::getLength() const
{
	return lengthLineEidt->text().toFloat();
}

void LightSourceWidget::getSelSurfaces(int & _NumSel, int *& index)
{
	_NumSel = NumSel;

	_NumSel = NumSel;
	index = new int[NumSel];
	for (int i = 0; i < NumSel; i++)
		index[i] = selectcombobox[i].currentIndex();	
}

void LightSourceWidget::setSelSurfaces(int _NumSel)
{
	NumSel = _NumSel;
	delete[]selectcombobox;
	delete[]selectlabel;
	selectcombobox = new QComboBox[_NumSel];
	selectlabel = new QLabel[_NumSel];
	for (int i = 0; i < _NumSel; i++)
	{
		selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
		selectcombobox[i].addItems(allSurfaceName);
		selectlayout->addWidget(&selectlabel[i], i, 0);
		selectlayout->addWidget(&selectcombobox[i], i, 1);
	}
	selectLineEidt->setText(QString::number(_NumSel));
}

void LightSourceWidget::selectLineEidtChange(const QString & text)
{
	bool ok;
	NumSel = text.toInt(&ok);
	if (ok)
	{
		delete[]selectcombobox;
		delete[]selectlabel;
		selectcombobox = new QComboBox[NumSel];
		selectlabel = new QLabel[NumSel];
		for (int i = 0; i < NumSel; i++)
		{
			selectlabel[i].setText(tr("Surface ") + QString::number(i + 1));
			selectcombobox[i].addItems(allSurfaceName);
			selectlayout->addWidget(&selectlabel[i], i, 0);
			selectlayout->addWidget(&selectcombobox[i], i, 1);
		}
	}
	else
	{
		QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
	}	
}

void LightSourceWidget::tabWidgetChange(int index)
{
	if(index == 0 || index == 1)
		emit sendData(5);
	else
		emit sendData(7);
}

void LightSourceWidget::Page3Change(QString t)
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
	var1LineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	var2LineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");
	lengthLineEidt->setStyleSheet("background-color:rgba(255,255,255,255)");

	emit sendData(7);
}

bool LightSourceWidget::getRectData(Surface * _modleData,
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

int LightSourceWidget::getType() const
{
	return combobox->currentIndex();
}

QString LightSourceWidget::getName() const
{
	return getGraphTransName();
}

void LightSourceWidget::init()
{
	tabWidget->setCurrentIndex(0);
	combobox->setCurrentIndex(0);
	comboChange(0);
}

void LightSourceWidget::on_DefChange(QString t)
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

void LightSourceWidget::changeText(int index, const string& t)
{
	if (index < 7)
		changeTransText(index, t);
	else if (index == 7)
		widthLineEidt->setText(QString::fromStdString(t));
	else if (index == 8)
		depthLineEidt->setText(QString::fromStdString(t));
	else if (index == 9)
		var1LineEidt->setText(QString::fromStdString(t));
	else if (index == 10)
		var2LineEidt->setText(QString::fromStdString(t));
	else if (index == 11)
		lengthLineEidt->setText(QString::fromStdString(t));
}