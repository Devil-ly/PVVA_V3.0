#include "mainwindow.h"

#include <QMessageBox>

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkRenderWindow.h>
#include <vtkCaptionActor2D.h>
#include <vtkTextProperty.h>
#include <vtkProperty.h>
#include <QApplication>
#include <QString>
#include <QFont>
#include <QFileDialog>

#include "Qt/include/ParaboloidWidget.h"
#include "Qt/include/AddVariableWidget.h"
#include "qt/include/CalculateThread.h"

mainWindow::mainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	//setWindowTitle(QString::fromLocal8Bit("北航微波工程实验室"));

	setWindowTitle(tr("PVVA-3.0")); // 设置中央窗口
	setCentralWidget(&widget);
	resize(1200, 800);

	// 初始化vtk窗口
	axes = vtkSmartPointer<vtkAxesActor>::New();
	axes->GetXAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(1, 0, 0);//修改X字体颜色为红色  
	axes->GetYAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 2, 0);//修改Y字体颜色为绿色  
	axes->GetZAxisCaptionActor2D()->GetCaptionTextProperty()->SetColor(0, 0, 3);//修改Z字体颜色为蓝色  
	axes->SetConeRadius(0.1);
	axes->SetConeResolution(20);
	//axes->SetTotalLength(15, 15, 15); 修改坐标尺寸
	//axes->SetXAxisLabelText("X/U");
	//axes->SetYAxisLabelText("Y/V");
	//axes->SetZAxisLabelText("Z/N");

	renderer = vtkSmartPointer<vtkRenderer>::New();
	renderer->SetBackground(1.0, 1.0, 1.0);
	renderer->AddActor(axes);

	auto window = widget.GetRenderWindow();

	interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	interactor->SetRenderWindow(window);

	auto style = vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
	interactor->SetInteractorStyle(style);

	interactor->Initialize();
	window->AddRenderer(renderer);
	window->Render();

	// 设置dock窗口
	leftWidget = new QDockWidget("Navigation", this);
	leftWidget->setFeatures(QDockWidget::DockWidgetMovable);
	leftWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, leftWidget);

	// treeWidget
	treeWidget = new QTreeWidget;
	treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(treeWidget, SIGNAL(customContextMenuRequested(QPoint)),
		this, SLOT(on_treeWidget_ContextMenuRequested(QPoint)));
	connect(treeWidget, SIGNAL(itemPressed(QTreeWidgetItem*, int)),
		this, SLOT(on_treeWidget_leftPressed(QTreeWidgetItem*, int)));
	treeWidget->setHeaderHidden(true);  // 隐藏表头
	leftWidget->setWidget(treeWidget);

	//RightWidget
	detailsDockWidget = new QDockWidget("Details", this);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetMovable);
	detailsDockWidget->setFeatures(QDockWidget::DockWidgetClosable);
	detailsDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
	//leftWidget->setWindowFlags(Qt::FramelessWindowHint);
	addDockWidget(Qt::LeftDockWidgetArea, detailsDockWidget);

	//treeWidget->setStyleSheet("QTreeView::branch {image:none;}"); 
	//QTreeWidgetItem *imageItem1 = new QTreeWidgetItem(treeWidget, QStringList(QString("Gaussian_Thread")));
	//QTreeWidgetItem *imageItem2 = new QTreeWidgetItem(imageItem1, QStringList(QString("test2")));

	createTreeWidgetItem();
	createActions();
	createMenus();
	createToolBars();
	createStatusBar();
	createRightMenu();
	createDetails();
	creareWindows();
	init();

}

mainWindow::~mainWindow()
{

}

void mainWindow::init()
{
	paraboloidNum = 0;
	LightSourceNum = 0;
	rectangleNum = 0;
	cylinderNum = 0;
	STLInputNum = 0;
	sphereNum = 0;
	FieldNum = 0;
	ResFieldNum = 0;
	ArbitrarySurfaceNum = 0;
	booleanNum = 0;
	isSet = 0; // 不是修改
	Unit_factor = 1;
	currentValue = 0;

	tempSurface = NULL;
	tempRaySource = NULL;
	tempField = NULL;
	tempImageActor = NULL;
	
	isExistenceOpenWin = false;
	parameterization.clearAll();
	parameterization.addPara("eps0", "8.85418781761e-12");
	parameterization.addPara("Pi", "3.14159265358979");
	parameterization.addPara("mu0", "Pi*4e-7");
	parameterization.addPara("c0", "1/sqrt(eps0*mu0)");
	parameterization.addPara("zf0", "sqrt(mu0/eps0");

	SelProperty = vtkSmartPointer<vtkProperty>::New();
	SelProperty->SetOpacity(0.3);
	SelProperty->SetColor(1, 0, 0);

	ChangeProperty = vtkSmartPointer<vtkProperty>::New();
	ChangeProperty->SetOpacity(0.4);
	ChangeProperty->SetColor(0, 0, 1);
}

void mainWindow::createActions()
{
	//**************菜单*****************
	//保存
	saveFileAction = new QAction(QIcon(tr("Qt/images/save.png")), tr("Save"), this);
	saveFileAction->setShortcut(tr("Ctrl +　s"));
	saveFileAction->setStatusTip(tr("Save a file"));
	connect(saveFileAction, SIGNAL(triggered()), this, SLOT(saveFile()));

	//打开
	openFileAction = new QAction(QIcon(tr("Qt/images/open.png")), tr("Open"), this);
	openFileAction->setShortcut(tr("Ctrl +　O"));
	openFileAction->setStatusTip(tr("Open a file"));
	connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFile()));

	//新建
	newFileAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	newFileAction->setShortcut(tr("Ctrl +　N"));
	newFileAction->setStatusTip(tr("New a file"));
	connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));

	//Details
	detailsAction = new QAction(QIcon(tr("Qt/images/new.png")), tr("New"), this);
	detailsAction->setStatusTip(tr("Show Details"));
	connect(detailsAction, SIGNAL(triggered()), this, SLOT(showDetails()));

	//初始化视角
	viewAction = new QAction(QIcon(tr("Qt/images/view.png")), tr("View"), this);
	viewAction->setStatusTip(tr("Initialize the view"));
	connect(viewAction, SIGNAL(triggered()), this, SLOT(viewInitFile()));

	//修改视角
	viewLabel = new QLabel(tr("View"));
	viewComboBox = new QComboBox;
	viewComboBox->addItem("View YZ plane(original)");
	viewComboBox->addItem("View XZ plane");
	viewComboBox->addItem("View XY plane");
	viewComboBox->addItem("View -YZ plane");
	viewComboBox->addItem("View -XZ plane");
	viewComboBox->addItem("View -XY plane");
	connect(viewComboBox, SIGNAL(activated(int)), this, SLOT(setView(int)));

	//**************模型*****************
	//矩形
	rectangleAction = new QAction(QIcon(tr("Qt/images/rectangle.png")), tr("Rectangle"), this);
	rectangleAction->setStatusTip(tr("Create a rectangle"));
	connect(rectangleAction, SIGNAL(triggered()), this, SLOT(createRectangle()));

	//任意面
	polygonAction = new QAction(QIcon(tr("Qt/images/polygon.png")), tr("Polygon"), this);
	polygonAction->setStatusTip(tr("Create a polygon"));
	connect(polygonAction, SIGNAL(triggered()), this, SLOT(createArbitrarySurface()));

	//抛物面
	paraboloidAction = new QAction(QIcon(tr("Qt/images/paraboloid.png")), tr("Paraboloid"), this);
	paraboloidAction->setStatusTip(tr("Create a paraboloid"));
	connect(paraboloidAction, SIGNAL(triggered()), this, SLOT(createParaboloid()));

	//圆柱体
	cylinderAction = new QAction(QIcon(tr("Qt/images/cylinder.png")), tr("Cylinder"), this);
	cylinderAction->setStatusTip(tr("Create a cylinder"));
	connect(cylinderAction, SIGNAL(triggered()), this, SLOT(createCylinder()));

	//球
	sphereAction = new QAction(QIcon(tr("Qt/images/sphere.png")), tr("Sphere"), this);
	sphereAction->setStatusTip(tr("Create a sphere"));
	connect(sphereAction, SIGNAL(triggered()), this, SLOT(createSphere()));

	//布尔运算
	booleanAction = new QAction(QIcon(tr("Qt/images/boolean.png")), tr("Boolean"), this);
	booleanAction->setStatusTip(tr("Boolean operation"));
	connect(booleanAction, SIGNAL(triggered()), this, SLOT(createBoolean()));

	//STL
	STLInputAction = new QAction(QIcon(tr("Qt/images/stlInput.png")), tr("STL"), this);
	STLInputAction->setStatusTip(tr("STL Input"));
	connect(STLInputAction, SIGNAL(triggered()), this, SLOT(createSTLInput()));

	//光线追踪源
	LightSourceAction = new QAction(QIcon(tr("Qt/images/LightSource.png")), tr("LightSource"), this);
	LightSourceAction->setStatusTip(tr("Create a LightSource"));
	connect(LightSourceAction, SIGNAL(triggered()), this, SLOT(createLightSource()));

	//口面场源
	ApertureFieldAction = new QAction(QIcon(tr("Qt/images/ApertureField.png")), tr("Aperture-Field"), this);
	ApertureFieldAction->setStatusTip(tr("Create a Aperture-Field source"));
	connect(ApertureFieldAction, SIGNAL(triggered()), this, SLOT(createApertureField()));

	//高斯源
	GaussianAction = new QAction(QIcon(tr("Qt/images/Gaussian.png")), tr("Gaussian"), this);
	GaussianAction->setStatusTip(tr("Create a Gaussian source"));
	connect(GaussianAction, SIGNAL(triggered()), this, SLOT(createGaussian()));

	//计算
	CalculationAction = new QAction(QIcon(tr("Qt/images/calculation.png")), tr("Calculate"), this);
	CalculationAction->setStatusTip(tr("Calculate"));
	connect(CalculationAction, SIGNAL(triggered()), this, SLOT(createCalculation()));

}

void mainWindow::createStatusBar()
{
	locationLabel = new QLabel("    ");
	locationLabel->setAlignment(Qt::AlignHCenter);
	locationLabel->setMinimumSize(locationLabel->sizeHint());
	this->statusBar()->addWidget(locationLabel, 90);

	unitBtn = new QPushButton(tr("Unit: m"));
	connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	//connect(unitBtn, SIGNAL(clicked()), this, SLOT(changeUnit()));
	this->statusBar()->addWidget(unitBtn);
}

