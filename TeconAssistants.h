#pragma once

#include "objectProject/EES.h"
#include "graphicEditor/graphiceditor.h"
#include "catalogProject/startWindow.h"

#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 
#include <QMenu> 
#include <QApplication>

#include <iostream>
#include "Windows.h"
#include <string>
#include <any>
#include <vector>
#include <fstream>
#include <conio.h> //��� getch

//#include <QAxObject> 

class TeconAssistants :public QWidget // public QMainWindow //
{
	Q_OBJECT

public:
	TeconAssistants(QWidget *parent = Q_NULLPTR);
	~TeconAssistants();

	void retranslateUi();

	void createMenu();



private:
	QWidget *gridMainWidget;
	QGridLayout *gridMain;
	QMenu *fileMenu;
	QSplitter *splitter;
	QWidget *gridLayoutWidget;
	QGridLayout *gridLayout;
	QTabWidget *tabWidget;
	QWidget *gridLayoutWidgetLeft;
	QGridLayout *gridLayoutLeft;
	QPushButton *lbAnyWidget;

	QMessageBox *msExit;


/* ��� ������ � Word
	QAxObject* pword;
	QAxObject* pdoc;
*/
	QMenuBar *menuBar;

	QWidget *tab_graphicEditor;//������� ������������ ���������
	QWidget *tab_dataBase;//������� ���� ������
	QWidget *tab_technologicalObject;//������� ��������������� ��������
	QWidget *tab_dataSource;//������� ���������� ������
	QWidget *tab_algorithm;//������� ���������� ��������������� �����������
	QWidget *tab_signalDistribution;//������� ������������� �������� �� ������� ������������

	QGridLayout *grid_ees_technologicalObject;
	EES *ees_technologicalObject;
	QGridLayout *grid_graphicsEditor_scheme;
	graphiceditor *graphicsEditor_scheme;
//	startWindow *startWindowWidght;


	void closeEvent(QCloseEvent * event);



public slots:
	void on_lbAnyWidget_clicked();
};