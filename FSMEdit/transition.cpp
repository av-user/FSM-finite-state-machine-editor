#include "transition.h"

#include "backbone.h"
#include "stateitem.h"
#include "eventitem.h"
#include "scene.h"
#include "../LogLib/logger.h"

QString Transition::toString() const {
	QString qstr = "todo";
	return qstr;
}

Transition::Transition(EventItem *pEventItem/*, const QString endItemName*/)
: QGraphicsPathItem (pEventItem)
{
}

