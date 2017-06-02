#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtWidgets/QMainWindow>
#include <QVTKWidget.h>
#include <QMenuBar>
#include <QMenu>
#include <QToolBar>
#include <QAction>
#include <QWidget>
#include <QDockWidget>
#include <QTreeWidget>
#include <QLabel>
#include <QComBobox>
#include <QstatusBar>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QProgressDialog>
#include <QTimer>

#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkAxesActor.h>

#include <list>
#include <unordered_map>
#include <queue>

#include "VTK/include/Surface.h"
#include "VTK/include/Paraboloid.h"
#include "VTK/include/Rectangle.h"
#include "VTK/include/Cylinder.h"
#include "VTK/include/Sphere.h"
#include "VTK/include/Boolean.h"
#include "VTK/include/STLInput.h"
#include "VTK/include/ReverseCone.h"
#include "VTK/include/RaySource.h"
#include "VTK/include/Field.h"
#include "VTK/include/Gaussain.h"

#include "Qt/include/ParaboloidWidget.h"
#include "Qt/include/rectanglewidget.h"
#include "Qt/include/CylinderWidget.h"
#include "Qt/include/SphereWidget.h"
#include "Qt/include/BooleanWidget.h"
#include "Qt/include/stlInputWidget.h"
#include "Qt/include/LightSourceWidget.h"
#include "Qt/include/GaussianWidget.h"
#include "Qt/include/ApertureFieldWidget.h"
#include "Qt/include/CalculationWidget.h"

#include "Tools/GraphTrans.h"
#include "Tools/ModelID.h"
#include "Tools/Parameterization.h"

#include"calculation/include/pvva.h"

class mainWindow : public QMainWindow
{
	Q_OBJECT

public:
	mainWindow(QWidget *parent = 0);
	~mainWindow();

private:
	void init();

	//------------------- 窗口设置------------------- 
	void createActions();
	void createMenus(); // 菜单
	void createToolBars();
	void createStatusBar();
	void createDetails();
	void creareWindows(); // 先创建好窗口 （由于没有解决delete的问题 采用这个方法避免）


	void createTreeWidgetItem(); // 创建tree
	void createRightMenu(); // 右键菜单

	// 添加子树
	QTreeWidgetItem * addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, int name, int index, int rightPar = 0);


private slots:
    // ------------------- 模型函数 ------------------------------------
	void createParaboloid();    // 抛物面
	void toReceiveParaboloid(int);

	void createRectangle(); // 矩形
	void toReceiveRectangle(int index);

	void createCylinder();   // 圆柱
	void toReceiveCylinder(int index);

	void createSphere();   //  球
	void toReceiveSphere(int index);

	void createBoolean();  // bool运算
	void toReceiveBoolean(int index);

	void createSTLInput();  // stl
	void toReceiveSTLInput(int index);  // stl

	void toModelActorChange(bool); // 接收模型窗口的变换

    // ------------------- 计算函数 ------------------------------------
	void createLightSource();  // 光线追踪
	void ToLightSource(int index);

	void createGaussian();      // 高斯
	void ToGaussian(int index);

	void createApertureField(); // 口面场
	void ToApertureField(int index);

	void createCalculation();
	void ToCalculation(int index);
	void receiveField(Field*);
	void updateProgressDialog();

    // ------------------- 左键右键函数 ----------------------------------
	void on_treeWidget_ContextMenuRequested(QPoint pos);// 右键菜单
	void on_treeWidget_leftPressed(QTreeWidgetItem *item, int column);
	void on_Details_FieldClicked(); //details Field 

	void SetGeometryPara();  //修改参数
	void DeleteGeometry(); // 删除模型
	void AddVariable();
	void SaveField(); // 保存Field
	void SetTransparency(); // 设置成透明
	void SetShow(); // 设置是否显示

private:

	// 改变3D显示窗口
	void showDetails(int, int);
	void showDetails();

	void updateVtk();
	void updateRaySource();

