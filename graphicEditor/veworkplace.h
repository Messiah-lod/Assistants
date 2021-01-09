#ifndef WORKPLACE_H
#define WORKPLACE_H

#include <QObject>
#include <QGraphicsScene>
#include <QtWidgets/QWidget>
#include <QtWidgets>

class QGraphicsSceneMouseEvent;
class QKeyEvent;

class VEWorkplace : public QGraphicsScene
{
    Q_OBJECT

    Q_PROPERTY(int currentAction READ currentAction WRITE setCurrentAction NOTIFY currentActionChanged)
    Q_PROPERTY(QPointF previousPosition READ previousPosition WRITE setPreviousPosition NOTIFY previousPositionChanged)

public:
    explicit VEWorkplace(QObject *parent = 0);
    ~VEWorkplace();

    enum ActionTypes {
        DefaultType,
        LineType,
        RectangleType,
        SelectionType
    };

    int currentAction() const;
    QPointF previousPosition() const;

    void setCurrentAction(const int type);
    void setPreviousPosition(const QPointF previousPosition);

signals:
    void previousPositionChanged();
    void currentActionChanged(int);
    void signalSelectItem(QGraphicsItem *item);
    void signalNewSelectItem(QGraphicsItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
//	void wheelEvent(QGraphicsSceneWheelEvent *event);
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void deselectItems();

public slots:
    void slotMove(QGraphicsItem *signalOwner, qreal dx, qreal dy);

private:
    QGraphicsItem *currentItem;//конкретный элемент сцены
    int m_currentAction;//выбранный тип объекта (линия,прямоугольник..)
    int m_previousAction;//предыдущий выбранный тип объекта (линия,прямоугольник..)
    QPointF m_previousPosition;//старая позиция курсора
    bool m_leftMouseButtonPressed;//сигнал нажатия левой кнопки мши
//	bool m_MidMouseButtonPressed;//сигнал нажатия колесика мыши
};

#endif // WORKPLACE_H