void mainWindow::createMenus()
{
	// 文件菜单
	//this->menuBar()
	fileMenu = this->menuBar()->addMenu(tr("Files"));

	fileMenu->addAction(saveFileAction);
	fileMenu->addAction(openFileAction);
	fileMenu->addAction(newFileAction);
	fileMenu->addSeparator();
	//fileMenu->addAction(LightSourceAction);

}

void mainWindow::createToolBars()
{
	//file 栏
	fileTool = addToolBar("Files");
	fileTool->addAction(saveFileAction);
	fileTool->addAction(openFileAction);
	fileTool->addAction(newFileAction);
	fileTool->addAction(detailsAction);
	fileTool->addSeparator();
	fileTool->addAction(viewAction);
	fileTool->addSeparator();
	fileTool->addWidget(viewLabel);
	fileTool->addWidget(viewComboBox);
	//fileTool->addAction(LightSourceAction);

	// 模型栏
	constructTool = addToolBar("Construct");
	constructTool->addAction(rectangleAction);
	constructTool->addAction(polygonAction);
	constructTool->addAction(paraboloidAction);

	constructTool->addSeparator();
	constructTool->addAction(cylinderAction);
	constructTool->addAction(sphereAction);

	constructTool->addSeparator();
	constructTool->addAction(STLInputAction);
	constructTool->addAction(booleanAction);
	constructTool->addAction(LightSourceAction);
	constructTool->addAction(ApertureFieldAction);
	constructTool->addAction(GaussianAction);
	constructTool->addSeparator();
	constructTool->addAction(CalculationAction);
}

void mainWindow::createDetails()
{
	dimensionGroupBtn = new QButtonGroup();
	ThreeDBtn = new QRadioButton(tr("3D"));
	TwoDBtn = new QRadioButton(tr("2D"));
	dimensionGroupBtn->addButton(ThreeDBtn, 0);
	dimensionGroupBtn->addButton(TwoDBtn, 1);
	connect(ThreeDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(TwoDBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * dimensionLayout = new QGridLayout;
	dimensionLayout->addWidget(ThreeDBtn, 0, 0);
	dimensionLayout->addWidget(TwoDBtn, 0, 1);

	QGroupBox * dimensionGroupBox = new QGroupBox;
	dimensionGroupBox->setTitle(tr("dimension"));
	dimensionGroupBox->setLayout(dimensionLayout);

	fieldGroupBtn = new QButtonGroup();
	ExBtn = new QRadioButton(tr("Ex"));
	EyBtn = new QRadioButton(tr("Ey"));
	EzBtn = new QRadioButton(tr("Ez"));
	HxBtn = new QRadioButton(tr("Hx"));
	HyBtn = new QRadioButton(tr("Hy"));
	HzBtn = new QRadioButton(tr("Hz"));

	fieldGroupBtn->addButton(ExBtn, 0);
	fieldGroupBtn->addButton(EyBtn, 1);
	fieldGroupBtn->addButton(EzBtn, 2);
	fieldGroupBtn->addButton(HxBtn, 3);
	fieldGroupBtn->addButton(HyBtn, 4);
	fieldGroupBtn->addButton(HzBtn, 5);

	connect(ExBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(EzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HxBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HyBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(HzBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * fieldLayout = new QGridLayout;
	fieldLayout->addWidget(ExBtn, 0, 0);
	fieldLayout->addWidget(EyBtn, 1, 0);
	fieldLayout->addWidget(EzBtn, 2, 0);
	fieldLayout->addWidget(HxBtn, 0, 1);
	fieldLayout->addWidget(HyBtn, 1, 1);
	fieldLayout->addWidget(HzBtn, 2, 1);

	QGroupBox * fieldGroupBox = new QGroupBox;
	fieldGroupBox->setTitle(tr("field"));
	fieldGroupBox->setLayout(fieldLayout);

	// pmGroupBox
	pmGroupBtn = new QButtonGroup();
	magnitudeBtn = new QRadioButton(tr("magnitude"));
	phaseBtn = new QRadioButton(tr("phase"));

	pmGroupBtn->addButton(magnitudeBtn, 0);
	pmGroupBtn->addButton(phaseBtn, 1);

	connect(magnitudeBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(phaseBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	QGridLayout * pmLayout = new QGridLayout;
	pmLayout->addWidget(magnitudeBtn, 0, 0);
	pmLayout->addWidget(phaseBtn, 0, 1);

	QGroupBox * pmGroupBox = new QGroupBox;
	pmGroupBox->setTitle(tr("parameter"));
	pmGroupBox->setLayout(pmLayout);

	// pmGroupBox
	powerGroupBtn = new QButtonGroup();
	linearBtn = new QRadioButton(tr("linear"));
	dbBtn = new QRadioButton(tr("dB"));
	powerGroupBtn->addButton(dbBtn, 0);
	powerGroupBtn->addButton(linearBtn, 1);


	QGridLayout * powerLayout = new QGridLayout;
	powerLayout->addWidget(linearBtn, 0, 0);
	powerLayout->addWidget(dbBtn, 0, 1);

	connect(linearBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));
	connect(dbBtn, SIGNAL(clicked()), this, SLOT(on_Details_FieldClicked()));

	// powerGroupBox
	QGroupBox * powerGroupBox = new QGroupBox;
	powerGroupBox->setTitle(tr("power"));
	powerGroupBox->setLayout(powerLayout);

	QVBoxLayout * boxLayout = new QVBoxLayout;
	boxLayout->addWidget(dimensionGroupBox);
	boxLayout->addWidget(fieldGroupBox);
	boxLayout->addWidget(pmGroupBox);
	boxLayout->addWidget(powerGroupBox);

	detailsWidget = new QWidget;
	detailsWidget->setLayout(boxLayout);
	detailsDockWidget->setWidget(detailsWidget);
	detailsDockWidget->close();
}

void mainWindow::creareWindows()
{
	LightSourceDialog = new LightSourceWidget(this);
	connect(LightSourceDialog, SIGNAL(sendData(int)),
		this, SLOT(ToLightSource(int)));
	LightSourceDialog->setWindowFlags(Qt::WindowStaysOnTopHint);
}

void mainWindow::createTreeWidgetItem()
{
	definitionsTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Definitions")));
	modelTreeItem = new QTreeWidgetItem(treeWidget, QStringList(QString("Model")));

	variablesTreeItem = new QTreeWidgetItem(QStringList(QString("Variables")));
	variablesTreeItem->setData(0, Qt::UserRole, QVariant(3));
	// 这里3是个标志

	QTreeWidgetItem *child1;
	child1 = new QTreeWidgetItem;
	child1->setText(0, tr("eps0") + tr(" = ") + 
		tr("8.85418781761e-12"));
	variablesTreeItem->addChild(child1);

	QTreeWidgetItem *child2;
	child2 = new QTreeWidgetItem;
	child2->setText(0, tr("Pi") + tr(" = ") +
		tr("3.14159265358979"));
	variablesTreeItem->addChild(child2);

	QTreeWidgetItem *child3;
	child3 = new QTreeWidgetItem;
	child3->setText(0, tr("mu0") + tr(" = ") +
		tr("Pi*4e-7"));
	variablesTreeItem->addChild(child3);

	QTreeWidgetItem *child4;
	child4 = new QTreeWidgetItem;
	child4->setText(0, tr("c0") + tr(" = ") +
		tr("1/sqrt(eps0*mu0)"));
	variablesTreeItem->addChild(child4);

	QTreeWidgetItem *child5;
	child5 = new QTreeWidgetItem;
	child5->setText(0, tr("zf0") + tr(" = ") +
		tr("sqrt(mu0/eps0))"));
	variablesTreeItem->addChild(child5);

	definitionsTreeItem->addChild(variablesTreeItem);
	definitionsTreeItem->setExpanded(true);

	geometryTreeItem = new QTreeWidgetItem(QStringList(QString("Geometry")));
	modelTreeItem->addChild(geometryTreeItem);

	lightTreeItem = new QTreeWidgetItem(QStringList(QString("Light")));
	modelTreeItem->addChild(lightTreeItem);

	sourceTreeItem = new QTreeWidgetItem(QStringList(QString("Field")));
	modelTreeItem->addChild(sourceTreeItem);

}

void mainWindow::createRightMenu()
{
	R_Tree_compenents_childMenu = new QMenu(this);
	R_BlankMenu = new QMenu(this);

	SetGeometryAction = new QAction(tr("Properties"), this);
	QFont font("Microsoft YaHei", 10, 75);
	SetGeometryAction->setFont(font);
	SetGeometryAction->setStatusTip(tr("Modifying parameters"));
	connect(SetGeometryAction, SIGNAL(triggered()), this, SLOT(SetGeometryPara()));

	DeleteGeometryAction = new QAction(QIcon(tr("Qt/images/delete.png")), tr("Delete"), this);
	DeleteGeometryAction->setStatusTip(tr("Delete parameters"));
	connect(DeleteGeometryAction, SIGNAL(triggered()), this, SLOT(DeleteGeometry()));

	Field2lightAction = new QAction(tr("Generation lightSource"), this);
	Field2lightAction->setStatusTip(tr("Generation lightSource"));
	connect(Field2lightAction, SIGNAL(triggered()), this, SLOT(Field2light()));

	SaveFieldAction = new QAction(tr("Save"), this);
	SaveFieldAction->setStatusTip(tr("Save Field"));
	connect(SaveFieldAction, SIGNAL(triggered()), this, SLOT(SaveField()));

	transparencyAction = new QAction(tr("Transparency"), this);
	transparencyAction->setStatusTip(tr("Transparency"));
	transparencyAction->setCheckable(true);
	transparencyAction->setChecked(true);
	connect(transparencyAction, SIGNAL(triggered()), this, SLOT(SetTransparency()));

	isShowAction = new QAction(tr("Show"), this);
	isShowAction->setStatusTip(tr("Show"));
	isShowAction->setCheckable(true);
	isShowAction->setChecked(true);
	connect(isShowAction, SIGNAL(triggered()), this, SLOT(SetShow()));

	AddVariableAction = new QAction(tr("Add Variable"), this);
	AddVariableAction->setStatusTip(tr("Add Variable"));
	connect(AddVariableAction, SIGNAL(triggered()), this, SLOT(AddVariable()));
}

QTreeWidgetItem * mainWindow::addTreeChild(QTreeWidgetItem * 
	treeFather, QString treeName, QString iconFile, 
	int name, int index, int rightPar)
{
	QTreeWidgetItem *child;
	child = new QTreeWidgetItem;
	child->setText(0, treeName);
	child->setIcon(0, QIcon(iconFile));

	QVariant var0(rightPar);        //设置右键参数
	QVariant var1(name);
	QVariant var2(index);
	child->setData(0, Qt::UserRole, var0);
	child->setData(1, Qt::UserRole, var1);
	child->setData(2, Qt::UserRole, var2);
	treeFather->addChild(child);

	modelTreeItem->setExpanded(true);
	treeFather->setExpanded(true);

	return child;
}


// -------------------- slots 函数 ------------------------------------

// -------------------- 模型函数 ------------------------------------
void mainWindow::createParaboloid()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	paraboloidDialog = new ParaboloidWidget(this, isSet);
	paraboloidDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(paraboloidDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveParaboloid(int)));

	if (isSet == 1) // 判断是否是新建还是修改 1为修改
	{
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// 将界面设置成选中的值
		paraboloidDialog->setNameLineEdit(rightSelectItem->text(0));

		// 找到需要改变的模型
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = SurfaceData[PARABOLOID][id]->getActor();
		
		paraboloidDialog->setWidgetData(SurfaceData[PARABOLOID][id]);
		
		isSet = 0;
	}
	else 
	{
		paraboloidDialog->setNameLineEdit("paraboloid" + 
			QString::number(paraboloidNum + 1));
	}
	tempSurface = new Paraboloid();
	isExistenceOpenWin = true;
	paraboloidDialog->show();
	toReceiveParaboloid(5);
}

void mainWindow::toReceiveParaboloid(int index)
{
	bool ok;
	ok = paraboloidDialog->getWidgetData(tempSurface, 
		&parameterization);
	if (!ok) // 判断是否读取成功
		return;
	if(index == 5)
		toModelActorChange(true);
	else if (index == 2 || index == 1) // 点击确认或应用
	{
		SurfaceData[PARABOLOID][paraboloidNum] = tempSurface;
		addTreeChild(geometryTreeItem, paraboloidDialog->getName(),
			"Qt/images/paraboloid.png", PARABOLOID, paraboloidNum);

		// 如果Varible为空说明没有参数
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);
		paraboloidNum++;

		toModelActorChange(false);
		tempActor = NULL;
		tempSurface = NULL;
		if (index == 1)
		{
			isExistenceOpenWin = false;
			delete paraboloidDialog;
		}
		else
		{
			paraboloidDialog->setNameLineEdit("paraboloid" +
				QString::number(paraboloidNum + 1));
			tempSurface = new Paraboloid();
			toReceiveParaboloid(5);
		}

	}
	else if (index == 3) // 修改时确认或应用
	{
		rightSelectItem->setText(0, paraboloidDialog->getName()); // 更新子树的名字
		
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		//不能替换掉原来的
		//只能更改原来的
		*SurfaceData[PARABOLOID][id] = *tempSurface;
		delete paraboloidDialog;
		paraboloidDialog = NULL;
		isExistenceOpenWin = false;

		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempSurface,
			SurfaceData[PARABOLOID][id]);
		parameterization.deleteSurface(tempSurface);
		delete tempSurface;
		tempSurface = SurfaceData[PARABOLOID][id];
		toModelActorChange(false);

		QVariant var = rightSelectItem->data(3, Qt::UserRole);
		if (var.isValid())
		{
			SurfaceData[BOOLEANOP][var.toInt()]->updateData();
		}
		tempSurface = NULL;
		tempActor = NULL;

		updateRaySource();
	}
	else if(index == 0)
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete paraboloidDialog;
		paraboloidDialog = NULL;
		isExistenceOpenWin = false;
	}
	else if (index == 4)
	{
		// 恢复到修改前的状态
		delete tempSurface;
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempSurface = SurfaceData[PARABOLOID][id];
		toModelActorChange(false);
		tempSurface = NULL;
		delete paraboloidDialog;
		paraboloidDialog = NULL;
		isExistenceOpenWin = false;
	}
	
}

void mainWindow::createRectangle()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");

		return;
	}
	rectangleDialog = new RectangleWidget(this, isSet);
	rectangleDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(rectangleDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveRectangle(int)));

	if (isSet == 1) // 判断是否是新建还是修改 1为修改
	{
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// 将界面设置成选中的值
		rectangleDialog->setNameLineEdit(rightSelectItem->text(0));

		// 找到需要改变的模型
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = SurfaceData[RECTANGLE][id]->getActor();

		rectangleDialog->setWidgetData(SurfaceData[RECTANGLE][id]);

		isSet = 0;
	}
	else
	{
		rectangleDialog->setNameLineEdit("rectangle" +
			QString::number(rectangleNum + 1));
	}
	tempSurface = new Rectangle();
	isExistenceOpenWin = true;
	rectangleDialog->show();
	toReceiveRectangle(5);
}

