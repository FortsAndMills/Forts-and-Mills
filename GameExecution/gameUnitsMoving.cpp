#include "GameUnitsMoving.h"

GameUnitsMoving::GameUnitsMoving(GameRules *rules, Random *rand) :
    GameField(rules, rand)
{
}

QSet <GameUnit *> GameUnitsMoving::findAll(SEARCH_TYPE ST, GameUnit * for_whom,
                          Coord staying, Coord going_to)
{
    QSet <GameUnit *> ans;
    for (int i = 0; i < rules->players.size(); ++i)
    {
        if ((ST == ENEMY && rules->players[i] != for_whom->color) ||
            (ST == ALLY && rules->players[i] == for_whom->color))
        {
            foreach (GameUnit * unit, players[rules->players[i]]->units)
            {
                if (unit->health > 0 && unit != for_whom &&
                     (staying == ANY || unit->position == staying) &&
                     (going_to == ANY || unit->going_to == going_to))
                {
                    ans << unit;
                }
            }
        }
    }

    return ans;
}

QList <GameUnit *> GameUnitsMoving::find(SEARCH_TYPE ST, GameUnit * for_whom,
                                Coord staying, Coord going_to, PRIORITY_ENDIAN PE)
{
    QList <GameUnit *> ans;
    for (int i = 0; i < rules->players.size(); ++i)
    {
        if ((ST == ENEMY && rules->players[i] != for_whom->color) ||
            (ST == ALLY && rules->players[i] == for_whom->color))
        {
            GameUnit * u = NULL;
            foreach (GameUnit * unit, players[rules->players[i]]->units)
            {
                if (unit->health > 0 && unit != for_whom &&
                     (staying == ANY || unit->position == staying) &&
                     (going_to == ANY || unit->going_to == going_to))
                {
                    if (u == NULL ||
                         (PE == LOW && unit->plan[time]->priority < u->plan[time]->priority) ||
                         (PE == HIGH && unit->plan[time]->priority > u->plan[time]->priority))
                    {
                        u = unit;
                    }
                }
            }

            if (u != NULL)
                ans << u;
        }
    }

    return ans;
}
