#ifndef GAMETURN_H
#define GAMETURN_H

#include "GameHelp.h"
#include "GameMessages.h"

class GameTurns : public GameHelp
{
public:
    QList <GameMessage *> events;

    QMap <PlayerColor, Coord> chosenHex;

    QMap <PlayerColor, bool> ready;
    bool isEveryoneReady()
    {
        foreach (PlayerColor pc, rules->players)
            if (!ready[pc] && !players[pc]->GiveUp)
                return false;
        return true;
    }

    explicit GameTurns(GameRules * rules, Random * rand);

    virtual GameMessage * AddEvent()
    {
        return (events << new GameMessage()).last();
    }

    void playerGiveUp(int index);

    bool CanUse(GameUnit * unit, OrderType order);
    QList<OrderType> whatCanUse(GameUnit *);

    bool ChooseOneOfTheStartHexes();
    void StartPlanning();
};

#endif // GAMETURN_H
