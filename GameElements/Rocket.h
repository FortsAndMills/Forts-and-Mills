#ifndef ROCKET_H
#define ROCKET_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "BasicElements/SpriteObject.h"

// Ракеты для выстрелов!
class Rocket : public GraphicObject
{
    Q_OBJECT

    Object * rocket;

public:
    Coord target;
    bool single;  // одноразовая или в цикле

    enum RocketType {ROCKET, BLAST} type;  // ракета или бласт от штурма
    // константы, различающиеся для разных типов ракет
    QString name;  // например, название картинки
    qreal wid_coeff;  // размеры и время полёта
    qreal heig_coeff;
    int appear_time, fly_time;

public:
    explicit Rocket(GraphicObject * parent, PlayerColor color, bool single = true, RocketType type = ROCKET, Coord target = NOWHERE)
        : GraphicObject(parent)
    {
        this->target = target;
        this->single = single;
        this->type = type;

        if (type == ROCKET)
        {
            name = color + "Rocket";
            wid_coeff = constants->rocketWidth;
            heig_coeff = constants->rocketHeight;
            fly_time = constants->rocketFlyTime;
            appear_time = constants->rocketAppearTime;
        }
        else if (type == BLAST)
        {
            name = "Blast";
            wid_coeff = constants->blastWidth;
            heig_coeff = constants->blastHeight;
            fly_time = constants->blastFlyTime;
            appear_time = constants->blastAppearTime;
        }

        rocket = new Object(this, name);
    }
    void Delete()
    {
        rocket->Delete();
        GraphicObject::Delete();
    }

    // неприятная геометрия для рассчёта положения ракеты и её поворота
    QRectF geometry;
    qreal rotation;
    void reconfigure(QPointF base, QPointF top)
    {
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
        connect(AnimationStart(OPACITY, 0, constants->orderDisappearTime), SIGNAL(finished()), SLOT(Delete()));
    }

    void setStartPosition()
    {
        rocket->setOpacity(0);
        rocket->setGeometry(width() / 2 - wid_coeff * constants->unitsSize / 2,
                                   height() - heig_coeff * constants->unitsSize / 2,
                                    wid_coeff * constants->unitsSize,
                                    heig_coeff * constants->unitsSize);
    }
    void animate()
    {
        if (isDeleted)
            return;

        rocket->AnimationStart(OPACITY, 1, appear_time);
        Animation * a = rocket->AnimationStart(Y_POS, -heig_coeff * constants->unitsSize / 2, fly_time);

        connect(a, SIGNAL(finished()), SLOT(explode()));
        connect(a, SIGNAL(finished()), SLOT(flied()));
    }

private slots:
    void flied()  // завершение цикла анимации
    {        
        setStartPosition();
        if (!single)
        {
            animate();
        }
        else
            QTimer::singleShot(constants->rocketExplosionTime, this, SLOT(Delete()));
    }
    void explode()  // взрыв!
    {
        if (isDeleted)
            return;

        SpriteObject * exp = new SpriteObject(this, 0, "SmallExplosion", constants->rocketExplosionTime, true);
        exp->resize(constants->smallExpWidth * constants->unitsSize,
                    constants->smallExpHeight * constants->unitsSize);
        exp->setPos(QPointF(width() / 2, 0) - exp->center());
        connect(exp, SIGNAL(looped()), exp, SLOT(Delete()));
    }
};

#endif // ROCKET_H
