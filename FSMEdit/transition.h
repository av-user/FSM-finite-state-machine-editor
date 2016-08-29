#ifndef TRANSITION_H
#define TRANSITION_H

#include <QGraphicsPathItem>

class EventItem;
class StateItem;
class Backbone;

class Transition : public QGraphicsPathItem {
public:
	enum { Type = UserType + 5 };
	int type() const {
	   return Type;
	}
	explicit Transition(EventItem *pEventItem);
	QString toString() const;
};


#endif // TRANSITION_H