void mainWindow::toReceiveRectangle(int index)
{
	bool ok;
	ok = rectangleDialog->getWidgetData(tempSurface,
		&parameterization);
	if (!ok) // 判断是否读取成功
		return;

	if (index == 5)
		toModelActorChange(true);
	else if (index == 2 || index == 1) // 点击确认或应用
	{
		SurfaceData[RECTANGLE][rectangleNum] = tempSurface;
		addTreeChild(geometryTreeItem, rectangleDialog->getName(),
			"Qt/images/rectangle.png", RECTANGLE, rectangleNum);

		// 如果Varible为空说明没有参数
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);
		rectangleNum++;

		toModelActorChange(false);
		tempActor = NULL;
		tempSurface = NULL;

		if (index == 1)
		{
			isExistenceOpenWin = false;
			delete rectangleDialog;
		}
		else
		{
			rectangleDialog->setNameLineEdit("rectangle" +
				QString::number(rectangleNum + 1));
			tempSurface = new Rectangle();
			toReceiveRectangle(5);
		}

	}
	else if (index == 3) // 修改时确认或应用
	{
		rightSelectItem->setText(0, rectangleDialog->getName()); // 更新子树的名字

		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		//不能替换掉原来的
		//只能更改原来的
		*SurfaceData[RECTANGLE][id] = *tempSurface;
		delete rectangleDialog;
		rectangleDialog = NULL;
		isExistenceOpenWin = false;

		// 将参数表中的临时surface换成数据中的surface
	    parameterization.swapSurface(tempSurface,
				SurfaceData[RECTANGLE][id]);
		parameterization.deleteSurface(tempSurface);
		delete tempSurface;
		tempSurface = SurfaceData[RECTANGLE][id];
		toModelActorChange(false);

		QVariant var = rightSelectItem->data(3, Qt::UserRole);
		if (var.isValid())
		{
			SurfaceData[BOOLEANOP][var.toInt()]->updateData();
		}
		tempSurface = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete rectangleDialog;
		rectangleDialog = NULL;
		isExistenceOpenWin = false;
	}
	else if (index == 4)
	{
		// 恢复到修改前的状态
		delete tempSurface;
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempSurface = SurfaceData[RECTANGLE][id];
		toModelActorChange(false);
		tempSurface = NULL;
		delete rectangleDialog;
		rectangleDialog = NULL;
		isExistenceOpenWin = false;
	}

}

void mainWindow::createCylinder()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning", 
			"A window has been opened. Please close and continue!");
		return;
	}
	cylinderDialog = new CylinderWidget(this, isSet);
	cylinderDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(cylinderDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveCylinder(int)));

	if (isSet == 1) // 判断是否是新建还是修改 1为修改
	{
		int paraModleIndex = rightSelectItem->data(2, Qt::UserRole).toInt();

		// 将界面设置成选中的值
		cylinderDialog->setNameLineEdit(rightSelectItem->text(0));

		// 找到需要改变的模型
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = SurfaceData[CYLINDER][id]->getActor();

		cylinderDialog->setWidgetData(SurfaceData[CYLINDER][id]);

		isSet = 0;
	}
	else
	{
		cylinderDialog->setNameLineEdit("cylinder" +
			QString::number(cylinderNum + 1));
	}
	tempSurface = new Cylinder();
	isExistenceOpenWin = true;
	cylinderDialog->show();
	toReceiveCylinder(5);
}

void mainWindow::toReceiveCylinder(int index)
{
	bool ok;
	ok = cylinderDialog->getWidgetData(tempSurface,
		&parameterization);
	if (!ok) // 判断是否读取成功
		return;

	if (index == 5)
		toModelActorChange(true);
	else if (index == 2 || index == 1) // 点击确认或应用
	{
		SurfaceData[CYLINDER][cylinderNum] = tempSurface;
		addTreeChild(geometryTreeItem, cylinderDialog->getName(),
			"Qt/images/cylinder.png", CYLINDER, cylinderNum);
		// 如果Varible为空说明没有参数
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);
		cylinderNum++;

		toModelActorChange(false);
		tempActor = NULL;
		tempSurface = NULL;
		if (index == 1)
		{
			isExistenceOpenWin = false;
			delete cylinderDialog;
		}
		else
		{
			cylinderDialog->setNameLineEdit("cylinder" +
				QString::number(cylinderNum + 1));
			tempSurface = new Cylinder();
			toReceiveCylinder(5);
		}

	}
	else if (index == 3) // 修改时确认或应用
	{
		rightSelectItem->setText(0, cylinderDialog->getName()); // 更新子树的名字
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		//不能替换掉原来的
		//只能更改原来的
		*SurfaceData[CYLINDER][id] = *tempSurface;
		delete cylinderDialog;
		cylinderDialog = NULL;
		isExistenceOpenWin = false;
		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempSurface,
			SurfaceData[CYLINDER][id]);
		parameterization.deleteSurface(tempSurface);
		delete tempSurface;
		tempSurface = SurfaceData[CYLINDER][id];
		toModelActorChange(false);

		QVariant var = rightSelectItem->data(3, Qt::UserRole);
		if (var.isValid())
		{
			SurfaceData[BOOLEANOP][var.toInt()]->updateData();
		}
		tempSurface = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete cylinderDialog;
		cylinderDialog = NULL;
		isExistenceOpenWin = false;
	}
	else if (index == 4)
	{
		// 恢复到修改前的状态
		delete tempSurface;
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempSurface = SurfaceData[CYLINDER][id];
		toModelActorChange(false);
		tempSurface = NULL;
		delete cylinderDialog;
		cylinderDialog = NULL;
		isExistenceOpenWin = false;
	}
}

