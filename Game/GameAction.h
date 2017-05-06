#ifndef GAMEACTION_H
#define GAMEACTION_H

#include "Technical/Headers.h"

// Отмечает базовое действие, которое может совершить юнит
// Игровой приказ может совмещать несколько действий
// Каждое действие может иметь параметр, который игроку предстоит выбрать
// В этом классе содержится категориальная информация об этом

class GameAction
{
public:
    // тип параметра действия
    enum GameActionParameterType {NONE,
                                                              ADJACENT_HEX_WHERE_CAN_GO,
                                                              VISIBLE_HEX_IN_RADIUS_2,
                                                              TYPE_OF_UNIT} p_type;

    // что по сути параметр: ничего, тип юнита или гекс.
    int whatParameter()
    {
        if (p_type == NONE)
            return 0;
        if (p_type == TYPE_OF_UNIT)
            return 1;
        return 2;
    }

    // Хранение значений параметров
    Coord target;
    UnitType unitType;

    // Возможные действия
    enum GameActionType {START_REALIZATION,
                                            AMBUSH,
                                            LEAVE_HEX,
                                            ENTER_HEX,
                                            ENTER_HEX_WITH_FIGHT,
                                            FINISH_ENTERING,
                                            SHOOT,
                                            CAPTURE_HEX,
                                            RECRUIT,
                                            LIBERATE,
                                            CURE,
                                            FORTIFICATE,
                                            FINISH_REALIZATION} type;

    int priority;  // его приоритет в списке всех действий раунда

    int amount;  // "мощность", обычно игровая константа - для удобства.

    GameAction()
    {
    }

    void StartRealization(int priority)
    {
        type = START_REALIZATION;
        p_type = NONE;
        this->priority = priority;
    }
    void Ambush(int priority, int damage)
    {
        type = AMBUSH;
        p_type = NONE;
        this->priority = priority;
        amount = damage;
    }
    void LeaveHex(int priority)
    {
        type = LEAVE_HEX;
        p_type = ADJACENT_HEX_WHERE_CAN_GO;
        this->priority = priority;
    }
    void EnterHexWithFight(int priority)
    {
        type = ENTER_HEX_WITH_FIGHT;
        p_type = NONE;
        this->priority = priority;
    }
    void FinishEntering(int priority)
    {
        type = FINISH_ENTERING;
        p_type = NONE;
        this->priority = priority;
    }
    void Shoot(int priority, int damage)
    {
        type = SHOOT;
        p_type = VISIBLE_HEX_IN_RADIUS_2;
        this->priority = priority;
        amount = damage;
    }
    void CaptureHex(int priority)
    {
        type = CAPTURE_HEX;
        p_type = NONE;
        this->priority = priority;
    }
    void Recruit(int priority)
    {
        type = RECRUIT;
        p_type = TYPE_OF_UNIT;
        this->priority = priority;
    }
    void Liberate(int priority)
    {
        type = LIBERATE;
        p_type = NONE;
        this->priority = priority;
    }
    void Cure(int priority, int amount)
    {
        type = CURE;
        p_type = NONE;
        this->priority = priority;
        this->amount = amount;
    }
    void Fortificate(int priority, int amount)
    {
        type = FORTIFICATE;
        p_type = NONE;
        this->priority = priority;
        this->amount = amount;
    }
    void FinishRealization(int priority)
    {
        type = FINISH_REALIZATION;
        p_type = NONE;
        this->priority = priority;
    }
};

#endif // GAMEACTION_H
