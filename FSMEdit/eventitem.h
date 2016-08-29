#ifndef EVENTITEM_H
#define EVENTITEM_H

#include <QGraphicsRectItem>
#include <QSet>
#include <QDomDocument>
#include <QDomElement>

class StateItem;
class QPointF;
class Transition;
class Scene;

class EventItem : public QGraphicsRectItem{
public:
	enum { Type = UserType + 4 };
	int type() const {
	   return Type;
	}
    explicit EventItem(StateItem *pTi2, QString name, QFont &font);
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
	Transition *m_pTransition;
    QSet<StateItem*> m_EndItemsSet;
    QStringList m_DelayedTransitions;
	Scene *getScene() const;
};

#endif // EVENTITEM_H
