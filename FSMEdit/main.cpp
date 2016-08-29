#include "dialog.h"
#include <QApplication>
#include <QDir>
#include "../LogLib/logger.h"
#include "../UtilLib/reg.h"

int main(int argc, char *argv[]){

	Log::Logger::initialize("./log.cfg", "TestScene");
    UtilLib::Reg::set_basedir((QDir::currentPath() + '/').toStdString().c_str());
	QApplication a(argc, argv);
	Dialog w;
    w.setWindowFlags(Qt::Window);
	w.show();

	return a.exec();
}
