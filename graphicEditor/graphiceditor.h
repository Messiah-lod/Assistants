#ifndef graphiceditor_H
#define graphiceditor_H

#include "veworkplaceview.h"
#include "polyline/vepolylinesettings.h"
#include "rectangle/verectanglesettings.h"
#include "Graphics_view_zoom.h"
#include "stamp.h"
#include "QDRuler.h"

#include <QMainWindow>
#include <QtWidgets/QWidget>
#include <QtWidgets>
#include <ActiveQt/QAxObject>
#include <QtGui>
#include <QPushButton>
#include <QTextStream> 
#include <QTextCodec>
#include <QSpacerItem>
#include <QVector>

class QGraphicsItem;
class VEWorkplace;

namespace Ui {
class MainWindow;
}

class graphiceditor : public QWidget
{
    Q_OBJECT

public:
    explicit graphiceditor(QWidget *parent = 0);
	void retranslateUi();

    ~graphiceditor();

private slots:
    void on_butSave_clicked();
    void on_butOpen_clicked();
	void on_butaddListScene_clicked();
	void on_butremoveListScene_clicked();

    void checkSelection();
    void checkActionStates();
    void selectItem(QGraphicsItem *item);
    void selectNewItem(QGraphicsItem *item);
    void slotHelp();



private:
	inline void create_scene();//метод создания сцены с листами

	QGridLayout *centralWidget;
	QSplitter *splitter;
	QWidget *toolsWidget;
	QGridLayout *toolsGridWidget;
	
	QToolButton *butDefault;
	QToolButton *butLine;
	QToolButton *butOpen;
	QToolButton *butRectangle;
	QToolButton *butSave;
	QToolButton *butaddListScene;
	QToolButton *butremoveListScene;

	QVector <QGraphicsPathItem*> numberPages;//вектор листов

	Graphics_view_zoom *zoom;//объект для зума вьюшки

	VEPolyLineSettings *polylineSettings;
	VERectangleSettings *rectangleSettings;

	QSpacerItem *verticalSpacer;

	VEWorkplaceView *workplaceView; //вид сцены
    VEWorkplace *workplaceScene; //сцена
    QString path;

	QMessageBox *msInfo;//информационное сообщание

};

#endif // MAINWINDOW_H
