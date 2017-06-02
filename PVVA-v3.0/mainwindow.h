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

	//------------------- ��������------------------- 
	void createActions();
	void createMenus(); // �˵�
	void createToolBars();
	void createStatusBar();
	void createDetails();
	void creareWindows(); // �ȴ����ô��� ������û�н��delete������ ��������������⣩


	void createTreeWidgetItem(); // ����tree
	void createRightMenu(); // �Ҽ��˵�

	// �������
	QTreeWidgetItem * addTreeChild(QTreeWidgetItem * treeFather, QString treeName,
		QString iconFile, int name, int index, int rightPar = 0);


private slots:
    // ------------------- ģ�ͺ��� ------------------------------------
	void createParaboloid();    // ������
	void toReceiveParaboloid(int);

	void createRectangle(); // ����
	void toReceiveRectangle(int index);

	void createCylinder();   // Բ��
	void toReceiveCylinder(int index);

	void createSphere();   //  ��
	void toReceiveSphere(int index);

	void createBoolean();  // bool����
	void toReceiveBoolean(int index);

	void createSTLInput();  // stl
	void toReceiveSTLInput(int index);  // stl

	void toModelActorChange(bool); // ����ģ�ʹ��ڵı任

    // ------------------- ���㺯�� ------------------------------------
	void createLightSource();  // ����׷��
	void ToLightSource(int index);

	void createGaussian();      // ��˹
	void ToGaussian(int index);

	void createApertureField(); // ���泡
	void ToApertureField(int index);

	void createCalculation();
	void ToCalculation(int index);
	void receiveField(Field*);
	void updateProgressDialog();

    // ------------------- ����Ҽ����� ----------------------------------
	void on_treeWidget_ContextMenuRequested(QPoint pos);// �Ҽ��˵�
	void on_treeWidget_leftPressed(QTreeWidgetItem *item, int column);
	void on_Details_FieldClicked(); //details Field 

	void SetGeometryPara();  //�޸Ĳ���
	void DeleteGeometry(); // ɾ��ģ��
	void AddVariable();
	void SaveField(); // ����Field
	void SetTransparency(); // ���ó�͸��
	void SetShow(); // �����Ƿ���ʾ

private:

	// �ı�3D��ʾ����
	void showDetails(int, int);
	void showDetails();

	void updateVtk();
	void updateRaySource();

private:
	//------------- VTK ------------------- 
	QVTKWidget widget; // vtk ��ʾ����
	vtkSmartPointer<vtkRenderWindowInteractor> interactor; // ����
	vtkSmartPointer<vtkRenderer> renderer;
	vtkSmartPointer<vtkAxesActor> axes; // ����
	vtkSmartPointer<vtkProperty> SelProperty; // ѡ��ʱ����ɫ��͸����
	vtkSmartPointer<vtkProperty> ChangeProperty; // �޸�ʱ����ɫ��͸����

	//std::list<vtkActor*> actors; // ����������Ա

	QDockWidget * leftWidget; //����treeWidget�Ĵ���
	QTreeWidget * treeWidget;

	QDockWidget * detailsDockWidget;
	QWidget * detailsWidget;

	QLabel * locationLabel;  // ״̬����ǩ
	QPushButton * unitBtn;   // ��λ��ť

	//------------- TreeWidgetItem------------------- 
	QTreeWidgetItem * definitionsTreeItem;
	QTreeWidgetItem * variablesTreeItem;
	QTreeWidgetItem * modelTreeItem;
	QTreeWidgetItem * geometryTreeItem;
	QTreeWidgetItem * sourceTreeItem;
	QTreeWidgetItem * lightTreeItem;

	QTreeWidgetItem * rightSelectItem;
	QTreeWidgetItem * leftSelectItem; // �Ҽ�ѡ�еĽڵ�

	//QVariant ItemVar1, ItemVar2;   //�ڵ�����

	//----------- Menu ----------------- 
	QMenu * fileMenu;  //�˵���

	//�Ҽ��˵�
	QMenu *R_Tree_compenents_childMenu;
	QMenu *R_BlankMenu;

	//----------- ToolBar ------------------- 
	QToolBar * fileTool;   //������
	QToolBar * constructTool;  //ģ����

	//----------- Action ----------------- 
	//�ļ��˵���
	QAction * saveFileAction;
	QAction * openFileAction;
	QAction * newFileAction;
	QAction * viewAction;  // �ӽ�
	QLabel * viewLabel;
	QComboBox * viewComboBox;
	QAction * detailsAction;

	//ģ��
	QAction * rectangleAction; //����
	QAction * polygonAction; // ����͹�����
	QAction * paraboloidAction; // ������

	QAction * cylinderAction; // Բ����
	QAction * sphereAction; // ��

	QAction * booleanAction; // bool

	QAction * STLInputAction; // STL����

	//�Ҽ��˵�
	QAction * SetGeometryAction;
	QAction * DeleteGeometryAction;
	QAction * SaveFieldAction;
	QAction * Field2lightAction; // �����ɹ�Դ
	QAction * transparencyAction;
	QAction * isShowAction;
	QAction * AddVariableAction; // ��Ӳ������Ĳ���

	//Դ
	QAction * LightSourceAction;  //����׷��Դ
	QAction * ApertureFieldAction;//���泡Դ
	QAction * GaussianAction;     //��˹��Դ

	//����
	QAction * CalculationAction;     //����

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

	//******��¼����ģ�͵ĸ���********
	int paraboloidNum;
	int LightSourceNum;
	int rectangleNum;
	int cylinderNum;
	int STLInputNum;
	int sphereNum;
	int FieldNum;
	int booleanNum;
	int ArbitrarySurfaceNum;
	int isSet; // �Ƿ����޸� 0Ϊ���� 1Ϊ��
	int ResFieldNum;

	bool isExistenceOpenWin; // �Ƿ���ڴ򿪵Ĵ���

	// -------------- ���ִ��ڵ�ָ�� ------------------------
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
	Surface* tempSurface; // ��ʱ���ģ��ָ��
	vtkSmartPointer<vtkActor> tempActor;  // ��ʱ���ģ��actor
	Field *tempField;
	stack<vtkActor*> stackCheckActor; // ���ڱ���checkʱ��actor

	Parameterization parameterization; // ������������

	double Unit_factor;

	// ������
	QProgressDialog* progDlg;
	int currentValue;
	QTimer *timer;

	int tempIndex;
	// ����ʱ������
	queue<QString> calName;
};

#endif // MAINWINDOW_H
