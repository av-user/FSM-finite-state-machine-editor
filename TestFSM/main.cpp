#include <QApplication>
#include <QQmlApplicationEngine>
#include <QtQml>

#include "handlers/Main/Framework/mainhdl.h"
#include "handlers/Comm/Framework/commhdl.h"
#include "translationutil.h"
#include "../LogLib/logger.h"

static QObject* transut_qobject_sngltntype_prvdr(QQmlEngine *engine, QJSEngine *scriptEngine){
	Q_UNUSED(engine)
	Q_UNUSED(scriptEngine)
	TranslationUtil *pTranslationUtil = new TranslationUtil();
	return pTranslationUtil;
}

int main(int argc, char *argv[]){
	QApplication app(argc, argv);
	QQmlApplicationEngine engine;

	qDebug() << "main() in thread " << QThread::currentThreadId();
	QString d = QDir::currentPath();
    Log::Logger::initialize("./log.cfg", "TestFSM");
    Log::Logger::traceV("GENERAL", "main thread id: %d", QThread::currentThreadId());

	qmlRegisterSingletonType<TranslationUtil>("TranslationUtil", 1, 0, "TranslationUtil", transut_qobject_sngltntype_prvdr);
	engine.rootContext()->setContextProperty("installPath", "file:///" + QDir::currentPath() + "/");
	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

    MainHdl mainHdl (nullptr);
    CommHdl commHdl (nullptr);
    mainHdl.setComm(&commHdl);
    commHdl.setMain(&mainHdl);

	if (engine.rootObjects().length() > 0){
        QObject::connect (engine.rootObjects().at(0), SIGNAL (tocpp(QString))
                        , &mainHdl, SLOT(receiveFromQml(QString)));
        QObject::connect(&mainHdl, SIGNAL(sendToQml(QVariant))
                        , engine.rootObjects().at(0), SLOT(sendToQml(QVariant)));
        mainHdl.start();
        commHdl.start();
		return app.exec();
	} else {
		return -1;
	}
}
