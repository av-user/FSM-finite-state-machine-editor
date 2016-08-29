#ifndef EVENT_H
#define EVENT_H

#include <QObject>

class State;
class QDomElement;

class Event : public QObject {
    Q_OBJECT
public:
    static void createEvent (State *pState, QDomElement *pNode);
    QStringList getTransitionNames();
    explicit Event(QObject *parent = 0);
signals:

public slots:
};

#endif // EVENT_H
