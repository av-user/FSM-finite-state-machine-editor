#include <QSet>
#include <QEvent>
#include "eventitem.h"
#include "StateItem.h"
#include "transition.h"
#include "backbone.h"
#include "scene.h"
#include "../LogLib/logger.h"

QBrush  EventItem::SelectedBrush = QBrush(QColor(255, 10, 10, 15));
QBrush  EventItem::DefaultBrush;
QPen    EventItem::BlackPen = QPen(QColor (0,0,0));
QPen	EventItem::RedPen = QPen(QColor(255, 10, 10));

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
void EventItem::transitionUp (QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone){
    StateItem *pStateItem = qgraphicsitem_cast<StateItem*>(parentItem());
    qreal myTopY = mapFromItem(parentItem()->parentItem(), QPointF(0, pStateItem->getHY(pBB))).ry();
    qreal diffY = myTopY - me.ry();
    qreal distance2bb = myTopY - bbY;
    if (diffY < 0){ // there are other transitions with the same bb up from me
        distance2bb = me.ry() - bbY;
        if (distance2bb < Scene::BevelSize){
            pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
        } else {
            pPath->lineTo(me.rx() + Scene::BevelSize, me.ry() - Scene::BevelSize);
            distance2bb = myTopY - bbY;
            if (distance2bb < 0){
                pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
            } else {
                if (distance2bb > Scene::BevelSize){
                    pPath->lineTo(me.rx() + Scene::BevelSize, myTopY - Scene::BevelSize);
                } else {
                    pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
                }
            }
        }
    } else if (diffY == 0){ // i am the top
        pPath->lineTo(me.rx() + Scene::BevelSize, myTopY
                      - (distance2bb > Scene::BevelSize ? Scene::BevelSize : distance2bb));
    }
    if (!lastbackbone){
        pPath->moveTo(me.rx(), me.ry());
        me.rx() += Scene::BackboneMargin;
        pPath->lineTo(me.rx(), me.ry());
    }
}
void EventItem::transitionDown (QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone){
    StateItem *pStateItem = qgraphicsitem_cast<StateItem*>(parentItem());
    qreal myBottomY = mapFromItem(parentItem()->parentItem(), QPointF(0, pStateItem->getLY(pBB))).ry();
    qreal diffY = myBottomY - me.ry();
    qreal distance2bb = 0;
    if (diffY > 0){ // there are other transitions with the same bb down from me
        distance2bb = bbY - me.ry();
        if (distance2bb < Scene::BevelSize){
            pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
        } else {
            pPath->lineTo(me.rx() + Scene::BevelSize, me.ry() + Scene::BevelSize);
            distance2bb = myBottomY - bbY;
            if (distance2bb > 0){
                pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
            } else {
                if (distance2bb > -Scene::BevelSize){
                    pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
                } else {
                    pPath->lineTo(me.rx() + Scene::BevelSize, myBottomY + Scene::BevelSize);
                }
            }
        }
    } else if (diffY == 0){ // i am the bottom
        distance2bb = bbY - myBottomY;
        pPath->lineTo(me.rx() + Scene::BevelSize, myBottomY
                      + (distance2bb > Scene::BevelSize ? Scene::BevelSize : distance2bb));
    }
    if (!lastbackbone){
        pPath->moveTo(me.rx(), me.ry());
        me.rx() += Scene::BackboneMargin;
        pPath->lineTo(me.rx(), me.ry());
    }
}
void EventItem::transitionRight(QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone){
    pPath->lineTo(me.rx() + Scene::BevelSize, me.ry());
    pPath->lineTo(me.rx() + Scene::BevelSize, bbY);
    if (!lastbackbone){
        me.rx() += Scene::BackboneMargin - Scene::BevelSize;
        pPath->lineTo(me.rx(), me.ry());
    }
}
void EventItem::updateTransitions (){
//    if (m_Name == "kkkkk"){
//        return;
//    }
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
            StateItem *pStateItem = qgraphicsitem_cast <StateItem*>(parentItem());
			for (int i = 0; i <= max_idx; i++){
				if (idx_set.contains(i)){
                    Backbone *pBb = backboneList.at(i);
                    pBb->setHover (m_Hover);
                    qreal endBBY = mapFromItem(parentItem()->parentItem(), pBb->getEndPoint()).ry();
                    if (endBBY < y){//up
                        transitionUp (&path, QPointF (x, y), endBBY, pBb, i == max_idx);
                    } else if (endBBY > y){//down
                        transitionDown (&path, QPointF (x, y), endBBY, pBb, i == max_idx);
					} else {//right
                        transitionRight(&path, QPointF (x, y), endBBY, pBb, i == max_idx);
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
        this->setHover();
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
void EventItem::hoverEnterEvent(QGraphicsSceneHoverEvent *pHoverEvent){
    m_Hover = true;
    this->updateTransitions();
}
void EventItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *pHoverEvent){
    m_Hover = false;
    this->updateTransitions();
}
void EventItem::setHover(){
    if (m_Hover){
        setPen (EventItem::RedPen);
        setBrush(EventItem::SelectedBrush);
    } else {
        setPen (EventItem::BlackPen);
        setBrush(EventItem::DefaultBrush);
    }
//    m_pTransition->setHover(m_Hover);
}
EventItem::EventItem(StateItem *pTi2, QString name, QFont &font)
: QGraphicsRectItem (pTi2)
, m_pTransition (nullptr)
, m_Name (name)
, m_Hover (false)
{
	QGraphicsTextItem *pText = new QGraphicsTextItem (name, this);
	pText->setFont(font);
	pText->setFlag(QGraphicsItem::ItemNegativeZStacksBehindParent);
	pText->setZValue(-1);
	m_pTransition = new Transition (this);
    setAcceptHoverEvents(true);
    EventItem::DefaultBrush = brush();
    setPen(EventItem::BlackPen);
}
