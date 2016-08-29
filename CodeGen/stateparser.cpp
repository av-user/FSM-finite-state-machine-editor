#include "stateparser.h"
#include "state.h"
#include "codegenerator.h"
#include <QRegularExpression>

bool StateParser::parseInit(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        m_BufStr = line;
        this->m_ParsingState = StateCustom;
        retval = true;
        break;
    case MarkUnknown:
        //todo ???
        break;
    case MarkMethod:
        m_BufStr = line;
        this->m_ParsingState = StateMethod;
        break;
    case MarkEnd:
        //todo ???
        break;
    case MarkInclude:
        m_BufStr = line;
        savePart(StateParser::CppPart::TypeInclude, "", StateInit);
        retval = true;
        break;
    case MarkNAME:
        m_BufStr = line;
        break;
    case MarkError:
        break;
    default:
        break;
    }
    return retval;
}
bool StateParser::parseInclude(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        this->savePart(StateParser::CppPart::TypeInclude, line, StateCustom);
        retval = true;
        break;
    case MarkUnknown:
        break;
    case MarkMethod:
        break;
    case MarkEnd:
        break;
    case MarkInclude:
        m_BufStr += line;
        retval = true;
        break;
    case MarkNAME:
        break;
    case MarkError:
        break;
    default:
        break;
    }
    return retval;
}
bool StateParser::parseUndefined(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        break;
    case MarkUnknown:
        break;
    case MarkMethod:
        break;
    case MarkEnd:
        break;
    case MarkInclude:
        break;
    case MarkNAME:
        break;
    case MarkError:
        break;
    default:
        break;
    }
    return retval;
}
void StateParser::savePart (StateParser::CppPart::Type type, QString line, ParsingState nextState){
    StateParser::CppPart *pPart = new StateParser::CppPart(type, m_BufStr);
    this->m_CppParts.append(pPart);
    this->m_BufStr = line;
    m_ParsingState = nextState;
}

bool StateParser::parseCustom(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        m_BufStr += line;
        retval = true;
        break;
    case MarkUnknown:
        break;
    case MarkMethod:
        this->savePart(StateParser::CppPart::TypeCustom, line, StateMethod);
        retval = true;
        break;
    case MarkEnd:
        break;
    case MarkInclude:
        savePart(StateParser::CppPart::TypeCustom, line, StateInit);
        savePart(StateParser::CppPart::TypeInclude, "", StateInit);
        retval = true;
        break;
    case MarkNAME:
        savePart(StateParser::CppPart::TypeCustom, line, StateInit);
        savePart(StateParser::CppPart::TypeNAME, "", StateInit);
        retval = true;
        break;
    case MarkError:
        break;
    default:
        break;
    }
    if (!retval){
        retval = false;
    }
    return retval;
}
bool StateParser::parseMethod(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        m_BufStr += line;
        retval = true;
        break;
    case MarkUnknown:
        break;
    case MarkMethod:
        break;
    case MarkEnd:
        m_BufStr += line;
        savePart(m_BufStr.contains(State::HDLMARK_TAG)
                    ? StateParser::CppPart::TypeEventHdl
                    : StateParser::CppPart::TypeMethod
                    , "", StateInit);
        retval = true;
        break;
    case MarkInclude:
        break;
    case MarkNAME:
        break;
    case MarkError:
        break;
    default:
        break;
    }
    return retval;
}
bool StateParser::parseError(QString line){
    bool retval = false;
    MarkedLineType mark = getMarkType(line);
    switch (mark){
    case MarkNo:
        break;
    case MarkUnknown:
        break;
    case MarkMethod:
        break;
    case MarkEnd:
        break;
    case MarkInclude:
        break;
    case MarkNAME:
        break;
    case MarkError:
        break;
    default:
        break;
    }
    return retval;
}
StateParser::MarkedLineType StateParser::getMarkType (QString line){
    MarkedLineType type = MarkNo;
    if (line.indexOf("//") != 0 && line.contains(State::FSM_DONTREMOVE_TAG)){
        if (line.indexOf("#include") == 0){
            type = MarkInclude;
        } else if (line.contains("::NAME = \"")){
            type = MarkNAME;
        } else if (line.at(0) == '}'){
            type = MarkEnd;
        } else {
            if (line.contains(State::METHODMARK_TAG)
                    || line.contains(State::HDLMARK_TAG)){
                type = MarkMethod;
            } else {
                type = MarkUnknown;
            }
        }
    }
    return type;
}
bool StateParser::finalize (){
    bool retval = false;
    switch (m_ParsingState){
    case StateInit:
        retval = true;
        break;
    case StateCustom:
        if (m_BufStr.indexOf(CodeGenerator::END_OF_LINE) == m_BufStr.length() - CodeGenerator::END_OF_LINE.length()){
            m_BufStr.remove(m_BufStr.indexOf(CodeGenerator::END_OF_LINE), CodeGenerator::END_OF_LINE.length());
        }
        savePart(StateParser::CppPart::TypeCustom, "", StateUndefined);
        retval = true;
        break;
    case StateMethod:
        break;
    case StateError:
        break;
    default:
        break;
    }
    return retval;
}

