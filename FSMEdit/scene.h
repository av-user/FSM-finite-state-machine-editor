#ifndef SCENE_H
#define SCENE_H

#include <QGraphicsScene>
#include <QDomElement>

class QGraphicsSceneMouseEvent;
class QPointF;
class QGraphicsLineItem;
class EventItem;
class StateItem;
class Backbone;

class Scene : public QGraphicsScene{
	Q_OBJECT
public:
	static qreal BackboneMargin;
	static qreal BevelSize;
    static qreal RectRadius;
    static qreal ArrowSize;
    static QColor StateColorDark;
    static QColor StateColorLight;
    static QBrush BackgroundBrush;

	explicit Scene(QObject *pParent = NULL);
	QStringList getItemList () const ;
    void renameItem (QString oldName, QString newName);
	QList<Backbone*> getOutBackboneList (QString startItemName) const ;
    const Backbone* getOutBackbone (QString statename, int idx) const ;
	bool backboneExists (QString startItemName, QString endItemName) const ;
	QList<Backbone*> getInBackboneList (QString endItemName) const ;
	enum State { InsertLine, MoveItem, Undefined, };
    void removeStateItem (StateItem *pItem);
    StateItem *getItem (QString name);
    void createBackbone(EventItem *pStartEventItem, StateItem *pEndItem);
    void removeBackbone(StateItem *pStartStateItem, QString endItemName);
    QStringList getEventsList() const ;
    QString serialize (bool all_events);
    bool deserialize (QString str);
    QString getName () const { return m_Name; }
    void setName (QString name) { m_Name = name; }
    void clear ();
    void setAvailableEvents (QStringList eventlist) { m_AvailableEvents = eventlist; }
    void addEventNames (QStringList newEventlist);
public slots:
	void setState (State state);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) Q_DECL_OVERRIDE;

private:
    void createBackbone (StateItem *pStartItem, StateItem *pEndItem);
    void makeTransition (EventItem *pStartItem, StateItem *pEndItem);
    void setDelayedTransitions ();
    QStringList getEventNames (QDomElement *pNode) const ;

	State m_State;
    QString m_Name;
	bool m_LeftButtonDown;
	QPointF m_StartPoint;
	QGraphicsLineItem *m_Line;
    QStringList m_AvailableEvents;

};

#endif // SCENE_H
