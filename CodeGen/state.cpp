#include "event.h"
#include "state.h"
#include "fsm.h"
#include "stateparser.h"
#include "codegenerator.h"
#include <QDomElement>
#include <QDir>

QString State::CUSTOM_INCLUDE_BEGIN_TAG = "//### <<< custom include beginn" + CodeGenerator::END_OF_LINE;
QString State::CUSTOM_INCLUDE_END_TAG = "//### custom include end >>>" + CodeGenerator::END_OF_LINE;
QString State::CUSTOM_PART_BEGIN_TAG = "//### <<< custom part beginn" + CodeGenerator::END_OF_LINE;
QString State::CUSTOM_PART_END_TAG = "//### custom part end >>>" + CodeGenerator::END_OF_LINE;
QString State::FSM_DONTREMOVE_TAG = "fsm_dontedit" + CodeGenerator::END_OF_LINE;

QString State::INCLUDEMARK_TAG = " //INC " + State::FSM_DONTREMOVE_TAG;
QString State::METHODMARK_TAG = " //MTD " + State::FSM_DONTREMOVE_TAG;
QString State::HDLMARK_TAG = " //HDL " + State::FSM_DONTREMOVE_TAG;
QString State::NAMEMARK_TAG = " //NME " + State::FSM_DONTREMOVE_TAG;
QString State::RETURN_THIS = "//\tpNewState = this;";

