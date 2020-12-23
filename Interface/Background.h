#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "BasicElements/StateObject.h"

// Задняя рамка всего приложения
class Background : public StateObject
{
    Q_OBJECT

    StateObject * Frame;

public:
    explicit Background(QGraphicsScene * scene) :
        StateObject(0, "Neutral", "NeutralMainWindow")
    {
        scene->addItem(this);
        this->setAcceptedMouseButtons(Qt::LeftButton);
        this->setAcceptHoverEvents(true);

        Frame = new StateObject(this, "Neutral", "NeutralMainWindowFrame", CHILD);
        Frame->setZValue(1);

        this->addPicture("Blue", "BlueMainWindow");
        this->addPicture("Red", "RedMainWindow");
        this->addPicture("Yellow", "YellowMainWindow");
        this->addPicture("Green", "GreenMainWindow");
        Frame->addPicture("Blue", "BlueMainWindowFrame");
        Frame->addPicture("Red", "RedMainWindowFrame");
        Frame->addPicture("Yellow", "YellowMainWindowFrame");
        Frame->addPicture("Green", "GreenMainWindowFrame");
    }

    void resizeChildren(qreal W, qreal H)
    {
        Frame->setGeometry(0, 0, W, H);
    }

    void setState(PlayerColor color)
    {
        this->setPictureState(color);
        Frame->setPictureState(color);
    }

private:
    QPointF drag_position;  // ВОЗМОЖНОСТЬ ПЕРЕТАСКИВАНИЯ
    bool dragging = false;
    void mousePressEvent(QGraphicsSceneMouseEvent * qme)
    {
        drag_position = qme->screenPos();
        if (qme->scenePos().y() <= constants->controlPanelHeight)
        {
            this->setCursor(Qt::ClosedHandCursor);
            dragging = true;
        }
    }
    void mouseMoveEvent(QGraphicsSceneMouseEvent * qme)
    {
        if (dragging)
        {
            emit moved(QPoint(qme->screenPos().x() - drag_position.x(),
                                             qme->screenPos().y() - drag_position.y()));
            drag_position = qme->screenPos();
        }
    }
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *)
    {
        this->setCursor(Qt::ArrowCursor);
        dragging = false;
    }

signals:
    void moved(QPoint delta);
};

#endif // BACKGROUND_H
