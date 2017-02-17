#ifndef GAMEORDER_H
#define GAMEORDER_H

#include "Technical/Headers.h"
#include "GameAction.h"

class GameOrderParameters
{
public:
    OrderType type = DefaultOrder;

    QList <GameAction> actions;
    bool canUse = true;

    int fightInfluence = 0;
    bool burnsWhenFight = false;

    GameOrderParameters(GameRules *rules, UnitType owner, OrderType type);

    GameAction * AddAction()
    {
        return &((actions << GameAction()).last());
    }
    bool containsAction(GameAction::GameActionType type)
    {
        int i = 0;
        while (i < actions.size() && actions[i].type != type) { ++i; }
        return i < actions.size();
    }
};

class GameOrder : public GameOrderParameters
{
public:
    qint16 priority;

    bool wasInFight = false;
    bool realizationStarted = false;
    bool realizationFinished = false;

    int actionWithNoParameter = UNDEFINED;
    GameAction::GameActionParameterType nextParameterType()
    {
        if (actionWithNoParameter == UNDEFINED)
            return GameAction::NONE;
        return actions[actionWithNoParameter].p_type;
    }
    void findNextActionWithNoParameter()
    {
        if (actionWithNoParameter == UNDEFINED)
            return;

        do
        {
            ++actionWithNoParameter;
        }
        while (actionWithNoParameter < actions.size() &&
               actions[actionWithNoParameter].p_type == GameAction::NONE);

        if (actionWithNoParameter == actions.size())
            actionWithNoParameter = UNDEFINED;
    }
    void setParameter(Coord c)
    {
        actions[actionWithNoParameter].target = c;
    }
    void setParameter(UnitType ut)
    {
        actions[actionWithNoParameter].unitType = ut;
    }

    GameOrder(GameRules * rules, UnitType owner, OrderType type, int priority) :
        GameOrderParameters(rules, owner, type)
    {
        this->priority = priority;        

        if (actions.size() > 0)
        {
            int lp = actions[0].priority;
            int hp = actions.last().priority;
            actions.push_front(GameAction());
            actions[0].StartRealization(lp - 1);
            AddAction()->FinishRealization(hp + 1);
        }

        actionWithNoParameter = -1;
        findNextActionWithNoParameter();
    }

    friend QTextStream &operator << (QTextStream &stream, const GameOrder *r);
    friend QTextStream &operator >>(QTextStream &stream, GameOrder *r);
};

#endif // GAMEORDER_H






