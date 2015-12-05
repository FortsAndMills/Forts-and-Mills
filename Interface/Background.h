#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "BasicElements/GraphicObject.h"

class Background : public GraphicObject
{
    Q_OBJECT

public:
    explicit Background(QGraphicsScene * scene, QString pictureName) :
        GraphicObject(0, 0, pictureName)
    {
        scene->addItem(this);
        this->setAcceptedMouseButtons(Qt::LeftButton);
        this->setAcceptHoverEvents(true);
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
