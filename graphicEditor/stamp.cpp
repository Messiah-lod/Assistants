#include "stamp.h"
//
stamp::stamp(QObject *parent) :
	QObject(parent)
{
	setAcceptHoverEvents(true);
	setFlags(ItemIsSelectable);//����� ���� ������
}

stamp::~stamp()
{

}

QPainterPath stamp::addMainShtamp()
{
	QPainterPath rectPath;//�������� ������������� � ������������
	rectPath.moveTo(0, 1337);
	rectPath.lineTo(1890, 1337);
	rectPath.lineTo(1890, 0);
	rectPath.lineTo(0, 0);
	rectPath.closeSubpath();
	//*************���������� �����
//	setPath(rectPath);

	return rectPath;
}

QPainterPath stamp::addSecondShtamp(int num_page)
{
	QPainterPath rectPath;//�������� ������������� � ������������
	rectPath.moveTo(1890 * (num_page-1), 1337);
	rectPath.lineTo(1890* (num_page), 1337);
	rectPath.lineTo(1890* num_page, 0);
	rectPath.lineTo(1890 * (num_page - 1), 0);
	rectPath.closeSubpath();
	//*************���������� �����
	return rectPath;
}

QRectF stamp::boundingRect() const
{
	return QRectF();
}

void stamp::paint(QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{


}

void stamp::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mousePressEvent(event);
}