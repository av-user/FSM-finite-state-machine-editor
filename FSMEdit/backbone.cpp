#include "backbone.h"
#include "StateItem.h"
#include "scene.h"
#include "../LogLib/logger.h"

QString Backbone::toString() const {
	QString qstr = "start: ";
	if (m_pStartItem != nullptr){
		qstr += m_pStartItem->toString() + "; ";
	} else {
		qstr += "nullptr";
	}
	qstr += "end: ";
	if (m_pEndItem != nullptr){
		qstr += m_pEndItem->toString();
	} else {
		qstr += "nullptr";
	}
	return qstr;
}
void Backbone::rename (QString oldName, QString newName) {

}
void Backbone::updatePosition(){
	updatePath();
}
QPointF Backbone::getEndPoint() const {
	if (m_pEndItem != nullptr){
        QPointF endpoint;
        QPointF pointStart = mapFromItem (m_pStartItem, m_pStartItem->getBackboneStartOffset(this));
        if (m_pEndItem->pos().x() > pointStart.x()){
            endpoint.rx() = m_pEndItem->pos().rx();
            endpoint.ry() = m_pEndItem->pos().ry() + Scene::RectRadius;
        } else if (m_pEndItem->pos().x() + m_pEndItem->boundingRect().width() < pointStart.x() ){
            endpoint.rx() = m_pEndItem->pos().rx() + m_pEndItem->boundingRect().width();
            endpoint.ry() = m_pEndItem->pos().ry() + Scene::RectRadius;
        } else {
            endpoint.rx() = pointStart.rx();
            endpoint.ry() = m_pEndItem->pos().ry();
        }
        return endpoint;
	} else {
		throw 1;// todo
	}
}
void Backbone::updatePath(){
	if (m_pStartItem != NULL && m_pEndItem != NULL){
		QPainterPath myPath;
        StateItem::BackboneDirection direction = m_pStartItem->getBackboneDirection(this);
		QPointF pointStart = mapFromItem (m_pStartItem, m_pStartItem->getBackboneStartOffset(this));
		Log::Logger::debugV("PAINT__", "Backbone::updatePath() pointStart (%5.2f, %5.2f)", pointStart.rx(), pointStart.ry());
		switch (direction){
        case StateItem::Up:
			pointStart.ry() -= Scene::BevelSize;
			break;
        case StateItem::Down:
        case StateItem::UpDown:
			pointStart.ry() += Scene::BevelSize;
			break;
			break;
		default:
			throw 1;//todo
		}
		myPath.moveTo(pointStart);
        QPointF pointEnd = this->getEndPoint();
		QPointF pnt;
		pnt.rx() = pointStart.rx();
		pnt.ry() = pointEnd.ry();
		QPointF pointStartLow = m_pStartItem->getBackboneStartLow(this);
        if (pointEnd.rx() == pointStart.rx()){
             myPath.lineTo(pnt.x(), pnt.y());
        } else {
            switch (direction){
            case StateItem::UpDown:
                myPath.lineTo(pointStartLow.x(), pointStartLow.y() - Scene::BevelSize);
                myPath.moveTo(pnt);
                break;
            case StateItem::Up:
                myPath.lineTo(pnt.x(), pnt.y() + Scene::BevelSize);
                if (pointEnd.x() > pnt.x()){
                    myPath.lineTo(pnt.x() + Scene::BevelSize, pnt.y());
                } else {
                    myPath.lineTo(pnt.x() - Scene::BevelSize, pnt.y());
                }
                break;
            case StateItem::Down:
                myPath.lineTo(pnt.x(), pnt.y() - Scene::BevelSize);
                if (pointEnd.x() > pnt.x()){
                    myPath.lineTo(pnt.x() + Scene::BevelSize, pnt.y());
                } else {
                    myPath.lineTo(pnt.x() - Scene::BevelSize, pnt.y());
                }
                break;
            default:
                throw 1;//todo
                break;
            }
        }
		Log::Logger::debugV("PAINT__", "Backbone::updatePath() pnt (%5.2f, %5.2f)", pnt.rx(), pnt.ry());
		myPath.lineTo(pointEnd);
        //draw arrow
        if (pointEnd.rx() == pointStart.rx()){
            if (pointEnd.ry() > pointStart.ry() - Scene::ArrowSize){ // down
                myPath.lineTo(pointEnd.rx() - Scene::ArrowSize/2, pointEnd.ry() - Scene::ArrowSize);
                myPath.moveTo(pointEnd);
                myPath.lineTo(pointEnd.rx() + Scene::ArrowSize/2, pointEnd.ry() - Scene::ArrowSize);
            } else if (pointEnd.ry() < pointStart.ry() + Scene::ArrowSize){ // up
                myPath.lineTo(pointEnd.rx() - Scene::ArrowSize/2, pointEnd.ry() + Scene::ArrowSize);
                myPath.moveTo(pointEnd);
                myPath.lineTo(pointEnd.rx() + Scene::ArrowSize/2, pointEnd.ry() + Scene::ArrowSize);
            }
        } else {
            if (pointEnd.rx() > pointStart.rx() + Scene::ArrowSize){ // right
                myPath.lineTo(pointEnd.rx() - Scene::ArrowSize, pointEnd.ry() - Scene::ArrowSize/2);
                myPath.moveTo(pointEnd);
                myPath.lineTo(pointEnd.rx() - Scene::ArrowSize, pointEnd.ry() + Scene::ArrowSize/2);
            } else if (pointEnd.rx() < pointStart.rx() - Scene::ArrowSize){ // left
                myPath.lineTo(pointEnd.rx() + Scene::ArrowSize, pointEnd.ry() - Scene::ArrowSize/2);
                myPath.moveTo(pointEnd);
                myPath.lineTo(pointEnd.rx() + Scene::ArrowSize, pointEnd.ry() + Scene::ArrowSize/2);
            }
        }
		Log::Logger::debugV("PAINT__", "Backbone::updatePath() pointEnd (%5.2f, %5.2f)", pointEnd.rx(), pointEnd.ry());
		setPath(myPath);
	}
}
bool Backbone::isLoop () const {
    return this->m_pStartItem == this->m_pEndItem;
}
const StateItem* const Backbone::getEndItem () const {
	return this->m_pEndItem;
}
StateItem* const Backbone::getStartItem () const{
	return this->m_pStartItem;
}

QString Backbone::getEndItemName () const{
	if (this->m_pEndItem == NULL){
		return "undefined";
	} else {
		return m_pEndItem->getName();
	}
}

Backbone::Backbone(StateItem *pStartItem, StateItem *pEndItem)
: m_pStartItem	(pStartItem)
, m_pEndItem	(pEndItem)
{
//	pStartItem->addOutgoingBackbone (this);
//	pEndItem->addIncomingBackbone(this);
	updatePath ();
}
