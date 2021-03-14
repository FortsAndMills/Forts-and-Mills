#ifndef DIGITOBJECT_H
#define DIGITOBJECT_H

#include "Technical/Headers.h"
#include "GraphicObject.h"

// Цифры (пока используются только для размера поля в главменю)

#define COLON_DIGIT -179
#define PAWN_DIGIT -1359

class DigitObject : public GraphicObject
{
public:
    int n;
    QString type;  // стиль: "Mill", "Fort" или цвет

    // отображает одну цифру, 'x', двоеточие или иконку коня
    DigitObject(GraphicObject * parent, int n, QString type = "Fort", Properties prop = 0) :
        GraphicObject(parent, prop, n >= 0 && n <= 9 ? type + "Digit" + QString::number(n) : (n == COLON_DIGIT ? "colon" : (n == PAWN_DIGIT ? type + "Pawn" : "x")))
    {
        this->set_rectangular_bounding_box();
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
