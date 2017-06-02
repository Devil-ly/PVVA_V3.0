#ifndef WAITINGDIALOG_H
#define WAITINGDIALOG_H

#include <QtWidgets/QDialog>
#include <QProgressBar>
#include <QTimer>  

class WaitingDialog : public QDialog
{
	Q_OBJECT
private:
	int m_CurrentValue;         //��ǰֵ  
	int m_UpdateInterval;       //���¼��  
	int m_MaxValue;             //���ֵ  
	QTimer *m_Timer;
	QProgressBar *m_ProgressBar;
public:
	WaitingDialog(QWidget *parent = 0);
	~WaitingDialog();
	void Stop();

public slots:
	void Start(int interval = 100, int maxValue = 100);

private slots:
	void UpdateSlot();
};
#endif 