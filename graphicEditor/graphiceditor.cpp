#include "graphiceditor.h"

#include <QSvgGenerator>
#include <QFileDialog>
#include <QDebug>
#include <QGraphicsItem>
#include <QMessageBox>
#include "svgreader.h"
#include "veworkplace.h"
#include ".\polyline\vepolyline.h"
#include ".\rectangle\verectangle.h"

graphiceditor::graphiceditor(QWidget *parent) 
	: QWidget(parent)
{
	msInfo = new QMessageBox();
	msInfo->setIcon(QMessageBox::Information);

	centralWidget = new QGridLayout();
	centralWidget->setContentsMargins(1, 1, 1, 1);
	splitter = new QSplitter();

	centralWidget->addWidget(splitter);

	toolsWidget = new QWidget();
	toolsGridWidget = new QGridLayout(toolsWidget);
	toolsGridWidget->setContentsMargins(1, 1, 1, 1);

	workplaceView = new VEWorkplaceView();

	QDRuler *mHorzRuler = new QDRuler(QDRuler::Horizontal);
	QDRuler *mVertRuler = new QDRuler(QDRuler::Vertical);

	splitter->setOrientation(Qt::Horizontal);
	splitter->addWidget(toolsWidget);
//	splitter->addWidget(mVertRuler);
	splitter->addWidget(workplaceView);
	splitter->setStretchFactor(0, 0);
	splitter->setStretchFactor(1, 1);

	workplaceScene = new VEWorkplace();

	numberPages.clear();
	QGraphicsPathItem *tmp = new QGraphicsPathItem();
	stamp *A3 = new stamp();
	tmp->setPath(A3->addMainShtamp());
	numberPages.append(tmp);//класс листов со штампом
	workplaceScene->addItem(tmp);//добавим на сцену прямоугольник
	create_scene();

	const QIcon icon_butDefault = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/arrow-top-left.png"));
	const QIcon icon_butLine = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/vector-polyline.png"));
	const QIcon icon_butOpen = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/folder-upload.png"));
	const QIcon icon_butRectangle = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/vector-rectangle.png"));
	const QIcon icon_butSave = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/content-save.png"));
	const QIcon icon_addListScene = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/plus_list_scene.png"));
	const QIcon icon_removeListScene = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/minus_list_scene.png"));

	butDefault = new QToolButton();
	butDefault->setIcon(icon_butDefault);
	butDefault->setMinimumSize(30,30);
	butLine = new QToolButton();
	butLine->setIcon(icon_butLine);
	butLine->setMinimumSize(30, 30);
	butOpen = new QToolButton();
	butOpen->setIcon(icon_butOpen);
	butOpen->setMinimumSize(30, 30);
	butRectangle = new QToolButton();
	butRectangle->setIcon(icon_butRectangle);
	butRectangle->setMinimumSize(30, 30);
	butSave = new QToolButton();
	butSave->setIcon(icon_butSave);
	butSave->setMinimumSize(30, 30);
	butaddListScene = new QToolButton();
	butaddListScene->setIcon(icon_addListScene);
	butaddListScene->setMinimumSize(30, 30);
	butremoveListScene = new QToolButton();
	butremoveListScene->setIcon(icon_removeListScene);
	butremoveListScene->setMinimumSize(30, 30);

	verticalSpacer = new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding);

	polylineSettings = new VEPolyLineSettings();
	rectangleSettings = new VERectangleSettings();

	rectangleSettings->setVisible(false);
	polylineSettings->setVisible(false);

	toolsGridWidget->addWidget(butOpen, 0, 0, 1, 1);
	toolsGridWidget->addWidget(butSave, 0, 1, 1, 1);
	toolsGridWidget->addWidget(butaddListScene, 1, 0, 1, 3);
	toolsGridWidget->addWidget(butremoveListScene, 1, 1, 1, 3);
	toolsGridWidget->addWidget(butDefault, 2, 0, 1, 1);
	toolsGridWidget->addWidget(butLine, 3, 0, 1, 1);
	toolsGridWidget->addWidget(butRectangle, 3, 1, 1, 1);
	toolsGridWidget->addWidget(polylineSettings, 4, 0, 1, 3);
	toolsGridWidget->addWidget(rectangleSettings , 5, 0, 1, 3);
	toolsGridWidget->addItem(verticalSpacer, 6, 0, 1, 3);
	
	//коннекты реализованы через лямбда функцию
	connect(butLine, &QToolButton::clicked, [=](){workplaceScene->setCurrentAction(VEWorkplace::LineType);});
    connect(butRectangle, &QToolButton::clicked, [=](){workplaceScene->setCurrentAction(VEWorkplace::RectangleType);});
    connect(butDefault, &QToolButton::clicked, [=](){workplaceScene->setCurrentAction(VEWorkplace::DefaultType);});

    connect(workplaceScene, &VEWorkplace::selectionChanged, this, &graphiceditor::checkSelection);
    connect(workplaceScene, &VEWorkplace::currentActionChanged, this, &graphiceditor::checkActionStates);
    connect(workplaceScene, &VEWorkplace::signalSelectItem, this, &graphiceditor::selectItem);
    connect(workplaceScene, &VEWorkplace::signalNewSelectItem, this, &graphiceditor::selectNewItem);
	
	QObject::connect(butOpen, SIGNAL(clicked()), this, SLOT(on_butOpen_clicked()));
	QObject::connect(butSave, SIGNAL(clicked()), this, SLOT(on_butSave_clicked()));
	QObject::connect(butaddListScene, SIGNAL(clicked()), this, SLOT(on_butaddListScene_clicked()));
	QObject::connect(butremoveListScene, SIGNAL(clicked()), this, SLOT(on_butremoveListScene_clicked()));

	setLayout(centralWidget);
	retranslateUi();

	 zoom = new  Graphics_view_zoom(workplaceView);
	 zoom->set_modifiers(Qt::NoModifier);//здесь можно добавить модифмкаторы (напр: масштабирование при зажатой клавише ctrl)
}

