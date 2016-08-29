#include "transition.h"
#include "event.h"
#include <QDomElement>

void Transition::createTransition (Event *pEvent, QDomElement *pNode){
    Transition *pTransition = new Transition (pEvent);
    pTransition->setObjectName(pNode->attribute("to", "null"));
}

Transition::Transition(QObject *parent) : QObject(parent){

}
