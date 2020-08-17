#ifndef GAMEORDEREXECUTION_H
#define GAMEORDEREXECUTION_H

#include "GameFight.h"

// исполнение действий юнитами

class GameOrderExecution : public GameFight
{
protected:
    // оказалось полезным завести такую структуру:
    // кто от какого приказа какое действие исполняет,
    // чтобы например сортировать их
    class Action
    {
    public:
        GameUnit * unit;
        GameOrder * order;
        GameAction action;

        explicit Action(GameUnit * unit, DayTime time, int k)
        {
            this->unit = unit;
            order = unit->plan[time];
            action = order->actions[k];
        }
    };

    explicit GameOrderExecution(GameRules *rules, Random *rand);

    bool CheckIfActionBurns(Action a);
    void Realize(QList<Action> act);
};

#endif // GAMEORDEREXECUTION_H
