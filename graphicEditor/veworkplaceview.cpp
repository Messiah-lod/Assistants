#include "veworkplaceview.h"

VEWorkplaceView::VEWorkplaceView(QWidget *parent) : QGraphicsView(parent)
{

}

void VEWorkplaceView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}
//событие по отжатию клавиши
void VEWorkplaceView::mouseReleaseEvent(QMouseEvent *event)
{
 //   QGraphicsView::mouseReleaseEvent(event);
 //   viewport()->setCursor(Qt::WaitCursor);
	viewport()->setCursor(Qt::CrossCursor);

	if (event->button() == Qt::MiddleButton)
	{
		//		отпускаем левую кнопку мыши которую виртуально зажали в mousePressEvent
		QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(), event->windowPos(),
			Qt::LeftButton, event->buttons() & ~Qt::LeftButton, event->modifiers());
		QGraphicsView::mouseReleaseEvent(&fakeEvent);

		setDragMode(QGraphicsView::RubberBandDrag);
//		setDragMode(QGraphicsView::NoDrag);
	}
	else
	{
		QGraphicsView::mouseReleaseEvent(event);

	}
}
//событие по нажатию клавиши
void VEWorkplaceView::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::MiddleButton)
	{
		// по нажатию средней кнопки мыши создаем событие ее отпускания выставляем моду перетаскивания и создаем событие зажатой левой кнопки мыши
		QMouseEvent releaseEvent(QEvent::MouseButtonRelease, event->localPos(), event->screenPos(), event->windowPos(),
			Qt::LeftButton, 0, event->modifiers());
		QGraphicsView::mouseReleaseEvent(&releaseEvent);

		setDragMode(QGraphicsView::ScrollHandDrag);

		QMouseEvent fakeEvent(event->type(), event->localPos(), event->screenPos(), event->windowPos(),
			Qt::LeftButton, event->buttons() | Qt::LeftButton, event->modifiers());
		QGraphicsView::mousePressEvent(&fakeEvent);
	}
	else if (event->button() == Qt::RightButton)
	{
		// это что бы при вызове контекстного меню ничего постороннего не было
		setDragMode(QGraphicsView::NoDrag);
		QGraphicsView::mousePressEvent(event);
	}
	else
	{
		// это для выделения рамкой  - работа по-умолчанию левой кнопки мыши
		QGraphicsView::mousePressEvent(event);
	}
}

