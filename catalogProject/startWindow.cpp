#include "startWindow.h"

#define BOOL_STR(b) (b?"true":"false")

//dataConnect.push_back(QString::fromLocal8Bit(DbName.c_str()));	//преобразование std::string в QString  с кирилицей
//sendDataStr.push_back(QString::fromUtf8(DbName.c_str()));			//преобразование std::string в QString  с кирилицей
//DbName = sendDataStr[0].toLocal8Bit();							//преобразование QString в  std::string с кирилицей


startWindow::startWindow(QWidget *parent)
	: QWidget(parent)
{
	grid = new QGridLayout();
	setLayout(grid);

	msExit = new QMessageBox();
	msExit->setIcon(QMessageBox::Information);

	tvCatalog = new QTreeView();
	tvCatalog->setEditTriggers(QAbstractItemView::NoEditTriggers);//запрет редактирования
//	m_catalogModel = new catalogModel();
	QStringList headers;
	QString dataTree = "";
	headers << tr("Name data base") << tr("Path data base");
	m_treeModel = new TreeModel(headers, dataTree);

	grid->addWidget(tvCatalog, 0, 0, 1, 3);
	setFixedSize(400, 300);
//	tvCatalog->setModel(m_catalogModel);
	tvCatalog->setModel(m_treeModel);

	bnAdd = new QPushButton();
	QObject::connect(bnAdd, SIGNAL(clicked()), this, SLOT(on_onAppend_clicked()));
	grid->addWidget(bnAdd, 1, 0, 1, 1);

	bnDel = new QPushButton();
	QObject::connect(bnDel, SIGNAL(clicked()), this, SLOT(on_onDel_clicked()));
	//Удаляем выделенную строку
//		QObject::connect(bnDel, SIGNAL(clicked()), m_treeModel, SLOT(removeRows(1,1)));
	grid->addWidget(bnDel, 1, 1, 1, 1);

	bnEdit = new QPushButton();
	QObject::connect(bnEdit, SIGNAL(clicked()), this, SLOT(on_onEdit_clicked()));
	grid->addWidget(bnEdit, 1, 2, 1, 1);

	//инициализируем окно коннекта
	m_connect = new dialogConnectParams(this);//инициализировать новую форму с этим родителем
	m_connect->setWindowFlag(Qt::Window);//запустить отдельным окном

		//свяжем двойной клик на представлении со слотом двойного клика в модели
		QObject::connect(this->tvCatalog, SIGNAL(doubleClicked(QModelIndex)), m_treeModel, SLOT(OnDoubleClicked(QModelIndex)));
		QObject::connect(m_treeModel, SIGNAL(closeWindow()), this, SLOT(start_Assistants()));
		//по одинарному клику выбираем проект и отдаем данные
		QObject::connect(this->tvCatalog, SIGNAL(clicked(QModelIndex)), m_treeModel, SLOT(OnClicked(QModelIndex)));
		QObject::connect(m_treeModel, SIGNAL(iClicked()), this, SLOT(dataForConnect()));
		//получаем данные из формы ввода пути, добаляем новую базу в модель
		QObject::connect(m_connect, SIGNAL(sendData(QVector <QString>)), this, SLOT(recieveData(QVector <QString>)));
		//отдаем данные в форму для их изменения
		QObject::connect(this, SIGNAL(sendData(QVector <QString>)), m_connect, SLOT(recieveData(QVector <QString>)));

	retranslateUi();

	//********Прочитаем файл настроек , найдем все пути и добавим их в модель
	QVector <QString> sendDataStr;//0,1,2 - имя,сервер,путь
	std::vector <std::string> vectorParam;

	settings::getNameProject(vectorParam);

	for (size_t i = 0; i < vectorParam.size(); i++)
	{
		DbName.clear();
		ServerName.clear();
		DbPath.clear();
		sendDataStr.clear();

		for (size_t j = 0; j < vectorParam[i].size(); j++)
		{
			if (vectorParam[i][j] != '=')
			{
				DbName.push_back(vectorParam[i][j]);
			}
			else
			{
				break;
			}
		}
		try
		{
			vectorParam[i].erase(0, DbName.size() + 1);
		}
		catch (std::out_of_range&)
		{
		}
		cutPath(vectorParam[i], ServerName, DbPath);
		logs << DbName << std::endl;
		logs << ServerName << std::endl;
		logs << DbPath << std::endl;
		sendDataStr.push_back(QString::fromUtf8(DbName.c_str()));
		sendDataStr.push_back(QString::fromUtf8(ServerName.c_str()));
		sendDataStr.push_back(QString::fromUtf8(DbPath.c_str()));

		recieveData(sendDataStr);
	}
	//********
}

