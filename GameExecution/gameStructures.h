#ifndef GAMESTRUCTURES_H
#define GAMESTRUCTURES_H

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

#endif // GAMESTRUCTURES_H
