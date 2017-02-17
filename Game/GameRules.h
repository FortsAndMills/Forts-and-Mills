#ifndef GAMERULES
#define GAMERULES

#include "Technical/Headers.h"

class GameRules
{
public:
    static QList <QString> AllPlayers;
    static QList <QString> AllOrders;
    static QList <QString> AllUnits;

    int fieldW;
    int fieldH;

    int NumOfPlayers;
    QList <PlayerColor> players;
    void FormPlayersList(Random * rand);

    QList <OrderType> ordersInGame;
    OrderType getOrderByIndex(qint8 i)
    {
        if (i == -1)
            return DefaultOrder;
        else
            return ordersInGame[i];
    }
    qint8 orderIndex(OrderType order)
    {
        if (order == DefaultOrder)
            return -1;

        qint8 i = 0;
        while (ordersInGame[i] != order) { ++i; }
        return i;
    }

    QList <UnitType> unitsInGame;
    qint8 unitTypeIndex(UnitType type)
    {
        qint8 i = 0;
        while (unitsInGame[i] != type) { ++i; }
        return i;
    }

    int dayTimes;
    bool changingDayTimes;

    QMap <HexType, Range> numOfHexTypes;
    QMap <OrderType, Range> numOfResourcesOnField;

    bool doesCaptureRecruits;
    bool doesEnteringEnemyHexLiberates;

    int damageToHomellesUnits;
    bool fortsAreCool;
    bool limitatingResources;

    explicit GameRules();
    explicit GameRules(QList <qint32> hash);
    void recountGenerationParameters();
    friend QTextStream &operator << (QTextStream &stream, const GameRules *r);
    friend QTextStream &operator >>(QTextStream &stream, GameRules *r);
    friend QDataStream &operator << (QDataStream &stream, const GameRules *r);
    friend QDataStream &operator >>(QDataStream &stream, GameRules *r);

    //QString isCorrect();
    QList <qint32> get();
};

#endif
