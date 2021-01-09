#ifndef stamp_H
#define stamp_H

#include <QObject>
#include <QGraphicsPathItem>
#include <QGraphicsItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainterPath>
#include <QGraphicsScene>
#include <QGraphicsPathItem>
#include <QDebug>
//
//
class stamp: public QObject, public QGraphicsPathItem
{
	Q_OBJECT
//		
public:
	explicit stamp(QObject *parent = 0);
	~stamp();

	QPainterPath addMainShtamp();
	QPainterPath addSecondShtamp(int num_page);
private:
	QRectF boundingRect() const;
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
protected:
	void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // stamp_H