#include "fsm.h"
#include "state.h"
#include "fwparser.h"
#include "codegenerator.h"
#include <QDomElement>
#include <QFileInfo>
#include <QDir>

bool Fsm::mkfsmsubdir (QDir *pDir){
    return pDir->cd("Framework") || pDir->mkdir("Framework");
}
bool Fsm::mkfsmdir (QFileInfo *pFsmFileinfo){
    bool retval = false;
    QDir dir (pFsmFileinfo->canonicalPath());
    if (dir.exists()){
        if (dir.cd(pFsmFileinfo->completeBaseName())){
            retval = mkfsmsubdir(&dir);
        } else if (dir.mkdir(pFsmFileinfo->completeBaseName())){
            if (dir.cd(pFsmFileinfo->completeBaseName())){
                retval = mkfsmsubdir(&dir);
            }
        }
    }
    return retval;
}
QStringList Fsm::getFsmList (QFileInfo *pFsmFileinfo){
    QStringList list;
    QDir dir (pFsmFileinfo->canonicalPath());
    if (dir.exists()){
        QStringList filters;
        filters << "*.fsm";
        foreach ( QString filename, dir.entryList(filters, QDir::Files)){
            QFileInfo finfo (filename);
            list.append(finfo.baseName());
        }
    }
    return list;
}
QString Fsm::get_siblings_interface_h(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = getFsmList (pFsmFileinfo);
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_sibl_interf_h.tplt");
    foreach (QString s, list) {
        if (s != pFsmFileinfo->completeBaseName()){
            QString tmp = templ;
            str += tmp.replace("%FSM_NAME%", s) + CodeGenerator::END_OF_LINE;
        }
    }
    return str;
}
QString Fsm::get_siblings_interface_cpp(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = getFsmList (pFsmFileinfo);
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_sibl_interf_cpp.tplt");
    foreach (QString s, list) {
        if (s != pFsmFileinfo->completeBaseName()){
            QString tmp = templ;
            str += tmp.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
                    .replace("%SIBLING%", s) + CodeGenerator::END_OF_LINE;
        }
    }
    return str;
}
QString Fsm::get_siblings_classes(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = getFsmList (pFsmFileinfo);
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_sibl_class_h.tplt");
    foreach (QString s, list) {
        if (s != pFsmFileinfo->completeBaseName()){
            QString tmp = templ;
            str += tmp.replace("%FSM_NAME%", s) + CodeGenerator::END_OF_LINE;
        }
    }
    return str;
}
QString Fsm::get_sibling_fsms(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = getFsmList (pFsmFileinfo);
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_sibl_h.tplt");
    foreach (QString s, list) {
        if (s != pFsmFileinfo->completeBaseName()){
            QString tmp = templ;
            str += tmp.replace("%FSM_NAME%", s) + CodeGenerator::END_OF_LINE;
        }
    }
    return str;
}
QString Fsm::get_siblings_includes_cpp(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = getFsmList (pFsmFileinfo);
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_sibl_inc_cpp.tplt");
    foreach (QString s, list) {
        if (s != pFsmFileinfo->completeBaseName()){
            QString tmp = templ;
            str += tmp.replace("%FSM_NAME%", s) + CodeGenerator::END_OF_LINE;
        }
    }
    return str;
}
QString Fsm::get_initialize(QFileInfo *pFsmFileinfo){
    return CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_initialize_cpp.tplt")
            .replace("%FSM_NAME%", pFsmFileinfo->completeBaseName());
}
QString Fsm::get_clear(QFileInfo *pFsmFileinfo){
    return CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_clear_cpp.tplt")
            .replace("%FSM_NAME%", pFsmFileinfo->completeBaseName());
}
QString Fsm::get_constructor(QFileInfo *pFsmFileinfo){
    QString tmpl = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_constructor_cpp.tplt");
    tmpl.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName());
    tmpl.replace("%METHODMARK%", State::METHODMARK_TAG);
    return tmpl;
}
bool Fsm::generateFwHdl_h (QFileInfo *pFsmFileinfo, QString filename
                      , QString custom_inc, QString custom_part){
    bool retval = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QString template_cpp = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_h.tplt");
        QString str = template_cpp.replace(QString("%FSM_NAME_U%"), pFsmFileinfo->completeBaseName().toUpper())
            .replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
            .replace("%SIBLING_FSMS_INCLUDES%", get_siblings_includes_cpp(pFsmFileinfo))
            .replace("%CUSTOM_INCLUDE%", custom_inc)
            .replace("%CUSTOM_PART%", custom_part)
            .replace("%SIBLING_FSMS_INTERFACE%", get_siblings_interface_h(pFsmFileinfo))
            .replace("%SIBLING_FSMS_CLASSES%", get_siblings_classes(pFsmFileinfo))
            .replace("%SIBLING_FSMS%", get_sibling_fsms(pFsmFileinfo));
        file.seek(0);
        file.write(str.toStdString().c_str());
        file.close();
    } else {
        QString errMsg = file.errorString();
        retval = false;
    }
    return retval;
}
bool Fsm::generateFwHdl_cpp (QFileInfo *pFsmFileinfo, QString filename
                      , QString custom_inc, QString custom_part){
    bool retval = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QString template_cpp = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwhdl_cpp.tplt");
        QString str = template_cpp.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
            .replace("%SIBLING_FSMS_INCLUDES%", get_siblings_includes_cpp(pFsmFileinfo))
            .replace("%CUSTOM_INCLUDE%", custom_inc)
            .replace("%CUSTOM_PART%", custom_part)
            .replace("%INITIALIZE%", get_initialize(pFsmFileinfo))
            .replace("%SIBLING_FSMS_INTERFACE%", get_siblings_interface_cpp(pFsmFileinfo))
            .replace("%CLEAR%", get_clear(pFsmFileinfo))
            .replace("%CONSTRUCTOR%", get_constructor(pFsmFileinfo));
        file.seek(0);
        file.write(str.toStdString().c_str());
        file.close();
    } else {
        QString errMsg = file.errorString();
        retval = false;
    }
    return retval;
}
bool Fsm::generateFwHdl (QFileInfo *pFsmFileinfo){
    bool retval = true;
    QString filebase = pFsmFileinfo->canonicalPath();
    filebase += '/';
    filebase += pFsmFileinfo->completeBaseName();
    filebase += "/Framework/";
    filebase += pFsmFileinfo->completeBaseName();
    filebase += "hdl";
    QString hdl_custom_inc_h;
    QString hdl_custom_part_h;
    QString hdl_custom_inc_cpp;
    QString hdl_custom_part_cpp;

    QString str = CodeGenerator::readTextFile(filebase + ".h");
    hdl_custom_inc_h = FWParser::get_hdl_custom_inc_h (str);
    hdl_custom_part_h = FWParser::get_hdl_custom_part_h (str);
    generateFwHdl_h (pFsmFileinfo, filebase + ".h", hdl_custom_inc_h, hdl_custom_part_h);
    str = CodeGenerator::readTextFile(filebase + ".cpp");
    hdl_custom_inc_cpp = FWParser::get_hdl_custom_inc_cpp (str);
    hdl_custom_part_cpp = FWParser::get_hdl_custom_part_cpp (str);
    generateFwHdl_cpp (pFsmFileinfo, filebase + ".cpp", hdl_custom_inc_cpp, hdl_custom_part_cpp);
    return retval;
}
QStringList Fsm::get_eventlist () const {
    QStringList list;
    foreach (QObject *pStateObj, children()) {
        foreach (QObject *pEventObj, pStateObj->children()) {
            list.append(pEventObj->objectName());
        }
    }
    list.removeDuplicates ();
    return list;
}
QString Fsm::get_event_handlers(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = get_eventlist();
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwstate_event_hdl_h.tplt");
    foreach (QString s, list) {
        QString tmp = templ;
        str += tmp.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
                .replace("%EVENT%", s)
                + CodeGenerator::END_OF_LINE;
    }
    return str;
}
QString Fsm::get_event_cases(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = get_eventlist();
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwstate_event_case_cpp.tplt");
    foreach (QString s, list) {
        QString tmp = templ;
        str += tmp.replace("%EVENT%", s) + CodeGenerator::END_OF_LINE;
    }
    return str;
}
QString Fsm::get_event_exceptions(QFileInfo *pFsmFileinfo){
    QString str;
    QStringList list = get_eventlist();
    QString templ = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwstate_event_exception_hdl_cpp.tplt");
    foreach (QString s, list) {
        QString tmp = templ;
        str += tmp.replace("%EVENT%", s).replace ("%FSM_NAME%", objectName()) + CodeGenerator::END_OF_LINE;
    }
    return str;
}
bool Fsm::generateFwState_h (QFileInfo *pFsmFileinfo, QString filename){
    bool retval = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QString template_h = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwstate_h.tplt");
        QString str = template_h.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
            .replace("%FSM_NAME_U%", pFsmFileinfo->completeBaseName().toUpper())
            .replace("%EVENT_HANDLERS%", get_event_handlers(pFsmFileinfo));
        file.seek(0);
        file.write(str.toStdString().c_str());
        file.close();
    } else {
        QString errMsg = file.errorString();
        retval = false;
    }
    return retval;
}
bool Fsm::generateFwState_cpp (QFileInfo *pFsmFileinfo, QString filename){
    bool retval = true;
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QString template_cpp = CodeGenerator::readTextFile(pFsmFileinfo->canonicalPath() + "/templates/template_fwstate_cpp.tplt");
        QString str = template_cpp.replace("%FSM_NAME%", pFsmFileinfo->completeBaseName())
            .replace("%EVENT_EXCEPTIONS%", get_event_exceptions(pFsmFileinfo))
            .replace("%EVENT_CASES%", get_event_cases(pFsmFileinfo));
        file.seek(0);
        file.write(str.toStdString().c_str());
        file.close();
    } else {
        QString errMsg = file.errorString();
        retval = false;
    }
    return retval;
}
bool Fsm::generateFwFsm(QFileInfo *pFsmFileinfo){
    bool retval = true;
    QString filebase = pFsmFileinfo->canonicalPath();
    filebase += '/';
    filebase += pFsmFileinfo->completeBaseName();
    filebase += "/Framework/State";
    filebase += pFsmFileinfo->completeBaseName();
    generateFwState_h (pFsmFileinfo, filebase + ".h");
    generateFwState_cpp (pFsmFileinfo, filebase + ".cpp");
    return retval;
}
bool Fsm::generateFramework (QFileInfo *pFsmFileinfo){
    bool retval = false;
    QDir dir (pFsmFileinfo->canonicalPath());
    if (dir.exists()){
        QString fsmname = pFsmFileinfo->baseName();
        retval = generateFwHdl (pFsmFileinfo) && generateFwFsm(pFsmFileinfo);
    }
    return retval;
}
bool Fsm::generate (QString filename){
    bool retval = true;
    QFileInfo fileinfo (filename);
    if (mkfsmdir (&fileinfo)){
        if (generateFramework (&fileinfo)){
            foreach (QObject *pO, children()) {
                QString classname = pO->metaObject()->className();
                if (classname == "State"){
                    State *pState = qobject_cast<State*> (pO);
                    if (pState == nullptr || !pState->generate (&fileinfo)){
                        retval = false;
                        break;
                    }
                }
            }
        }
    } else {
        retval = false;
    }
    return retval;
}

Fsm* Fsm::createFsm (QDomElement *pNode){
    Fsm *pFsm = new Fsm ();
    pFsm->setObjectName(pNode->attribute("name"));
    QDomElement node = pNode->firstChildElement("state");
    QString name = pFsm->objectName();
    while(!node.isNull()){
        State::createState (pFsm, &node);
        node = node.nextSiblingElement("state");
    }
    return pFsm;
}

Fsm::Fsm(QObject *parent) : QObject(parent){

}
