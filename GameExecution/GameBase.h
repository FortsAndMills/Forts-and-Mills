#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "Technical/Headers.h"
#include "Game/GameRules.h"
#include "Game/GameHex.h"
#include "Game/GamePlayer.h"
#include "Game/GameUnit.h"
#include "Game/GameOrder.h"
#include "Game/GameAction.h"


// виды статусов юнита в бою
enum FightStatus {FS_ATTACK, FS_DEFEND, FS_DISTANT};

// варианты того, чем может сражаться команда
enum FightBrid {NOTHING = 0,
                FORTIFICATION,
                HEX_DEFENCE,
                ORDER_BONUS,
                DISTANT,
                UNIT_DEFENCE,
                UNIT_HEALTH};

class Strike
{
public:
    QMap<GameUnit*, FightBrid> fb;
    QMap<GameUnit*, int> amount;
    QMap<GameUnit*, int> penalty;
    QMap<GameUnit*, GameHex *> positions;
    QMap<PlayerColor, int> team_amount;
    QMap<PlayerColor, int> granularity;
    bool fight_finished = false;
};


class GameBase
{
public:
    Random * rand;
    GameRules * rules;

    QMap <PlayerColor, GamePlayer *> players;
    QList <QList <GameHex *> > hexes;
    GameHex * hex(Coord Pos)
    {
        return hexes[Pos.x][Pos.y];
    }

    explicit GameBase(GameRules * rules, Random * rand);
    virtual ~GameBase() {}

    // вспомогательные функции навигации по полю
    WAY oppositeWay(WAY way);
    Coord adjacentHex(Coord which, WAY way);
    QList<Coord> visible_hexes(Coord my, int radius = 2);

    QMap<WAY, Coord > adjacentHexesMap(Coord which);
    QList<Coord> adjacentHexes(Coord which);

    WAY whereIs(Coord which, Coord from);
    bool isAdjacent(Coord first, Coord second);
};

#endif // GAMEBASE_H
