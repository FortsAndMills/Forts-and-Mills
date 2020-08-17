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
bool GameHelp::isOccupied(GameHex *hex)
{
    foreach (GamePlayer * p, players)
        if (p->color != hex->color)
            foreach (GameUnit * u, p->units)
                 if (u->position == hex->coord)
                     return true;
    return false;
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

bool GameHelp::isAgitatedByEnemy(Coord which, QString me)
{
    foreach (PlayerColor color, hex(which)->agitated)
        if (color != me)
            return true;
    return false;
}

GameUnit *GameHelp::getUnitById(qint16 id)
{
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            if (unit->id == id)
                return unit;
        }
    }

    qDebug() << "ERROR: unit not found by id" << endl;
    return NULL;
}
