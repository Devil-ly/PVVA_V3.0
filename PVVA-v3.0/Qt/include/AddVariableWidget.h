#ifndef AddVariableWidget_H
#define AddVariableWidget_H

#include <QtWidgets/QDialog>
#include <QTabWidget>
#include <QGroupBox>
#include <QGridLayout>
#include <Qlabel>
#include <QLineEdit>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QString>
#include <string>
using namespace std;
class AddVariableWidget : public QDialog
{
	Q_OBJECT

public:
	AddVariableWidget(QWidget *parent = 0, int wayButton = 0);
	~AddVariableWidget();

	int Num();
	void setName(const string & _name);
	void setExpress(const string & _express);
	QString getName() const;
	QString getExpress() const;

	bool isLegitimate(double & val);

private slots:
    void buttonOk();
    void buttonCancel();
	void buttonEvaluate();
private:

	QLineEdit * nameLineEdit;
	QLineEdit * expressLineEdit;
	QLineEdit * valLineEdit;

	QPushButton * evaluateBtn;
	QPushButton * okBtn;
	QPushButton * cancelBtn;

	int returnNum;
};

#endif // AddVariableWidget_H
