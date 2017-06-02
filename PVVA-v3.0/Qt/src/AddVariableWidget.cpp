#include "..\include\AddVariableWidget.h"
#include "Tools/Parameterization.h"
#include <QMessageBox>

AddVariableWidget::AddVariableWidget(QWidget *parent, int wayButton)
	: QDialog(parent)
{
	setWindowTitle(tr("Create variable"));

	QLabel *nameLabel = new QLabel(tr("Name"));
	nameLineEdit = new QLineEdit;
	QLabel *expressLabel = new QLabel(tr("Express"));
	expressLineEdit = new QLineEdit;
	QLabel *valueLabel = new QLabel(tr("Value  "));
	valLineEdit = new QLineEdit;
	evaluateBtn = new QPushButton(tr("Evaluate"));
	connect(evaluateBtn, SIGNAL(clicked()), this, SLOT(buttonEvaluate()));
	QGridLayout * layout= new QGridLayout;
	layout->addWidget(nameLabel, 0, 0);
	layout->addWidget(nameLineEdit, 0, 1);
	layout->addWidget(expressLabel, 1, 0);
	layout->addWidget(expressLineEdit, 1, 1);
	QGridLayout * layoutVal = new QGridLayout;
	layoutVal->addWidget(valueLabel, 1, 0);
	layoutVal->addWidget(valLineEdit, 1, 1);
	layoutVal->addWidget(evaluateBtn, 1, 2);

	okBtn = new QPushButton(tr("OK"));
	cancelBtn = new QPushButton(tr("Cancel"));
	connect(okBtn, SIGNAL(clicked()), this, SLOT(buttonOk()));
	connect(cancelBtn, SIGNAL(clicked()), this, SLOT(buttonCancel()));

	QHBoxLayout * layoutbtn = new QHBoxLayout;
	layoutbtn->addSpacing(50);
	layoutbtn->addWidget(okBtn);
	layoutbtn->addWidget(cancelBtn);

	QVBoxLayout * VBoxLayout = new QVBoxLayout(this);
	VBoxLayout->addLayout(layout);
	VBoxLayout->addLayout(layoutVal);
	VBoxLayout->addLayout(layoutbtn);

	returnNum = -1;
}

AddVariableWidget::~AddVariableWidget()
{
}

int AddVariableWidget::Num()
{
	return returnNum;
}

void AddVariableWidget::setName(const string & _name)
{
	nameLineEdit->setText(QString::fromStdString(_name));
}

void AddVariableWidget::setExpress(const string & _express)
{
	expressLineEdit->setText(QString::fromStdString(_express));
}

QString AddVariableWidget::getName() const
{
	return nameLineEdit->text();
}

QString AddVariableWidget::getExpress() const
{
	return expressLineEdit->text();
}

bool AddVariableWidget::isLegitimate(double & val)
{
	try
	{
		val = Parameterization::getVal(
			expressLineEdit->text().toStdString());
	}
	catch (errorOperator &e)
	{
		int s = e.what();
		if (s == 0) //未能识别的操作符
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Failed to identify the operator!");
			return false;
		}
		else if (s == 1) //无法计算
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Failed to Calculate!");
			return false;
		}
		else if (s == 2) //未找到
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, Not find the corresponding parameters!");
			return false;
		}
		else
		{
			QMessageBox::warning(NULL, "Warning",
				"Invalid data, The input expression is incorrect!");
			return false;
		}
	}

	return true;
}

void AddVariableWidget::buttonCancel()
{
	close();
}

void AddVariableWidget::buttonEvaluate()
{
	//Parameterization par;
	double val;
	if(isLegitimate(val))
		valLineEdit->setText(QString::number(val));

}

void AddVariableWidget::buttonOk()
{
	double val;
	if (isLegitimate(val))
	{
		returnNum = 1;
		close();
	}
}