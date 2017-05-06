#ifndef GAMEORDER_H
#define GAMEORDER_H

#include "Technical/Headers.h"
#include "GameAction.h"

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
    GameAction * AddAction()
    {
        return &((actions << GameAction()).last());
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
    qint16 priority;  // выставленный приоритет для определения порядка в спорных ситуациях

    // флаги
    bool wasInFight = false;
    bool realizationStarted = false;
    bool realizationFinished = false;

    // Навигация по непроставленным параметрам действий
    int actionWithNoParameter;
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
    // Какой параметр нужно выбрать
    GameAction::GameActionParameterType nextParameterType()
    {
        if (actionWithNoParameter == UNDEFINED)
            return GameAction::NONE;
        return actions[actionWithNoParameter].p_type;
    }
    // Установка значений параметра
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

        if (actions.size() > 0)  // если приказ не бездельничать и там есть действия
        {
            // добавление действий "начал выполнять" и "закончил выполнять"

            int lp = actions[0].priority;
            int hp = actions.last().priority;
            actions.push_front(GameAction());
            actions[0].StartRealization(lp - 1);
            AddAction()->FinishRealization(hp + 1);
        }

        // поиск первого действия, для которого нужно выбрать параметр
        actionWithNoParameter = -1;
        findNextActionWithNoParameter();
    }

    friend QTextStream &operator << (QTextStream &stream, const GameOrder *r);
    friend QTextStream &operator >>(QTextStream &stream, GameOrder *r);
};

#endif // GAMEORDER_H