bool StateParser::parsecpp (QString str){
    bool retval = true;
    clearParts();
    m_BufStr.clear();
    QStringList lines = str.split(CodeGenerator::END_OF_LINE);
    m_ParsingState = StateInit;
    foreach (QString line, lines){
        switch (m_ParsingState){
        case StateInit:
            retval = parseInit(line + CodeGenerator::END_OF_LINE);
            break;
        case StateCustom:
            retval = parseCustom(line + CodeGenerator::END_OF_LINE);
            break;
        case StateMethod:
            retval = parseMethod(line + CodeGenerator::END_OF_LINE);
            break;
        case StateError:
            retval = parseError(line + CodeGenerator::END_OF_LINE);
            break;
        default:
            retval = false;
            break;
        }
        if (!retval){
            break;
        }
    }
    finalize();
    return retval;
}
QStringList StateParser::getIncludeList () const {
    QList<QString>  list;
    foreach (StateParser::CppPart *pPart, m_CppParts) {
        if (pPart->type() == StateParser::CppPart::TypeInclude){
            list.append(pPart->name());
        }
    }
    return list;
}
QStringList StateParser::getEventHdlList () const {
    QList<QString>  list;
    foreach (StateParser::CppPart *pPart, m_CppParts) {
        if (pPart->type() == StateParser::CppPart::TypeEventHdl){
            list.append(pPart->name());
        }
    }
    return list;
}
void StateParser::commentout (QString eventhdl){
    foreach (StateParser::CppPart *pPart, m_CppParts) {
        if (pPart->type() == StateParser::CppPart::TypeEventHdl){
            if (pPart->name() == eventhdl){
                pPart->commentout();
            }
        }
    }
}
QStringList StateParser::getMethodList () const {
    QList<QString>  list;
    foreach (StateParser::CppPart *pPart, m_CppParts) {
        if (pPart->type() == StateParser::CppPart::TypeMethod){
            list.append(pPart->name());
        }
    }
    return list;
}

void StateParser::clearParts (){
    foreach (CppPart *pPart, m_CppParts){
        delete pPart;
    }
    m_CppParts.clear();
}
QString StateParser::toString() const {
    QString str;
    foreach (StateParser::CppPart *pPart, m_CppParts) {
        str.append(pPart->toString());
    }
    return str;
}

StateParser::StateParser(const State * pState)
: m_pState (pState)
, m_ParsingState (StateUndefined)
{

}

StateParser::~StateParser(){
    clearParts();
}

////////////////////////////
QString StateParser::CppPart::name() const {
    QString str;
    QRegularExpression re;
    QRegularExpressionMatch match;
    switch (m_Type) {
    case StateParser::CppPart::TypeCustom:
        str = "custom";
        break;
    case StateParser::CppPart::TypeInclude:
        re.setPattern("(?<=^#include \").+(?=.h)");
        match = re.match(this->m_Content);
        if (match.hasMatch()){
            str = match.captured();
        }
        break;
    case StateParser::CppPart::TypeMethod:
        re.setPattern("(?<=::)(?:(?!::).)*(?=\\s?\\()");
        match = re.match(this->m_Content);
        if (match.hasMatch()){
            str = match.captured().trimmed();
        }
        break;
    case StateParser::CppPart::TypeEventHdl:
        re.setPattern("(?<=::)(?:(?!::).)*(?=\\s?\\()");
        match = re.match(this->m_Content);
        if (match.hasMatch()){
            str = match.captured();
        }
        break;
    case StateParser::CppPart::TypeNAME:
        break;
    case StateParser::CppPart::TypeUnknown:
        break;
    default:
        break;
    }
    return str;
}
void StateParser::CppPart::commentout (){
    QString comment_tag = "//";
    m_Content.replace(CodeGenerator::END_OF_LINE, CodeGenerator::END_OF_LINE + comment_tag);
    m_Content.insert(0, comment_tag);
    m_Content = m_Content.left(m_Content.length() - comment_tag.length());
}
QString StateParser::CppPart::toString() const {
    return m_Content;
}

StateParser::CppPart::CppPart (Type t, QString content)
: m_Type (t)
, m_Content (content)
{

}