startWindow::~startWindow()
{
}

void startWindow::retranslateUi()
{
	setWindowTitle(tr("Catalog project"));
	bnAdd->setText(tr("Add"));
	bnDel->setText(tr("Delete"));
	bnEdit->setText(tr("Edit"));
}

void startWindow::on_onAppend_clicked()
{
	m_pushEdit = false;//обнуляем нажатие кнопки ИЗМЕНИТЬ пр нажатии всех других кнопок
	//вызываем окно ввода данных для новой БД
	m_connect->show();
}

void startWindow::on_onDel_clicked()
{
	//Получаем модельный индекс и модель элемента:
	QModelIndex index = tvCatalog->selectionModel()->currentIndex();
	QAbstractItemModel *model = tvCatalog->model();
	bool isRoot;//если принадлежит корневому узлу

	isRoot = model->parent(index).data().toString() == "";
	if (isRoot)//весь метод выполняем только если узел корневой и он еще не подключался!
	{
		m_pushEdit = false;//обнуляем нажатие кнопки ИЗМЕНИТЬ пр нажатии всех других кнопок
		QModelIndex index = tvCatalog->selectionModel()->currentIndex();
		QAbstractItemModel *model = tvCatalog->model();
		try
		{
			//удаляем выдленный элемент
			model->removeRow(index.row(), index.parent());
		}
		catch (const std::exception&)
		{
			msExit->setText(tr("The project is not deleted!"));
			msExit->exec();
		}
	}
}

void startWindow::on_onEdit_clicked()
{
	m_pushEdit = true;//устанавливаем нажатие кнопки изменить
	initProject = false;//сбрасываем подключение к прошлому проекту
	find_data_on_push();//набиваем вектор данных по нажатию

	emit sendData(dataConnect); // вызываем сигнал, в котором передаём введённые данные
	m_connect->show();
}

//выполняется по двойному клику, только на ревизии должно щелкаться
void startWindow::start_Assistants()
{
	//Получаем модельный индекс и модель элемента:
	QModelIndex index = tvCatalog->selectionModel()->currentIndex();
	QAbstractItemModel *model = tvCatalog->model();
	//если клик на конечном элементе (ревизии)
	if (model->rowCount(index)== 0)//если клик на ревизии
	{
		//*****************************
		//*****************************
		//ОРГАНИЗОВАТЬ ОТПРАВКУ ДАННЫХ***
		//*****************************
		//*****************************

		TeconAssistants *ta;
		ta = new TeconAssistants();
		ta->showMaximized();
		this->close();
	}
}

//выполняется по одинарному клику, далаем коннект к БД и вычитываем все ревизии проекта
void startWindow::dataForConnect()
{
	//Получаем модельный индекс и модель элемента:
	QModelIndex index = tvCatalog->selectionModel()->currentIndex();
	QAbstractItemModel *model = tvCatalog->model();
	index = model->index(index.row(), 0, index.parent());

	bool isRoot;//если принадлежит корневому узлу
	bool isNotConnected;//он еще не подключался!
	isRoot = model->parent(index).data().toString() == "";
	isNotConnected = index.data().toString()[0] != '[';
	if (isRoot && isNotConnected)//весь метод выполняем только если узел корневой и он еще не подключался!
	{
		find_data_on_push();//набиваем вектор данных по нажатию
		dataBase = new SQL_to_FB(ServerName, DbPath, UserName, Password);
		dataBase->connect();
		if (dataBase->connected())
		{
			std::vector <std::any> project;
			std::vector <std::any> name_project;
			//формируем запрос на получение всех проектов в базе
			query.clear();
			query = "SELECT PROJECT, NAME_FOR_TABLE FROM MAIN_TABLE";
			dataBase->read(query, "MAIN_TABLE", "PROJECT", project);
			dataBase->read(query, "MAIN_TABLE", "NAME_FOR_TABLE", name_project);
			if (project.size() > 1)//если проектов больше одного
			{
				//организуем цикл перебора всех проектов в базе
				for (size_t i = 0; i < project.size(); i++)
				{
					//добавим проект
					QModelIndex index_ch = addChildRevision(QString::fromLocal8Bit(dataBase->any_to_str(project[i]).c_str()),
						"", index, model);
					add_child(name_project[i], index_ch, model);
				}
			}
			else 
			{
				add_child(name_project.front(), index, model);
			}
		}
		else
		{
			msExit->setText(tr("Database connection error!"));
			msExit->exec();
		}
		model->setData(index, QVariant('['+index.data().toString()+']'), Qt::EditRole);//Переименовали подключенный проект
	}
}

