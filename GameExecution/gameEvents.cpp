#include "Game.h"

void Game::CaptureHex(GameHex *hex, PlayerColor color)
{
    if (hex->defenceBonusWhenCaptured != 0)
    {
        hex->defence = hex->defenceBonusWhenCaptured;
        AddEvent()->DefenceBonusAppears(NULL, hex, color, hex->defence);
    }

    hex->color = color;
    AddEvent()->HexCaptured(hex, color);
}
void Game::DecaptureHex(GameHex *hex, GameUnit *who)
{
    if (hex->defenceBonusWhenCaptured != 0)
    {
        hex->defence = 0;
        AddEvent()->DefenceBonusDisappears(who, hex);
    }

    KillRecruited(hex, who);

    if (hex->status == GameHex::NOT_A_HOME || hex->status == GameHex::NOT_CONNECTED)
    {
        hex->status = GameHex::TO_BE_CONQUERED;
        AddEvent()->HexStatusChanged(hex, hex->status, hex->color, who);
    }

    foreach (GameUnit * unit, players[hex->color]->units)
    {
        if (unit->home == hex->coord)
            AddEvent()->UnitLoosesHome(unit, hex, who);
    }

    hex->color = "Neutral";
    AddEvent()->UnitDecapturesHex(who, hex);
}

void Game::KillRecruited(GameHex * hex, GameUnit * who)
{
    for (int i = 0; i < recruitedUnits.size(); ++i)
    {
        if (recruitedUnits[i].where == hex->coord)
        {
            recruitedUnits.removeAt(i);
            hex->status = GameHex::NOT_CONNECTED;
            AddEvent()->RecruitedKilled(who, hex);
        }
    }
}
void Game::RecheckMillConnection(PlayerColor color, GameUnit * who)
{
    QSet<GameHex*> connected = Connected(color);

    foreach (QList <GameHex *> hex_row, hexes)
    {
        foreach (GameHex * hex, hex_row)
        {
            if (connected.contains(hex) &&
                (hex->status == GameHex::TO_BE_CONQUERED || hex->status == GameHex::NOT_CONNECTED))
            {
                // подсоединение клетки к мельнице
                hex->status = GameHex::NOT_A_HOME;
                AddEvent()->HexStatusChanged(hex, hex->status, color, who);
            }
            else if (!connected.contains(hex) && hex->color == color &&
                     (hex->status == GameHex::TO_BE_CONQUERED || hex->status == GameHex::NOT_A_HOME || hex->status == GameHex::RECRUITING))
            {
                // отсоединение клетки от мельницы
                KillRecruited(hex, who);
                hex->status = GameHex::NOT_CONNECTED;
                AddEvent()->HexStatusChanged(hex, hex->status, color, who);
            }
            else if (!connected.contains(hex) && hex->color == "Neutral" &&
                     (hex->status == GameHex::NOT_CONNECTED || hex->status == GameHex::NOT_A_HOME))
            {
                // освобождение клетки
                hex->status = GameHex::TO_BE_CONQUERED;
                AddEvent()->HexStatusChanged(hex, hex->status, color, who);
            }
        }
    }
}

void Game::NewDayTimeStarted(DayTime time)
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

GameUnit * Game::NewUnit(GamePlayer * player, UnitType type, Coord where)
{
    GameUnit * New = new GameUnit(rules, type, player->color, where, basicId);
    ++basicId;

    player->units << New;
    AddEvent()->NewUnitAppear(New, hex(where));

    // установка статуса клетки и кролики
    hex(where)->generated_units += 1;
    if (hex(where)->status != GameHex::HOME)
    {
        hex(where)->status = GameHex::HOME;
        if (New->doubles)
            NewUnit(player, type, where);
    }
    return New;
}
void Game::DestroyUnit(GameUnit *unit)
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

    hex(unit->home)->generated_units -= 1;
    if (hex(unit->home)->generated_units == 0)
    {
        hex(unit->home)->status = GameHex::TOMBSTONE;
        AddEvent()->HexStatusChanged(hex(unit->home), GameHex::TOMBSTONE, unit->color, unit, unit->death_authors);
    }

    AddEvent()->UnitDies(unit, burned, activeOrderBurns, unit->death_authors);
    africa << unit;
}
bool Game::Damage(GameUnit *unit, int dam, QSet <GameUnit *> authors)
{
    unit->health -= dam;
    if (unit->health <= 0)
    {
        unit->death_authors = authors;
        return false;
    }
    return true;
}

void Game::GatherResources(GameHex * hex, PlayerColor color, QList <Resource> resources)
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

