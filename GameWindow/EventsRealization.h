#ifndef EVENTSREALIZATION_H
#define EVENTSREALIZATION_H

#include "GameLog.h"

class EventsRealization : public GameLog
{
    Q_OBJECT

public:
    int Substate = 0;

    explicit EventsRealization(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    enum W_TYPE {WAIT_FOR_INTERACTION, NO_WAIT, BUTTON};
    class WaitingType
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

    QList<QPointF> UnitsFightObject(QList<GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions);
    bool UnitsFight(QList<GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions, int value, QList<QPointF> points);
};

#endif // EVENTSREALIZATION_H