QModelIndex startWindow::addChildRevision(QString revision, QString desc, QModelIndex _index, QAbstractItemModel *_model)
{
	//Вставляем данные:
	if (_model->columnCount(_index) == 0) 
	{
		if (!_model->insertColumn(0, _index)) return QModelIndex();
	}
	if (!_model->insertRow(0, _index)) return QModelIndex();
	//Инициализируем их:
	for (int column = 0; column < _model->columnCount(_index); ++column) {
		QModelIndex child = _model->index(0, 0, _index);//добавим только в нулевой столбец имя ревизии
		_model->setData(child, QVariant(revision), Qt::EditRole);
					child = _model->index(0, 1, _index);//добавим только в нулевой столбец имя ревизии
		_model->setData(child, QVariant(desc), Qt::EditRole);
//		if (!model->headerData(column, Qt::Horizontal).isValid())
//			model->setHeaderData(column, Qt::Horizontal, QVariant("Столбец"), Qt::EditRole);
	}
	//Выбираем вставленный узел, для раскрытия дерева
	tvCatalog->selectionModel()->setCurrentIndex(_model->index(0, 0, _index),QItemSelectionModel::ClearAndSelect);
	return _model->index(0, 0, _index);
	//очистим выбор
//	tvCatalog->selectionModel()->clear();
}
	
void startWindow::recieveData(QVector <QString> sendDataStr)
{
	//очищаем вектор для новых подключений
	dataConnect.clear();
	DbName.clear();
	ServerName.clear();
	DbPath.clear();
	QAbstractItemModel *model = tvCatalog->model();
	//получаем индексы из модели, куда вставлять
	QModelIndex index = tvCatalog->selectionModel()->currentIndex();
	QVariant data_name = model->data(index, Qt::EditRole);
	if (m_pushEdit)//если мы делаем изменение
	{
		on_onDel_clicked();//удаляем старый проект для реконекта
	}

	//Собираем в 2 колонки данные от формы ввода
	dataConnect.push_back(sendDataStr[0]);
	DbName = sendDataStr[0].toLocal8Bit();//преобразование QString в  std::string с кирилицей
	dataConnect.push_back(sendDataStr[1] + ":" + sendDataStr[2]);
	ServerName = sendDataStr[1].toLocal8Bit();
	DbPath = sendDataStr[2].toLocal8Bit();
	//проверка на пустые данные
	if (!sendDataStr[0].isEmpty() && !sendDataStr[1].isEmpty() && !sendDataStr[2].isEmpty())
	{
		//получаем индексы из модели, куда вставлять
		index = tvCatalog->selectionModel()->currentIndex();
		//вставка новой строки
		model->insertRow(index.row() + 1, index.parent());

		//наполняем строку, пробегаясь по 1 и 2 колонке
		for (int column = 0; column < model->columnCount(index.parent()); ++column)
		{
			try
			{
				QModelIndex child = model->index(index.row() + 1, column, index.parent());
				model->setData(child, QVariant(dataConnect[column]), Qt::EditRole);
			}
			catch (const std::exception&)
			{
				msExit->setText(tr("The project is not added!"));
				msExit->exec();
			}
		}
	}
	else
	{
		msExit->setText(tr("The entered data is incorrect!"));
		msExit->exec();
	}
}

void startWindow::cutPath(std::string source, std::string &server, std::string &path)
{
	//Запишем WEB-сервер в строку параметра
	for (size_t i = 0; i < source.size(); i++)
	{
		if (source[i] != ':')
		{
			server.push_back(source[i]);
		}
		else
		{
			break;
		}
	}
	//вырежем web-сервер из параметра "путь"
	try
	{
		source.erase(0, server.size() + 1);
	}
	catch (std::out_of_range&)
	{
	}
	path = source;
}

void startWindow::find_data_on_push()
{
	//очищаем вектор для новых подключений
	dataConnect.clear();
	DbName.clear();
	ServerName.clear();
	DbPath.clear();

	//Получаем модельный индекс и модель элемента:
	QModelIndex m_index = tvCatalog->selectionModel()->currentIndex();
	QAbstractItemModel *model = tvCatalog->model();
	QVariant data_name;
	QVariant data_path;
	std::string tmp;

	//получаем данные их модели по выбранному индексу
	m_index = tvCatalog->model()->index(m_index.row(), 0);
	data_name = model->data(m_index, Qt::EditRole);
	//меняем индекс на соседнюю колонку с путем к проекту и получаем ее
	m_index = tvCatalog->model()->index(m_index.row(), 1);
	data_path = model->data(m_index, Qt::EditRole);

	//записываем имя проекта в переменную
	DbName = data_name.toString().toLocal8Bit();
	//разберем сервер и путь на составляющие
	tmp = data_path.toString().toLocal8Bit();
	cutPath(tmp, ServerName, DbPath);
	//Соберем данные для отправки в другую форму
	dataConnect.push_back(QString::fromLocal8Bit(DbName.c_str()));//преобразование std::string в QString  с кирилицей
	dataConnect.push_back(QString::fromLocal8Bit(ServerName.c_str()));
	dataConnect.push_back(QString::fromLocal8Bit(DbPath.c_str()));
}

