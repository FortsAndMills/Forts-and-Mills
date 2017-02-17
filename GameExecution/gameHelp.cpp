#include "GameHelp.h"

GameHelp::GameHelp(GameRules *rules, Random *rand) :
    GameBase(rules, rand)
{

}

GameUnit *GameHelp::whoHasHomeAt(Coord c)
{
    foreach (GamePlayer * p, players)
        foreach (GameUnit * u, p->units)
             if (u->home == c)
                 return u;
    return NULL;
}

QSet <GameUnit *> GameHelp::alliesOnTheSameHex(GameUnit * tar)
{
    QSet <GameUnit *> ans;
    foreach (GameUnit * unit, players[tar->color]->units)
    {
        if (unit != tar && unit->position == tar->position)
        {
            ans << unit;
        }
    }
    return ans;
}

void GameHelp::deleteAllies(QList <GameUnit *> & e)
{
    for (int i = 1; i < e.size(); ++i)
    {
        int j = 0;
        while (j < i && e[j]->color != e[i]->color)
            ++j;

        if (j < i)
        {
            e.removeAt(i);
            --i;
        }
    }
}