void graphiceditor::retranslateUi()
{
	polylineSettings->retranslateUi();
//	rectangleSettings->retranslateUi();
}

graphiceditor::~graphiceditor()
{
    delete workplaceScene;
}

void graphiceditor::on_butSave_clicked()
{
    QString newPath = QFileDialog::getSaveFileName(this, trUtf8("Save SVG"),
        path, tr("SVG files (*.svg)"));

    if (newPath.isEmpty())
        return;

    path = newPath;

    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(workplaceScene->width(), workplaceScene->height()));
    generator.setViewBox(QRect(0, 0, workplaceScene->width(), workplaceScene->height()));
    generator.setTitle(trUtf8("Vector Editor"));
    generator.setDescription(trUtf8("File created by Vector Editor."));

    QPainter painter;
    painter.begin(&generator);
    workplaceScene->render(&painter);
    painter.end();
}

void graphiceditor::on_butOpen_clicked()
{
    QString newPath = QFileDialog::getOpenFileName(this, trUtf8("Open SVG"),
                                                   path, tr("SVG files (*.svg)"));
    if (newPath.isEmpty())
        return;

    path = newPath;
    workplaceScene->clear();

    workplaceScene->setSceneRect(SvgReader::getSizes(path));

    foreach (QGraphicsItem *item, SvgReader::getElements(path)) {
        switch (item->type()) {
        case QGraphicsPathItem::Type: {
            VEPolyline *polyline = qgraphicsitem_cast<VEPolyline*>(item);
            workplaceScene->addItem(polyline);
            connect(polyline, &VEPolyline::clicked, workplaceScene, &VEWorkplace::signalSelectItem);
            connect(polyline, &VEPolyline::signalMove, workplaceScene, &VEWorkplace::slotMove);
            break;
        }
        case QGraphicsRectItem::Type: {
            VERectangle *rect = qgraphicsitem_cast<VERectangle*>(item);
            workplaceScene->addItem(rect);
            connect(rect, &VERectangle::clicked, workplaceScene, &VEWorkplace::signalSelectItem);
            connect(rect, &VERectangle::signalMove, workplaceScene, &VEWorkplace::slotMove);
            break;
        }
        default:
            break;
        }
    }
}

void graphiceditor::checkSelection()
{
    checkActionStates();
    switch (workplaceScene->selectedItems().length()) {
    case 0:
 //       rectangleSettings->deselect();
        polylineSettings->deselect();
        checkActionStates();
        toolsWidget->setEnabled(true);
        break;
    case 1:
        switch (workplaceScene->selectedItems().at(0)->type()) {
        case QGraphicsRectItem::Type:{
           polylineSettings->deselect();
           polylineSettings->setVisible(false);
//		   rectangleSettings->setVisible(true);
            break;
        }
        default: {
 //           rectangleSettings->deselect();
 //           rectangleSettings->setVisible(false);
            polylineSettings->setVisible(true);
            break;
        }
        }
        checkActionStates();
        toolsWidget->setEnabled(true);
        break;
    default:
//        rectangleSettings->deselect();
        polylineSettings->deselect();
        toolsWidget->setEnabled(false);
        break;
    }
}