void mainWindow::createSphere()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");
		return;
	}
	SphereDialog = new SphereWidget(this, isSet);
	SphereDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(SphereDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveSphere(int)));

	if (isSet == 1) // 判断是否是新建还是修改 1为修改
	{
		// 将界面设置成选中的值
		SphereDialog->setNameLineEdit(rightSelectItem->text(0));

		// 找到需要改变的模型
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = SurfaceData[SPHERE][id]->getActor();

		SphereDialog->setWidgetData(SurfaceData[SPHERE][id]);

		isSet = 0;
	}
	else
	{
		SphereDialog->setNameLineEdit("sphere" +
			QString::number(sphereNum + 1));
	}
	tempSurface = new Sphere();
	isExistenceOpenWin = true;
	SphereDialog->show();
	toReceiveSphere(5);
}

void mainWindow::toReceiveSphere(int index)
{
	bool ok;
	ok = SphereDialog->getWidgetData(tempSurface,
		&parameterization);
	if (!ok) // 判断是否读取成功
		return;

	if (index == 5)
		toModelActorChange(true);
	else if (index == 2 || index == 1) // 点击确认或应用
	{
		SurfaceData[SPHERE][sphereNum] = tempSurface;
		addTreeChild(geometryTreeItem, SphereDialog->getName(),
			"Qt/images/sphere.png", SPHERE, sphereNum);
		// 如果Varible为空说明没有参数
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);
		sphereNum++;

		toModelActorChange(false);
		tempActor = NULL;
		tempSurface = NULL;
		if (index == 1)
		{
			isExistenceOpenWin = false;
			delete SphereDialog;
		}
		else
		{
			SphereDialog->setNameLineEdit("sphere" +
				QString::number(sphereNum + 1));
			tempSurface = new Sphere();
			toReceiveSphere(5);
		}

	}
	else if (index == 3) // 修改时确认或应用
	{
		rightSelectItem->setText(0, SphereDialog->getName()); // 更新子树的名字
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		//不能替换掉原来的
		//只能更改原来的
		*SurfaceData[SPHERE][id] = *tempSurface;
		delete SphereDialog;
		SphereDialog = NULL;
		isExistenceOpenWin = false;

		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempSurface,
			SurfaceData[SPHERE][id]);
		parameterization.deleteSurface(tempSurface);
		delete tempSurface;
		tempSurface = SurfaceData[SPHERE][id];

		QVariant var = rightSelectItem->data(3, Qt::UserRole);
		if (var.isValid())
		{
			SurfaceData[BOOLEANOP][var.toInt()]->updateData();
		}
		toModelActorChange(false);
		tempSurface = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete SphereDialog;
		SphereDialog = NULL;
		isExistenceOpenWin = false;
	}
	else if (index == 4)
	{
		// 恢复到修改前的状态
		delete tempSurface;
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempSurface = SurfaceData[SPHERE][id];
		toModelActorChange(false);
		tempSurface = NULL;
		delete SphereDialog;
		SphereDialog = NULL;
		isExistenceOpenWin = false;
	}
}

void mainWindow::createBoolean()
{
	int nChild = geometryTreeItem->childCount();
	if (nChild == 0)
	{
		QMessageBox::warning(NULL, "Warning", "Not find any model!");
		return;
	}
	booleanDialog = new BooleanWidget(this, isSet);
	connect(booleanDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveBoolean(int)));

	QStringList allSurfaceName;
	for (int i = 0; i < nChild; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);

	if (isSet == 1)
	{
		allSurfaceName << rightSelectItem->child(0)->text(0);
		allSurfaceName << rightSelectItem->child(1)->text(0);
		booleanDialog->addAllSurface(allSurfaceName);
		booleanDialog->setComboboxIndex(nChild, nChild + 1);
		isSet = 0;
	}
	else
		booleanDialog->addAllSurface(allSurfaceName);


	tempSurface = new Boolean();
	booleanDialog->show();
	toReceiveBoolean(-1);
}

void mainWindow::toReceiveBoolean(int index)
{
	if (index != -1 && index != 3 && index != 1)  // 恢复
	{
		stackSurface.top()->updateProperty();
		stackSurface.pop();
		stackSurface.top()->updateProperty();
		stackSurface.pop();
	}
	if (index == 5 || index == -1)
	{
		int SelIndex1 = booleanDialog->getModel1();
		int SelIndex2 = booleanDialog->getModel2();
		int indexSum = geometryTreeItem->childCount();
		QTreeWidgetItem *tempTree;
		if (SelIndex1 < indexSum)
			tempTree = geometryTreeItem->child(SelIndex1);
		else
			tempTree = rightSelectItem->child(SelIndex1 - indexSum);

		int name1 = tempTree->data(1, Qt::UserRole).toInt();
		int index1 = tempTree->data(2, Qt::UserRole).toInt();

		if (SelIndex2 < indexSum)
			tempTree = geometryTreeItem->child(SelIndex2);
		else
			tempTree = rightSelectItem->child(SelIndex2 - indexSum);

		int name2 = tempTree->data(1, Qt::UserRole).toInt();
		int index2 = tempTree->data(2, Qt::UserRole).toInt();

		stackSurface.push(SurfaceData[name1][index1]);
		stackSurface.push(SurfaceData[name2][index2]);
		tempActor = SurfaceData[name1][index1]->getActor();
		tempActor->SetProperty(SelProperty);
	
		tempActor = SurfaceData[name2][index2]->getActor();
		tempActor->SetProperty(SelProperty);

	}
	else if (index == 1 || index == 3) // 3 表示修改时的确定
	{
		int SelIndex1 = booleanDialog->getModel1();
		int SelIndex2 = booleanDialog->getModel2();
		int indexSum = geometryTreeItem->childCount();
		QTreeWidgetItem *tempTree1, *tempTree2;
		if (SelIndex1 < indexSum)
			tempTree1 = geometryTreeItem->child(SelIndex1);
		else
			tempTree1 = rightSelectItem->child(SelIndex1 - indexSum);

		int name1 = tempTree1->data(1, Qt::UserRole).toInt();
		int index1 = tempTree1->data(2, Qt::UserRole).toInt();

		if (SelIndex2 < indexSum)
			tempTree2 = geometryTreeItem->child(SelIndex2);
		else
			tempTree2 = rightSelectItem->child(SelIndex2 - indexSum);

		int name2 = tempTree2->data(1, Qt::UserRole).toInt();
		int index2 = tempTree2->data(2, Qt::UserRole).toInt();

		Surface* a = SurfaceData[name1][index1];
		Surface* b = SurfaceData[name2][index2];

		int operation = booleanDialog->getOperation();
		if (operation == 1)
		{
			if (name1 == PARABOLOID)
			{
				a = SurfaceData[name1][index1];
				b = SurfaceData[name2][index2];
				operation = 3;
			}
			else if (name2 == PARABOLOID)
			{
				b = SurfaceData[name1][index1];
				a = SurfaceData[name2][index2];
				operation = 3;
			}
		}

		vector<double> data(8);
		data[7] = operation;
		tempSurface->setBoolean(a, b);
		tempSurface->setData(data);
		tempSurface->calPolyData();
		tempSurface->calActor();
		renderer->AddActor(tempSurface->getActor());

		QString nameOperation;
		if (operation == 0)
			nameOperation = "Union";
		else if (operation == 1 || operation == 3)
			nameOperation = "Intersection";
		else
			nameOperation = "Difference";

		parameterization.addParaSurface(a, tempSurface);
		parameterization.addParaSurface(b, tempSurface);
		if (index == 1)
		{
			SurfaceData[BOOLEANOP][booleanNum] = tempSurface;
			QTreeWidgetItem *tempTree = addTreeChild(geometryTreeItem,
				nameOperation + QString::number(booleanNum + 1),
				"Qt/images/boolean.png", BOOLEANOP, booleanNum);
			geometryTreeItem->removeChild(tempTree1);
			geometryTreeItem->removeChild(tempTree2);
			tempTree->addChild(tempTree1);
			tempTree->addChild(tempTree2);
			tempTree1->setData(3, Qt::UserRole, booleanNum);
			tempTree2->setData(3, Qt::UserRole, booleanNum);
			booleanNum++;
		}
		else
		{
			int id = rightSelectItem->data(3, Qt::UserRole).toInt();
			rightSelectItem->setText(0, nameOperation + QString::number(id + 1));
			QTreeWidgetItem *tempTree3 = rightSelectItem->child(0);
			QTreeWidgetItem *tempTree4 = rightSelectItem->child(1);

			SurfaceData[tempTree3->data(1, Qt::UserRole).toInt()]
				[tempTree3->data(2, Qt::UserRole).toInt()]->setIsShow(true);
			SurfaceData[tempTree4->data(1, Qt::UserRole).toInt()]
				[tempTree4->data(2, Qt::UserRole).toInt()]->setIsShow(true);

			rightSelectItem->removeChild(tempTree3);
			rightSelectItem->removeChild(tempTree4);
			geometryTreeItem->addChild(tempTree3);
			geometryTreeItem->addChild(tempTree4);
			tempTree3->setData(3, Qt::UserRole, "inval");
			tempTree4->setData(3, Qt::UserRole, "inval");

			tempTree1->setData(3, Qt::UserRole, id);
			tempTree2->setData(3, Qt::UserRole, id);

			geometryTreeItem->removeChild(tempTree1);
			geometryTreeItem->removeChild(tempTree2);
			rightSelectItem->addChild(tempTree1);
			rightSelectItem->addChild(tempTree2);

			renderer->RemoveActor(SurfaceData[BOOLEANOP][id]->getActor());
			*SurfaceData[BOOLEANOP][id] = *tempSurface;
			SurfaceData[BOOLEANOP][id]->setBoolean(a, b);
			parameterization.swapSurface(tempSurface,
				SurfaceData[BOOLEANOP][id]);
			parameterization.deleteSurface(tempSurface);
			delete tempSurface;
		}

		stackSurface.pop();
		stackSurface.pop();
		a->setIsShow(false);
		b->setIsShow(false);

		tempActor = NULL;
		tempSurface = NULL;
		delete booleanDialog;
		booleanDialog = NULL;
	}
	else
	{
		tempActor = NULL;
		delete booleanDialog;
		booleanDialog = NULL;
	}
	updateVtk();
}

