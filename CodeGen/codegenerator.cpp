#include "codegenerator.h"
#include <QDir>
#include <QDomDocument>
#include <QString>
#include "fsm.h"
#include "state.h"

QString     CodeGenerator::END_OF_LINE = "\n";
QString     CodeGenerator::STATE_MODEL_DIRNAME = "statemodel";
QString     CodeGenerator::LOG_INCLUDE = "#include \"../LogLib/logger.h\"";
QString     CodeGenerator::LOG_INCLUDE_FW = "#include \"../../LogLib/logger.h\"";

QString CodeGenerator::readTextFile (QString filename){
    QString str;
    QFile file (filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        str = file.readAll();
    }
    return str;
}
Fsm *CodeGenerator::getFsm (QString filename) const {
    Fsm *pFsm = nullptr;
    QFile file (filename);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text)){
        QDomDocument domdoc;
        domdoc.setContent(QString (file.readAll()));
        QDomElement root = domdoc.documentElement();
        if (!root.isNull()){
            pFsm = Fsm::createFsm(&root);
        }
    }
    return pFsm;
}

bool CodeGenerator::generate(QString filename){
    bool retval = false;
    Fsm *pFsm = getFsm(filename);
    if (pFsm != nullptr){
        retval = pFsm->generate (filename);
    }
    delete pFsm;
    pFsm = nullptr;
    return retval;
}
QStringList CodeGenerator::get_all_eventnames(QString dirname){
    QStringList eventList;
    QDir dir (dirname);
    if (dir.exists()){
        QStringList filters;
        filters << "*.fsm";
        foreach ( QString filename, dir.entryList(filters, QDir::Files)){
            QString filepath = QString::asprintf("%s/%s"
                       , dirname.toStdString().c_str()
                       ,  filename.toStdString().c_str());
            Fsm *pFsm = getFsm(filepath);
            if (pFsm != nullptr){
                eventList.append(pFsm->get_eventlist());
            }
            delete pFsm;
            pFsm = nullptr;
        }
        eventList.append("Timeout");
        eventList.removeDuplicates();
    }
    return eventList;
}
void CodeGenerator::generate_model_handler(QDir *pDir){
    QString template_filename;
    QString templ;
    QFile fileH (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/handler.h");
    if (fileH.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_handler_h.tplt";
        templ = readTextFile(template_filename);
        fileH.seek(0);
        fileH.write(templ.toStdString().c_str());
        fileH.close();
    }
    QFile fileCPP (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/handler.cpp");
    if (fileCPP.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_handler_cpp.tplt";
        templ = readTextFile(template_filename);
        fileCPP.seek(0);
        fileCPP.write(templ.toStdString().c_str());
        fileCPP.close();
    }
}
QString CodeGenerator::get_event_enums (QDir *pDir, const QStringList *pEventList) const {
    QString str;
    QString templ = readTextFile(pDir->canonicalPath() + "/handlers/templates/template_model_msg_enum_event.tplt");
    foreach (QString eventname, *pEventList){
        QString t = templ;
        str.append(t.replace("%EVENT_NAME%", eventname) + END_OF_LINE);
    }
    if (str.indexOf(CodeGenerator::END_OF_LINE) == str.length() - CodeGenerator::END_OF_LINE.length()){
        str.remove(str.indexOf(CodeGenerator::END_OF_LINE), CodeGenerator::END_OF_LINE.length());
    }
    return str;
}
QString CodeGenerator::get_event_enum_cases (QDir *pDir, const QStringList *pEventList) const {
    QString str;
    QString templ = readTextFile(pDir->canonicalPath() + "/handlers/templates/template_model_msg_enum_event_case.tplt");
    foreach (QString eventname, *pEventList){
        QString t = templ;
        str.append(t.replace("%EVENT_NAME%", eventname) + END_OF_LINE);
    }
    if (str.indexOf(CodeGenerator::END_OF_LINE) == str.length() - CodeGenerator::END_OF_LINE.length()){
        str.remove(str.indexOf(CodeGenerator::END_OF_LINE), CodeGenerator::END_OF_LINE.length());
    }
    return str;
}
void CodeGenerator::generate_model_msg(QDir *pDir, const QStringList *pEventList){
    QString template_filename;
    QString templ;
    QFile fileH (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/msg.h");
    if (fileH.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_msg_h.tplt";
        templ = readTextFile(template_filename);
        templ.replace("%ENUM_VALUES%", get_event_enums (pDir, pEventList));
        fileH.seek(0);
        fileH.write(templ.toStdString().c_str());
        fileH.close();
    }
    QFile fileCPP (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/msg.cpp");
    if (fileCPP.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_msg_cpp.tplt";
        templ = readTextFile(template_filename);
        fileCPP.seek(0);
        fileCPP.write(templ.replace("%ENUM_CASES%", get_event_enum_cases (pDir, pEventList))
                      .toStdString().c_str());
        fileCPP.close();
    }
}
void CodeGenerator::generate_model_state(QDir *pDir){
    QString template_filename;
    QString templ;
    QFile fileH (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/state.h");
    if (fileH.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_state_h.tplt";
        templ = readTextFile(template_filename);
        fileH.seek(0);
        fileH.write(templ.toStdString().c_str());
        fileH.close();
    }
    QFile fileCPP (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/state.cpp");
    if (fileCPP.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_state_cpp.tplt";
        templ = readTextFile(template_filename);
        fileCPP.seek(0);
        fileCPP.write(templ.toStdString().c_str());
        fileCPP.close();
    }
}
void CodeGenerator::generate_model_timer(QDir *pDir){
    QString template_filename;
    QString templ;
    QFile fileH (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/timer.h");
    if (fileH.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_timer_h.tplt";
        templ = readTextFile(template_filename);
        fileH.seek(0);
        fileH.write(templ.toStdString().c_str());
        fileH.close();
    }
    QFile fileCPP (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/timer.cpp");
    if (fileCPP.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_timer_cpp.tplt";
        templ = readTextFile(template_filename);
        fileCPP.seek(0);
        fileCPP.write(templ.toStdString().c_str());
        fileCPP.close();
    }
}
void CodeGenerator::generate_model_timerthread(QDir *pDir){
    QString template_filename;
    QString templ;
    QFile fileH (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/timerthread.h");
    if (fileH.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_timerthread_h.tplt";
        templ = readTextFile(template_filename);
        fileH.seek(0);
        fileH.write(templ.toStdString().c_str());
        fileH.close();
    }
    QFile fileCPP (pDir->canonicalPath() + "/" + STATE_MODEL_DIRNAME + "/timerthread.cpp");
    if (fileCPP.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        template_filename = pDir->canonicalPath() + "/handlers/templates/template_model_timerthread_cpp.tplt";
        templ = readTextFile(template_filename);
        fileCPP.seek(0);
        fileCPP.write(templ.toStdString().c_str());
        fileCPP.close();
    }
}
void CodeGenerator::generatelib(QString dirname){
    QDir dir (dirname);
    if (dir.exists()){
        QStringList eventList = get_all_eventnames(dirname);
        dir.cdUp();
        if (!dir.cd(STATE_MODEL_DIRNAME)){
            dir.mkdir(STATE_MODEL_DIRNAME);
        } else {
            dir.cdUp();
        }
        generate_model_handler (&dir);
        generate_model_msg (&dir, &eventList);
        generate_model_state (&dir);
        generate_model_timer (&dir);
        generate_model_timerthread (&dir);
    }
}
bool CodeGenerator::generate () {
    bool retval = false;
    if (m_Filename != ""){
        retval = generate (m_Filename);
    } else if (m_Dirname != ""){
        QDir dir (m_Dirname);
        if (dir.exists()){
            QStringList filters;
            filters << "*.fsm";
            foreach ( QString filename, dir.entryList(filters, QDir::Files)){
                QString str = QString::asprintf("%s/%s"
                           , m_Dirname.toStdString().c_str()
                           ,  filename.toStdString().c_str());
                if (!generate(str)){
                    break;
                }
            }
            retval = true;
        }
    }
    return retval;
}

CodeGenerator::CodeGenerator(CodeType ctype, QString endofline)
:   m_CodeType (ctype)
{
    END_OF_LINE = endofline;
}
