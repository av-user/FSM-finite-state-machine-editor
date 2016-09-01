#include "backbone.h"
#include "StateItem.h"
#include "scene.h"
#include "../LogLib/logger.h"

QBrush      Backbone::DefaultBrush;
QBrush      Backbone::SelectedBrush = QBrush (QColor (255, 10, 10));
QPen		Backbone::DefaultPen;
QPen		Backbone::SelectedPen = QPen (SelectedBrush, 3.0);

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
QPointF Backbone::getEndPoint(qreal startX) const {
    if (m_pEndItem != nullptr){
        QPointF endpoint;
        if (m_pEndItem->pos().x() > startX){
            endpoint.rx() = m_pEndItem->pos().rx();
            endpoint.ry() = m_pEndItem->pos().ry() + Scene::RectRadius;
        } else if (m_pEndItem->pos().x() + m_pEndItem->boundingRect().width() < startX ){
            endpoint.rx() = m_pEndItem->pos().rx() + m_pEndItem->boundingRect().width();
            endpoint.ry() = m_pEndItem->pos().ry() + Scene::RectRadius;
        } else {
            endpoint.rx() = startX;
            endpoint.ry() = m_pEndItem->pos().ry();
        }
        return endpoint;
    } else {
        throw 1;// todo
    }
}
void Backbone::setHover (bool hover){
    if (hover){
        setPen(SelectedPen);
        setZValue(20);
    } else {
        setPen(DefaultPen);
        setZValue(-2);
    }
}
void Backbone::drawArrow (QPainterPath *pPath, QPointF pointStart, QPointF pointEnd){
    if (pointEnd.rx() == pointStart.rx()){
        if (pointEnd.ry() > pointStart.ry() - Scene::ArrowSize){ // down
            pPath->lineTo(pointEnd.rx() - Scene::ArrowSize/2, pointEnd.ry() - Scene::ArrowSize);
            pPath->moveTo(pointEnd);
            pPath->lineTo(pointEnd.rx() + Scene::ArrowSize/2, pointEnd.ry() - Scene::ArrowSize);
        } else if (pointEnd.ry() < pointStart.ry() + Scene::ArrowSize){ // up
            pPath->lineTo(pointEnd.rx() - Scene::ArrowSize/2, pointEnd.ry() + Scene::ArrowSize);
            pPath->moveTo(pointEnd);
            pPath->lineTo(pointEnd.rx() + Scene::ArrowSize/2, pointEnd.ry() + Scene::ArrowSize);
        }
    } else {
        if (pointEnd.rx() > pointStart.rx() + Scene::ArrowSize){ // right
            pPath->lineTo(pointEnd.rx() - Scene::ArrowSize, pointEnd.ry() - Scene::ArrowSize/2);
            pPath->moveTo(pointEnd);
            pPath->lineTo(pointEnd.rx() - Scene::ArrowSize, pointEnd.ry() + Scene::ArrowSize/2);
        } else if (pointEnd.rx() < pointStart.rx() - Scene::ArrowSize){ // left
            pPath->lineTo(pointEnd.rx() + Scene::ArrowSize, pointEnd.ry() - Scene::ArrowSize/2);
            pPath->moveTo(pointEnd);
            pPath->lineTo(pointEnd.rx() + Scene::ArrowSize, pointEnd.ry() + Scene::ArrowSize/2);
        }
    }
}
void Backbone::updatePathUnbeveled(QPointF pointStart, QPointF pointEnd){
    QPainterPath myPath;
    myPath.moveTo(pointStart);
    QPointF pnt;
    pnt.rx() = pointStart.rx();
    pnt.ry() = pointEnd.ry();
    if (pointEnd.rx() == pointStart.rx()){
         myPath.lineTo(pnt.x(), pnt.y());
    } else {
        myPath.lineTo(pointEnd);
    }
    drawArrow(&myPath, pointStart, pointEnd);
    setPath(myPath);
}
void Backbone::updatePath(){
	if (m_pStartItem != NULL && m_pEndItem != NULL){
		QPainterPath myPath;
		QPointF pointStart = mapFromItem (m_pStartItem, m_pStartItem->getBackboneStartOffset(this));
		Log::Logger::debugV("PAINT__", "Backbone::updatePath() pointStart (%5.2f, %5.2f)", pointStart.rx(), pointStart.ry());
        QPointF pointEnd = this->getEndPoint();
		QPointF pnt;
        qreal diffY = pointEnd.ry() - pointStart.ry();
        if (diffY == 0.0){
            myPath.moveTo(pointStart.rx(), pointStart.ry());
        } else if (abs(diffY) < Scene::BevelSize){
            myPath.moveTo(pointStart.rx(), pointEnd.ry());
        } else if (diffY < -Scene::BevelSize*2){ // up
            myPath.moveTo(pointStart.rx(), pointStart.ry() - Scene::BevelSize);
            myPath.lineTo(pointStart.rx(), pointEnd.ry() + Scene::BevelSize);
            myPath.lineTo(pointStart.rx() + Scene::BevelSize, pointEnd.ry());
        } else if (diffY < -Scene::BevelSize){ // "half up"
            myPath.moveTo(pointStart.rx(), pointStart.ry() - Scene::BevelSize);
            myPath.lineTo(pointStart.rx() - diffY - Scene::BevelSize, pointEnd.ry());
        } else if (diffY < Scene::BevelSize*2){
            myPath.moveTo(pointStart.rx(), pointStart.ry() + Scene::BevelSize);
            myPath.lineTo(pointStart.rx() + diffY - Scene::BevelSize, pointEnd.ry());
        } else {
            myPath.moveTo(pointStart.rx(), pointStart.ry() + Scene::BevelSize);
            myPath.lineTo(pointStart.rx(), pointEnd.ry() - Scene::BevelSize);
            myPath.lineTo(pointStart.rx() + Scene::BevelSize, pointEnd.ry());
        }
		myPath.lineTo(pointEnd);
        drawArrow(&myPath, pointStart, pointEnd);
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
    DefaultPen = pen();
    DefaultBrush = brush();
	updatePath ();
}
