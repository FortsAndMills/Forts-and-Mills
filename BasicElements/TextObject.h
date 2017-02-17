#ifndef TEXTOBJECT_H
#define TEXTOBJECT_H

#include "Technical/Headers.h"
#include "Technical/Pictures.h"
#include "GraphicObject.h"
#include <QPainter>

class TextObject : public GraphicObject
{
    Q_OBJECT
    QGraphicsTextItem * Label;
public:
    QString text;

public:
    explicit TextObject(GraphicObject *parent, QString text) :
        GraphicObject(parent)
    {
        Label = new QGraphicsTextItem(this);

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
        QString newText = "";

        newText += "<font style=\"color:rgba(0,0,0,255)\">";
        newText += text;
        newText += "</font>";

        return "<center>" + newText + "</center>";
    }
    void resizeChildren(qreal W, qreal H)
    {
        int i = 1;
        do
        {
            Label->setFont(QFont(constants->MainFontFamily, i, 100));
            Label->setTextWidth(W);
            ++i;
        }
        while (i < constants->MaxTextSize &&
               Label->boundingRect().height() < H);

        i -= 2;
        Label->setFont(QFont(constants->MainFontFamily, i, 100));
        Label->setTextWidth(W);
        Label->setPos(0, (H - Label->boundingRect().height()) / 2);
    }

    void setText(QString text)
    {
        this->text = text;
        Label->setHtml(processText(text));
        resizeChildren(width(), height());
    }
};

#endif // TEXTOBJECT_H
