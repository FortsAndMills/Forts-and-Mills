#ifndef EVENTSREALIZATION_H
#define EVENTSREALIZATION_H

#include "GameLog.h"

// Реализации игровых событий
class EventsRealization : public GameLog
{
    Q_OBJECT

public:
    explicit EventsRealization(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    // По окончании реализации также выдаются указания к дальнейшим действиям
    // например, можно попросить подождать какого-то действия пользователя
    // или нажатия на кнопку go
    // или через некоторое время перейти к...
    // либо к следующему действию, либо к продолжению выполнения текущего
    // за последний случай отвечает эта переменная
    int Substate = 0;
    enum W_TYPE {WAIT_FOR_INTERACTION, NO_WAIT, BUTTON};

    class WaitingType  // эта информация хранится вот в такой структуре
    {
    public:
        W_TYPE type;
        bool finished_this_event;
        int time;

        WaitingType(W_TYPE type = NO_WAIT, int time = 0, bool finished_this_event = true)
        {
            this->type = type;
            this->time = time;
            this->finished_this_event = finished_this_event;
        }
    };
    virtual WaitingType RealizeEvent();  // обработать игровое событие

    // реализации события сражения достойны выделения в отдельные функции
    QList<QPointF> UnitsFightObject(QList<GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions);
    bool UnitsFight(QList<GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions, int value, QList<QPointF> points);
};

#endif // EVENTSREALIZATION_H
