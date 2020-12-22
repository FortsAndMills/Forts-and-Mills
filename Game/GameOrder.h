#ifndef GAMEORDER_H
#define GAMEORDER_H

#include "Technical/Headers.h"
#include "GameAction.h"
#include "GameUnit.h"

// Параметры приказов и их разбиение на действия (см. GameAction)
// Зависят только от названия приказа, а также типа юнита, которому был отдан

class GameOrderParameters
{
public:
    OrderType type = DefaultOrder;

    QList <GameAction> actions;
    bool canUse = true;

    int fightInfluence = 0;  // модификатор боя
    bool burnsWhenFight = false;  // сгорает ли при попадании в битву

    GameOrderParameters(GameRules *rules, UnitType owner, OrderType type);

    // тех. функция для удобства
    GameAction * AddAction(int priority)
    {
        return &((actions << GameAction(priority)).last());
    }
    // проверка на наличие того или иного действия - для удобства
    bool containsAction(GameAction::GameActionType type)
    {
        int i = 0;
        while (i < actions.size() && actions[i].type != type) { ++i; }
        return i < actions.size();
    }
};

// Класс игрового приказа
class GameOrder : public GameOrderParameters
{
public:
    // флаги
    bool wasInFight = false;
    bool realizationStarted = false;
    bool realizationFinished = false;

    // Навигация по непроставленным параметрам действий
    int actionWithParameter;
    void findNextActionWithNoParameter()
    {
        if (actionWithParameter == UNDEFINED)
            return;

        do
        {
            ++actionWithParameter;
        }
        while (actionWithParameter < actions.size() &&
               actions[actionWithParameter].parameter == GameAction::NONE);

        if (actionWithParameter == actions.size())
            actionWithParameter = UNDEFINED;
    }
    // Какой параметр нужно выбрать
    GameAction::GameActionParameter nextParameterType()
    {
        if (actionWithParameter == UNDEFINED)
            return GameAction::NONE;
        return actions[actionWithParameter].parameter;
    }
    // Установка значений параметра
    void setParameter(Coord c)
    {
        actions[actionWithParameter].target = c;
    }
    void setParameter(UnitType ut)
    {
        actions[actionWithParameter].unitType = ut;
    }
    void setParameter(GameUnit* unit)
    {
        actions[actionWithParameter].unit = unit;
    }

    GameOrder(GameRules * rules, UnitType owner, OrderType type) :
        GameOrderParameters(rules, owner, type)
    {
        if (actions.size() > 0)  // если приказ не бездельничать и там есть действия
        {
            // добавление действий "начал выполнять" и "закончил выполнять"
            int lp = actions[0].priority;
            int hp = actions.last().priority;
            actions.push_front(GameAction(lp - 1));
            actions[0].StartRealization();
            AddAction(hp + 1)->FinishRealization();
        }

        // поиск первого действия, для которого нужно выбрать параметр
        actionWithParameter = -1;
        findNextActionWithNoParameter();
    }

    // куда перемещается юнит после исполнения
    Coord moves(Coord current)
    {
        foreach (GameAction action, actions)
        {
            if (action.type == GameAction::LEAVE_HEX)
            {
                return action.target;
            }
        }
        return current;
    }

    friend QTextStream &operator << (QTextStream &stream, const GameOrder *r);
    friend QTextStream &operator >>(QTextStream &stream, GameOrder *r);
};

#endif // GAMEORDER_H






