#include "scene.h"
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsLineItem>
#include <QGraphicsView>
#include <QDomDocument>
#include "StateItem.h"
#include "eventitem.h"
#include "backbone.h"
#include "transition.h"
#include "../LogLib/logger.h"

qreal Scene::BackboneMargin = 30.0;
qreal Scene::BevelSize = 5.0;
qreal Scene::RectRadius = 10.0;
qreal Scene::ArrowSize = 10.0;
//QBrush Scene::StateBrush = QBrush(QColor(250, 248, 248));
QColor Scene::StateColorDark (232, 223, 223);
QColor Scene::StateColorLight (255, 254, 254);
QBrush Scene::BackgroundBrush = QBrush(QColor(255, 255, 240));


void Scene::clear (){
    foreach (QGraphicsItem *pGrItem, this->views().first()->items()) {
        if (pGrItem->type() == Backbone::Type || pGrItem->type() == StateItem::Type){
            removeItem(pGrItem);
            delete pGrItem;
        }
    }
}
QStringList Scene::getEventNames (QDomElement *pNode) const {
    QStringList list;
    QDomElement node = pNode->firstChildElement("eventname");
    while(!node.isNull()){
        list.append(node.firstChild().nodeValue());
        node = node.nextSiblingElement("eventname");
    }
    return list;
}
bool Scene::deserialize (QString str){
    bool retval = false;
    QDomDocument domdoc;
    domdoc.setContent(str);
    QDomElement root = domdoc.documentElement();
    if (!root.isNull()){
        QDomElement node = root.firstChildElement("state");
        while(!node.isNull()){
            QPointF pos;
            pos.rx() = node.attribute("x", "0.0").toDouble();
            pos.ry() = node.attribute("y", "0.0").toDouble();
            StateItem *pState = StateItem::createState (&node);
            if (pState != nullptr){
                pState->setPos(pos);
                pState->setName(node.attribute("name", "noname"));
                addItem(pState);
                m_AvailableEvents.append(pState->getEventsList());
                m_AvailableEvents.removeDuplicates();
            }
            node = node.nextSiblingElement("state");
        }
        setDelayedTransitions ();
        update();
        node = root.firstChildElement("eventlist");
        if (!node.isNull()){
            addEventNames(getEventNames (&node));
        }
    }
    return retval;
}
QString Scene::serialize(bool all_events){
    QDomDocument domdoc("FSM");
    QDomElement root = domdoc.createElement("FSM");
    root.setAttribute("name", m_Name);
    domdoc.appendChild(root);
    foreach (QGraphicsItem *pItem, this->views().first()->items()) {
        if (pItem->type() == StateItem::Type){
            QDomElement node = qgraphicsitem_cast<StateItem*>(pItem)->getNode(&domdoc);
            if (!node.isNull()){
                node.setAttribute("y", pItem->pos().ry());
                node.setAttribute("x", pItem->pos().rx());
                root.appendChild(node);
            }
        }
    }
    if (all_events && !m_AvailableEvents.empty()){
        QDomElement listnode = domdoc.createElement("eventlist");
        if (!listnode.isNull()){
            foreach (QString name, m_AvailableEvents){
                QDomElement eventnode = domdoc.createElement("eventname");
                if (!eventnode.isNull()){
                    QDomText textNode = domdoc.createTextNode(name);
                    eventnode.appendChild(textNode);
                    listnode.appendChild(eventnode);
                }
            }
            root.appendChild(listnode);
        }
    }
    return domdoc.toString();
}
void Scene::setDelayedTransitions () {
    foreach (QGraphicsItem *pItem, this->views().first()->items()) {
        if (pItem->type() == StateItem::Type){
            StateItem *pStateItem = (qgraphicsitem_cast<StateItem*>(pItem));
            QList<EventItem*> events = pStateItem->getEvents();
            foreach (EventItem *pEventItem, events) {
                QStringList delayed_transitions = pEventItem->getDelayedTransitions();
                foreach (QString transition, delayed_transitions){
                    makeTransition(pEventItem, getItem (transition));
                }
                pEventItem->clearDelayedTransitions();
            }
        }
    }
}
QStringList Scene::getItemList () const {
	QStringList list;
	foreach (QGraphicsItem *pItem, this->views().first()->items()) {
        if (pItem->type() == StateItem::Type){
            list.append(qgraphicsitem_cast<StateItem*>(pItem)->getName());
		}
	}
    qSort(list);
	return list;
}
StateItem* Scene::getItem (QString name){
    foreach (QGraphicsItem *pItem, this->views().first()->items()) {
        if (pItem->type() == StateItem::Type){
            StateItem *pStateItem = qgraphicsitem_cast<StateItem*>(pItem);
            if (pStateItem->getName() == name){
                return pStateItem;
            }
        }
    }
    return nullptr;
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent){
	if (mouseEvent->button() == Qt::LeftButton){
		QGraphicsItem *pItem = this->itemAt(mouseEvent->scenePos(), this->views().first()->transform());
		this->m_State = Undefined;
		if (pItem != NULL){
			int type = pItem->type();
			Log::Logger::debugV("PAINT__", "Scene::mousePressEvent pItem->type(): %d", type);
			switch (pItem->type()){
			case EventItem::Type:
				Log::Logger::debug("PAINT__", "EventItem::Type");
				m_Line = new QGraphicsLineItem(QLineF(mouseEvent->scenePos(), mouseEvent->scenePos()));
				m_Line->setPen(QPen(Qt::red, 10));
				addItem(m_Line);
				this->m_State = InsertLine;
				break;
            case StateItem::Type:
                Log::Logger::debug("PAINT__", "StateItem::Type");
				QGraphicsScene::mousePressEvent(mouseEvent);
				this->m_State = MoveItem;
				break;
			case Backbone::Type:
				Log::Logger::debug("PAINT__", "Backbone::Type");
				QGraphicsScene::mousePressEvent(mouseEvent);
				break;
			default:
				Log::Logger::debug("PAINT__", "???");
				QGraphicsScene::mousePressEvent(mouseEvent);
				break;
			}
			invalidate();
		}
	}
}
void Scene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent){
	switch (m_State) {
	case InsertLine:
		if (m_Line != NULL){
			QLineF newLine(m_Line->line().p1(), mouseEvent->scenePos());
			m_Line->setLine(newLine);
		}
		break;
	case MoveItem:
		QGraphicsScene::mouseMoveEvent(mouseEvent);
		break;
	default:
		QGraphicsScene::mouseMoveEvent(mouseEvent);
		break;
	}
}
void Scene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent){
	switch (m_State) {
	case InsertLine:
		if (m_Line != NULL){
			QList<QGraphicsItem *> startItems = items(m_Line->line().p1());
			if (startItems.count() && startItems.first() == m_Line){
				startItems.removeFirst();
			}
			QList<QGraphicsItem *> endItems = items(m_Line->line().p2());
			if (endItems.count() && endItems.first() == m_Line){
				endItems.removeFirst();
			}
			removeItem(m_Line);
			delete m_Line;
			m_Line = NULL;
			if (startItems.count() > 0
			&&	endItems.count() > 0
			&&	startItems.first()->type() == EventItem::Type
            &&	(endItems.first()->type() == StateItem::Type || endItems.first()->type() == EventItem::Type)
			&&	startItems.first() != endItems.first()) {
				EventItem *pStartEventItem = qgraphicsitem_cast<EventItem *>(startItems.first());
                StateItem *pEndItem = NULL;
				EventItem *pEventItem = NULL;
				switch ( endItems.first()->type()){
                case StateItem::Type:
                    pEndItem = qgraphicsitem_cast<StateItem *>(endItems.first());
					break;
				case EventItem::Type:
					pEventItem = qgraphicsitem_cast<EventItem*>(endItems.first());
                    pEndItem = qgraphicsitem_cast<StateItem*>(endItems.first()->parentItem());
					break;
				default:
					break;
				}
				if (pEndItem != NULL){
					makeTransition (pStartEventItem, pEndItem);
				}
			}
		}
		break;
	}
	QGraphicsScene::mouseReleaseEvent(mouseEvent);
	invalidate();
	m_State = State::Undefined;
}
void Scene::removeStateItem (StateItem *pItem){
    QList<Backbone*> bblist = getOutBackboneList (pItem->getName());
    foreach (Backbone *pBackbone, bblist) {
        removeItem(pBackbone);
        delete pBackbone;
    }
    bblist = getInBackboneList (pItem->getName());
    foreach (Backbone *pBackbone, bblist) {
        StateItem *pStartItem = pBackbone->getStartItem();
        pStartItem->removeTransition(pItem);
        removeItem(pBackbone);
        delete pBackbone;
    }
    removeItem(pItem);
}
QList<Backbone*> Scene::getOutBackboneList (QString startItemName) const {
	QList<Backbone*> list;
	foreach (QGraphicsItem *pGrItem , this->views().first()->items()) {
		if (pGrItem->type() == Backbone::Type){
			Backbone *pBackbone = qgraphicsitem_cast<Backbone*>(pGrItem);
			if (pBackbone->getStartItem()->getName() == startItemName){
				list.append(pBackbone);
			}
		}
	}
    qSort(list);
	return list;
}
const Backbone* Scene::getOutBackbone (QString statename, int idx) const {
	Backbone *pBackbone = nullptr;
    QList<Backbone*> list = getOutBackboneList (statename);
    if (idx < list.count()){
        pBackbone = list.at(idx);
    }
//	int i = -1;
//	foreach (QGraphicsItem *pGrItem , this->views().first()->items()) {
//		if (pGrItem->type() == Backbone::Type){
//			if (++i == idx){
//				pBackbone = qgraphicsitem_cast<Backbone*>(pGrItem);
//				break;
//			}
//		}
//	}
	return pBackbone;
}
bool Scene::backboneExists (QString startItemName, QString endItemName) const {
	bool exists = false;
	foreach (QGraphicsItem *pGrItem , this->views().first()->items()) {
		if (pGrItem->type() == Backbone::Type){
			Backbone *pBb = qgraphicsitem_cast<Backbone*>(pGrItem);
			if (pBb->getStartItem()->getName() == startItemName
			&& pBb->getEndItemName() == endItemName){
				exists = true;
				break;
			}
		}
	}
	return exists;
}
QList<Backbone*> Scene::getInBackboneList (QString endItemName) const {
	QList<Backbone*> list;
	foreach (QGraphicsItem *pGrItem , this->views().first()->items()) {
		if (pGrItem->type() == Backbone::Type){
			Backbone *pBackbone = qgraphicsitem_cast<Backbone*>(pGrItem);
			if (pBackbone->getEndItemName() == endItemName){
				list.append(pBackbone);
			}
		}
	}
    qSort (list);
	return list;
}
void Scene::createBackbone(EventItem *pStartEventItem, StateItem *pEndItem){
    StateItem *pStartTestItem = qgraphicsitem_cast<StateItem*>(pStartEventItem->parentItem());
    createBackbone(pStartTestItem, pEndItem);
}
void Scene::createBackbone(StateItem *pStartItem, StateItem *pEndItem){
	if (!backboneExists(pStartItem->getName(), pEndItem->getName())){
		Backbone* pBackboneNew = new Backbone (pStartItem, pEndItem);
		this->addItem(pBackboneNew);
		pBackboneNew->setZValue(pStartItem->zValue() - 1000);
	}
}
void Scene::removeBackbone(StateItem *pStartStateItem, QString endItemName){
    foreach (QGraphicsItem *pGrItem , this->views().first()->items()) {
        if (pGrItem->type() == Backbone::Type){
            Backbone *pBackbone = qgraphicsitem_cast<Backbone*>(pGrItem);
            if (pBackbone->getStartItem() == pStartStateItem && pBackbone->getEndItemName() == endItemName){
                delete pBackbone;
            }
        }
    }
}
void Scene::makeTransition (EventItem *pStartItem, StateItem *pEndItem){
	Log::Logger::debug("PAINT__", "Scene::makeTransition() entered");
    createBackbone(qgraphicsitem_cast<StateItem*>(pStartItem->parentItem()), pEndItem);
	pStartItem->addTransition(pEndItem);
    qgraphicsitem_cast<StateItem*>(pStartItem->parentItem())->updateTransitions();
	Log::Logger::debug("PAINT__", "Scene::makeTransition() leaved");
}
void Scene::renameItem (QString oldName, QString newName){
    foreach (QGraphicsItem *pItem, this->views().first()->items()) {
        if (pItem->type() == StateItem::Type){
            StateItem *pStateItem = qgraphicsitem_cast<StateItem*>(pItem);
            if (pStateItem->getName() == oldName){
                pStateItem->rename (newName);
            }
        }
    }
}
void Scene::setState(State state){
	m_State = state;
}
QStringList Scene::getEventsList() const {
    return m_AvailableEvents;
}
void Scene::addEventNames (QStringList newEventlist){
    m_AvailableEvents += newEventlist;
    m_AvailableEvents.removeDuplicates();
}

Scene::Scene(QObject *pParent)
: QGraphicsScene(pParent)
, m_Name ("noname")
{
    setBackgroundBrush(Scene::BackgroundBrush);

//    <state code="0" >Init</state>
//                Request->ReplayJoker
//    <state code="2">Terminate</state>
//    <state code="1" >ActionLotto</state>
//                Init->ActionLotto
//                GuiEvent->ActionLotto
//    <state code="3" >Host</state>
//                HostResponse->Print
//                Init->Print
//                HostResponse->PromoTest
//                HostResponse->PromoVoucher
//                HostResponse->Terminate
//    <state code="6" >Print</state>
//                PrintSuccess->Terminate
//                PrintSuccess->PromoWager
//    <state code="4" >PromoText</state>
//                HostResponse->Print
//                HostResponse->PromoVoucher
//                HostResponse->PromoText
//    <state code="5" >PromoVoucher</state>
//                PrintSuccess->Print
//    <state code="7" >PromoWager</state>
//                ActionResponse->Terminate
//    <state code="8" >ReplayJoker</state>
//                Init->Host
//                GuiEvent->Host
//                GuiEvent->ReplayJoker
}
