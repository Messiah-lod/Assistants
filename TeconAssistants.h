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
#include <conio.h> //для getch

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


/* Для работы с Word
	QAxObject* pword;
	QAxObject* pdoc;
*/
	QMenuBar *menuBar;

	QWidget *tab_graphicEditor;//вкладка графического редактора
	QWidget *tab_dataBase;//вкладка базы данных
	QWidget *tab_technologicalObject;//вкладка технологических объектов
	QWidget *tab_dataSource;//вкладка источников данных
	QWidget *tab_algorithm;//вкладка алгоритмов математического обеспечения
	QWidget *tab_signalDistribution;//вкладка распределения сигналов по каналам контроллеров

	QGridLayout *grid_ees_technologicalObject;
	EES *ees_technologicalObject;
	QGridLayout *grid_graphicsEditor_scheme;
	graphiceditor *graphicsEditor_scheme;
//	startWindow *startWindowWidght;


	void closeEvent(QCloseEvent * event);



public slots:
	void on_lbAnyWidget_clicked();
};