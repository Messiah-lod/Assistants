#pragma once

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 


class dialogConnectParams : public QWidget
{
	Q_OBJECT

public:
	dialogConnectParams(QWidget *parent = 0);
	~dialogConnectParams();
	void retranslateUi();
private:

	QGridLayout* mainGrid;

	QLineEdit* lblServer;
	QLineEdit* lblPath;
	QLineEdit* lblName;

	QPushButton* btnOK;
	QPushButton* btnCancel;

	QLabel* txtServer;
	QLabel* txtPath;
	QLabel* txtName;

	QString m_server;
	QString m_path;
	QString m_projectName;

	QVector <QString> sendDataStr;//вектор данных для отправки в другие формы

	void dialogConnectParams::closeEvent(QCloseEvent *event);//при нажатии кнопки крестика у окна
	void dialogConnectParams::exit();
signals:
	void sendData(QVector <QString> sendDataStr);

private slots:
	void push_OK();
	void push_Cancel();
	void recieveData(QVector <QString> sendDataStr);
};

