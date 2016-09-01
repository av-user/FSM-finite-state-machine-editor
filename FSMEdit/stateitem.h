#ifndef STATEITEM_H
#define STATEITEM_H

#include <QPainter>
#include <QGraphicsItem>
#include <QDomDocument>
#include <QDomElement>

class Backbone;
class EventItem;
class Scene;

class StateItem : public QGraphicsItem {
public:
	static qreal Space;
	static qreal Margin;
	static qreal BarWidth;

	enum { Type = UserType + 3 };
	enum BackboneDirection { Up, Down, UpDown };
    explicit StateItem ();

	QRectF boundingRect() const Q_DECL_OVERRIDE;
	void paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget);
	int type() const {
	   return Type;
	}
	void setName(const char *pName);
    void setName(QString name) { m_Name = name; }
	void setEvents(const QList<QString> event_list);
    void rename (QString newName);
	void updateEvents ();
    void updateEvents (QStringList eventList);
	void removeEventItem (EventItem *pItem);
	double getWidth () const;
	double getHeight () const;
	const QFont font() const { return *m_pFont; }
	const QList<QString> getMenuList(const QPoint& pos) const;
	void updateTransitions ();
	QString getName() const { return m_Name;}
    QStringList getEventsList () const ;
    QList<EventItem*> getEvents();
	QPointF getBackboneStartOffset(const Backbone *pBackbone) const;
	QPointF getBackboneStartLow(const Backbone *pBackbone) const;
	QPointF getBackboneEnd(const Backbone *pBackbone) const;
	QString toString() const;
	BackboneDirection getBackboneDirection(const Backbone *pBackbone) const;
	int getOutBackboneIdx(QString endItemName);
	void removeOutBackbone (QString endItemName);
    void setTransitions(EventItem *pStartEventItem, QStringList endItemsList);
    void removeTransition(StateItem *pEndItem);
    void removeTransition(EventItem *pStartEventItem, StateItem *pEndItem);
	bool eventTransitionCollidesWithBackbone(EventItem *pEventItem, int backboneIdx);
    QDomElement getNode(QDomDocument *pDomDoc) const ;
    QString serialize () const ;
    bool deserialize (QString str);
    static StateItem *createState (QDomElement *pNode);
    QFont *getFont() { return m_pFont; }
    double getHY (const Backbone *pBackbone) const;
    double getLY (const Backbone *pBackbone) const;
private:
    bool containsTransition (QString endItemName) const ;
    void updateOutBackbones(bool position, bool transitions) const ;
    void updateInBackbones(bool position, bool transitions) const ;
	int getOutgoingBackboneIndex(const Backbone *pBackbone) const;
	const EventItem* getUpperEvent (QString endItemName) const;
	const EventItem* getLowerEvent (QString endItemName) const;
	bool m_Pressed;
	QString m_Name;
	QFont *m_pFont;
	int m_NormalPenWidth;
	int m_PressedPenWidth;
	int m_Unsensitivity;
	Scene *getScene() const;
	QList<Backbone*> backboneList () const;
protected:
	QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;
	bool m_PositionChanged;
	// overriding mouse events
	void mousePressEvent(QGraphicsSceneMouseEvent *pEvent);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *pEvent);

};

#endif // StateItem_H
