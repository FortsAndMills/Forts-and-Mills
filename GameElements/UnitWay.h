#ifndef UNITWAY_H
#define UNITWAY_H

#include "Technical/Headers.h"
#include "GameExecution/Game.h"
#include "BasicElements/SpriteObject.h"
#include "Unit.h"

class UnitWay : public SpriteObject
{
    Q_OBJECT

public:
    Coord from, to;
    int from_point;
    int to_point;

public:
    explicit UnitWay(GraphicObject * parent, Unit * unit,
                     Coord from, int from_point,
                     Coord to, int to_point) :
        SpriteObject(parent, 0, unit->prototype->color + "Way")
    {
        this->setZValue(constants->wayZPos);

        this->from = from;
        this->from_point = from_point;
        this->to = to;
        this->to_point = to_point;
    }
    void setStartPosition(QPointF base, QPointF top)
    {
        reconfigure(base, top);
        setGeometry(base.x(), base.y(), geometry.width(), 0);
        setRotation(rotation);
    }

    QRectF geometry;
    qreal rotation;
private:
    QPointF base;
public:
    void reconfigure(QPointF base, QPointF top)
    {
        this->base = base;
        qreal x1 = base.x();
        qreal y1 = base.y();
        qreal x2 = top.x();
        qreal y2 = top.y();

        qreal width = constants->unitWayWidth * constants->hexWidth;
        qreal length = qSqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));

        qreal axis_x = (x1 + x2) / 2;
        qreal axis_y = (y1 + y2) / 2;

        qreal top_x = (x2 - axis_x) * (y2 - y1) / length - (y2 - axis_y) * (x2 - x1) / length + axis_x;
        qreal top_y = -(x2 - axis_x) * (x2 - x1) / length - (y2 - axis_y) * (y2 - y1) / length + axis_y;
        top_x -= width / 2;

        geometry = QRectF(top_x, top_y, width, length);

        if (y2 <= y1)
            rotation = qAsin((x2 - x1) / length) * 180 / 3.1415;
        else
            rotation = 180 - qAsin((x2 - x1) / length) * 180 / 3.1415;
    }

    void disappear()
    {
        this->AnimationStart(OPACITY, 0, constants->unitReconfigureTime);
        connect(this, SIGNAL(movieFinished()), SLOT(Delete()));
    }
    void hide()
    {
        this->AnimationStart(X_POS, base.x(), constants->unitReconfigureTime);
        this->AnimationStart(Y_POS, base.y(), constants->unitReconfigureTime);
        this->AnimationStart(HEIGHT, 0, constants->unitReconfigureTime);
        connect(this, SIGNAL(movieFinished()), SLOT(Delete()));
    }
};

#endif // UNITWAY_H
