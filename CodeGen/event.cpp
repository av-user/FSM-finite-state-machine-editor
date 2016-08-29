#include "event.h"
#include "state.h"
#include "transition.h"
#include <QDomElement>

QStringList Event::getTransitionNames(){
    QStringList list;
    foreach (QObject *pObj, children()) {
        list.append(pObj->objectName());
    }
    return list;
}
void Event::createEvent (State *pState, QDomElement *pNode) {
    Event *pEvent = new Event (pState);
    pEvent->setObjectName(pNode->attribute("name"));
    QDomElement node = pNode->firstChildElement("transition");
    while(!node.isNull()){
        Transition::createTransition(pEvent, &node);
        node = node.nextSiblingElement("transition");
    }
}


Event::Event(QObject *parent) : QObject(parent)
{

}
