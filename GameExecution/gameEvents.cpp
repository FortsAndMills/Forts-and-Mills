#include "GameEvents.h"

GameEvents::GameEvents(GameRules *rules, Random *rand) :
    GameField(rules, rand)
{

}

void GameEvents::CaptureHex(GameHex *hex, PlayerColor color)
{
    if (hex->defenceBonusWhenCaptured != 0)
    {
        hex->defence = hex->defenceBonusWhenCaptured;
        AddEvent()->DefenceBonusAppears(NULL, hex, color, hex->defence);
    }

    hex->color = color;
    AddEvent()->HexCaptured(hex, color);
}
void GameEvents::KillRecruited(GameHex * hex, GameUnit * who)
{
    for (int i = 0; i < recruitedUnits.size(); ++i)
    {
        if (recruitedUnits[i].where == who->position)
        {
            recruitedUnits.removeAt(i);
            --i;
            AddEvent()->RecruitedKilled(who, hex);
        }
    }
}
void GameEvents::DecaptureHex(GameHex *hex, GameUnit *who)
{
    if (hex->defenceBonusWhenCaptured != 0)
    {
        hex->defence = 0;
        AddEvent()->DefenceBonusDisappears(who, hex);
    }

    KillRecruited(hex, who);

    if (isHexAHome(hex->coord, hex->color))
    {
        foreach (GameUnit * unit, players[hex->color]->units)
        {
            if (unit->home == hex->coord)
                AddEvent()->UnitLoosesHome(unit, hex, who);
        }
    }

    hex->color = "Neutral";
    AddEvent()->UnitDecapturesHex(who, hex);
}

void GameEvents::NewDayTimeStarted(DayTime time)
{
    QMap<GameUnit *, OrderType> today_plan;
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            today_plan[unit] = unit->plan[time]->type;
        }
    }

    AddEvent()->TimeStarted(time, today_plan);
}

GameUnit * GameEvents::NewUnit(GamePlayer * player, UnitType type, Coord where)
{
    GameUnit * New = new GameUnit(rules, type, player->color, where, basicId);
    ++basicId;
    hex(where)->recruited << player->color;

    player->units << New;
    AddEvent()->NewUnitAppear(New, hex(where));
    return New;
}
void GameEvents::DestroyUnit(GameUnit *unit)
{
    players[unit->color]->units.removeAll(unit);

    QList <Resource> burned;
    bool activeOrderBurns = false;
    for (int i = time; i < rules->dayTimes; ++i)
    {
        if (!unit->plan[i]->realizationFinished && unit->plan[i]->type != DefaultOrder)
        {
            --players[unit->color]->resources[unit->plan[i]->type];
            burned << unit->plan[i]->type;

            if (time == i)
                activeOrderBurns = true;
        }
    }

    if (hex(unit->home)->color == unit->color && !rules->no_rebirth)
        AddEvent()->HexIsNotAHomeAnymore(hex(unit->home), unit->color, unit->death_authors);

    AddEvent()->UnitDies(unit, burned, activeOrderBurns, unit->death_authors);
    africa << unit;
}
bool GameEvents::Damage(GameUnit *unit, int dam, QSet <GameUnit *> authors)
{
    unit->health -= dam;
    if (unit->health <= 0)
    {
        unit->death_authors = authors;
        return false;
    }
    return true;
}

void GameEvents::GatherResources(GameHex * hex, PlayerColor color, QList <Resource> resources)
{
    QList <bool> burn;
    foreach (Resource R, resources)
    {
        if (!rules->limitatingResources ||
            players[color]->resources[R] < resourcesLimit(color))
        {
            burn << false;
            ++players[color]->resources[R];
        }
        else
            burn << true;
    }

    AddEvent()->GatherResources(hex, color, resources, burn);
}