std::string startWindow::translate(std::string text)
{
	char rus[75] = { 'А', 'а', 'Б', 'б',    'В', 'в',
		'Г', 'г', 'Ґ', 'ґ', 'Д', 'д', 'Е', 'е', 'Є', 'є',
		'Ж', 'ж', 'З', 'з', 'И', 'и', 'І', 'і', 'Ї', 'ї', 'Й', 'й', 'К', 'к',
	   'Л', 'л', 'М', 'м', 'Н', 'н', 'О', 'о', 'П', 'п', 'Р',
		'р', 'С', 'с', 'Т', 'т', 'У', 'у', 'Ф', 'ф', 'Х',
		'х', 'Ц', 'ц', 'Ч', 'ч', 'Ш', 'ш',
	   'Щ', 'щ', 'Ь', 'ь', 'Ю', 'ю', 'Я', 'я', 'Ы', 'ы', 'Ъ',
		'ъ', 'Ё', 'ё', 'Э', 'э' , ' ' };

	std::string eng[75] = { "A", "a", "B", "b",    "V", "v",
		"G", "g", "G", "g", "D", "d", "E", "e", "E", "E", "Zh",
		"zh", "Z", "z", "I", "i", "I", "I", "Yi", "yi", "J", "j", "K", "k",
		"L", "l", "M", "m", "N", "n", "O", "o", "P", "p",   "R",
		"r", "S", "s", "T", "t", "U", "u", "F", "f",   "H", "h",
		"Ts", "ts", "ch", "ch", "Sh", "sh",
		"Shh", "shh", "'", "'", "Yu", "yu","Ya", "ya", "Y", "y", "",
		"", "Yo", "yo", "E", "e" , "_"};

	bool find = false;
	std::string ret;
	for (int i = 0; i <= text.length(); i++)
	{
		find = false;
		for (int j = 0; j < 75; j++)
		{
			if (text[i] == rus[j]) 
			{ 
				ret += eng[j];
				find = true;
				//i++;
				break;
			}
		}
		if (!find) ret += text.substr(i, 1);
	}
	return ret;
}

void startWindow::add_child(std::any name_project, QModelIndex _index, QAbstractItemModel *_model)
{
	std::vector <std::any> revision;
	std::vector <std::any> desc_revision;
	//запросим все ревизии в проекте
	query.clear();
	revision.clear();
	desc_revision.clear();
	query = "SELECT ID, DESC FROM " + (dataBase->any_to_str(name_project));
	dataBase->read(query, dataBase->any_to_str(name_project), "ID", revision);
	dataBase->read(query, dataBase->any_to_str(name_project), "DESC", desc_revision);
	//наполняем проект ревизями

	for (size_t j = 0; j < revision.size(); j++)
	{
		addChildRevision(QString::fromLocal8Bit(dataBase->any_to_str(revision[j]).c_str()),
		QString::fromLocal8Bit(dataBase->any_to_str(desc_revision[j]).c_str()), _index, _model);
	}
}

//
//SYSDBA
//
//
//nI*#Sp28

//запрос на создание нового проекта
//CREATE TABLE ewe_proect
//(ID       SMALLINT         NOT NULL,
//	DESC    VARCHAR(155) CHARACTER SET WIN1251,
//	OBJECT     blob,
//	PRIMARY KEY(ID))


