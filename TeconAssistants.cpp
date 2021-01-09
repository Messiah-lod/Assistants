#include "TeconAssistants.h"

TeconAssistants::TeconAssistants(QWidget *parent)
	: QWidget(parent)
//	: QMainWindow(parent)
{
	setMinimumSize(800,600);
	//инициализируем элементы графического интерфейса
	lbAnyWidget = new QPushButton();
	//	добавим разделитель
	splitter = new QSplitter();
	//добавляем поле слева от сплиттера, сначала виджет ставим, потом в него грид
	gridLayoutWidgetLeft = new QWidget();
	gridLayoutLeft = new QGridLayout(gridLayoutWidgetLeft);
	gridLayoutLeft->addWidget(lbAnyWidget);
	//добавляем поле справа от сплиттера, сначала виджет ставим, потом в него грид
	gridLayoutWidget = new QWidget();
	gridLayout = new QGridLayout(gridLayoutWidget);
	//добавим в основное поле виджет вкладок
	tabWidget = new QTabWidget(gridLayoutWidget);
		//инициализируем все влкдаки основного пространства
		tab_graphicEditor = new QWidget();
		tab_dataBase = new QWidget();
		tab_technologicalObject = new QWidget();
		tab_dataSource = new QWidget();
		tab_algorithm = new QWidget();
		tab_signalDistribution = new QWidget();

		//добавим все созданные вкладки в виджет вкладок
	//	tabWidget->setTabPosition(QTabWidget::South); //задаем положение вкладок
		tabWidget->setObjectName(QString::fromUtf8("tabWidget"));
		tabWidget->addTab(tab_graphicEditor, QString());
		tabWidget->addTab(tab_dataBase, QString());
		tabWidget->addTab(tab_signalDistribution, QString());
		tabWidget->addTab(tab_technologicalObject, QString());
		tabWidget->addTab(tab_dataSource, QString());
		tabWidget->addTab(tab_algorithm, QString());
		//выберем вкладку, которая открывается по умолчанию, счет с нуля
		tabWidget->setCurrentIndex(0);

	//установим ориентацию сплиттера и добавим элементы для разделения
	splitter->setOrientation(Qt::Horizontal);
	splitter->addWidget(gridLayoutWidgetLeft);
	splitter->addWidget(gridLayoutWidget);
	//настроим грид по отступам снаружи и установим в него виджет вкладок
//	gridLayout->setSpacing(20);
	gridLayout->setContentsMargins(5, 5, 5, 5);
	gridLayout->addWidget(tabWidget, 0, 0, 1, 1);
	//инициализируем основной экран
	gridMainWidget = new QWidget(this);
	gridMain = new QGridLayout(gridMainWidget);
	gridMain->setContentsMargins(1, 1, 1, 1);
	gridMain->addWidget(splitter, 1, 0, 1, 1);


//	setCentralWidget(gridMainWidget);  /для отображения в мейн виндовс

	//добавим текст в лейбл для теста
	lbAnyWidget->setText("Any widget");
	lbAnyWidget->resize(100, 100);

	//добавим класс для работы с тех. объекты
	grid_ees_technologicalObject = new QGridLayout(tab_technologicalObject);
	ees_technologicalObject = new EES();
	grid_ees_technologicalObject->addWidget(ees_technologicalObject);
	//добавим графический редактор
	grid_graphicsEditor_scheme = new QGridLayout(tab_graphicEditor);
	graphicsEditor_scheme = new graphiceditor();
	grid_graphicsEditor_scheme->addWidget(graphicsEditor_scheme);

	//**************************************************************
	//******************РАБОТА В WORD**********************
	//**************************************************************
/*
	pword = new QAxObject("Word.Application");
	pdoc = new QAxObject();
	pdoc = pword->querySubObject("Documents");

//	pdoc = pdoc->querySubObject("Open(path)", "С:\\nameWordFile.docx"); //открыть существующий
	pdoc = pdoc->querySubObject("Add()"); //создать новый документ

	QAxObject *prange = pdoc->querySubObject("Range()");
	prange->dynamicCall("SetRange(int, int)", 0, 100);
	prange->setProperty("Text", "Вот текст");

	pword->setProperty("Visible", true);
*/
	//**************************************************************
	//******************РАБОТА В WORD**********************
	//**************************************************************


	//Связь сигналов кнопок с методами
	QObject::connect(lbAnyWidget, SIGNAL(clicked()), this, SLOT(on_lbAnyWidget_clicked()));




//	label = new QLabel("Hello world", this);
	//создадим меню
	createMenu();
	//переведем интерфейс программы
	retranslateUi();
	//отобразим интерфейс на слое
	setLayout(gridMain);
}

