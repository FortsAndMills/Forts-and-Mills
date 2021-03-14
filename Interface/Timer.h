#ifndef TIMER_H
#define TIMER_H

#include "Technical/HelpManager.h"
#include "BasicElements/DigitObject.h"
#include "GameExecution/Game.h"

class Timer : public GraphicObject
{
    Q_OBJECT

    QTimer * timer;
    QVector < DigitObject * > digits;
    int seconds = 360;
    bool hidden = true;

    QSound * soundeffect;

public:
    Timer(GraphicObject * parent) : GraphicObject(parent, RIGHT_CLICKABLE)
    {
        this->set_rectangular_bounding_box();
        this->setVisible(false);

        // цифры
        digits.push_back(new DigitObject(this, 6, "Mill"));
        digits.push_back(new DigitObject(this, COLON_DIGIT, "Fort"));
        digits.push_back(new DigitObject(this, 0, "Mill"));
        digits.push_back(new DigitObject(this, 0, "Fort"));

        // таймер
        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(second()));

        // звук
        soundeffect = new QSound("qrc:/Content/Audio/ticking.wav", this);
        soundeffect->setLoops(QSound::Infinite);
        //connect(soundeffect, SIGNAL(stateChanged(QMediaPlayer::State)), this, SLOT(soundeefectStateChanged(QMediaPlayer::State)));
    }
    virtual void Delete()
    {
        foreach (DigitObject * d, digits)
            d->Delete();
        soundeffect->deleteLater();

        GraphicObject::Delete();
    }

private:
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

    // появление и исчезновение таймера
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
        timer->stop();
        seconds = 0;
        soundeffect->stop();
        this->AnimationStart(OPACITY, 0, constants->gameMainPhaseStartPanelsAppearTime);
    }

public:
    // запуск в момент прихода плана оппонента
    void activate_on_opponent_plan_received(Game * game)
    {
        int seconds = 0;
        if (hidden && game->round > 0)
        {
            if (game->state == Game::GS_CHOOSE_HEX)
                seconds = game->rules->timer_per_choice_after_opponent[game->rules->timer];
            else
                seconds = game->rules->timer_per_plan_after_opponent[game->rules->timer];

            if (seconds != 0)
                launch(seconds);
        }
    }
    // ручной запуск
    void launch(int seconds)
    {
        if (seconds == 0)
        {
            timer->stop();
            soundeffect->stop();
            if (!hidden) disappear();
            return;
        }

        if (hidden) appear();

        set_seconds(seconds);
        timer->start(1000);
    }

private:
    // установка времени и звукового эффекта
    void set_seconds(int seconds)
    {
        this->seconds = seconds;
        digits[0]->setN(seconds / 60);
        digits[2]->setN((seconds % 60) / 10);
        digits[3]->setN((seconds % 60) % 10);

        if (seconds == 30)
            soundeffect->play();
        else if (seconds > 30)
            soundeffect->stop();
    }

private slots:
    // по прошествии одной секунды
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
            soundeffect->stop();
            emit expired();
        }
    }

signals:
    void expired();
};

#endif // TIMER_H
