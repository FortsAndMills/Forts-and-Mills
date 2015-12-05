#include "Game.h"
#include "GamePlayer.h"
#include "GameRules.h"
#include "GameUnit.h"

GamePlayer::GamePlayer(Game * game, GameRules *rules, QString color) : QObject(game)
{
    this->game = game;
    this->color = color;
    this->rules = rules;
}

int GamePlayer::limit()
{
    return numOfMills * rules->orderLimitPerMill;
}

GameUnit *GamePlayer::firstFreeUnit()
{
    int i = 0;
    while (units[i]->x != UNDEFINED) { ++i; }
    return units[i];
}
GameUnit *GamePlayer::lastFreeUnit()
{
    int i = units.size() - 1;
    while (units[i]->x != UNDEFINED) { --i; }
    return units[i];
}
void GamePlayer::getStartUnits(int numOfStartResources)
{
    for (int i = 0; i < numOfStartResources; ++i)
    {
        units << new GameUnit(game,
                              rules->unitsInGame[game->rand->next() % rules->unitsInGame.size()],
                                                  color);
    }
}
int GamePlayer::numOfFreeUnits()
{
    int ans = 0;
    foreach (GameUnit * unit, units)
        if (unit->x == UNDEFINED)
            ++ans;
    return ans;
}

GameUnit * GamePlayer::newUnit(UnitType type)
{
    GameUnit * ans = new GameUnit(game, type, this->color);
    units << ans;
    return ans;
}
void GamePlayer::deleteUnit(GameUnit *unit)
{
    units.removeAll(unit);
}
