#pragma once

#include "..\TeconAssistants.h"
#include "catalogModel.h"
#include "treemodel.h"
#include "..\objectProject\ObjectProject.h"
#include "..\SQL_to_FB.h"
#include "dialogConnectParams.h"
#include "..\settings.h"

#include "..\ibpp\ibpp.h"
#include <exception>

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 
#include <QTextCodec>

class startWindow : public QWidget
{
	Q_OBJECT

public:
	startWindow(QWidget *parent = 0);
	~startWindow();
	void retranslateUi();

	bool _Success;				// if true after all tests, everything succeeded
	int _WriteMode;				// 0 == default, 1 == speed, 2 == safety

	//void Test4();
	//void Test5();

private:
	TreeModel *m_treeModel;
	SQL_to_FB *dataBase;
//	TeconAssistants *TA;

	QTreeView *tvCatalog;
	QGridLayout *grid;

	QPushButton* bnAdd;
	QPushButton* bnDel;
	QPushButton* bnEdit;

	QMessageBox *msExit;

	dialogConnectParams* m_connect;

	QVector <QString> dataConnect;//данные для подключения к базе данных от формы ввода данных:имя:сервер:путь

	std::ofstream logs;//файл логов

	QModelIndex addChildRevision(QString revision, QString desc, QModelIndex _index, QAbstractItemModel *_model);
	void add_child(std::any name_project, QModelIndex _index, QAbstractItemModel *_model);
//	void exportToXml(const QString& fileName);
	void cutPath(std::string source, std::string & server, std::string & path);//вырезает из пути сервер и расположение файла
	void find_data_on_push();
	std::string translate(std::string text);



	std::string DbName = "New BD";
	std::string ServerName = "127.0.0.1";
	std::string DbPath = "D:/база СМТО 2.1.2.10/TeconAssistants.gdb";
	std::string UserName = "SYSDBA";
	std::string Password = "masterkey";// "nI*#Sp28";
	std::string query;

	bool m_pushEdit = false;//сигнал нажатия кнопки ИЗМЕНИТЬ
	bool initProject = false;//сигнал единоразового подключения к проекту
signals:
	void sendData(QVector <QString> sendDataStr);

public slots:
	void on_onAppend_clicked();
	void on_onDel_clicked();
	void on_onEdit_clicked();
	void start_Assistants();
	void dataForConnect();
	void recieveData(QVector <QString> sendDataStr);
};