#include "veworkplaceview.h"

VEWorkplaceView::VEWorkplaceView(QWidget *parent) : QGraphicsView(parent)
{

}

void VEWorkplaceView::enterEvent(QEvent *event)
{
    QGraphicsView::enterEvent(event);
    viewport()->setCursor(Qt::CrossCursor);
}
//������� �� ������� �������
void VEWorkplaceView::mouseReleaseEvent(QMouseEvent *event)
{
 //   QGraphicsView::mouseReleaseEvent(event);
 //   viewport()->setCursor(Qt::WaitCursor);
	viewport()->setCursor(Qt::CrossCursor);

	if (event->button() == Qt::MiddleButton)
	{
		//		��������� ����� ������ ���� ������� ���������� ������ � mousePressEvent
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
//������� �� ������� �������
void VEWorkplaceView::mousePressEvent(QMouseEvent *event)
{
	if (event->buttons() & Qt::MiddleButton)
	{
		// �� ������� ������� ������ ���� ������� ������� �� ���������� ���������� ���� �������������� � ������� ������� ������� ����� ������ ����
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
		// ��� ��� �� ��� ������ ������������ ���� ������ ������������ �� ����
		setDragMode(QGraphicsView::NoDrag);
		QGraphicsView::mousePressEvent(event);
	}
	else
	{
		// ��� ��� ��������� ������  - ������ ��-��������� ����� ������ ����
		QGraphicsView::mousePressEvent(event);
	}
}

