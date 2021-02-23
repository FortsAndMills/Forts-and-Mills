#ifndef DIGITOBJECT_H
#define DIGITOBJECT_H

#include "Technical/Headers.h"
#include "GraphicObject.h"

// Цифры (пока используются только для размера поля в главменю)

#define COLON_DIGIT -179

class DigitObject : public GraphicObject
{
public:
    int n;
    QString type;  // стиль: "Mill" или "Fort"

    // отображает одну цифру или 'x'
    DigitObject(GraphicObject * parent, int n, QString type = "Fort", Properties prop = 0) :
        GraphicObject(parent, prop, n >= 0 && n <= 9 ? type + "Digit" + QString::number(n) : (n == COLON_DIGIT ? "colon" : "x"))
    {
        this->set_rectangular_boundig_box();
        this->type = type;
        this->n = n;
    }

    // изменение цифры
    void setN(int n)
    {
        this->n = n;
        this->setPicture(type + "Digit" + QString::number(n));
    }
};

#endif // DIGITOBJECT_H
