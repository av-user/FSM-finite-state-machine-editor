#ifndef FSM_H
#define FSM_H

#include <QObject>

class QDomElement;
class QFileInfo;
class QDir;

class Fsm : public QObject {
    Q_OBJECT
public:
    static Fsm* createFsm (QDomElement *pNode);
    bool generate (QString filename);
    QStringList get_eventlist () const ;

    explicit Fsm(QObject *parent = 0);

signals:

public slots:
private:
    bool mkfsmdir (QFileInfo *pFileinfo);
    bool mkfsmsubdir (QDir *pDir);
    bool generateFramework (QFileInfo *pFsmFileinfo);
    bool generateFwHdl (QFileInfo *pFsmFileinfo);
    bool generateFwFsm(QFileInfo *pFsmFileinfo);
    bool generateFwState_h (QFileInfo *pFsmFileinfo, QString filename);
    bool generateFwState_cpp (QFileInfo *pFsmFileinfo, QString filename);
    bool generateFwHdl_h (QFileInfo *pFsmFileinfo, QString filename
                          , QString custom_inc, QString custom_part);
    bool generateFwHdl_cpp (QFileInfo *pFsmFileinfo, QString filename
                          , QString custom_inc, QString custom_part);
    QString get_siblings_interface_h(QFileInfo *pFsmFileinfo);
    QString get_siblings_interface_cpp(QFileInfo *pFsmFileinfo);
    QString get_siblings_classes(QFileInfo *pFsmFileinfo);
    QString get_siblings_includes_cpp(QFileInfo *pFsmFileinfo);
    QString get_initialize(QFileInfo *pFsmFileinfo);
    QString get_clear(QFileInfo *pFsmFileinfo);
    QString get_constructor(QFileInfo *pFsmFileinfo);
    QString get_sibling_fsms(QFileInfo *pFsmFileinfo);
    QString get_event_handlers(QFileInfo *pFsmFileinfo);
    QString get_event_cases(QFileInfo *pFsmFileinfo);
    QString get_event_exceptions(QFileInfo *pFsmFileinfo);
    QStringList getFsmList (QFileInfo *pFsmFileinfo);


};

#endif // FSM_H
