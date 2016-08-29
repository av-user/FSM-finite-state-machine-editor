#ifndef STATEPARSER_H
#define STATEPARSER_H

#include <QString>
#include <QList>

class State;

class StateParser {
public:
    class CppPart {
    public:
        enum Type { TypeUnknown, TypeCustom, TypeMethod, TypeEventHdl, TypeInclude, TypeNAME, MarkError };
        CppPart (Type t, QString content);
        Type type() const { return m_Type; }
        QString name() const ;
        void commentout ();
        QString toString() const ;
    private:
        Type m_Type;
        QString m_Content;
    };

    bool parsecpp (QString str);
    void commentout (QString eventhdl);
    QStringList getIncludeList () const ;
    QStringList getEventList () const ;
    QStringList getEventHdlList () const ;
    QStringList getMethodList () const ;
    QString toString() const ;
    StateParser(const State * pState);
    virtual ~StateParser();
private:
    enum ParsingState { StateUndefined, StateInit, StateCustom
                        , /*StateInclude, */StateMethod, StateError };
    enum MarkedLineType { MarkNo, MarkUnknown, MarkMethod, MarkEnd, MarkInclude, MarkNAME, MarkError };
    void clearParts ();
    void savePart (StateParser::CppPart::Type type, QString line, ParsingState nextState);
    bool parseInit(QString line);
    bool parseInclude(QString line);
    bool parseUndefined(QString line);
    bool parseCustom(QString line);
    bool parseMethod(QString line);
    bool parseError(QString line);
    bool finalize ();

    MarkedLineType getMarkType (QString line);

    QString m_BufStr;
    const State * m_pState;
    ParsingState m_ParsingState;
    QList<StateParser::CppPart*> m_CppParts;
};

#endif // STATEPARSER_H