void graphiceditor::checkActionStates()
{
    workplaceView->setDragMode(QGraphicsView::NoDrag);
    rectangleSettings->setVisible(false);
    polylineSettings->setVisible(false);
    switch (workplaceScene->currentAction()){
    case VEWorkplace::LineType:
        polylineSettings->setVisible(true);
        break;
    case VEWorkplace::RectangleType:
        rectangleSettings->setVisible(true);
        break;
    case VEWorkplace::SelectionType:
        break;
    default:
//        workplaceView->setDragMode(QGraphicsView::ScrollHandDrag);
		workplaceView->setDragMode(QGraphicsView::NoDrag);
        break;
    }
}

void graphiceditor::selectItem(QGraphicsItem *item)
{
    switch (item->type()) {
    case QGraphicsRectItem::Type: {
        VERectangle *rect = qgraphicsitem_cast<VERectangle *>(item);
 //       rectangleSettings->loadRectangle(rect);
        break;
    }
    case QGraphicsPathItem::Type: {
        VEPolyline *polyline = qgraphicsitem_cast<VEPolyline *>(item);
        polylineSettings->loadPolyline(polyline);
        break;
    }
    default:
        break;
    }
}

void graphiceditor::selectNewItem(QGraphicsItem *item)
{
    switch (item->type()) {
    case QGraphicsRectItem::Type: {
        VERectangle *rect = qgraphicsitem_cast<VERectangle *>(item);
  //      rectangleSettings->newRectangle(rect);
        break;
    }
    case QGraphicsPathItem::Type: {
        VEPolyline *polyline = qgraphicsitem_cast<VEPolyline *>(item);
        polylineSettings->newPolyline(polyline);
        break;
    }
    default:
        break;
    }
}

void graphiceditor::slotHelp()
{
    QMessageBox::information(this, trUtf8("Справка"),
                             trUtf8("Простой векторный редактор для работы с ломанными линиями и прямоугольниками.\n"
                                    "Частично поддерживает формат SVG - прямоугольники и полилинии.\n"
                                    "\n"
                                    "Горячие клавиши:\n"
                                    "Shift+ЛКМ+Перемещение Мыши - Выделение;\n"
                                    "Del - удаление выделенных объектов;\n"
                                    "Ctrl+A - выделение всех объектов.\n"
                                    "\n"
                                    "Двойной клик по прямоугольнику изменяет режим трансформаций.\n"
                                    "Прямоугольники поддерживают изменение размера и поворот."));
}

void graphiceditor::create_scene()
{
	//********************************************
	//*******  1 мм = 4,5 пикселя  ******************
	//********************************************
	workplaceScene->setSceneRect(0, 0, numberPages.size() * 1890, 1337);//начальный размер сцены исходя из рабочей области листа А3
	workplaceView->setScene(workplaceScene);
	workplaceView->setViewportUpdateMode(QGraphicsView::BoundingRectViewportUpdate);
	workplaceView->setDragMode(QGraphicsView::ScrollHandDrag);
	workplaceView->setCursor(QCursor());
}

void graphiceditor::on_butaddListScene_clicked()
{
	stamp *A3 = new stamp();
	QGraphicsPathItem *tmp = new QGraphicsPathItem();
	numberPages.append(tmp);//класс листов со штампом
	tmp->setPath(A3->addSecondShtamp(numberPages.size()));
	
	workplaceScene->addItem(tmp);//добавим на сцену следующий лист
	create_scene();
	A3->deleteLater();
}

void graphiceditor::on_butremoveListScene_clicked()
{
	if (numberPages.size() > 1)
	{
		workplaceScene->removeItem(numberPages.back());//по сути чтобы удалить объект со сцены мы всегда должны иметь на него ссылку
		//простое удаление объекта, не приведет к его уалению со сцены
		numberPages.pop_back();//удалить последний лист
		create_scene();
	}
	else
	{
		msInfo->setText(tr("Deletionis not possible. The minimum number of sheets has been reached!"));
		msInfo->exec();
	}

}