void mainWindow::createSTLInput()
{
	if (isExistenceOpenWin)
	{
		// 已经有窗口打开了
		QMessageBox::warning(NULL, "Warning",
			"A window has been opened. Please close and continue!");
		return;
	}
	isExistenceOpenWin = true;
	STLInputDialog = new STLInputWidget(this, isSet);
	STLInputDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(STLInputDialog, SIGNAL(sendData(int)),
		this, SLOT(toReceiveSTLInput(int)));

	if (isSet == 0)
	{
		QString filename = QFileDialog::getOpenFileName(this,
			tr("Open the STL"),
			"",
			tr("*.stl"));
		if (!filename.isEmpty())
		{
			tempSurface = new STLInput(filename.toStdString());
			tempSurface->calPolyData();	

			STLInputDialog->setNameLineEdit(tr("STLInput") +
				QString::number(STLInputNum + 1));
			STLInputDialog->show();
			toReceiveSTLInput(5);
		}
		else
		{
			delete STLInputDialog;
			isExistenceOpenWin = false;
		}

	}
	else
	{
		isSet = 0;

		// 将界面设置成选中的值
		STLInputDialog->setNameLineEdit(rightSelectItem->text(0));

		// 找到需要改变的模型
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempActor = SurfaceData[STLINPUT][id]->getActor();
		STLInput *tempPtr = dynamic_cast<STLInput *>(SurfaceData[STLINPUT][id]);
		tempSurface = new STLInput(*tempPtr);
		//*tempSurface = *SurfaceData[STLINPUT][id];
		STLInputDialog->setWidgetData(SurfaceData[STLINPUT][id]);
		STLInputDialog->show();
		toReceiveSTLInput(5);
	}

}

void mainWindow::toReceiveSTLInput(int index)
{
	bool ok;
	ok = STLInputDialog->getWidgetData(tempSurface,
		&parameterization);
	if (!ok) // 判断是否读取成功
		return;

	if (index == 5)
		toModelActorChange(true);
	else if (index == 2 || index == 1) // 点击确认或应用
	{
		SurfaceData[STLINPUT][STLInputNum] = tempSurface;
		addTreeChild(geometryTreeItem, tr("STLInput") +
			QString::number(STLInputNum + 1),
			"Qt/images/stlInput.png", STLINPUT, STLInputNum);
		STLInputNum++;
		// 如果Varible为空说明没有参数
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		toModelActorChange(false);
		tempActor = NULL;
		tempSurface = NULL;

	
		isExistenceOpenWin = false;
		delete STLInputDialog;
		if (index == 2)
		{
			createSTLInput();
		}

	}
	else if (index == 3) // 修改时确认或应用
	{
		rightSelectItem->setText(0, STLInputDialog->getName()); // 更新子树的名字
		if (tempSurface->getVarible().empty())
			parameterization.deleteSurface(tempSurface);

		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		//不能替换掉原来的
		//只能更改原来的
		*SurfaceData[STLINPUT][id] = *tempSurface;
		delete STLInputDialog;
		STLInputDialog = NULL;
		isExistenceOpenWin = false;

		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempSurface,
			SurfaceData[STLINPUT][id]);
		parameterization.deleteSurface(tempSurface);
		delete tempSurface;
		tempSurface = SurfaceData[STLINPUT][id];
		toModelActorChange(false);

		QVariant var = rightSelectItem->data(3, Qt::UserRole);
		if (var.isValid())
		{
			SurfaceData[BOOLEANOP][var.toInt()]->updateData();
		}
		tempSurface = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		delete STLInputDialog;
		STLInputDialog = NULL;
		isExistenceOpenWin = false;
	}
	else if (index == 4)
	{
		// 恢复到修改前的状态
		delete tempSurface;
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempSurface = SurfaceData[STLINPUT][id];
		toModelActorChange(false);
		tempSurface = NULL;
		delete STLInputDialog;
		STLInputDialog = NULL;
		isExistenceOpenWin = false;
	}
}

void mainWindow::toModelActorChange(bool isTran) // 是否透明
{
	renderer->RemoveActor(tempActor);
	tempSurface->updateData();
	tempActor = tempSurface->getActor();
	if (isTran)
		tempActor->SetProperty(ChangeProperty);
	else
		tempSurface->updateProperty();

	renderer->AddActor(tempActor);

	updateVtk();
}

// ------------------- 计算函数和设置源 ------------------------------------
void mainWindow::createLightSource()
{
	QStringList allSurfaceName;
	int nChild1 = geometryTreeItem->childCount();
	for (int i = 0; i < nChild1; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);
	LightSourceDialog->addAllSurface(allSurfaceName);

	tempRaySource = new RaySource;
	if (isSet == 1)
	{
		// 找到需要改变的源
		int id = rightSelectItem->data(2, Qt::UserRole).toInt();
		disconnect(LightSourceDialog, SIGNAL(sendData(int)),
			this, SLOT(ToLightSource(int)));
		LightSourceDialog->setWidgetData(raySourceHash[id]);
		connect(LightSourceDialog, SIGNAL(sendData(int)),
			this, SLOT(ToLightSource(int)));
		LightSourceDialog->setNameLineEdit(rightSelectItem->text(0));
		LightSourceDialog->setWayButton(1);
		list<vtkActor*> listactor = raySourceHash[id]->getActors();
		for (auto x : listactor)
			stackActor.push(x);
		tempActor = raySourceHash[id]->getRayActor();
		isSet = 0;
	}
	else
	{
		LightSourceDialog->setWayButton(0);
		LightSourceDialog->setNameLineEdit("LightSource" +
			QString::number(LightSourceNum + 1));
		LightSourceDialog->init();
		ToLightSource(5);
	}
	
	LightSourceDialog->show();
}

