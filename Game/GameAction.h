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
    enum GameActionParameter {NONE,
                              ADJACENT_HEX_WHERE_CAN_GO,
                              VISIBLE_HEX_IN_RADIUS_2,
                              ENEMY_UNIT,
                              TYPE_OF_UNIT} parameter;

    enum GameActionParameterType {PT_NOTHING,
                                  PT_HEX,
                                  PT_UNIT,
                                  PT_UNIT_TYPE} p_type;


    // что по сути параметр: ничего, тип юнита, юнит или гекс.
    GameActionParameterType whatParameter()
    {
        if (parameter == NONE)
            return PT_NOTHING;
        if (parameter == TYPE_OF_UNIT)
            return PT_UNIT_TYPE;
        if (parameter == ENEMY_UNIT)
            return PT_UNIT;
        return PT_HEX;
    }

    // Хранение значений параметров
    Coord target;
    UnitType unitType;
    GameUnit* unit;

    // Возможные действия
    enum GameActionType {START_REALIZATION,
                         AGITE,
                         AMBUSH,
                         LEAVE_HEX,
                         PURSUE,
                         CROSS,
                         ENTER_HEX_WITH_FIGHT,
                         RETURN,
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

    GameAction(int priority = 0)
    {
        parameter = NONE;
        this->priority = priority;
    }

    void StartRealization()
    {
        type = START_REALIZATION;
    }
    void Agite()
    {
        type = AGITE;
        parameter = ADJACENT_HEX_WHERE_CAN_GO;
    }
    void Ambush(int damage)
    {
        type = AMBUSH;
        amount = damage;
    }
    void LeaveHex()
    {
        type = LEAVE_HEX;
        parameter = ADJACENT_HEX_WHERE_CAN_GO;
    }
    void Pursue()
    {
        type = PURSUE;
        parameter = ENEMY_UNIT;
    }
    void Cross()
    {
        type = CROSS;
    }
    void EnterHexWithFight()
    {
        type = ENTER_HEX_WITH_FIGHT;
    }
    void Return()
    {
        type = RETURN;
    }
    void FinishEntering()
    {
        type = FINISH_ENTERING;
    }
    void Shoot(int damage)
    {
        type = SHOOT;
        parameter = VISIBLE_HEX_IN_RADIUS_2;
        amount = damage;
    }
    void CaptureHex()
    {
        type = CAPTURE_HEX;
    }
    void Recruit()
    {
        type = RECRUIT;
        parameter = TYPE_OF_UNIT;
    }
    void Liberate()
    {
        type = LIBERATE;
    }
    void Cure(int amount)
    {
        type = CURE;
        this->amount = amount;
    }
    void Fortificate(int amount)
    {
        type = FORTIFICATE;
        this->amount = amount;
    }
    void FinishRealization()
    {
        type = FINISH_REALIZATION;
    }
};

#endif // GAMEACTION_H
