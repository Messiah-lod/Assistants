#include "TeconAssistants.h"
#include "catalogProject/startWindow.h"
#include <QtWidgets/QApplication>
#include <QtWidgets/QWidget>

int main(int argc, char *argv[])
{
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	QApplication a(argc, argv);

	QTranslator myTranslator;
//	myTranslator.load(QLocale::system().name());
	myTranslator.load(QCoreApplication::applicationDirPath() + "/resource/translation/" + QLocale::system().name());
	a.installTranslator(&myTranslator);

	const QIcon mainIcon = QIcon::fromTheme("mainIcon", QIcon(QCoreApplication::applicationDirPath() + "/resource/icon/icon.ico"));
	a.setWindowIcon(mainIcon);

	startWindow w;
	w.show();

	return a.exec();
}