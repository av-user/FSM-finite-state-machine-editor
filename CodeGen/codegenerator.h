#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "codegen_global.h"
#include <QString>
#include <QObject>

class QDomElement;
class Fsm;
class QDir;

class CODEGENSHARED_EXPORT CodeGenerator {
public:
    static QString      END_OF_LINE;
    static QString      LOG_INCLUDE;
    static QString      LOG_INCLUDE_FW;
    static QString      STATE_MODEL_DIRNAME;
    enum CodeType { Qt, java, CSharp, CPP };

    void setFilename (QString filename) { m_Filename = filename; }
    void setDirname (QString dirname) { m_Dirname = dirname; }
    void generatelib(QString dirname);
    bool generate ();
    static QString readTextFile (QString filepath);

    CodeGenerator(CodeType ctype, QString endofline);
private:
    bool generate(QString filename);
    QStringList get_all_eventnames(QString dirname);
    void generate_model_handler(QDir *pDir);
    void generate_model_msg(QDir *pDir, const QStringList *pEventList);

    QString get_event_enums (QDir *pDir,const QStringList *pEventList) const ;
    QString get_event_enum_cases (QDir *pDir, const QStringList *pEventList) const;

    void generate_model_state(QDir *pDir);
    void generate_model_timer(QDir *pDir);
    void generate_model_timerthread(QDir *pDir);

    Fsm *getFsm (QString filename) const ;

    CodeType    m_CodeType;
    QString     m_Filename;
    QString     m_Dirname;
};

#endif // CODEGENERATOR_H
