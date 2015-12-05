#ifndef DIALOG_H
#define DIALOG_H

#include "Technical/Headers.h"
#include "BasicElements/TextObject.h"

class Dialog : public GraphicObject
{
    Q_OBJECT

protected:
    TextObject * message;

    Object * WaitingPicture;

public:
    explicit Dialog(GraphicObject * parent, QString message = "") :
        GraphicObject(parent, DRAGABLE, "DialogWindowFrame")
    {
        this->message = new TextObject(this, "DialogTextGradient", message);

        WaitingPicture = NULL;
    }
    virtual void Delete()
    {
        message->Delete();
        if (WaitingPicture != NULL)
            WaitingPicture->Delete();
        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        message->setGeometry(0, 0, W, H);

        if (WaitingPicture != NULL)
            WaitingPicture->setGeometry(W / 2 - H / 8, 5 * H / 8, H / 4, H / 4);
    }

    void setText(QString text)
    {
        message->setText(text);
    }

public slots:
    void rotateWaitingPicture()
    {
        WaitingPicture->setRotation(0);
        Animation * a = WaitingPicture->AnimationStart(ROTATION, 360);
        connect(a, SIGNAL(finished()), SLOT(rotateWaitingPicture()));
    }
    void setWaiting(bool state)
    {
        if (state && WaitingPicture == NULL)
        {
            WaitingPicture = new Object(this, "SandClock");
            rotateWaitingPicture();

            resizeChildren(width(), height());
        }
        else
        {
            WaitingPicture->Delete();
        }
    }
};

#endif // DIALOG_H