//
////тест записи BLOB объектов в firebird
//void startWindow::Test4()
//{
//	printf(_("Test 4 --- Populate database and exercise Blobs and Arrays (100 rows)\n"));
//
//	IBPP::Database db1;
//	db1 = IBPP::DatabaseFactory(ServerName, DbName, UserName, Password);
//	db1->Connect();
//
//	// The following transaction configuration values are the defaults and
//	// those parameters could have as well be omitted to simplify writing.
//	IBPP::Transaction tr1 = IBPP::TransactionFactory(db1,
//		IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
//	tr1->Start();
//
//	IBPP::Statement st1 = IBPP::StatementFactory(db1, tr1);
//
//	IBPP::Blob b1 = IBPP::BlobFactory(st1->DatabasePtr(), st1->TransactionPtr());
//	IBPP::Blob bb = IBPP::BlobFactory(db1, tr1);
//	IBPP::Array ar1 = IBPP::ArrayFactory(db1, tr1);
//	IBPP::Array ar2 = IBPP::ArrayFactory(db1, tr1);
//	//IBPP::Array ar4 = IBPP::ArrayFactory(db1, tr1);
//	IBPP::Array da = IBPP::ArrayFactory(db1, tr1);
//
//	// Checking the new date and time support
//	st1->ExecuteImmediate("insert into test(D, T, TS) "
//		"values('2004-02-29', '10:11:12.1314', '1858-11-18 10:11:12.1314')");
//
//	st1->Execute("select D, T, TS from test");
//	if (st1->Sql().compare("select D, T, TS from test") != 0)
//	{
//		_Success = false;
//		printf("Statement::Sql() failed.");
//		return;
//	}
//	st1->Fetch();
//	IBPP::Date dt;
//	IBPP::Time tm;
//	IBPP::Timestamp ts;
//	st1->Get(1, dt);
//	st1->Get(2, tm);
//	st1->Get(3, ts);
//
//	int y, m, d, h, min, s, t;
//	dt.GetDate(y, m, d);
//	if (y != 2004 || m != 2 || d != 29)
//	{
//		_Success = false;
//		printf("Date storage anomaly : %d / %d / %d\n", y, m, d);
//		return;
//	}
//
//	tm.GetTime(h, min, s, t);
//	if (h != 10 || min != 11 || s != 12 || t != 1314)
//	{
//		_Success = false;
//		printf("Time storage anomaly : %d : %d : %d : %d\n", h, min, s, t);
//		return;
//	}
//
//	ts.GetDate(y, m, d);
//	ts.GetTime(h, min, s, t);
//	if (y != 1858 || m != 11 || d != 18 ||
//		h != 10 || min != 11 || s != 12 || t != 1314)
//	{
//		_Success = false;
//		printf("Timestamp storage anomaly : %d / %d / %d, %d : %d : %d : %d\n",
//			y, m, d, h, min, s, t);
//		return;
//	}
//	st1->ExecuteImmediate("delete from test");
//
//	// Now simulating (more or less) a connection loss, and a re-connection using
//	// the same host variables. Let's consider the db1 "lost" and re-use it.
//
//	db1 = IBPP::DatabaseFactory(ServerName, DbName, UserName, Password);
//	db1->Connect();
//	tr1 = IBPP::TransactionFactory(db1,
//		IBPP::amWrite, IBPP::ilConcurrency, IBPP::lrWait);
//	tr1->Start();
//
//	st1 = IBPP::StatementFactory(db1, tr1);
//	b1 = IBPP::BlobFactory(db1, tr1);
//	bb = IBPP::BlobFactory(db1, tr1);
//	ar1 = IBPP::ArrayFactory(db1, tr1);
//	ar2 = IBPP::ArrayFactory(db1, tr1);
//	da = IBPP::ArrayFactory(db1, tr1);
//
//	// Continue on to other tests...
//	st1->Prepare("insert into test(N2,N6,N5,D,B,BB,TF,ID,A1,A2,DA,TX,VX,TB,VB) "
//		"values(?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
//	if (st1->Parameters() != 15)
//	{
//		_Success = false;
//		printf(_("Statement::Parameters() failed to return correct count of parameters."));
//	}
//	else
//	{
//		if (st1->ParameterType(13) != IBPP::sdString ||
//			st1->ParameterSubtype(5) != 1 ||
//			st1->ParameterSize(13) != 30)
//		{
//			_Success = false;
//			printf("Type(13) == %d\n", st1->ParameterType(13));
//			printf("SubType(5) == %d\n", st1->ParameterSubtype(5));
//			printf("Size(13) == %d\n", st1->ParameterSize(13));
//			printf(_("Statement::ParameterType(), Subtype() or Size() failed to return correct value."));
//		}
//	}
//	st1->Set(1, 12.3456);
//	st1->Set(2, 12.3456);
//	st1->Set(3, 987.65432);
//	IBPP::Date dt1;
//	dt1.SetDate(1850, 2, 12);
//	IBPP::Date dt2 = dt1;
//	st1->Set(4, dt2);
//
//	double a1[8] = { 11.123, 12.126, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0 };
//	char a2[4][4][31] = {
//							{"UN", "DEUX", "TROIS", "QUATRE"},
//							{"CINQ", "SIX", "SEPT", "HUIT"},
//							{"NEUF", "DIX", "ONZE", "DOUZE"},
//							{"TREIZE", "QUATORZE", "QUINZE", "SEIZE"} };
//	IBPP::Timestamp da1[2];
//	da1[0].SetDate(2002, 8, 14);
//	da1[1].SetDate(2003, 2, 28);
//	da1[0].SetTime(15, 16, 17);
//	da1[1].SetTime(18, 19, 20);
//
//	ar1->Describe("TEST", "A1");
//	ar2->Describe("TEST", "A2");
//	da->Describe("TEST", "DA");
//
//	int i;
//	/*
//	printf("A1 type  = %d\n", ar1->ElementType());
//	printf("A1 size  = %d\n", ar1->ElementSize());
//	printf("A1 scale = %d\n", ar1->ElementScale());
//	printf("A1 dim   = %d\n", ar1->Dimensions());
//
//	for (i = 0; i < ar1->Dimensions(); i++)
//	{
//		int low, high;
//		ar1->Bounds(i, &low, &high);
//		printf("Dim %i = [%d:%d]\n", i, low, high);
//	}
//	*/
//
//	/*
//	// Huge array testing
//	int* pa4 = new int[1200];
//	for (i = 0; i < 1200; i++) pa4[i] = i;
//	ar4->Describe("TEST", "A3");
//	*/
//
//	std::string stdstring = "STD::STRING";
//
//	int total = 0;
//	int somebytes[10] = { 1, 2, 3, 0, 5, 6, 7, 8, 0, 10 };
//
//	FILE* file = fopen("blob.txt", "w");
//	for (i = 0; i < 1000; i++)
//		fputs(_("Dummy blob data for running some blob input/output tests.\n"), file);
//	fclose(file);
//	for (i = 0; i < 100; i++)
//	{
//		// Writing a blob, using the low-level interface
//		char buffer[10240];
//		int len;
//		b1->Create();
//		FILE* file = fopen("blob.txt", "r");
//		while ((len = (int)fread(buffer, 1, 10240, file)) == 10240)
//		{
//			b1->Write(buffer, 10240);
//			total += 10240;
//		}
//		b1->Write(buffer, len);
//		total += len;
//		b1->Close();
//		fclose(file);
//		st1->Set(5, b1);
//
//		// Writing a blob, using the std:string interface
//		std::string bbs;
//		file = fopen("blob.txt", "r");
//		bbs.resize(40000);
//		fread(const_cast<char*>(bbs.data()), 1, 40000, file);
//		fclose(file);
//		//bb->Save(bbs);
//		//st1->Set(6, bb);
//
//		// Third, direct way of writing a std::string to a blob
//		st1->Set(6, bbs);
//
//		st1->Set(7, (i % 2) != 0);
//#ifdef __DMC__
//		st1->Set(8, (int32_t)i);
//#else
//		st1->Set(8, i);
//#endif
//
//		ar1->WriteFrom(IBPP::adDouble, a1, sizeof(a1) / sizeof(double));
//		st1->Set(9, ar1);
//		ar2->WriteFrom(IBPP::adString, a2, sizeof(a2) / sizeof(char[31]));
//		st1->Set(10, ar2);
//		da->WriteFrom(IBPP::adTimestamp, da1, sizeof(da1) / sizeof(IBPP::Timestamp));
//		st1->Set(11, da);
//
//		st1->Set(12, "C-STRING");
//		st1->Set(13, stdstring);
//
//		st1->Set(14, (char*)somebytes, 40);
//		st1->Set(15, (char*)somebytes, 32);
//
//		/*
//		ar4->WriteFrom(IBPP::adInt, pa4, 1200);
//		st1->Set(16, ar4);
//		*/
//
//		st1->Execute();
//	}
//	total /= 100;
//
////	DeleteFile("blob.txt");
//
//	tr1->CommitRetain();
//
//	st1->Prepare("select B, BB, A2 from test where ID = 1");
//
//	std::string plan;
//	st1->Plan(plan);
//	printf("           Plan: ");
//	printf(plan.c_str());
//	printf("\n");
//
//	st1->Execute();
//	IBPP::Row row;
//	st1->Fetch(row);
//	IBPP::Row row2 = row->Clone();	// row2 is a real copy not a second pointer to same data
//
//	IBPP::Blob b2 = IBPP::BlobFactory(db1, tr1);
//	IBPP::Blob bb2 = IBPP::BlobFactory(db1, tr1);
//	IBPP::Array ar3 = IBPP::ArrayFactory(db1, tr1);
//	ar3->Describe("TEST", "A2");
//	char buffer[1024];
//	int size, largest, segments;
//
//	row2->Get(1, b2);
//	b2->Open();
//	b2->Info(&size, &largest, &segments);
//	if (size != total)
//	{
//		_Success = false;
//		printf(_("Blob size incorrect on read back from the database.\n"));
//		printf(_("Read %d bytes, expected %d\n"), size, total);
//	}
//	b2->Read(buffer, 1024);
//	fflush(stdout);
//	b2->Close();
//
//	std::string bbs;
//	//row2->Get(2, bb2);
//	//bb2->Load(bbs);
//	row2->Get(2, bbs);
//	//printf("Size = %d\n", bbs.size());
//
//	row2->Get(3, ar3);
//	char a3[2][2][31] = { {"", ""},
//							{"", ""} };
//	// The stored array is 4 x 4 (0:3 by 1:4).
//	// We only want to retrieve the central squarre of 2 x 2 (1:2 by 2:3)
//	ar3->SetBounds(0, 1, 2);
//	ar3->SetBounds(1, 2, 3);
//	ar3->ReadTo(IBPP::adString, a3, sizeof(a3) / sizeof(char[31]));
//	/*
//	printf("%s %s\n", a3[0][0], a3[0][1]);
//	printf("%s %s\n", a3[1][0], a3[1][1]);
//	*/
//	if (strcmp(a3[0][0], "SIX") != 0 ||
//		strcmp(a3[0][1], "SEPT") != 0 ||
//		strcmp(a3[1][0], "DIX") != 0 ||
//		strcmp(a3[1][1], "ONZE") != 0)
//	{
//		_Success = false;
//		printf(_("Array testing didn't returned the expected values.\n"));
//	}
//
//	/*
//	ar3->Describe("TEST", "A3");
//	st1->Get(4, ar3);
//	for (i = 0; i < 1200; i++) pa4[i] = -1;
//	ar3->ReadTo(IBPP::adInt, pa4, 1200);
//	for (i = 0; i < 1200; i++)
//	{
//		if (pa4[i] != i)
//		{
//			_Success = false;
//			printf(_("Big array testing didn't returned the expected values.\n"));
//			printf(_("Got %d for element %d instead of %d.\n"), pa4[i], i, i);
//			break;
//		}
//	}
//
//	delete [] pa4;
//	*/
//
//	st1->Close();	// unneeded : just to test an old bug fix
//	tr1->Commit();
//
//	// Testing old weird cursor close issues on commits/rollbacks
//	tr1->Start();
//	st1->Prepare("select B, BB, A2 from test where ID = 1 for update");
//	st1->CursorExecute("name");
//	st1->Fetch();
//	tr1->Commit();	// This used to be an issue with CursorFree()
//
//	db1->Disconnect();
//} //тест записи BLOB объектов в firebird
////тест записи BLOB объектов в firebird
//void startWindow::Test5()
//{
//	printf(_("Test 5 --- Cocktail of DML statements (100 rows)\n"));
//
//	IBPP::Database db1;
//	db1 = IBPP::DatabaseFactory(ServerName, DbName, UserName, Password);
//	db1->Connect();
//
//	// The following transaction configuration values are the defaults and
//	// those parameters could have as well be omitted to simplify writing.
//	IBPP::Transaction tr1 = IBPP::TransactionFactory(db1, IBPP::amWrite,
//		IBPP::ilConcurrency, IBPP::lrWait, IBPP::tfNoAutoUndo);
//	tr1->AddReservation(db1, "TEST", IBPP::trProtectedWrite);
//	tr1->Start();
//
//	IBPP::Statement st1 = IBPP::StatementFactory(db1, tr1);
//
//	IBPP::Blob b1 = IBPP::BlobFactory(db1, tr1);
//
//	st1->Prepare("select ID, rdb$db_key, N2, D, TF, TX, VX from TEST FOR UPDATE");
//	st1->CursorExecute("MYCURSOR");
//	
//	/*
//	for (int ii = 1; ii <= st1->Columns(); ii++)
//	{
//		printf("Column %s, Alias %s, Table %s, Type %d, Size %d, Scale %d\n",
//			st1->ColumnName(ii), st1->ColumnAlias(ii), st1->ColumnTable(ii),
//			(int)st1->ColumnType(ii), st1->ColumnSize(ii), st1->ColumnScale(ii));
//	}
//	*/
//
//	IBPP::Statement st2 = IBPP::StatementFactory(db1, tr1,
//		"UPDATE TEST set n2 = ?, TF = ? where current of MYCURSOR");
//	int incr = 0;
//	while (st1->Fetch())
//	{
//		IBPP::Date d2;
//		IBPP::Date d3;
//		int y, m, d;
//		int temp;
//		char cstring[31];
//		std::string stdstring;
//
//		st1->Get(4, d2);
//		temp = d2.GetDate();
//		d3 = temp;
//		d3.Add(-incr);
//		incr++;
//		d3.GetDate(y, m, d);
//		//printf("%d, %d, %d ",  y, m, d);
//		double tmp;
//		st1->Get(3, &tmp);
//		st2->Set(1, tmp*40.0);
//		bool b;
//		st1->Get(5, &b);
//		bool c;
//		st1->Get(5, c);
//		if (b != c)
//		{
//			_Success = false;
//			printf(_("Statement::Get(int, bool&) is not working.\n"));
//		}
//		//printf("%s\n", b ? "true" : "false");
//		/*
//		printf("%s\n", row->Get("TF"));
//		printf("%d\n", row->Get("ID"));
//		*/
//		st1->Get(6, cstring);
//		st1->Get(7, stdstring);
//
//		st2->Set(2, !b);
//		st2->Execute();
//	}
//
//	st1->Prepare("select sum(N2),sum(N6) from test");
//	st1->Execute();
//	while (st1->Fetch())
//	{
//		double n2, n6;
//		int ni2, ni6;
//		st1->Get(1, n2);
//		st1->Get(2, n6);
//		st1->Get(1, ni2);
//		st1->Get(2, ni6);
//		//printf("%g, %g, %d, %d\n", n2, n6, ni2, ni6);
//	}
//
//	//printf(_("Select returning no rows...\n"));
//	st1->Prepare("select N2 from test where N2 = 456278");
//	st1->Execute();
//	while (st1->Fetch())
//	{
//		_Success = false;
//		printf(_("Statement::Fetch() not working.\n"
//			"Returned row when there is none.\n"));
//		//double n2, n6;
//		//st1->Get(1, n2);
//		//st1->Get(2, n6);
//		//printf("%g, %g\n", n2, n6);
//	}
//
//	//printf(_("Again, without prepare again...\n"));
//	st1->Execute();
//	while (st1->Fetch())
//	{
//		_Success = false;
//		printf(_("Statement::Fetch() not working.\n"
//			"Returned row when there is none.\n"));
//		//double n2, n6;
//		//st1->Get(1, n2);
//		//st1->Get(2, n6);
//		//printf("%g, %g\n", n2, n6);
//	}
//
//
//	//	printf(_("Executing 10 times an Execute, Prepared once...\n"));
//	st1->Prepare("select N2, N6 from test");
//	for (int i = 0; i < 10; i++)
//	{
//		//printf(_("Loop %d\n"), i);
//		st1->Execute();
//		while (st1->Fetch())
//		{
//			double n2, n6;
//			st1->Get(1, &n2);
//			st1->Get(2, &n6);
//			//printf("%g, %g\n", n2, n6);
//		}
//	}
//
//	//	printf(_("Testing IBPP::Row...\n"));
//	std::vector<IBPP::Row> rows;
//	IBPP::Row r;
//	st1->Execute("select N2, N6 from test");
//	while (st1->Fetch(r))
//		rows.push_back(r);
//
//	for (unsigned i = 0; i < rows.size(); i++)
//	{
//		double n2, n6;
//		rows[i]->Get(1, n2);
//		rows[i]->Get(2, n6);
//		//printf("%g, %g\n", n2, n6);
//	}
//
//	// The rows vector will be deleted when function returns, which
//	// will release all individual rows and their storage. Check for leaks !
//
//	// Now, run a select, fetch all but the last row
//	st1->Execute("select N2, N6 from test");
//	for (unsigned i = 0; i < rows.size() - 1; i++)
//		st1->Fetch();
//
//	// And commit
//	tr1->Commit();
//} //тест записи BLOB объектов в firebird
//
////сериализация модели в XML
//void startWindow::exportToXml(const QString& fileName)
//{
//QString _fileName = "test.xml";
//QFile file(fileName);
//if (!file.open(QIODevice::WriteOnly)) {
//	QMessageBox::critical(this, "", tr("Error save to file.\n%1").arg(file.errorString()));
//	return;
//}
//
//QXmlStreamWriter stream(&file);
//stream.setAutoFormatting(true);
//stream.writeStartDocument();
//stream.writeStartElement("ROWDATA");
//
//for (int i = 0, rowCount = tvCatalog->model()->rowCount(); i < rowCount; i++) {
//	stream.writeStartElement("ROW");
//
//	for (int j = 0, columnCount = tvCatalog->model()->columnCount(); j < columnCount; j++) {
//		const QVariant& value = tvCatalog->model()->index(i, j).data(Qt::DisplayRole);
//		stream.writeTextElement(tvCatalog->model()->headerData(j, Qt::Horizontal, Qt::EditRole).toString(),
//			value.toString());
//	}
//	stream.writeEndElement();
//}
//
//stream.writeEndElement();
//stream.writeEndDocument();
//file.close();
//
//QMessageBox::information(this, "", tr("Export finished"));
//}