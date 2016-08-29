#include "mainwindow.h"
#include <QApplication>
#include "../LogLib/logger.h"
#include <QThread>

int main(int argc, char *argv[]){
	Log::Logger::initialize("./log.cfg", "HostSim");
	Log::Logger::traceV("HOSTSIM", "main thread id: %d", QThread::currentThreadId());
	QApplication a(argc, argv);
	MainWindow w;
	w.show();

	return a.exec();
}
