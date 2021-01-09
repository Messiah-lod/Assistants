#include "dialogConnectParams.h"



dialogConnectParams::dialogConnectParams(QWidget *parent)
	: QWidget(parent)
{
	mainGrid = new QGridLayout();
	lblServer = new QLineEdit();
	lblPath = new QLineEdit();
	lblName = new QLineEdit();

	btnOK = new QPushButton();
	btnCancel = new QPushButton();

	txtServer = new  QLabel();
	txtPath = new QLabel();
	txtName = new QLabel();

	mainGrid->addWidget(txtName, 0, 0, 1, 2);
	mainGrid->addWidget(lblName, 1, 0, 1, 2);
	mainGrid->addWidget(txtServer, 2, 0, 1, 2);
	mainGrid->addWidget(lblServer, 3, 0, 1, 2);
	mainGrid->addWidget(txtPath, 4, 0, 1, 2);
	mainGrid->addWidget(lblPath, 5, 0, 1, 2);
	mainGrid->addWidget(btnOK, 6, 0, 1, 1);
	mainGrid->addWidget(btnCancel, 6, 1, 1, 1);

	QObject::connect(this->btnOK, SIGNAL (clicked()), this, SLOT(push_OK()));
	QObject::connect(this->btnCancel, SIGNAL(clicked()), this, SLOT(push_Cancel()));

	setLayout(mainGrid);
	setFixedSize(400, 200);
	retranslateUi();
}


dialogConnectParams::~dialogConnectParams()
{
}

void dialogConnectParams::retranslateUi()
{
	txtPath->setText(tr("Insert path to data base"));
	txtServer->setText(tr("Insert WEB-server to data base"));
	txtName->setText(tr("Insert name to data base"));
	btnOK->setText("OK");
	btnCancel->setText(tr("Cancel"));
}


void dialogConnectParams::recieveData(QVector<QString> sendDataStr)
{
	lblName->setText(sendDataStr[0]);
	lblServer->setText(sendDataStr[1]);
	lblPath->setText(sendDataStr[2]);
}

void dialogConnectParams::push_OK()
{
	//чистим вектор от старых данных
	sendDataStr.clear();
	//набираем вектор данных для отправки
	sendDataStr.push_back(lblName->text());
	sendDataStr.push_back(lblServer->text());
	sendDataStr.push_back(lblPath->text());

	emit sendData(sendDataStr); // вызываем сигнал, в котором передаём введённые данные

	//Чистим поля ввода и закрываем форму
	lblName->setText("");
	lblServer->setText("");
	lblPath->setText("");
	this->close();
}

void dialogConnectParams::closeEvent(QCloseEvent *event)
{
	exit();
}

void dialogConnectParams::push_Cancel()
{
	exit();
}

void dialogConnectParams::exit()
{
	lblName->setText("");
	lblServer->setText("");
	lblPath->setText("");
	this->close();
}