void mainWindow::ToLightSource(int index)
{
	if (index == 5) // 定义矩形或锥形（点源）
	{
		tempIndex = 5;
		bool ok;
		if (LightSourceDialog->getType() == 2)  // 倒锤形
		{
			if (tempSurface != NULL)
				delete tempSurface;
			tempSurface = new ReverseCone();
		}
		else
		{
			if (tempSurface != NULL)
				delete tempSurface;
			tempSurface = new Rectangle();
		}

		// 由于这个数据不需要进入到参数列表中 定义个临时的tempPar
		Parameterization tempPar;
		ok = LightSourceDialog->getRectData(tempSurface, 
			&tempPar);
		if (!ok) // 判断是否读取成功
		{
			QMessageBox::warning(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		tempSurface->updateData();
		tempActor = tempSurface->getActor();
		tempActor->SetProperty(ChangeProperty);
		renderer->AddActor(tempActor);
	}
	else if (index == 7) // 定义射线个数，并临时显示出来
	{
		if (LightSourceDialog->getType() == 3)
			return;
		tempIndex = 7;
		bool ok;
		ok = LightSourceDialog->getWidgetData(tempRaySource);
		if (!ok) // 判断是否读取成功
			return;

		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		tempRaySource->calRayTrace();
		tempRaySource->copyPlane();
		tempRaySource->calRayActor();
		tempActor = tempRaySource->getRayActor();
		renderer->AddActor(tempActor);
	}
	else if (index == 2 || index == 6) // 点击应用
	{
		if (tempIndex != 7)
			ToLightSource(7);
		tempIndex = 2;
		// 读取计算信息
		if (LightSourceDialog->getType() == 3)
		{
			int SelSource = geometryTreeItem->child(LightSourceDialog->
				getSelSources())->data(2, Qt::UserRole).toInt();
			tempRaySource = raySourceHash[SelSource];
			tempActor = tempRaySource->getRayActor();
			tempRaySource->setLength(LightSourceDialog->getLength());
			list<vtkActor*> listactor = tempRaySource->getActors();
			for (auto x : listactor)
			{
				stackActor.push(x);
			}
		}

		renderer->RemoveActor(tempActor);
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		int NumSurface;
		int* treeChildIndex;
		LightSourceDialog->getSelSurfaces(NumSurface, treeChildIndex);

		int nameSel, indexSel;
		list<Surface*> SelSurface;
		for (int i = 0; i < NumSurface; i++)
		{
			nameSel = geometryTreeItem->child(treeChildIndex[i])->
				data(1, Qt::UserRole).toInt();
			indexSel = geometryTreeItem->child(treeChildIndex[i])->
				data(2, Qt::UserRole).toInt();
			SelSurface.push_back(SurfaceData[nameSel][indexSel]);
		}
		tempRaySource->setSelSurface(SelSurface);
		tempRaySource->updateData();

		list<vtkActor*> listactor = tempRaySource->getActors();
		for (auto x : listactor)
		{
			renderer->AddActor(x);
			stackActor.push(x);
		}
		tempActor = tempRaySource->getRayActor();
		renderer->AddActor(tempActor);

	}
	else if (index == 1 || index == 3)
	{
		if (tempIndex != 2) // 未点击应用按钮 
		{
			ToLightSource(2);
		}
		tempIndex = 1;

		if (index == 1)
		{
			raySourceHash[LightSourceNum] = tempRaySource;
			addTreeChild(lightTreeItem, LightSourceDialog->getName(),
				"Qt/images/lightSource.png", LIGHTSOURCE, LightSourceNum, 2);

			LightSourceNum++;
		}
		else
		{
			int id = rightSelectItem->data(2, Qt::UserRole).toInt();
			rightSelectItem->setText(0, LightSourceDialog->getName());
			RaySource *temp = raySourceHash[id];
			raySourceHash[id] = tempRaySource;
			delete temp;
		}
		tempRaySource = NULL;
		tempActor = NULL;
		while (!stackActor.empty())
		{
			stackActor.pop();
		}

		LightSourceDialog->close();
		tempActor = NULL;
		if (tempSurface != NULL)
		{
			delete tempSurface;
			tempSurface = NULL;
		}
	}
	else if (index == 0 || index == 4) // 关闭
	{
		if (index == 4)
		{
			int id = rightSelectItem->data(2, Qt::UserRole).toInt();
			list<vtkActor*> listactor = raySourceHash[id]->getActors();
			for (auto x : listactor)
				renderer->AddActor(x);
			renderer->AddActor(raySourceHash[id]->getRayActor());
		}
		renderer->RemoveActor(tempActor);
		tempActor = NULL;
		
		delete tempRaySource;
		tempRaySource = NULL;
	
		while (!stackActor.empty())
		{
			renderer->RemoveActor(stackActor.top());
			stackActor.pop();
		}
		delete tempRaySource;
		tempRaySource = NULL;
		LightSourceDialog->close();
	}
	updateVtk();
}

void mainWindow::createGaussian()
{
	GaussianDialog = new GaussianWidget(this, isSet);
	//GaussianDialog;
	GaussianDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(GaussianDialog, SIGNAL(sendData(int)),
		this, SLOT(ToGaussian(int)));

	GaussianDialog->show();

	if (isSet == 1)
	{
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();

		GaussianDialog->setWidgetData(fieldHash[Index]);
		GaussianDialog->setNameLineEdit(rightSelectItem->text(0));
		isSet = 0;

		tempImageActor = fieldHash[Index]->getActor();
		renderer->RemoveActor(fieldHash[Index]->getActor3D());
	}
	else
	{
		GaussianDialog->setNameLineEdit("Gaussian-Field" +
			QString::number(FieldNum + 1));

	}
	tempField = new Gaussain(Unit_factor);

}

void mainWindow::ToGaussian(int index)
{
	if (index == 2 || index == 6) // 表示点击应用
	{
		tempIndex = 2;
		bool ok;
		ok = GaussianDialog->getWidgetData(tempField,
			&parameterization);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		if (tempField->getVarible().empty())
			parameterization.deleteSurface(tempField);
	
		tempField->set3D(false);
		tempField->updateData();

		renderer->RemoveActor(tempImageActor);
		tempImageActor = tempField->getActor();

		renderer->AddActor(tempImageActor);

	}
	else if (index == 1)  // 点击确认
	{
		if (tempIndex != 2)// 点击应用后如无修改比不进行计算
		{
			ToGaussian(2);
			tempIndex = 1;
		}

		tempIndex = 1;

		fieldHash[FieldNum] = tempField;
		addTreeChild(sourceTreeItem, GaussianDialog->getName(),
			"Qt/images/Gaussian.png", GAUSSIAN, FieldNum, 1);

		FieldNum++;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;
	}
	else if (index == 3)   // 修改时的确认
	{
		if (tempIndex != 2) // 点击应用后如无修改比不进行计算
		{
			ToGaussian(2);
			tempIndex = 3;
		}

		tempIndex = 3;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempImageActor = fieldHash[Index]->getActor();
		renderer->RemoveActor(tempImageActor);

		*dynamic_cast<Gaussain*>(fieldHash[Index]) = *dynamic_cast<Gaussain*>(tempField);

		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempField,
			fieldHash[Index]);
		parameterization.deleteSurface(tempField);

		delete tempField;

		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		tempIndex = 0;
		renderer->RemoveActor(tempActor);
		renderer->RemoveActor(tempImageActor);
		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
	}
	else if (index == 4)  // 表示修改时的关闭 恢复到修改前的状态
	{
		tempIndex = 4;
		renderer->RemoveActor(tempImageActor);

		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		if(fieldHash[Index]->get3D())
			renderer->AddActor(fieldHash[Index]->getActor3D());
		else
			renderer->AddActor(fieldHash[Index]->getActor());

		delete tempField;
		tempField = NULL;
		delete GaussianDialog;
		GaussianDialog = NULL;
	}
	updateVtk();
}

void mainWindow::createApertureField()
{
	ApertureFieldDialog = new ApertureFieldWidget(this, isSet);

	ApertureFieldDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶
	connect(ApertureFieldDialog, SIGNAL(sendData(int)),
		this, SLOT(ToApertureField(int)));

	if (isSet == 1)
	{
		isSet = 0;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		ApertureFieldDialog->setWidgetData(dynamic_cast<ApertureField*>
			(fieldHash[Index]));
		ApertureFieldDialog->setNameLineEdit(rightSelectItem->text(0));
		tempImageActor = fieldHash[Index]->getActor();
		renderer->RemoveActor(fieldHash[Index]->getActor3D());
	}
	else
	{
		ApertureFieldDialog->setNameLineEdit("Aperture-Field"
			+ QString::number(FieldNum + 1));
		ToApertureField(5);
	}

	ApertureFieldDialog->show();
	tempField = new ApertureField(Unit_factor);
}

void mainWindow::ToApertureField(int index)
{
	if (index == 5) // 定义矩形
	{
		bool ok;
		if (tempSurface != NULL)
			delete tempSurface;
		tempSurface = new Rectangle();

		ok = ApertureFieldDialog->getRectData(tempSurface);
		if (!ok) // 判断是否读取成功
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, please enter number!");
			return;
		}
		tempSurface->updateData();
		renderer->RemoveActor(tempActor);
		tempActor = tempSurface->getActor();
		tempActor->SetProperty(ChangeProperty);
		renderer->AddActor(tempActor);
	}
	else if (index == 2 || index == 6) // 点击应用
	{
		ApertureField * tempAperture = dynamic_cast<ApertureField*>(tempField);
		tempIndex = 2;
		bool ok;
		ok = ApertureFieldDialog->getWidgetData(tempAperture,&parameterization);
		if (!ok)
		{
			QMessageBox::about(NULL, "Warning", "Invalid data, please enter number!");
			return;
		}
		if (tempField->getVarible().empty())
			parameterization.deleteSurface(tempField);
		// 导入ExEy
		tempAperture->readExEy();

		renderer->RemoveActor(tempActor);
		tempField->updateData();
		renderer->AddActor(tempField->getActor());
	}
	else if (index == 1)  // 点击确认
	{
		if (tempIndex != 2) // 未点击应用按钮 
		{
			ToApertureField(2);
			tempIndex = 1;
		}
		fieldHash[FieldNum] = tempField;
		addTreeChild(sourceTreeItem, ApertureFieldDialog->getName(),
			"Qt/images/Aperture.png", APERTURE, FieldNum, 1);

		FieldNum++;
		tempField = NULL;
		delete ApertureFieldDialog;
		ApertureFieldDialog = NULL;
		tempActor = NULL;
		delete tempSurface;
		tempSurface = NULL;
	}
	else if (index == 3)   // 修改时的确认
	{
		if (tempIndex != 2) // 未点击应用按钮 
		{
			ToApertureField(2);
			tempIndex = 1;
		}

		tempIndex = 3;
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		rightSelectItem->setText(0,ApertureFieldDialog->getName());
		rightSelectItem->setText(0, ApertureFieldDialog->getName());
		tempImageActor = fieldHash[Index]->getActor();
		renderer->RemoveActor(tempImageActor);

		*dynamic_cast<ApertureField*>(fieldHash[Index]) = 
			*dynamic_cast<ApertureField*>(tempField);

		// 将参数表中的临时surface换成数据中的surface
		parameterization.swapSurface(tempField,
			fieldHash[Index]);
		parameterization.deleteSurface(tempField);

		delete tempField;
		tempField = NULL;
		delete ApertureFieldDialog;
		ApertureFieldDialog = NULL;
		tempActor = NULL;
	}
	else if (index == 0)
	{
		tempIndex = 0;
		renderer->RemoveActor(tempActor);
		delete tempField;
		tempField = NULL;
		delete ApertureFieldDialog;
		ApertureFieldDialog = NULL;
		tempActor = NULL;
		delete tempSurface;
		tempSurface = NULL;
	}
	else if (index == 4)  // 表示修改时的关闭 恢复到修改前的状态
	{
		tempIndex = 4;
		renderer->RemoveActor(tempImageActor);
		renderer->RemoveActor(tempActor);
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		if (fieldHash[Index]->get3D())
			renderer->AddActor(fieldHash[Index]->getActor3D());
		else
			renderer->AddActor(fieldHash[Index]->getActor());

		delete tempField;
		tempField = NULL;
		delete ApertureFieldDialog;
		ApertureFieldDialog = NULL;
	}
	updateVtk();
}

void mainWindow::createCalculation()
{
	if (FieldNum == 0)
	{
		QMessageBox::warning(NULL, "Warning", "Please add a source first!");
		return;
	}
	calculationDialog = new CalculationWidget(this);
	connect(calculationDialog, SIGNAL(sendData(int)),
		this, SLOT(ToCalculation(int)));
	//calculationDialog->setWindowFlags(Qt::WindowStaysOnTopHint); // 子窗口保持置顶

	// 找到所有的面
	int nChild1 = geometryTreeItem->childCount();
	int nChild2 = sourceTreeItem->childCount();
	QStringList allSurfaceName;
	QStringList allSourcesName;
	//vector<int> treeChildIndexOfRec;

	for (int i = 0; i < nChild1; i++)
		allSurfaceName << geometryTreeItem->child(i)->text(0);

	for (int i = 0; i < nChild2; i++)
		allSourcesName << sourceTreeItem->child(i)->text(0);

	calculationDialog->addAllSurface(allSurfaceName);
	calculationDialog->addAllSources(allSourcesName);
	//calculationDialog->addAllRectangles(allRectangleName);
	calculationDialog->setResRectangle(fieldHash.begin()->second->getWidth(),
		fieldHash.begin()->second->getDepth());

	calculationDialog->show();
	//delete calculationDialog;
}