bool State::parseh (QString str){
    bool retval = true;
    QString s = CUSTOM_INCLUDE_BEGIN_TAG;
    int idxbeg = str.indexOf(CUSTOM_INCLUDE_BEGIN_TAG);
    if (idxbeg != -1){
        int idxend = str.indexOf(CUSTOM_INCLUDE_END_TAG);
        if (idxend > idxbeg){
            m_CustomInclude = str.mid(idxbeg + CUSTOM_INCLUDE_BEGIN_TAG.length()
                                , idxend - idxbeg - CUSTOM_INCLUDE_BEGIN_TAG.length());
            idxbeg = str.indexOf(CUSTOM_PART_BEGIN_TAG);
            if (idxbeg > idxend){
                idxend = str.indexOf(CUSTOM_PART_END_TAG);
                if (idxend > idxbeg){
                    m_CustomPart = str.mid(idxbeg + CUSTOM_PART_BEGIN_TAG.length()
                                , idxend - idxbeg - CUSTOM_PART_BEGIN_TAG.length());
                }
            }
        }
    }
    return retval;
}
QString State::getEventsH (QString fsm_filedir){
    QString str;
    QString tmplt = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_event_hdl_h.tplt");
    foreach (QObject *pObj, children()){
        QString handlerStr = tmplt.replace("%FSM_NAME%", parent()->objectName());
        str += handlerStr.replace("%EVENT%", pObj->objectName());
    }
    return str;
}
bool State::generateh (QString filename, QString fsm_filedir){
    bool retval = true;
    parseh (CodeGenerator::readTextFile(filename));
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QFileInfo finfo(file);
        QString str;
        QString template_h = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_h.tplt");
        QString event_hdls_h = getEventsH (fsm_filedir);
        str = template_h.replace("%STATENAME_UPPER%", objectName().toUpper());
        str = str.replace ("%FSM_NAME%", parent()->objectName());
        str = str.replace("%STATE_NAME%", objectName());
        str = str.replace("%CUSTOM_INCLUDE%", this->m_CustomInclude);
        str = str.replace("%CUSTOM_PART%", this->m_CustomPart);
        str = str.replace("%EVENT_HANDLERS%", event_hdls_h);
        file.seek(0);
        file.write(str.toStdString().c_str());
    } else {
        retval = false;
    }
    return retval;
}
QStringList State::getTransitions () const {
    QStringList list;
    foreach (QObject *pObj, children()) {
        Event *pEvent = qobject_cast<Event*> (pObj);
        if (pEvent != nullptr){
            list.append(pEvent->getTransitionNames());
        }
    }
    list.removeDuplicates();
    return list;
}
QStringList State::getEvents () const {
    QStringList list;
    foreach (QObject *pObj, children()) {
        list.append(pObj->objectName());
    }
    return list;
}
QString State::getFWHdlInclude (QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_fwhdl_include_cpp.tplt");
    return str.replace("%FSM_NAME%", parent()->objectName())
                .replace("%INCLUDEMARK%", INCLUDEMARK_TAG);
}
QString State::get_includes (QString fsm_filedir) const {
    QStringList transitions = getTransitions ();
    QString str = getFWHdlInclude(fsm_filedir);
    str += getIncludeStr (objectName(), fsm_filedir);
    foreach (QString statename, transitions) {
        if (statename != objectName()){
            str += getIncludeStr (statename, fsm_filedir);
        }
    }
    str += CodeGenerator::LOG_INCLUDE;
    return str;
}
QString State::get_NAME(QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_name_cpp.tplt");
    return str.replace("%FSM_NAME%", parent()->objectName())
                .replace("%STATE_NAME%", objectName())
                .replace("%NAMEMARK%", NAMEMARK_TAG);
}
QString State::get_initialize(QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_initialize_cpp.tplt");
    QString loggertag = parent()->objectName().length() >= 4
            ? parent()->objectName().left(4).toUpper() + "HDL" : "TODOHDL";
    return str.replace("%FSM_NAME%", parent()->objectName())
                .replace("%STATE_NAME%", objectName())
                .replace("%LOGGER_TAG%", loggertag)
                .replace("%METHODMARK%", METHODMARK_TAG);
}
QString State::get_allowed_returns(const QObject *pEventObj, QString fsm_filedir) const {
    QString str;
    QString tmplt = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_allowed_return_cpp.tplt");
    QString tmp;
    foreach (QObject *pObj, pEventObj->children()) {
        if (this->objectName()== pObj->objectName()){
            str.append(RETURN_THIS + CodeGenerator::END_OF_LINE);
        } else {
            tmp = tmplt;
            str.append(tmp.replace("%STATE_NAME%", pObj->objectName())
                       .replace("%FSM_NAME%", parent()->objectName()) + CodeGenerator::END_OF_LINE);
        }
    }
    if (str.indexOf(CodeGenerator::END_OF_LINE) == str.length() - CodeGenerator::END_OF_LINE.length()){
        str.remove(str.indexOf(CodeGenerator::END_OF_LINE), CodeGenerator::END_OF_LINE.length());
    }
    return str;
}
QString State::get_allowed_returns(QString eventname, QString fsm_filedir) const {
    QString str;
    QString tmplt = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_allowed_return_cpp.tplt");
    QString tmp;
    foreach (QObject *pEventObj, children()) {
        if (pEventObj->objectName() == eventname){
            foreach (QObject *pObj, pEventObj->children()) {
                if (this->objectName()== pObj->objectName()){
                    str.append(RETURN_THIS + CodeGenerator::END_OF_LINE);
                } else {
                    tmp = tmplt;
                    str.append(tmp.replace("%STATE_NAME%", pObj->objectName())
                               .replace("%FSM_NAME%", parent()->objectName()) + CodeGenerator::END_OF_LINE);
                }
            }
            if (str.indexOf(CodeGenerator::END_OF_LINE) == str.length() - CodeGenerator::END_OF_LINE.length()){
                str.remove(str.indexOf(CodeGenerator::END_OF_LINE), CodeGenerator::END_OF_LINE.length());
            }
        }
        break;
    }
    return str;
}
QString State::get_eventhandlers(QString fsm_filedir) const {
    QString str;
    QString tmplt = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_event_hdl_cpp.tplt");
    foreach (QObject *pObj, children()){
        QString tmp = tmplt;
        QString handlerStr = tmp.replace("%FSM_NAME%", parent()->objectName())
                .replace("%STATE_NAME%", objectName())
                .replace("%EVENT%", pObj->objectName())
                .replace("%ALLOWED_RETURNS%", get_allowed_returns(pObj, fsm_filedir))
                .replace("%HDLMARK%", HDLMARK_TAG);
        str += handlerStr;
    }
    return str;
}
QString State::get_tostring(QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_tostring_cpp.tplt");
    return str.replace("%FSM_NAME%", parent()->objectName())
                .replace("%STATE_NAME%", objectName())
                .replace("%METHODMARK%", METHODMARK_TAG);
}
QString State::get_constructor(QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_constructor_cpp.tplt");
    return str.replace("%FSM_NAME%", parent()->objectName())
            .replace("%STATE_NAME%", objectName())
            .replace("%METHODMARK%", METHODMARK_TAG);
}
QString State::getEventsCpp (QString fsm_filedir){
    QString str;
    foreach (QObject *pObj, children()){
        append_eventhdl(&str, pObj->objectName(), fsm_filedir);
    }
    return str;
}
bool State::cpp_from_template (QString filename, QString fsm_filedir){
    bool retval = false;
    QFile file (filename);
    if (file.open(QIODevice::ReadWrite | QIODevice::Truncate | QIODevice::Text)){
        QString template_cpp = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_state_cpp.tplt");
        QString str = template_cpp.replace(QString("%INCLUDES%"), get_includes (fsm_filedir))
            .replace("%NAME%", get_NAME(fsm_filedir))
            .replace("%INITIALIZE%", get_initialize(fsm_filedir))
            .replace("%EVENT_HANDLERS%", get_eventhandlers(fsm_filedir))
            .replace("%CONSTRUCTOR%", get_constructor(fsm_filedir));
        file.seek(0);
        file.write(str.toStdString().c_str());
        file.close();
    } else {
        retval = false;
    }
    return retval;
}
QString State::getIncludeStr (QString statename, QString fsm_filedir) const {
    QString str = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_include_cpp.tplt");
    return str.replace("%STATE%", statename)
              .replace("%INCLUDEMARK%", INCLUDEMARK_TAG);
}
void State::append_initialize (QString *pStr, QString fsm_filedir) const {
    QString template_cpp = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_initialize_cpp.tplt");
    QString str = template_cpp.replace("%FSM_NAME%", parent()->objectName())
        .replace("%STATE_NAME%", objectName())
        .replace("%HDLMARK%", State::INCLUDEMARK_TAG);
    pStr->append(str);
}
void State::append_toString (QString *pStr, QString fsm_filedir) const {
    QString template_cpp = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_tostring_cpp.tplt");
    QString str = template_cpp.replace("%FSM_NAME%", parent()->objectName())
        .replace("%HDLMARK%", State::METHODMARK_TAG);
    pStr->append(str);
}
void State::append_constructor (QString *pStr, QString fsm_filedir) const {
    QString template_cpp = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_constructor_cpp.tplt");
    QString str = template_cpp.replace("%FSM_NAME%", parent()->objectName())
        .replace("%STATE_NAME%", objectName())
        .replace("%HDLMARK%", METHODMARK_TAG);
    pStr->append(str);
}
void State::append_eventhdl (QString *pStr, QString eventname, QString fsm_filedir) const {
    QString tmplt = CodeGenerator::readTextFile(fsm_filedir + "/templates/template_event_hdl_cpp.tplt");
    pStr->append(tmplt.replace("%FSM_NAME%", parent()->objectName())
            .replace("%STATE_NAME%", objectName())
            .replace("%EVENT%", eventname)
            .replace("%ALLOWED_RETURNS%", get_allowed_returns(eventname, fsm_filedir))
            .replace("%HDLMARK%", State::HDLMARK_TAG));
}
bool State::generatecpp (QString filename, QString fsm_filedir){
    bool retval = true;
    QString str = CodeGenerator::readTextFile(filename);
    if (str.length() == 0){
        return cpp_from_template (filename, fsm_filedir);
    } else {
        StateParser parser (this);
        if (parser.parsecpp (str)){
            QString newstr;
            QStringList includes = parser.getIncludeList ();
            QStringList transitions = getTransitions();
            // add missing includes
            foreach (QString trans, transitions) {
                if (!includes.contains(trans)){
                    newstr += getIncludeStr (trans, fsm_filedir);
                }
            }
            QStringList parsed_eventhdl_list = parser.getEventHdlList ();
            QStringList parsed_method_list = parser.getMethodList ();
            QStringList events = getEvents();
            QStringList removedEventHandlers = getRemovedEventHdls (&parsed_eventhdl_list, &events);
            foreach (QString eventhdl, removedEventHandlers) {
                parser.commentout (eventhdl);
            }
            newstr += parser.toString();
            // ensure initialize, toString and constructor
            if (!parsed_method_list.contains("initialize")){
                append_initialize (&newstr, fsm_filedir);
            }
            foreach (QString eventName, events) {
                if (!isEventInList (&parsed_eventhdl_list, eventName)){
                    append_eventhdl (&newstr, eventName, fsm_filedir);
                }
            }
//            if (!parsed_method_list.contains("toString")){
//                append_toString (&newstr, fsm_filedir);
//            }
            if (!parsed_method_list.contains(objectName())){
                append_constructor (&newstr, fsm_filedir);
            }

            QFile file (filename);
            if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text)){
                file.write(newstr.toStdString().c_str());
                file.close();
            } else {
                retval = false;
            }
        }
    }
    return retval;
}
bool State::isHdlEventInList (QString hdlname, const QStringList *pEvents) const {
    bool retval = false;
    foreach (QString eventname, *pEvents) {
        if (hdlname.contains(eventname)){
            retval = true;
            break;
        }
    }
    return retval;
}
QStringList State::getRemovedEventHdls (const QStringList *pEventhdls, const QStringList *pEvents) const {
    QStringList list;
    foreach (QString hdlname, *pEventhdls) {
        if (!isHdlEventInList (hdlname, pEvents)){
            list.append(hdlname);
        }
    }
    return list;
}
bool State::isEventInList (const QStringList *pEventHdlNames, QString eventname) const {
    bool retval = false;
    foreach (QString hdlname, *pEventHdlNames){
        if (hdlname.contains(eventname)){
            retval = true;
            break;
        }
    }
    return retval;
}
bool State::generate (QFileInfo *pFileinfo){
    bool retval = true;
    QDir dir (pFileinfo->canonicalPath());
    if (dir.exists()){
        if (dir.cd(pFileinfo->completeBaseName())){
            QString filename = dir.canonicalPath();
            filename += '/';
            filename += this->objectName();
            generateh (filename + ".h", pFileinfo->canonicalPath());
            generatecpp (filename + ".cpp", pFileinfo->canonicalPath());
        }
    } else {
        retval = false;
    }
    return retval;
}

void State::createState (Fsm *pFsm, QDomElement *pNode) {
    State *pState = new State (pFsm);
    pState->setObjectName(pNode->attribute("name"));
    QDomElement node = pNode->firstChildElement("event");
    while(!node.isNull()){
        Event::createEvent (pState, &node);
        node = node.nextSiblingElement("event");
    }
}

State::State(QObject *parent)
: QObject(parent)
{

}
