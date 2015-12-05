#ifndef GAMEORDER_H
#define GAMEORDER_H

#include "Technical/Headers.h"
#include "GameUnit.h"

class GameOrder : public QObject
{
    Q_OBJECT

public:
    OrderType type;

    int parameter_x, parameter_y;
    UnitType parameter_unit_type;

    bool isPeaceful;
    bool isMovingUnit;
    enum ORDER_PARAMETER {NONE,
                                                        ADJACENT_HEX_WHERE_CAN_GO,
                                                        VISIBLE_HEX_IN_RADIUS_2,
                                                         TYPE_OF_UNIT} parameterType;

    GameOrder(Game * game, OrderType type) : QObject((QObject *)game)
    {
        setType(type);
    }
    void setType(OrderType type);
};

#endif // GAMEORDER_H






