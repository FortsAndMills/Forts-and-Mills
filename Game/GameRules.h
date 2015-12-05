#ifndef GAMERULES
#define GAMERULES

#include "Technical/Headers.h"
#include "GameHex.h"

class GameRules
{
public:
    int fieldW;
    int fieldH;

    QList <PlayerColor> players;

    QList <OrderType> ordersInGame;

    QMap <HexType, Range> numOfHexes;
    QMap <OrderType, Range> numOfResources;

    int orderLimitPerMill;

    QMap <UnitType, QList <Resource> > startResources;

    QList <UnitType> unitsInGame;

    QList <QString> dayTimes;

    explicit GameRules();
};

#endif
