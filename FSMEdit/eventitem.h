#ifndef EVENTITEM_H
#define EVENTITEM_H

#include <QGraphicsRectItem>
#include <QSet>
#include <QDomDocument>
#include <QDomElement>

class StateItem;
class Backbone;
class QPointF;
class Transition;
class Scene;

class EventItem : public QGraphicsRectItem{
private:
    static QBrush	SelectedBrush;
    static QBrush	DefaultBrush;
    static QColor   EventColorDark;
    static QColor   EventColorLight;
    static QPen		BlackPen;
    static QPen		RedPen;

public:
	enum { Type = UserType + 4 };
	int type() const {
	   return Type;
	}
    explicit EventItem(StateItem *pTi2, QString name, QFont &font);

    virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *pHoverEvent);
    virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *pHoverEvent);

    void addTransition(StateItem *pEndItem);
	bool containsTransition (QString endItemName) const;
	void updateTransitions ();
    const StateItem* getItem() const;
	QString getName () const {return m_Name;}
	QString toString() const;
    QSet<StateItem*> getEndItemsSet() const { return m_EndItemsSet; }
    void removeTransition(StateItem *pEndItem);
    QStringList getTransitionList() const ;
    QDomElement getNode(QDomDocument *pDomDoc) const ;
    void setDelayedTransitions (const QDomElement *pNode);
    bool deserialize(QString str);
    QString serialize() const ;
    QStringList getDelayedTransitions () const ;
    void clearDelayedTransitions() { m_DelayedTransitions.clear(); }
private:
	QString m_Name;
    bool m_Hover;
	Transition *m_pTransition;
    QSet<StateItem*> m_EndItemsSet;
    QStringList m_DelayedTransitions;
	Scene *getScene() const;
    void transitionUp (QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone);
    void transitionDown (QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone);
    void transitionRight (QPainterPath *pPath, QPointF me, qreal bbY, Backbone *pBB, bool lastbackbone);
    void setHover ();
};

#endif // EVENTITEM_H
