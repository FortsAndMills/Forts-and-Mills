#include "GameOrder.h"

void GameOrder::setType(OrderType type)
{
    this->type = type;
    this->parameterType = NONE; // дефолтные состояния
    this->isPeaceful = false;
    this->isMovingUnit = false;  // подразумевается, что перемещение идёт на параметр

    if (type == "Capture")  // особенности
    {
        isPeaceful = true;
        parameterType = TYPE_OF_UNIT;
    }
    else if (type == "Attack")
    {
        parameterType = ADJACENT_HEX_WHERE_CAN_GO;
        isMovingUnit = true;
    }
    else if (type == "Go")
    {
        isPeaceful = true;
        parameterType = ADJACENT_HEX_WHERE_CAN_GO;
        isMovingUnit = true;
    }
    else if (type == "Siege")
    {
        parameterType = ADJACENT_HEX_WHERE_CAN_GO;
        isMovingUnit = true;
    }
    else if (type == "Fire")
    {
        parameterType = VISIBLE_HEX_IN_RADIUS_2;
    }
    else if (type == "Cure")
    {
        isPeaceful = true;
    }
    else if (type == "Fortify")
    {
        isPeaceful = true;
    }
}
