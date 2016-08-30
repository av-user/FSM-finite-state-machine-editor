#ifndef TRANSITION_H
#define TRANSITION_H

#include <QGraphicsPathItem>

class EventItem;
class StateItem;
class Backbone;

class Transition : public QGraphicsPathItem {
private:
    static QPen		DefaultPen;
    static QPen		SelectedPen;
    static QBrush  DefaultBrush;
    static QBrush  SelectedBrush;
public:
	enum { Type = UserType + 5 };
	int type() const {
	   return Type;
	}
    void setHover (bool hover);
	explicit Transition(EventItem *pEventItem);
	QString toString() const;
};


#endif // TRANSITION_H
