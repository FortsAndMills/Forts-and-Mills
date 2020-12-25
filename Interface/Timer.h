#ifndef TIMER_H
#define TIMER_H

#include "Technical/HelpManager.h"
#include "BasicElements/DigitObject.h"

class Timer : public GraphicObject
{
    Q_OBJECT

    QTimer * timer;
    QVector < DigitObject * > digits;
    int seconds = 360;
    bool hidden = true;

public:
    Timer(GraphicObject * parent) : GraphicObject(parent, RIGHT_CLICKABLE)
    {
        digits.push_back(new DigitObject(this, 6, "Mill"));
        digits.push_back(new DigitObject(this, COLON_DIGIT, "Fort"));
        digits.push_back(new DigitObject(this, 0, "Mill"));
        digits.push_back(new DigitObject(this, 0, "Fort"));

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(second()));

        this->setVisible(false);
    }
    virtual void Delete()
    {
        foreach (DigitObject * d, digits)
            d->Delete();

        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        qreal sizeX = W / (4 - 3 * constants->textXShift);
        qreal X = 0;
        for (int i = 0; i < 4; ++i)
        {
            digits[i]->setGeometry(X, 0, sizeX, H);
            X += sizeX * (1 - constants->textXShift);
        }
    }

    void rightClick() { emit help->HelpAsked("Timer"); }

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

    void appear()
    {
        hidden = false;
        this->setVisible(true);
        this->setOpacity(0);
        this->AnimationStart(OPACITY, 1, constants->gameMainPhaseStartPanelsAppearTime);
    }
    void disappear()
    {
        hidden = true;
        this->AnimationStart(OPACITY, 0, constants->gameMainPhaseStartPanelsAppearTime);
    }

    void launch(int seconds)
    {
        if (seconds == 0)
        {
            timer->stop();
            if (!hidden) disappear();
            return;
        }

        if (hidden) appear();

        set_seconds(seconds);
        timer->start(1000);
    }

private:
    void set_seconds(int seconds)
    {
        this->seconds = seconds;
        digits[0]->setN(seconds / 60);
        digits[2]->setN((seconds % 60) / 10);
        digits[3]->setN((seconds % 60) % 10);
    }
private slots:
    void second()
    {
        if (seconds > 0)
        {
            seconds -= 1;
            this->set_seconds(seconds);
        }
        else
        {
            timer->stop();
            emit expired();
        }
    }

signals:
    void expired();
};

#endif // TIMER_H