private:
	//------------- VTK ------------------- 
	QVTKWidget widget; // vtk 显示窗口
	vtkSmartPointer<vtkRenderWindowInteractor> interactor; // 交互
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkAxesActor> axes; // 坐标
	vtkSmartPointer<vtkProperty> SelProperty; // 选中时的颜色和透明度
	vtkSmartPointer<vtkProperty> ChangeProperty; // 修改时的颜色和透明度

	//std::list<vtkActor*> actors; // 保存所有演员

	QDockWidget * leftWidget; //放置treeWidget的窗口
	QTreeWidget * treeWidget;

	QDockWidget * detailsDockWidget;
	QWidget * detailsWidget;

	QLabel * locationLabel;  // 状态栏标签
	QPushButton * unitBtn;   // 单位按钮

	//------------- TreeWidgetItem------------------- 
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * lightTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // 右键选中的节点

	//QVariant ItemVar1, ItemVar2;   //节点属性

	//----------- Menu ----------------- 
	QMenu * fileMenu;  //菜单栏

	//右键菜单
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//----------- ToolBar ------------------- 
	QToolBar * fileTool;   //工具栏
	QToolBar * constructTool;  //模型栏

	//----------- Action ----------------- 
	//文件菜单项
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	QAction * viewAction;  // 视角
	QLabel * viewLabel;
	QComboBox * viewComboBox;
	QAction * detailsAction;

	//模型
	QAction * rectangleAction; //矩形
	QAction * polygonAction; // 任意凸多边形
	QAction * paraboloidAction; // 抛物面

	QAction * cylinderAction; // 圆柱体
	QAction * sphereAction; // 球

	QAction * booleanAction; // bool

	QAction * STLInputAction; // STL输入

	//右键菜单
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;
	QAction * SaveFieldAction;
	QAction * Field2lightAction; // 场生成光源
	QAction * transparencyAction;
	QAction * isShowAction;
	QAction * AddVariableAction; // 添加参数化的参数

	//源
	QAction * LightSourceAction;  //光线追踪源
	QAction * ApertureFieldAction;//口面场源
	QAction * GaussianAction;     //高斯场源

	//仿真
	QAction * CalculationAction;     //计算

	//****** Details********
	QButtonGroup * dimensionGroupBtn;
	QRadioButton * ThreeDBtn;
	QRadioButton * TwoDBtn;
	QButtonGroup * fieldGroupBtn;
	QRadioButton * ExBtn;
	QRadioButton * EyBtn;
	QRadioButton * EzBtn;
	QRadioButton * HxBtn;
	QRadioButton * HyBtn;
	QRadioButton * HzBtn;
	QButtonGroup * pmGroupBtn;
	QRadioButton * magnitudeBtn;
	QRadioButton * phaseBtn;
	QButtonGroup * powerGroupBtn;
	QRadioButton * linearBtn;
	QRadioButton * dbBtn;

	//******记录各个模型的个数********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int cylinderNum;
	int STLInputNum;
	int sphereNum;
	int FieldNum;
	int booleanNum;
	int ArbitrarySurfaceNum;
	int isSet; // 是否是修改 0为不是 1为是
	int ResFieldNum;

	bool isExistenceOpenWin; // 是否存在打开的窗口

	// -------------- 各种窗口的指针 ------------------------
	ParaboloidWidget *paraboloidDialog;
	RectangleWidget *rectangleDialog;
	CylinderWidget *cylinderDialog;
	SphereWidget * SphereDialog;
	BooleanWidget * booleanDialog;
	STLInputWidget * STLInputDialog;
	LightSourceWidget *LightSourceDialog;
	GaussianWidget * GaussianDialog;
	ApertureFieldWidget *ApertureFieldDialog;
	CalculationWidget * calculationDialog;

	// ------------------   data ----------------------------
	unordered_map<int, unordered_map<int, Surface*>>  SurfaceData;
	unordered_map<int, RaySource*> raySourceHash;
	unordered_map<int, Field*> fieldHash;

	vtkSmartPointer<vtkImageActor> tempImageActor;
	stack<vtkActor*> stackActor;
	stack<Surface*> stackSurface;
	RaySource* tempRaySource;
	Surface* tempSurface; // 临时存放模型指针
	vtkSmartPointer<vtkActor> tempActor;  // 临时存放模型actor
	Field *tempField;
	stack<vtkActor*> stackCheckActor; // 用于保存check时的actor

	Parameterization parameterization; // 参数化的数据

	double Unit_factor;

	// 进度条
	QProgressDialog* progDlg;
	int currentValue;
	QTimer *timer;

	int tempIndex;
	// 计算时的名字
	queue<QString> calName;
};

#endif // MAINWINDOW_H