void mainWindow::ToCalculation(int index)
{
	if (index == 1)
	{
		// 读取计算信息
		int NumSurface, NumRectangle;
		int* treeChildIndex;
		int sourceIndex;

		// 剖分信息 
		int meshsize = calculationDialog->getMeshSize();

		// 计算模式信息 剖分优先还是解析解优先
		int calmodel = calculationDialog->getCalMod();

		// 结果面
		calculationDialog->getSelSurfaces(NumSurface, treeChildIndex);
		sourceIndex = calculationDialog->getSelSources();

		// 得到最后一个面的信息
		double *data = new double[3];
		double dis, width, depth;
		bool isdefualt;
		if (!calculationDialog->getResRectangle(isdefualt, data))
		{
			QMessageBox::warning(NULL, "Warning", "Please add a source first!");
			return;
		}
		if (isdefualt)  // 判断是否为默认
		{
			dis = 1;
			width = fieldHash[sourceIndex]->getWidth();
			depth = fieldHash[sourceIndex]->getDepth();
		}
		else
		{
			dis = data[0];
			width = data[1];
			depth = data[2];
		}

		double fre = calculationDialog->getFre() * 1e9;

		int *nameSel = new int[NumSurface];
		int *indexSel = new int[NumSurface];

		QString name;
		sourceIndex = sourceTreeItem->child(sourceIndex)->data(2, Qt::UserRole).toInt();
		name = sourceTreeItem->child(sourceIndex)->text(0);
		name += tr("_");
		for (int i = 0; i < NumSurface; i++)
		{
			nameSel[i] = geometryTreeItem->child(treeChildIndex[i])->data(1, Qt::UserRole).toInt();
			indexSel[i] = geometryTreeItem->child(treeChildIndex[i])->data(2, Qt::UserRole).toInt();
			name += geometryTreeItem->child(treeChildIndex[i])->text(0);
			name += tr("_");
		}
		//name +=
		name += QString::number(dis);
		calName.push(name);
		CalculateThread *calThr = new CalculateThread(
			fre,                     // 频率
			Unit_factor,             // 比例尺
			nameSel, indexSel,       // 选择的反射面信息 
			NumSurface,              // 选择的反射面数量 
			&SurfaceData,            // 所有模型数据
			fieldHash[sourceIndex],  // 起始源
			dis, width, depth,       // 得到的最后一个面的信息
			meshsize,                // 剖分密度信息 
			calmodel                 // 计算模式信息 剖分优先还是解析解优先
		);

		delete[] treeChildIndex;

		connect(calThr, SIGNAL(sendField(Field*)), this, SLOT(receiveField(Field*)));
		connect(calThr, SIGNAL(finished()), calThr, SLOT(deleteLater()));
		calThr->start();
		
		progDlg = new QProgressDialog();
		progDlg->setWindowTitle("Please wait...");
		progDlg->setFixedWidth(200);
		progDlg->setRange(0, 100);
		progDlg->setWindowFlags(Qt::WindowStaysOnTopHint);
		progDlg->show();
		timer = new QTimer();
		currentValue = 0;
		progDlg->setValue(currentValue);
		connect(timer, SIGNAL(timeout()), this, SLOT(updateProgressDialog()));
		timer->start(50);

	}
	else if (index == 2) // check 
	{
		while (!stackCheckActor.empty())
		{
			renderer->RemoveActor(stackCheckActor.top());
			stackCheckActor.pop();
		}

		// 读取计算信息
		int NumSurface, NumRectangle;

		// 计算模式信息 剖分优先还是解析解优先
		int calmodel = calculationDialog->getCalMod();

		int* treeChildIndex;

		//modleID ResRectangle;  // 结果面
		calculationDialog->getSelSurfaces(NumSurface, treeChildIndex);
		int sourceIndex = calculationDialog->getSelSources();
		sourceIndex = sourceTreeItem->child(sourceIndex)->data(2, Qt::UserRole).toInt();

		double fre = calculationDialog->getFre() * 1e9;
		PVVA pvva;
		// 设置单位
		pvva.setUnit(Unit_factor);
		// 设置频率
		pvva.setFre(fre);
		// 读入源并分配内存
		pvva.setSource(fieldHash[sourceIndex]); //?
		Vector3 point1, n_point1;
		Vector3 point2, n_point2;
		pvva.getSourcePoint(point2, n_point2);
		for (int i = 0; i < NumSurface; i++)
		{
			int nameSel = geometryTreeItem->child(treeChildIndex[i])->data(1, Qt::UserRole).toInt();
			int indexSel = geometryTreeItem->child(treeChildIndex[i])->data(2, Qt::UserRole).toInt();

			PVVA pvva1;
			// 设置单位
			pvva1.setUnit(Unit_factor);
			// 设置频率
			pvva1.setFre(fre);
			pvva1.setSourcePoint(point2, n_point2);
			point1 = point2;
			n_point1 = n_point2;

			pvva1.setModle(SurfaceData[nameSel][indexSel]->getPolyData());
			if (!pvva1.CheckModle(point2, n_point2))
			{
				QString a = "There is no intersection between the ";
				a += QString::number(i + 1);
				a += "th surface and the source. \n Please check it!";
				QMessageBox::warning(NULL, "Warning", a);
				while (!stackCheckActor.empty())
				{
					renderer->RemoveActor(stackCheckActor.top());
					stackCheckActor.pop();
				}
				return;
			}
			tempActor = Surface::getLineActor(point1, point2);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		double *data = new double[3];
		bool isdefualt;
		GraphTrans graphTrans = PVVA::getSourceGraphTrans(n_point2);
		if (!calculationDialog->getResRectangle(isdefualt, data))
		{
			QMessageBox::warning(NULL, "Warning", "Please add a source first!");
			while (!stackCheckActor.empty())
			{
				renderer->RemoveActor(stackCheckActor.top());
				stackCheckActor.pop();
			}
			return;
		}
		if (isdefualt) // 默认值
		{
			tempActor = Surface::getLineActor(point2, point2 + n_point2);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
			graphTrans.updateTranslate(point2 + n_point2);
			tempActor = Surface::getRectangleActor(graphTrans, 
				fieldHash[sourceIndex]->getWidth(),
				fieldHash[sourceIndex]->getDepth());
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		else // 用户自定义
		{
			tempActor = Surface::getLineActor(point2, point2 + n_point2*data[0]);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
			graphTrans.updateTranslate(point2 + n_point2*data[0]);
			tempActor = Surface::getRectangleActor(graphTrans, data[1], data[2]);
			stackCheckActor.push(tempActor);
			renderer->AddActor(tempActor);
		}
		delete[] data;
		tempActor = NULL;
		updateVtk();
		QMessageBox::warning(NULL, "OK", "OK");

	}
	else
	{
		while (!stackCheckActor.empty())
		{
			renderer->RemoveActor(stackCheckActor.top());
			stackCheckActor.pop();
		}
		delete calculationDialog;
	}
}

void mainWindow::receiveField(Field * field)
{
	timer->stop();//停止定时器
	if (currentValue != 100)
		currentValue = 100;
	progDlg->setValue(currentValue);//进度达到最大值
	delete progDlg;//关闭进度对话框
	delete timer;

	field->set3D(false);
	field->calActor();
	renderer->AddActor(field->getActor());

	fieldHash[FieldNum] = field;

	updateVtk();

	QString name = calName.front();
	calName.pop();
	QTreeWidgetItem * child = addTreeChild(sourceTreeItem,
		tr("Result")+QString::number(++ResFieldNum),
		"Qt/images/Field.png", RESFIELD, FieldNum++, 1);
	child->setStatusTip(1, name);
	FieldNum++;
}

void mainWindow::updateProgressDialog()
{
	currentValue += 2;
	if (currentValue >= 100)
		currentValue = 0;
	progDlg->setValue(currentValue);
	QCoreApplication::processEvents();//避免界面冻结
	if (progDlg->wasCanceled())
		progDlg->setHidden(true);//隐藏对话框
}

// ---------------------- 左键右键函数 ------------------------------------
void mainWindow::on_treeWidget_ContextMenuRequested(QPoint pos)
{
	rightSelectItem = treeWidget->itemAt(pos);
	if (rightSelectItem == NULL)
	{
		return;
	}
	QVariant var = rightSelectItem->data(0, Qt::UserRole);

	if (var == 0)      //data(...)返回的data已经在之前建立节点时用setdata()设置好  
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->addAction(transparencyAction);
			R_Tree_compenents_childMenu->addSeparator();
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
			R_Tree_compenents_childMenu->addAction(isShowAction);
			int varName = rightSelectItem->data(1, Qt::UserRole).toInt();
			int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
			transparencyAction->setChecked(SurfaceData[varName][varIndex]->getIsTransparent());
			isShowAction->setChecked(SurfaceData[varName][varIndex]->getIsShow());
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);
			R_Tree_compenents_childMenu->removeAction(AddVariableAction);
		}
		//菜单出现的位置为当前鼠标的位置  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if (var == 1)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->addAction(Field2lightAction);
			R_Tree_compenents_childMenu->addAction(SaveFieldAction);
			R_Tree_compenents_childMenu->addAction(isShowAction);
			int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
			isShowAction->setChecked(fieldHash[varIndex]->getIsShow());

			R_Tree_compenents_childMenu->removeAction(AddVariableAction);
			R_Tree_compenents_childMenu->addSeparator();
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
		}
		//菜单出现的位置为当前鼠标的位置  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if (var == 2)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);
			R_Tree_compenents_childMenu->removeAction(AddVariableAction);
			R_Tree_compenents_childMenu->addAction(isShowAction);
			int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
			isShowAction->setChecked(raySourceHash[varIndex]->getIsShow());
			R_Tree_compenents_childMenu->addSeparator();
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
		}
		//菜单出现的位置为当前鼠标的位置  
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if (var == 3)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->removeAction(SetGeometryAction);
			R_Tree_compenents_childMenu->removeAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);

			R_Tree_compenents_childMenu->addAction(AddVariableAction);
		}
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
	else if (var == 4)
	{
		if (R_BlankMenu->isEmpty())
		{
			R_Tree_compenents_childMenu->removeAction(transparencyAction);
			R_Tree_compenents_childMenu->removeAction(Field2lightAction);
			R_Tree_compenents_childMenu->removeAction(SaveFieldAction);

			R_Tree_compenents_childMenu->addAction(DeleteGeometryAction);
			R_Tree_compenents_childMenu->addAction(AddVariableAction);
			R_Tree_compenents_childMenu->addSeparator();
			R_Tree_compenents_childMenu->addAction(SetGeometryAction);
		}
		R_Tree_compenents_childMenu->exec(QCursor::pos());
	}
}

