#ifndef STATE_H
#define STATE_H

#include <QObject>

class Fsm;
class QDomElement;
class QFileInfo;
class QFile;
class CppPartsContainer;

class State : public QObject {
    Q_OBJECT
public:
    static QString CUSTOM_INCLUDE_BEGIN_TAG;
    static QString CUSTOM_INCLUDE_END_TAG;
    static QString CUSTOM_PART_BEGIN_TAG;
    static QString CUSTOM_PART_END_TAG;
    static QString FSM_DONTREMOVE_TAG;

    static QString INCLUDEMARK_TAG;
    static QString METHODMARK_TAG;
    static QString HDLMARK_TAG;
    static QString NAMEMARK_TAG;
    static QString RETURN_THIS;


    static void createState (Fsm *pFsm, QDomElement *pNode);
    bool generate (QFileInfo *pFileinfo);
    explicit State(QObject *parent = 0);

signals:

public slots:
private:
    bool generateh (QString filename, QString fsm_filedir);
    bool generatecpp (QString filename, QString fsm_filedir);
    bool cpp_from_template (QString filename, QString fsm_filedir);
    QString get_includes (QString fsm_filedir) const ;
    QString getFWHdlInclude (QString fsm_filedir) const ;
    QString get_NAME(QString fsm_filedir) const ;
    QString get_initialize(QString fsm_filedir) const ;
    QString get_eventhandlers(QString fsm_filedir) const ;
    QString get_tostring(QString fsm_filedir) const ;
    QString get_constructor(QString fsm_filedir) const ;
    QString get_allowed_returns(const QObject *pEventObj, QString fsm_filedir) const ;
    QString get_allowed_returns(QString eventname, QString fsm_filedir) const ;

    bool parseh (QString str);
    void parsecpp (CppPartsContainer *pContainer);
    QString getEventsH (QString fsm_filedir);
    QString getEventsCpp (QString fsm_filedir);

    bool isEventInList (const QStringList *pEventHdlNames, QString eventname) const ;

    QStringList getTransitions () const ;
    QStringList getEvents () const ;
    QStringList getRemovedEventHdls (const QStringList *pEventhdls, const QStringList *pEvents) const ;
    bool isHdlEventInList (QString hdlname, const QStringList *pEvents) const ;
    QString State::getIncludeStr (QString statename, QString fsm_filedir) const ;
    void append_initialize (QString *pStr, QString fsm_filedir) const ;
    void append_toString (QString *pStr, QString fsm_filedir) const ;
    void append_constructor (QString *pStr, QString fsm_filedir) const ;
    void append_eventhdl (QString *pStr, QString eventname, QString fsm_filedir) const ;

    QString m_CustomInclude;
    QString m_CustomPart;
};

#endif // STATE_H
