#include <QSet>
#include "eventitem.h"
#include "StateItem.h"
#include "transition.h"
#include "backbone.h"
#include "scene.h"
#include "../LogLib/logger.h"

QString EventItem::toString() const {
	QString qstr = "event: ";
	foreach (QGraphicsItem *pGrItem, this->childItems()){
		if (pGrItem->type() == Transition::Type){
			Transition *pTr = qgraphicsitem_cast<Transition*>(pGrItem);
			qstr += pTr->toString() + "; ";
		}
	}
	return qstr;
}
void EventItem::setDelayedTransitions (const QDomElement *pNode){
    this->m_DelayedTransitions.clear();
    QDomElement node = pNode->firstChildElement("transition");
    while(!node.isNull()){
        m_DelayedTransitions.append(node.attribute("to", "null"));
        node = node.nextSiblingElement("transition");
    }
}
QStringList EventItem::getDelayedTransitions () const {
    return m_DelayedTransitions;
}
QDomElement EventItem::getNode(QDomDocument *pDomDoc) const {
    QDomElement element = pDomDoc->createElement("event");
    element.setAttribute("name", m_Name);
    QStringList transitions = getTransitionList();
    foreach (QString transition, transitions) {
        QDomElement node = pDomDoc->createElement("transition");
        node.setAttribute("to", transition);
        element.appendChild(node);
    }
    return element;
}
bool EventItem::deserialize(QString str){
    bool retval = false;
    return retval;
}
QString EventItem::serialize() const {
    QStringList transitions = getTransitionList();
    QString str;
    if (transitions.count() == 0){
        str = QString::asprintf("\t\t<event name=\"%s\"/>\n", m_Name.toStdString().c_str());
    } else {
        str = QString::asprintf("\t\t<event name=\"%s\">\n", m_Name.toStdString().c_str());
        foreach (QString transition, transitions) {
            str += QString::asprintf("\t\t\t<transition to=\"%s\"/\n", transition.toStdString().c_str());
        }
        str += "\t\t</event>\n";
    }
    return str;
}
Scene* EventItem::getScene() const {
	return dynamic_cast<Scene*>(scene());
}
void EventItem::updateTransitions (){
	Log::Logger::debug("PAINT__", "EventItem::updateTransitions()");
	QSet<int> idx_set;
    StateItem *pParentItem = qgraphicsitem_cast<StateItem*>(parentItem());//we need it to get some dimensions
	if (pParentItem != nullptr){
		QList<Backbone*> &backboneList = getScene()->getOutBackboneList(pParentItem->getName());
		int max_idx = -1;
		for (int i = 0; i < backboneList.count(); i++){
			Backbone *pBackbone = backboneList.at(i);
            if (containsTransition(pBackbone->getEndItem()->getName())){
				max_idx = i;
				idx_set.insert(i);
			}
		}
		QPainterPath path;
		if (this->m_EndItemsSet.count() > 0){
			double y = rect().height()/2;
			qreal x = pos().rx();
			qreal rightSpace = pParentItem->boundingRect().width() - rect().width() - x;
			qreal linelen = rightSpace + Scene::BackboneMargin - Scene::BevelSize;
			x = rect().width();
			path.moveTo(x, y);
			x += linelen;
			path.lineTo(x, y);
			for (int i = 0; i <= max_idx; i++){
				if (idx_set.contains(i)){
					Backbone *pBb = backboneList.at(i);
					qreal yEnd = mapFromItem (pBb, pBb->getEndPoint()).ry();
					if (yEnd < y){//up
						path.lineTo(x + Scene::BevelSize, y - Scene::BevelSize);
						if (i != max_idx){
							path.moveTo(x, y);
							x += Scene::BackboneMargin;
							path.lineTo(x, y);
						}
					} else if (yEnd > y){//down
						path.lineTo(x + Scene::BevelSize, y + Scene::BevelSize);
						if (i != max_idx){
							path.moveTo(x, y);
							x += Scene::BackboneMargin;
							path.lineTo(x, y);
						}
					} else {//right
						path.lineTo(x + Scene::BevelSize, y);
						if (i != max_idx){
							x += Scene::BackboneMargin - Scene::BevelSize;
							path.lineTo(x, y);
						}
					}
				} else {
					bool collides = pParentItem->eventTransitionCollidesWithBackbone(this, i);
					if (collides){
						path.arcTo(x, y - Scene::BevelSize, Scene::BevelSize*2, Scene::BevelSize*2, 180, 180);
					} else {
						path.lineTo(x + Scene::BevelSize, y);
					}
					x += Scene::BackboneMargin;
					path.lineTo(x, y);
				}
			}
		}
		this->m_pTransition->setPath(path);
	} else {
		throw 1;//todo
	}
}
void EventItem::addTransition (StateItem *pEndItem){
    if (!containsTransition(pEndItem->getName())){
        m_EndItemsSet.insert(pEndItem);
		updateTransitions ();
	}
}
void EventItem::removeTransition(StateItem *pEndItem){
    m_EndItemsSet.remove(pEndItem);
    updateTransitions();
}
bool EventItem::containsTransition (QString endItemName) const{
    bool retval = false;
    foreach (StateItem *pItem, m_EndItemsSet){
        if (pItem->getName() == endItemName){
            retval = true;
            break;
        }
    }
    return retval;
}
const StateItem* EventItem::getItem() const{
    StateItem *pTI2 = qgraphicsitem_cast<StateItem*>(this->parentItem());
	return pTI2;
}
QStringList EventItem::getTransitionList() const {
    QStringList list;
    foreach (StateItem *pItem, m_EndItemsSet){
        list.append(pItem->getName());
    }
    return list;
}
EventItem::EventItem(StateItem *pTi2, QString name, QFont &font)
: QGraphicsRectItem (pTi2)
, m_pTransition (nullptr)
, m_Name (name)
{
	QGraphicsTextItem *pText = new QGraphicsTextItem (name, this);
	pText->setFont(font);
	pText->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent);
	pText->setZValue(-1);
	m_pTransition = new Transition (this);
}
