#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "Technical/Headers.h"
#include "Technical/Pictures.h"
#include "GraphicObject.h"
#include <QPainter>

// Текстовый объект

class TextObject : public GraphicObject
{
    Q_OBJECT
    QGraphicsTextItem * Label;  // обёртка вокруг этой штуки
public:
    QString text;

public:
    explicit TextObject(GraphicObject *parent, QString text) :
        GraphicObject(parent)
    {
        Label = new QGraphicsTextItem(this);

        // сильно повышает читабельнось текста
        QGraphicsDropShadowEffect * effect = new QGraphicsDropShadowEffect(this);
        effect->setXOffset(0);
        effect->setYOffset(0);
        effect->setBlurRadius(20);
        effect->setColor(Qt::white);
        Label->setGraphicsEffect(effect);

        this->text = text;
        Label->setHtml(processText(text));
    }

    QString processText(QString text)
    {
        // оформление текста. Потенциально можно делать что-то хитрее
        QString newText = "";

        newText += "<font style=\"color:rgba(0,0,0,255)\">";
        newText += text;
        newText += "</font>";

        return "<center>" + newText + "</center>";
    }
    void resizeChildren(qreal W, qreal H)
    {
        // нетривиальность заключается в размещении текста в заданных рамках
        // с максимально возможным шрифтом. Шрифт увеличивается до тех пор,
        // пока текст "помещается" в рамку.

        int i = 1;
        do
        {
            Label->setFont(QFont(constants->MainFontFamily, i, 100));
            Label->setTextWidth(W);
            ++i;
        }
        while (i < constants->MaxTextSize &&
               Label->boundingRect().height() < H);

        i -= 2;  // не 1, потому что так красивее
        Label->setFont(QFont(constants->MainFontFamily, i, 100));
        Label->setTextWidth(W);
        Label->setPos(0, (H - Label->boundingRect().height()) / 2);
    }

    void setText(QString text)
    {
        // изменение текста также ведёт к пересчёту размера
        this->text = text;
        Label->setHtml(processText(text));
        resizeChildren(width(), height());
    }
};

#endif // TEXTOBJECT_H