TeconAssistants::~TeconAssistants()
	{
//		delete this;
	}

void TeconAssistants::retranslateUi()
{
	setWindowTitle(QApplication::translate("setWindowTitle", "Tecon Assistants", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_graphicEditor), QApplication::translate("tab_graphicEditor", "Graphic Editor", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_dataBase), QApplication::translate("tab_dataBase", "Data Base", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_technologicalObject), QApplication::translate("tab_technologicalObject", "Technological Object", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_dataSource), QApplication::translate("tab_dataSource", "Data Source", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_algorithm), QApplication::translate("tab_algorithm", "Algorithm", nullptr));
	tabWidget->setTabText(tabWidget->indexOf(tab_signalDistribution), QApplication::translate("tab_signalDistribution", "Signal Distribution", nullptr));
	ees_technologicalObject->retranslateUi();
}

void TeconAssistants::createMenu()
{
	//создадим меню программы
	menuBar = new QMenuBar();
	menuBar->setMaximumHeight(20);
	//поместим на основной грид
	gridMain->addWidget(menuBar, 0, 0, 1, 1);
	//добавим пункт меню ФАЙЛ
	QMenu *fileMenu = menuBar->addMenu(QApplication::translate("fileMenu", "&File", nullptr));

	fileMenu->setTearOffEnabled(true);//сделать отрывное меню

//	QToolBar *fileToolBar = addToolBar(tr("File"));
	//создадим подпункты МЕНЮ
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon("D:/YandexDisk/development/QT/tecon assistants/tecon assistants/images/new.png"));
	QAction *newAct = new QAction(newIcon, tr("&New"), this);
	fileMenu->addAction(newAct);

//	fileMenu->addAction("&Exit", *parent, SLOT(quit()));

	QMenu *editMenu = menuBar->addMenu(QApplication::translate("editMenu", "&Edit", nullptr));





