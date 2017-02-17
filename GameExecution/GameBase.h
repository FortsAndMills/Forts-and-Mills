#ifndef GAMEBASE_H
#define GAMEBASE_H

#include "Technical/Headers.h"
#include "Game/GameRules.h"
#include "Game/GameHex.h"
#include "Game/GamePlayer.h"
#include "Game/GameUnit.h"
#include "Game/GameOrder.h"
#include "Game/GameAction.h"

enum FightStatus {FS_ATTACK, FS_DEFEND, FS_DISTANT};
enum FightBrid {ENEMY_ORDER_PENALTY,
                             FORTIFICATION,
                             HEX_DEFENCE,
                             ORDER_BONUS,
                             UNIT_DEFENCE,
                             DISTANT,
                             UNIT_HEALTH,
                             STOP_FIGHT,
                             NOTHING};

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

    WAY oppositeWay(WAY way);
    Coord adjacentHex(Coord which, WAY way);
    QList<Coord> visible_hexes(Coord my, int radius = 2);

    QMap<WAY, Coord > adjacentHexesMap(Coord which);
    QList<Coord> adjacentHexes(Coord which);

    WAY whereIs(Coord which, Coord from);
};

#endif // GAMEBASE_H