void mainWindow::on_treeWidget_leftPressed(QTreeWidgetItem * item, int column)
{
	if (item->parent() != NULL)     // 根节点
		if (qApp->mouseButtons() == Qt::LeftButton)
		{
			//RectangleWidget dialog(this, isSet); 
			if (item->data(0, Qt::UserRole) == 1)
			{
				int name = item->data(1, Qt::UserRole).toInt();
				int index = item->data(2, Qt::UserRole).toInt();
				leftSelectItem = item;
				showDetails(name, index);
			}

		}
}

void mainWindow::on_Details_FieldClicked()
{
	int index = leftSelectItem->data(2, Qt::UserRole).toInt();
	int dim = dimensionGroupBtn->checkedId();
	if (dim == 0) // 3D
	{
		fieldHash[index]->set3D(true);
		renderer->RemoveActor(fieldHash[index]->getActor());
		renderer->RemoveActor(fieldHash[index]->getActor3D());
		fieldHash[index]->setShowPara(fieldGroupBtn->checkedId(),
			powerGroupBtn->checkedId(), pmGroupBtn->checkedId());
		magnitudeBtn->setChecked(true);
		phaseBtn->setEnabled(false);
		magnitudeBtn->setEnabled(false);
		fieldHash[index]->calActor3D();
		renderer->AddActor(fieldHash[index]->getActor3D());
	}
	else // 2D
	{
		fieldHash[index]->set3D(false);
		phaseBtn->setEnabled(true);
		magnitudeBtn->setEnabled(true);

		int i = pmGroupBtn->checkedId();
		if (i == 0)  // 相位没有db形式
		{
			linearBtn->setEnabled(true);
			dbBtn->setEnabled(true);
		}
		else
		{
			linearBtn->setEnabled(false);
			dbBtn->setEnabled(false);
		}
		fieldHash[index]->setShowPara(fieldGroupBtn->checkedId(), powerGroupBtn->checkedId(), i);

		renderer->RemoveActor(fieldHash[index]->getActor());
		renderer->RemoveActor(fieldHash[index]->getActor3D());
		fieldHash[index]->calActor();
		renderer->AddActor(fieldHash[index]->getActor());
	}

	updateVtk();
}

void mainWindow::SetGeometryPara()
{
	isSet = 1;
	if (rightSelectItem->data(0, Qt::UserRole) == 4)
	{
		AddVariable();
		return;
	}
	int var = rightSelectItem->data(1, Qt::UserRole).toInt();
	switch (var)
	{
	case PARABOLOID:
		createParaboloid();
		break;
	case RECTANGLE:
		createRectangle();
		break; 
	case SPHERE:
		createSphere();
			break;
	case CYLINDER:
		createCylinder();
		break;
	case BOOLEANOP:
		createBoolean();
		break;
	case STLINPUT:
		createSTLInput();
		break;
	case LIGHTSOURCE:
		createLightSource();
		break;
	case GAUSSIAN:
		createGaussian();
		break;
	case APERTURE:
		createApertureField();
		break;
	default:
		break;
	}
	isSet = 0;
}

void mainWindow::DeleteGeometry()
{
	if (rightSelectItem->data(0, Qt::UserRole) == 4)
	{
		string temp = rightSelectItem->text(0).toStdString();
		int index = temp.find('=');
		string tempName = temp.substr(0, index - 1);
		parameterization.deletePara(tempName);
		variablesTreeItem->removeChild(rightSelectItem);
		return;
	}
	QVariant varName = rightSelectItem->data(1, Qt::UserRole);
	QVariant varIndex = rightSelectItem->data(2, Qt::UserRole);
	if (varName == PARABOLOID || varName == RECTANGLE || varName == CYLINDER
		|| varName == STLINPUT || varName == SPHERE)
	{
		geometryTreeItem->removeChild(rightSelectItem);
		renderer->RemoveActor(SurfaceData[varName.toInt()][varIndex.toInt()]
			->getActor());
		// 先删除参数关联
		parameterization.deleteSurface(SurfaceData[varName.toInt()][varIndex.toInt()]);
		SurfaceData[varName.toInt()].erase(varIndex.toInt());
	}
	else if (varName == LIGHTSOURCE)
	{
		lightTreeItem->removeChild(rightSelectItem);
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		tempRaySource = raySourceHash[Index];
		raySourceHash.erase(Index);
		list<vtkActor*> listactor = tempRaySource->getActors();
		for (auto x : listactor)
		{
			renderer->RemoveActor(x);
		}
		renderer->RemoveActor(tempRaySource->getRayActor());
		delete tempRaySource; 
		tempRaySource = NULL;
	}
	else if (varName == GAUSSIAN || APERTURE)
	{
		sourceTreeItem->removeChild(rightSelectItem);
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		renderer->RemoveActor(fieldHash[Index]->getActor3D());
		renderer->RemoveActor(fieldHash[Index]->getActor());
		fieldHash.erase(Index);

	}
	updateVtk();// 更新opengl
}

void mainWindow::AddVariable()
{
	AddVariableWidget AddVariableDialog(this, isSet);
	bool isNew = true;
	string tempName;
	if (isSet == 1)
	{
		string temp = rightSelectItem->text(0).toStdString();
		int index = temp.find('=');
		tempName = temp.substr(0, index-1);
		string tempEpx = temp.substr(index + 2);
		AddVariableDialog.setName(tempName);
		AddVariableDialog.setExpress(tempEpx);
		isSet = 0;
		isNew = false;
	}
	AddVariableDialog.exec();
	if (AddVariableDialog.Num() == 1)
	{
		if (isNew)
		{
			QTreeWidgetItem *child;
			child = new QTreeWidgetItem;
			child->setText(0, AddVariableDialog.getName() +
				tr(" = ") + AddVariableDialog.getExpress());
			child->setData(0, Qt::UserRole, QVariant(4));
			variablesTreeItem->addChild(child);
			variablesTreeItem->setExpanded(true);

			parameterization.addPara(AddVariableDialog.getName().toStdString(),
				AddVariableDialog.getExpress().toStdString());
		}
		else
		{
			rightSelectItem->setText(0, AddVariableDialog.getName() +
				tr(" = ") + AddVariableDialog.getExpress());

			variablesTreeItem->setExpanded(true);

			parameterization.addPara(AddVariableDialog.getName().toStdString(),
				AddVariableDialog.getExpress().toStdString());
			parameterization.updateSurface(AddVariableDialog.getName().toStdString());
			updateRaySource();
			//updateSource();
		}
			
	}

}

void mainWindow::SaveField()
{
	QString filename = QFileDialog::getExistingDirectory(this,
		"Select the path to save field...", "./");
	if (!filename.isEmpty())
	{
		int Index = rightSelectItem->data(2, Qt::UserRole).toInt();
		fieldHash[Index]->save(filename.toStdString() + "//" +
			rightSelectItem->text(0).toStdString());
	}
}

void mainWindow::SetTransparency()
{
	int varName = rightSelectItem->data(1, Qt::UserRole).toInt();
	int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
	if (transparencyAction->isChecked()) //  选择是否透明
	{
		SurfaceData[varName][varIndex]->setIsTransparent(true);
	}
	else
	{
		SurfaceData[varName][varIndex]->setIsTransparent(false);

	}

	updateVtk();
}

void mainWindow::SetShow()
{
	int varName = rightSelectItem->data(1, Qt::UserRole).toInt();
	int varIndex = rightSelectItem->data(2, Qt::UserRole).toInt();
	VaribleBase * temp;
	if (varName == LIGHTSOURCE)
		temp = raySourceHash[varIndex];
	else if (varName == GAUSSIAN || varName == APERTURE || varName == RESFIELD)
	{
		temp = fieldHash[varIndex];
	}
	else
	{
		temp = SurfaceData[varName][varIndex];
	}
	if (isShowAction->isChecked()) //  选择是否透明
		temp->setIsShow(true);
	else
		temp->setIsShow(false);
	updateVtk();
}


void mainWindow::showDetails(int name, int index)
{
	if (name == GAUSSIAN || name == APERTURE || name == RESFIELD)
	{
		int index = leftSelectItem->data(2, Qt::UserRole).toInt();
		int content; bool isLinear; bool isPhs;
		fieldHash[index]->getShowPara(content, isLinear, isPhs);

		switch (fieldHash[index]->get3D())
		{
		case 0:
			TwoDBtn->setChecked(true);
			phaseBtn->setEnabled(true);
			magnitudeBtn->setEnabled(true);
			break;
		case 1:
			ThreeDBtn->setChecked(true);
			magnitudeBtn->setChecked(true);
			phaseBtn->setEnabled(false);
			magnitudeBtn->setEnabled(false);
			break;
		default:
			break;
		}

		switch (content)
		{
		case 0:
			ExBtn->setChecked(true);
			break;
		case 1:
			EyBtn->setChecked(true);
			break;
		case 2:
			EzBtn->setChecked(true);
			break;
		case 3:
			HxBtn->setChecked(true);
			break;
		case 4:
			HyBtn->setChecked(true);
			break;
		case 5:
			HzBtn->setChecked(true);
			break;
		default:
			break;
		}

		switch (isPhs)
		{
		case true:
			phaseBtn->setChecked(true);
			linearBtn->setEnabled(false);
			dbBtn->setEnabled(false);
			break;
		case false:
			magnitudeBtn->setChecked(true);
			linearBtn->setEnabled(true);
			dbBtn->setEnabled(true);
			break;
		default:
			break;
		}

		switch (isLinear)
		{
		case true:
			linearBtn->setChecked(true);
			break;
		case false:
			dbBtn->setChecked(true);
			break;
		default:
			break;
		}

		if (name == GAUSSIAN || name == APERTURE)  // 如果是源 EzHxHyHz 不让点
		{
			EzBtn->setEnabled(false);
			HxBtn->setEnabled(false);
			HyBtn->setEnabled(false);
			HzBtn->setEnabled(false);
		}
		else
		{
			EzBtn->setEnabled(true);
			HxBtn->setEnabled(true);
			HyBtn->setEnabled(true);
			HzBtn->setEnabled(true);
		}
	}
	showDetails();
}

void mainWindow::showDetails()
{
	detailsDockWidget->show();
}

// -------------------- vtk  ------------------------------------
void mainWindow::updateVtk()
{
	auto window = widget.GetRenderWindow();

	window->AddRenderer(renderer);
	window->Render();
}

void mainWindow::updateRaySource()
{
	if (!raySourceHash.empty())
	{
		for (auto x : raySourceHash)
			x.second->updateData();
	}
}