//	QMenu *fileMenu = menuBar->addMenu(tr("&File"));
//	



	/*
	QToolBar *fileToolBar = new QToolBar();
//				fileToolBar->addToolBar(tr("File"));
	const QIcon newIcon = QIcon::fromTheme("document-new", QIcon(":/images/new.png"));
	QAction *newAct = new QAction(newIcon, tr("&New"), this);
	newAct->setShortcuts(QKeySequence::New);
	newAct->setStatusTip(tr("Create a new file"));
	connect(newAct, &QAction::triggered, this, &MainWindow::newFile);
	fileMenu->addAction(newAct);
	fileToolBar->addAction(newAct);

	const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
	QAction *openAct = new QAction(openIcon, tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	openAct->setStatusTip(tr("Open an existing file"));
	connect(openAct, &QAction::triggered, this, &MainWindow::open);
	fileMenu->addAction(openAct);
	fileToolBar->addAction(openAct);

	const QIcon saveIcon = QIcon::fromTheme("document-save", QIcon(":/images/save.png"));
	QAction *saveAct = new QAction(saveIcon, tr("&Save"), this);
	saveAct->setShortcuts(QKeySequence::Save);
	saveAct->setStatusTip(tr("Save the document to disk"));
	connect(saveAct, &QAction::triggered, this, &MainWindow::save);
	fileMenu->addAction(saveAct);
	fileToolBar->addAction(saveAct);

	const QIcon saveAsIcon = QIcon::fromTheme("document-save-as");
	QAction *saveAsAct = fileMenu->addAction(saveAsIcon, tr("Save &As..."), this, &MainWindow::saveAs);
	saveAsAct->setShortcuts(QKeySequence::SaveAs);
	saveAsAct->setStatusTip(tr("Save the document under a new name"));


	fileMenu->addSeparator();

	const QIcon exitIcon = QIcon::fromTheme("application-exit");
	QAction *exitAct = fileMenu->addAction(exitIcon, tr("E&xit"), this, &QWidget::close);
	exitAct->setShortcuts(QKeySequence::Quit);
	exitAct->setStatusTip(tr("Exit the application"));

	QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));
	QToolBar *editToolBar = addToolBar(tr("Edit"));

#ifndef QT_NO_CLIPBOARD //буфер
	const QIcon cutIcon = QIcon::fromTheme("edit-cut", QIcon(":/images/cut.png"));
	QAction *cutAct = new QAction(cutIcon, tr("Cu&t"), this);

	cutAct->setShortcuts(QKeySequence::Cut);
	cutAct->setStatusTip(tr("Cut the current selection's contents to the "
		"clipboard"));
	connect(cutAct, &QAction::triggered, textEdit, &QPlainTextEdit::cut);
	editMenu->addAction(cutAct);
	editToolBar->addAction(cutAct);

	const QIcon copyIcon = QIcon::fromTheme("edit-copy", QIcon(":/images/copy.png"));
	QAction *copyAct = new QAction(copyIcon, tr("&Copy"), this);
	copyAct->setShortcuts(QKeySequence::Copy);
	copyAct->setStatusTip(tr("Copy the current selection's contents to the "
		"clipboard"));
	connect(copyAct, &QAction::triggered, textEdit, &QPlainTextEdit::copy);
	editMenu->addAction(copyAct);
	editToolBar->addAction(copyAct);

	const QIcon pasteIcon = QIcon::fromTheme("edit-paste", QIcon(":/images/paste.png"));
	QAction *pasteAct = new QAction(pasteIcon, tr("&Paste"), this);
	pasteAct->setShortcuts(QKeySequence::Paste);
	pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
		"selection"));
	connect(pasteAct, &QAction::triggered, textEdit, &QPlainTextEdit::paste);
	editMenu->addAction(pasteAct);
	editToolBar->addAction(pasteAct);

	menuBar()->addSeparator();

#endif // !QT_NO_CLIPBOARD

	QMenu *helpMenu = menuBar()->addMenu(tr("&Help"));
	QAction *aboutAct = helpMenu->addAction(tr("&About"), this, &MainWindow::about);
	aboutAct->setStatusTip(tr("Show the application's About box"));


	QAction *aboutQtAct = helpMenu->addAction(tr("About &Qt"), qApp, &QApplication::aboutQt);
	aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));

#ifndef QT_NO_CLIPBOARD //буфер
	cutAct->setEnabled(false);
	copyAct->setEnabled(false);
	connect(textEdit, &QPlainTextEdit::copyAvailable, cutAct, &QAction::setEnabled);
	connect(textEdit, &QPlainTextEdit::copyAvailable, copyAct, &QAction::setEnabled);
#endif // !QT_NO_CLIPBOARD

*/
}

void TeconAssistants::on_lbAnyWidget_clicked()
{
//	startWindowWidght = new startWindow(this);//главное указать родителя, тогда все закроется!!!!
//	startWindowWidght->setWindowFlag(Qt::Window);//тип открытия - в окне
//	startWindowWidght->show();
}

void TeconAssistants::closeEvent(QCloseEvent *event)
{
	msExit = new QMessageBox();

	event->ignore();

	QAbstractButton *yes = msExit->addButton(tr("Yes"), QMessageBox::YesRole);
	QAbstractButton *no = msExit->addButton(tr("No"), QMessageBox::NoRole);

	msExit->setWindowTitle(tr("Exit"));
	msExit->setText(tr("To exit the programm?"));
	msExit->setIcon(QMessageBox::Information);
	msExit->exec();

	if (msExit->clickedButton() == yes)// 
	{
		event->accept();
//		qApp->setQuitOnLastWindowClosed(true);
	}
}