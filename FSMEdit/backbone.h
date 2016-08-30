#ifndef BACKBONE_H
#define BACKBONE_H

#include <QGraphicsPathItem>

class StateItem;

class Backbone : public QGraphicsPathItem {
private:
    static QPen		DefaultPen;
    static QPen		SelectedPen;
    static QBrush  DefaultBrush;
    static QBrush  SelectedBrush;
public:
	enum { Type = UserType + 2 };
	int type() const {
	   return Type;
	}
    explicit Backbone(StateItem *pStartItem, StateItem *pEndItem);
    void rename (QString oldName, QString newName);
	void updatePosition();
    void setHover (bool hover);
    const StateItem* const getEndItem () const;
    StateItem* const getStartItem () const;
	QString getEndItemName () const;
	QPointF getEndPoint() const;
	QString toString() const;
    bool isLoop () const;
private:
	void updatePath();
    StateItem *m_pStartItem;
    const StateItem *m_pEndItem;
};

#endif // BACKBONE_H
