#ifndef TRANSITION_H
#define TRANSITION_H

#include <QObject>
#include <QStringList>

class Event;
class QDomElement;

class Transition : public QObject{
    Q_OBJECT
public:
    static void createTransition (Event *pEvent, QDomElement *pNode);
    explicit Transition(QObject *parent = 0);

signals:

public slots:
};

#endif // TRANSITION_H
