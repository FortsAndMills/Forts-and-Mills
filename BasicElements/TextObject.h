#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "Technical/Headers.h"
#include "Technical/Pictures.h"
#include "GraphicObject.h"
#include <QPainter>

class TextObject : public GraphicObject
{
    Q_OBJECT
    QPixmap gradient;
public:
    QString text;

public:
    explicit TextObject(GraphicObject *parent, QString gradientName, QString text) :
        GraphicObject(parent)
    {
        this->gradient = pictures->get(gradientName);
        this->text = text;
    }
    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);
        recountFontSize();
    }

private:
    qreal fontSize;
    void recountFontSize()
    {
        fontSize = 25;  // TODO вычисление размера надписи
    }

public:
    void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
                   QWidget * widget = 0)
    {
        QFont font = QFont(constants->MainFontFamily, fontSize, 100);
        painter->setFont(font);

        QPen pen;
        QRect rect = QRect(0, 0, this->width(), this->height());
        pen.setBrush(QBrush(gradient.scaled(this->width(), this->height(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        painter->setPen(pen);
        painter->drawText(rect, Qt::AlignCenter, text);

        Object::paint(painter, option, widget);
    }
    QRectF boundingRect() const  // для корректной обрезки
    {
        return QRectF(0, 0, this->width(), this->height());
    }

    void setText(QString text)
    {
        this->text = text;
        recountFontSize();
        this->update();
    }
};

#endif // TEXTOBJECT_H
