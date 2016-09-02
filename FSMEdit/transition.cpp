#include "transition.h"

#include "backbone.h"
#include "stateitem.h"
#include "eventitem.h"
#include "scene.h"
#include "../LogLib/logger.h"

QBrush      Transition::DefaultBrush;
QBrush      Transition::SelectedBrush = QBrush (QColor (255, 10, 10));
QPen		Transition::DefaultPen;
QPen		Transition::SelectedPen = QPen (SelectedBrush, 3.0);

QString Transition::toString() const {
	QString qstr = "todo";
	return qstr;
}
void Transition::setHover (bool hover){
    if (hover){
        setPen(SelectedPen);
        setZValue(20);
    } else {
        setPen(DefaultPen);
        setZValue(-2);
    }
}
Transition::Transition(EventItem *pEventItem/*, const QString endItemName*/)
: QGraphicsPathItem (pEventItem)
{
    DefaultPen = pen();
    DefaultBrush = brush();
}

