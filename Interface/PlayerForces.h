#ifndef PLAYERFORCES_H
#define PLAYERFORCES_H

#include "BasicElements/DigitObject.h"
#include "Technical/HelpManager.h"

class PlayerForces : public GraphicObject
{
    Q_OBJECT

    QVector < DigitObject * > digits;

    int sum = 0;
    int target_sum = 0;

    QTimer * timer;

public:
    PlayerForces(GraphicObject * parent, PlayerColor color) : GraphicObject(parent, RIGHT_CLICKABLE)
    {
        this->set_rectangular_bounding_box();

        // цифры
        digits.push_back(new DigitObject(this, PAWN_DIGIT, color));
        digits.push_back(new DigitObject(this, 0, color));
        digits.push_back(new DigitObject(this, 0, color));

        digits[1]->setVisible(false);

        timer = new QTimer(this);
        timer->setInterval(constants->digitsChangeTime);
        connect(timer, SIGNAL(timeout()), this, SLOT(change_number()));
    }
    virtual void Delete()
    {
        foreach (DigitObject * d, digits)
            d->Delete();
        timer->deleteLater();
        GraphicObject::Delete();
    }

private:
    void resizeChildren(qreal W, qreal H)
    {
        // 2 или 3 символа нужно отражать
        int N = digits[1]->n == 0 ? 2 : 3;

        qreal desiredSizeX = W / (N - (N - 1) * constants->textXShift);
        qreal sizeX = qMin(desiredSizeX, H * constants->digitWtoH);

        qreal realWidth = (N - (N - 1) * constants->textXShift) * sizeX;
        qreal X = (W - realWidth) / 2;
        for (int i = 0; i < 3; ++i)
        {
            if (N == 2 && i == 1) continue;

            digits[i]->setGeometry(X, 0, sizeX, H);
            X += sizeX * (1 - constants->textXShift);
        }
    }

    void rightClick() { emit help->HelpAsked("Forces"); }

public:
    void increase(int N)
    {
        this->target_sum += N;
        this->timer->start();
    }
    void decrease(int N)
    {
        this->target_sum -= N;
        this->timer->start();
    }

private:
    void set()
    {
        digits[1]->setN(sum / 10);
        digits[2]->setN(sum % 10);

        digits[1]->setVisible(digits[1]->n > 0);
        resize(width(), height());
    }

private slots:
    void change_number()
    {
        if (target_sum > sum)
        {
            sum += 1;
            set();
        }
        else if (target_sum < sum)
        {
            sum -= 1;
            set();
        }
        else
            this->timer->stop();
    }
};

#endif // PLAYERFORCES_H
