#include "StateItem.h"
#include <QGraphicsSceneContextMenuEvent>
#include <QFontMetrics>
#include <QGraphicsItem>
#include <qgraphicsscene.h>
#include <qgraphicsview.h>
#include "eventitem.h"
#include "backbone.h"
#include "scene.h"
#include "../LogLib/logger.h"

qreal StateItem::Space		= 5;
qreal StateItem::Margin		= 5;
qreal StateItem::BarWidth	= 15;

Scene* StateItem::getScene() const {
	return dynamic_cast<Scene*>(scene());
}
bool StateItem::deserialize(QString str){
    bool retval = false;
    return retval;
}
StateItem* StateItem::createState (QDomElement *pNode){
    StateItem *pState = new StateItem();
    pState->setName(pNode->attribute("name"));
    QDomElement node = pNode->firstChildElement("event");
    while(!node.isNull()){
        EventItem *pEvent = new EventItem(pState, node.attribute("name", "noname"), *pState->getFont());
        pEvent->setDelayedTransitions (&node);
        node = node.nextSiblingElement("event");
    }
    pState->updateEvents();
    return pState;
}
QDomElement StateItem::getNode(QDomDocument *pDomDoc) const {
    QDomElement element = pDomDoc->createElement("state");
    element.setAttribute("name", m_Name);
    foreach (QGraphicsItem *pItem, this->childItems()) {
        if (pItem->type() == EventItem::Type){
            QDomElement node = qgraphicsitem_cast<EventItem*>(pItem)->getNode(pDomDoc);
            if (!node.isNull()){
                element.appendChild(node);
            }
        }
    }
    return element;
}
QString StateItem::serialize() const {
    QStringList events = getEventsList();
    QString str;
    if (events.count() == 0){
        str = QString::asprintf("\t<state name=\"%s\"/>\n", m_Name.toStdString().c_str());
    } else {
        str = QString::asprintf("\t<state name=\"%s\">\n", m_Name.toStdString().c_str());
        foreach (QGraphicsItem *pItem, this->childItems()) {
            if (pItem->type() == EventItem::Type){
                EventItem *pEI = qgraphicsitem_cast<EventItem*>(pItem);
                str += pEI->serialize ();
            }
        }
        str += "\t</state>\n";
    }
    return str;
}
int StateItem::getOutBackboneIdx(QString endItemName){
	int idx = -1;
	for (int i = 0; i < backboneList().count(); i++){
		Backbone *pB = backboneList().at(i);
		if (pB->getEndItemName() == endItemName){
			idx = i;
			break;
		}
	}
	return idx;
}
void StateItem::updateTransitions (){
	foreach (QGraphicsItem *pItem, this->childItems()) {
		if (pItem->type() == EventItem::Type){
			EventItem *pEI = qgraphicsitem_cast<EventItem*>(pItem);
			pEI->updateTransitions ();
		}
	}
	foreach (Backbone *pB, backboneList()) {
		pB->updatePosition();
	}
}
const EventItem* StateItem::getUpperEvent (QString endItemName) const{
	EventItem *pEventItem = NULL;
	double y = this->getHeight();
	foreach (QGraphicsItem *pItem, this->childItems()) {
		if (pItem->type() == EventItem::Type){
			EventItem *pEI = qgraphicsitem_cast<EventItem*>(pItem);
			double yyy = pEI->pos().y();

            Log::Logger::debugV("PAINT__", "StateItem::getUpperEvent() yyy: %6.2f", yyy);


			if (pEI->containsTransition(endItemName) && pEI->pos().y() < y){
				pEventItem = pEI;
				y = pEI->pos().y();
			}
		}
	}
	return pEventItem;
}
const EventItem* StateItem::getLowerEvent (QString endItemName) const{
	EventItem *pEventItem = NULL;
	double y = 0.0;
	foreach (QGraphicsItem *pItem, this->childItems()) {
		if (pItem->type() == EventItem::Type){
			EventItem *pEI = qgraphicsitem_cast<EventItem*>(pItem);
			if (pEI->containsTransition(endItemName) && pEI->pos().y() > y){
				pEventItem = pEI;
				y = pEI->pos().y();
			}
		}
	}
	return pEventItem;
}
QPointF StateItem::getBackboneStartOffset(Backbone const* pBackbone) const{
// offset from (0,0) point of this item
// 30.08.2016 to this point the backbone ran from the lowest event up,
// or from highest down. now we change it so that it will begin at the
// lowest if goes down, and every transition will run until the backbone's
// start point. otherwise highlighting would be very complex.
	QPointF	pnt;
    QPointF bbendpnt;
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneStartOffset() this: %s", this->toString().toStdString().c_str());
	const EventItem *pEventItem = nullptr;
    StateItem::BackboneDirection bbdir = this->getBackboneDirection(pBackbone);
	int idx = getOutgoingBackboneIndex(pBackbone);
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneStart() idx: %d", idx);
	switch (bbdir){
    case StateItem::Up:
        Log::Logger::debug("PAINT__", "StateItem::getBackboneStartOffset() up");
        pEventItem = getUpperEvent(pBackbone->getEndItemName());
		if (pEventItem != NULL){// may not be null, but we'll check it
            Log::Logger::debugV("PAINT__", "StateItem::getBackboneStart() lower event: %s", pEventItem->toString().toStdString().c_str());
            pnt.setY(pEventItem->pos().y() + pEventItem->rect().height()/2);
		}
		break;
    case StateItem::Down:
        Log::Logger::debug("PAINT__", "StateItem::getBackboneStartOffset() down");
        pEventItem = getLowerEvent (pBackbone->getEndItemName());
		if (pEventItem != NULL){// may not be null, but we'll check it
            Log::Logger::debugV("PAINT__", "StateItem::getBackboneStartOffset() upper event: %s", pEventItem->toString().toStdString().c_str());
			pnt.setY (pEventItem->pos().y() + pEventItem->rect().height()/2);
		}
		break;
    case StateItem::UpDown:
        Log::Logger::debug("PAINT__", "StateItem::getBackboneStartOffset() updown");
        bbendpnt = mapFromItem(pBackbone, pBackbone->getEndPoint(this->getWidth() + Scene::BackboneMargin*(idx+1)));
        Log::Logger::debugV("PAINT__", "StateItem::getBackboneStartOffset() bbendpnt: %6.2f, %6.2f", bbendpnt.rx(), bbendpnt.ry());
        pnt.setY(bbendpnt.ry());
		break;
	default:
		break;
	}
	pnt.setX(this->getWidth() + Scene::BackboneMargin*(idx+1));
	return pnt;
}
QPointF StateItem::getBackboneStartLow(Backbone const* pBackbone) const{
	QPointF	pnt;
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneStartLow() this: %s", this->toString().toStdString().c_str());
	const EventItem *pEventItem = nullptr;
	pnt = this->pos();
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneStart() pnt (%5.2f, %5.2f)", pnt.rx(), pnt.ry());
	int idx = getOutgoingBackboneIndex(pBackbone);
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneStart() idx: %d", idx);
	pEventItem = getLowerEvent (pBackbone->getEndItemName());
	if (pEventItem != NULL){// may not be null, but we'll check it
        Log::Logger::debugV("PAINT__", "StateItem::getBackboneStart() lower event: %s", pEventItem->toString().toStdString().c_str());
		pnt.ry()+= pEventItem->pos().y() + pEventItem->rect().height()/2;
	}
	pnt.rx() += this->getWidth() + Scene::BackboneMargin*(idx+1);
	return pnt;
}
int StateItem::getOutgoingBackboneIndex(const Backbone *  pBackbone) const{
	int count = backboneList().count();
    Log::Logger::debugV("PAINT__", "StateItem::getOutgoingBackboneIndex() count: %d, pBackbone %s nullptr", count, pBackbone == nullptr ? "==" : "!=");
	int idx = -1;
	for (int i = 0; i < count; i++){
		if (backboneList().at(i) == pBackbone){
			idx = i;
			break;
		}
	}
	return idx;
}
double StateItem::myHY (const Backbone *pBackbone) const {
	double y = this->pos().y();
	const EventItem *pEvent = getUpperEvent (pBackbone->getEndItemName());
	if (pEvent != nullptr){
		y =  pos().y() + pEvent->pos().y() + pEvent->rect().height()/2;
	} else {
        Log::Logger::debug("PAINT__", "StateItem::myHY() pEvent nullptr");
	}
	return y;
}
double StateItem::myLY (const Backbone *pBackbone) const {
	double y = this->pos().y();
	const EventItem *pEvent = getLowerEvent (pBackbone->getEndItemName());
	if (pEvent != nullptr){
		y = pos().y() + pEvent->pos().y() + pEvent->rect().height()/2;
	} else {
        Log::Logger::debug("PAINT__", "StateItem::myHY() pEvent nullptr");
	}
	return y;
}
StateItem::BackboneDirection StateItem::getBackboneDirection(const Backbone *pBackbone) const {
	BackboneDirection direction = Up;
	double endY = pBackbone->getEndItem()->pos().ry();
	double myHighY = myHY (pBackbone);
	double myLowY = myLY (pBackbone);
    Log::Logger::debugV("PAINT__", "StateItem::getBackboneDirection() endY: %6.2f, myHY: %6.2f, myLY: %6.2f"
	, endY, myHighY, myLowY);
	if (myHighY > endY){
		;
	} else if (myLowY < endY){
		direction = Down;
	} else {
		direction = UpDown;
	}
	return direction;
}
QPointF StateItem::getBackboneEnd(const Backbone *pBackbone) const{
	QPointF	pnt;
	pnt = this->pos();
    pnt.ry() += Scene::RectRadius;
	return pnt;
}
const QList<QString> StateItem::getMenuList(const QPoint& pos) const {
	QList<QString> list;
	list.append("Edit");
	list.append("Delete");
	return list;
}
void StateItem::setName(const char *pName){
    if (m_Name != pName){
        this->m_Name = pName;
    }
}
void StateItem::rename (QString newName) {
    if (m_Name != newName){
        this->m_Name = newName;
        updateEvents();
        updateOutBackbones(true, true);
        updateInBackbones(true, true);
    }
}
QStringList StateItem::getEventsList () const {
    QStringList list;
    foreach (QGraphicsItem *pGrItem, this->childItems()){
        if (pGrItem->type() == EventItem::Type){
            EventItem *pEvItem = qgraphicsitem_cast<EventItem*>(pGrItem);
            list.append(pEvItem->getName());
        }
    }
    return list;
}
void StateItem::updateEvents (){
	QFontMetrics fm(this->font());
	int width = fm.width(m_Name);
	int w = 0;
	foreach (QGraphicsItem *pGrItem, this->childItems()){
		if (pGrItem->type() == EventItem::Type){
			EventItem *pEvItem = qgraphicsitem_cast<EventItem*>(pGrItem);
			w = fm.width(pEvItem->getName());
			if (w > width){
				width = w;
			}
		}
	}
	int h = Margin + fm.height() + Space;
	foreach (QGraphicsItem *pGrItem, this->childItems()){
		if (pGrItem->type() == EventItem::Type){
			EventItem *pEvItem = qgraphicsitem_cast<EventItem*>(pGrItem);
			pEvItem->setPos(Margin + BarWidth, h);
			pEvItem->setRect(0, 0, width + Space*2, fm.height() + Space);
			h += fm.height() + Space;
		}
	}
}
void StateItem::updateEvents (QStringList eventList){
    QList<EventItem*> toDelete;
    foreach (QGraphicsItem *pGrItem, this->childItems()){
        if (pGrItem->type() == EventItem::Type){
            EventItem *pEvItem = qgraphicsitem_cast<EventItem*>(pGrItem);
            if (eventList.contains(pEvItem->getName())){
                eventList.removeOne(pEvItem->getName());
            } else {
                toDelete.append(pEvItem);
            }
        }
    }
    foreach (EventItem *pEv, toDelete) {
        this->removeEventItem(pEv);
    }
    foreach (QString name, eventList){
        new EventItem (this, name, *m_pFont);
    }
    updateEvents();
    updateOutBackbones(true, true);
    updateInBackbones(true, true);
}
void StateItem::setEvents(const QList<QString> event_list){
	foreach (QString str, event_list){
		new EventItem (this, str, *m_pFont);
	}
	updateEvents();
}
QList<Backbone*> StateItem::backboneList () const {
	QList<Backbone*> list = getScene()->getOutBackboneList(m_Name);
	return list;
}
void StateItem::removeTransition(StateItem *pEndItem){
    foreach (QGraphicsItem *pGrItem, childItems()){
        if (pGrItem->type() == EventItem::Type){
            EventItem *pEvIt = qgraphicsitem_cast<EventItem*>(pGrItem);
            pEvIt->removeTransition (pEndItem);
        }
    }
}
bool StateItem::containsTransition (QString endItemName) const {
    bool retval = false;
    foreach (QGraphicsItem *pGrItem, childItems()){
        if (pGrItem->type() == EventItem::Type){
            EventItem *pEvIt = qgraphicsitem_cast<EventItem*>(pGrItem);
            if (pEvIt->containsTransition(endItemName)){
                retval = true;
                break;
            }
        }
    }
    return retval;
}
void StateItem::setTransitions(EventItem *pStartEventItem, QStringList endItemsList){
    QStringList currentTransitions = pStartEventItem->getTransitionList();
    foreach (QString name, currentTransitions) {
        if (!endItemsList.contains(name)){
            pStartEventItem->removeTransition(getScene()->getItem(name));
            if (!containsTransition(name)){
                getScene()->removeBackbone(this, name);
            }
        } else {
            endItemsList.removeOne(name);
        }
    }
    foreach (QString name, endItemsList) {
        getScene()->createBackbone(pStartEventItem, getScene()->getItem(name));
        pStartEventItem->addTransition(getScene()->getItem(name));
    }
    this->updateTransitions();
}
void StateItem::removeOutBackbone (QString endItemName){
	foreach (Backbone *pBackbone, getScene()->getOutBackboneList(m_Name)) {
		if (pBackbone->getEndItem()->getName() == endItemName){
			delete pBackbone;
			break;
		}
	}
}
void StateItem::updateOutBackbones(bool position, bool transitions) const {
    if (position || transitions){
        foreach (Backbone *pBackbone, backboneList()) {
            if (position){
                pBackbone->updatePosition();
            }
            if (transitions){
                pBackbone->getStartItem()->updateTransitions();
            }
        }
    }
}
void StateItem::updateInBackbones(bool position, bool transitions) const {
    if (position || transitions){
        QList<Backbone*> bboneList = getScene()->getInBackboneList(m_Name);
        foreach (Backbone *pBackbone, bboneList) {
            if (!pBackbone->isLoop()){
                if (position){
                    pBackbone->updatePosition();
                }
                if (transitions){
                    pBackbone->getStartItem()->updateTransitions();
                }
            }
        }
    }
}
QList<EventItem*> StateItem::getEvents(){
    QList<EventItem*> events;
    foreach (QGraphicsItem *pGrItem, this->childItems()){
        if (pGrItem->type() == EventItem::Type){
            events.append(qgraphicsitem_cast<EventItem*>(pGrItem));
        }
    }
    return events;
}
void StateItem::removeEventItem (EventItem *pItem){
    QSet<StateItem*> endItemsSet = pItem->getEndItemsSet();
	delete pItem;
    foreach (StateItem *pEndItem, endItemsSet) {
		bool thereAreMoreTransitions = false;
		foreach (QGraphicsItem *pGrItem, this->childItems()){
			if (pGrItem->type() == EventItem::Type){
				EventItem *pEvItem = qgraphicsitem_cast<EventItem*>(pGrItem);
                if (pEvItem->containsTransition(pEndItem->getName())){
					thereAreMoreTransitions = true;
					break;
				}
			}
		}
		if (!thereAreMoreTransitions){
            removeOutBackbone (pEndItem->getName());
		}
	}
	updateEvents();
    updateOutBackbones(true, true);
    updateInBackbones(true, true);
}
double StateItem::getWidth () const {
	QFontMetrics fm(this->font());
	int width = fm.width(m_Name);
	int w = 0;
	foreach (QGraphicsItem *pItem, this->childItems()){
		if (pItem->type() == EventItem::Type){
			w = pItem->boundingRect().width();
			if (w > width){
				width = w;
			}
		}
	}
	return Margin + BarWidth + Space*2 + width;
}
double StateItem::getHeight () const {
	QFontMetrics fm(this->font());
	int height = fm.height() + Margin + Space;
	foreach (QGraphicsItem *pItem, childItems()){
		if (pItem->type() == EventItem::Type){
			height += pItem->boundingRect().height();
		}
	}
	return height;
}
QRectF StateItem::boundingRect() const {
	return QRectF(0,0,getWidth(),getHeight());
}
bool StateItem::eventTransitionCollidesWithBackbone(EventItem *pEventItem, int backboneIdx){
// apparently given event has no transition on this backbone
// and should decide if an arc is to be painted or not
	bool collides = false;
	bool aboveGivenEventItem = true;
	bool eventOnBackboneFound = false;
    const Backbone *pBackbone = getScene()->getOutBackbone(this->m_Name, backboneIdx);
    if (pBackbone != nullptr){
        qreal yEnd = mapFromItem (pBackbone, pBackbone->getEndPoint()).ry();
        qreal yEventItem = pEventItem->pos().ry() + pEventItem->rect().height()/2;
        foreach (QGraphicsItem *pGrItem, childItems()){
            if (pGrItem->type() == EventItem::Type){
                EventItem *pEvIt = qgraphicsitem_cast<EventItem*>(pGrItem);
                if (pEvIt == pEventItem){
                    aboveGivenEventItem = false;
                    // given item reached
                    if (eventOnBackboneFound && yEnd >= pEventItem->pos().ry() + pEventItem->rect().height()/2){
                        collides = true;
                        break;
                    }
                } else if (aboveGivenEventItem){
                    if (!eventOnBackboneFound){
                        eventOnBackboneFound = pEvIt->containsTransition(pBackbone->getEndItemName());
                    }
                } else {
                    // we are below given item
                    if (pEvIt->containsTransition(pBackbone->getEndItemName())){
                        if (eventOnBackboneFound){
                            collides = true;
                            break;
                        } else {
                            eventOnBackboneFound = true;// does not matter, but...
                            collides = yEnd < yEventItem;
                            break;
                        }
                    } else {
                        ;//todo?
                    }
                }
            }
        }
    }
	return collides;
}
void StateItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Log::Logger::debugV("PAINT__", "StateItem::paint() %s entered", this->m_Name.toStdString().c_str());
	QRectF rect = boundingRect();
	QPen pen;
	if(m_Pressed){
		pen.setBrush(Qt::red);
		pen.setWidth(m_PressedPenWidth);
	} else {
		pen.setBrush(Qt::black);
		pen.setWidth(m_NormalPenWidth);
	}
	painter->setPen(pen);

    QLinearGradient gradient;
    gradient.setStart(0, rect.height()/2);
    gradient.setFinalStop(rect.width(), rect.height()/2);
    gradient.setColorAt(0.0, Scene::StateColorDark);
    gradient.setColorAt(1.0, Scene::StateColorLight);

    painter->setBrush(QBrush(gradient));
	painter->setFont(*m_pFont);
    painter->drawRoundedRect(rect, Scene::RectRadius, Scene::RectRadius);
	QFontMetrics fm(this->font());
	int ascent = fm.ascent();
	painter->drawText(Margin + BarWidth + Space, Margin + ascent, m_Name);
	if (m_PositionChanged){
		m_PositionChanged = false;
        updateInBackbones(true, true);
        updateOutBackbones(true, true);
	}
    Log::Logger::debugV("PAINT__", "StateItem::paint() %s leaved", this->m_Name.toStdString().c_str());
}
void StateItem::mousePressEvent(QGraphicsSceneMouseEvent *event){
	m_Pressed = true;
	QGraphicsItem::mousePressEvent(event);
}
void StateItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event){
	m_Pressed = false;

	QGraphicsItem::mouseReleaseEvent(event);
}
QVariant StateItem::itemChange(GraphicsItemChange change, const QVariant &value){
    Log::Logger::debugV("PAINT__", "StateItem::itemChange() %s entered", this->m_Name.toStdString().c_str());
	if (change == QGraphicsItem::ItemPositionChange) {
        Log::Logger::debugV("PAINT__", "StateItem::itemChange() %s ItemPositionChange", this->m_Name.toStdString().c_str());
		m_PositionChanged = true;
	}
    Log::Logger::debugV("PAINT__", "StateItem::itemChange() %s leaved", this->m_Name.toStdString().c_str());
	return value;
}

QString StateItem::toString() const {
    QString qstr = "StateItem " + m_Name;
	return qstr;
}

StateItem::StateItem()
: m_Pressed			(false)
, m_pFont			(NULL)
, m_NormalPenWidth	(1)
, m_PressedPenWidth	(3)
, m_Unsensitivity	(3)
, m_PositionChanged	(false)
{
	QGraphicsItem *p = this->parentItem();
	setFlag(ItemIsMovable);
	setFlag(QGraphicsItem::ItemSendsGeometryChanges);
	m_pFont = new QFont ("times", 26);

}
