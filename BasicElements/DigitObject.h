#ifndef DIGITOBJECT_H
#define DIGITOBJECT_H

#include "Technical/Headers.h"
#include "GraphicObject.h"

// Цифры (пока используются только для размера поля в главменю)

class DigitObject : public GraphicObject
{
public:
    int n;
    QString type;  // стиль: "Mill" или "Fort"

    // отображает одну цифру или 'x'
    DigitObject(GraphicObject * parent, int n, QString type, Properties prop) :
        GraphicObject(parent, prop, n >= 0 && n <= 9 ? type + "Digit" + QString::number(n) : "x")
    {
        this->type = type;
        this->n = n;
    }

    // изменение цифры
    void setN(int n)
    {
        this->n = n;
        this->setPicture(type + "Digit" + QString::number(n));
    }

    // переопределение функций для корректного взаимодействия с мышкой
    QRectF boundingRect() const
    {
        return QRectF(0, 0, width(), height());
    }
    QPainterPath shape() const
    {
        QPainterPath qp;
        qp.addRect(boundingRect());
        return qp;
    }
    bool contains(const QPointF &point) const
    {
        return boundingRect().contains(point);
    }
};

#endif // DIGITOBJECT_H
