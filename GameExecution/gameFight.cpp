#include "GameFight.h"

GameFight::GameFight(GameRules *rules, Random *rand) :
    GameEvents(rules, rand)
{
}

FightStatus GameFight::getFightStatus(GameUnit *unit)
{
    if (unit->going_to != NOWHERE)
        return FS_ATTACK;
    return FS_DEFEND;
}
QList <FightBrid> GameFight::getStrikeTypes(QList <GameUnit *> units, QList <FightStatus> fs, int &val)
{
    QSet <int> Penalties;
    int min_penalty = 0;
    for (int i = 0; i < units.size(); ++i)
    {
        if (fs[i] != FS_DEFEND ||
            ((hex(units[i]->position)->fortification <= 0 || hex(units[i]->position)->fortificationColor != units[i]->color) &&
            (hex(units[i]->position)->defence <= 0)))
        {
                if (units[i]->plan[time]->fightInfluence < 0 && fs[i] != FS_DISTANT)
                {
                    int k = units[i]->plan[time]->fightInfluence;

                    Penalties << i;
                    min_penalty = min_penalty == 0 ? -k : qMin(min_penalty, -k);
                }
        }
    }

    if (Penalties.size() == units.size())
        Penalties.clear();

    QList <FightBrid> ans;
    QList <int> v;
    for (int i = 0; i < units.size(); ++i)
    {
        if (min_penalty != 0 && !Penalties.contains(i))
        {
            v << min_penalty;
            ans << ENEMY_ORDER_PENALTY;
        }
        else if (fs[i] == FS_DEFEND && hex(units[i]->position)->fortification > 0 && hex(units[i]->position)->fortificationColor == units[i]->color)
        {
            v << hex(units[i]->position)->fortification;
            ans << FORTIFICATION;
        }
        else if (fs[i] == FS_DEFEND && hex(units[i]->position)->defence > 0)
        {
            v << hex(units[i]->position)->defence;
            ans << HEX_DEFENCE;
        }
        else if (fs[i] == FS_DISTANT && units[i]->distantAttack > 0)
        {
            v << units[i]->distantAttack;
            ans << DISTANT;
        }
        else if (fs[i] == FS_DISTANT)
        {
            v << 0;
            ans << STOP_FIGHT;
        }
        else if (units[i]->plan[time]->fightInfluence > 0)
        {
            v << units[i]->plan[time]->fightInfluence;
            ans << ORDER_BONUS;
        }
        else if (fs[i] == FS_DEFEND && units[i]->defenceBonus > 0)
        {
            v << units[i]->defenceBonus;
            ans << UNIT_DEFENCE;
        }
        else
        {
            v << units[i]->health;
            ans << UNIT_HEALTH;
        }
    }

    val = INF;
    foreach (int V, v)
        if (V < val)
            val = V;
    return ans;
}
void GameFight::checkIfGetToFight(GameUnit * unit, FightBrid fb)
{
    if (fb != FORTIFICATION && fb != HEX_DEFENCE)
    {
        if (!unit->plan[time]->wasInFight)
        {
            unit->plan[time]->wasInFight = true;

            if (unit->plan[time]->burnsWhenFight)
            {
                players[unit->color]->resources[unit->plan[time]->type]--;
                unit->plan[time]->realizationFinished = true;
                AddEvent()->OrderBurnsInFight(unit, unit->color, unit->plan[time]->type);
            }
        }
    }
}
void GameFight::CountStrike(QList <GameUnit *> fighters, QList <FightBrid> types, int value)
{
    int penalty = 0;
    for (int i = 0; i < types.size(); ++i)
    {
        if (types[i] == ENEMY_ORDER_PENALTY)
            ++penalty;
    }

    for (int i = 0; i < fighters.size(); ++i)
    {
        if ((types[i] != ENEMY_ORDER_PENALTY && penalty > 0) || penalty == fighters.size())
            fighters[i]->plan[time]->fightInfluence += value;

        if (types[i] == FORTIFICATION)
            hex(fighters[i]->position)->fortification -= value;
        else if (types[i] == HEX_DEFENCE)
           hex(fighters[i]->position)->defence -= value;
        else if (types[i] == UNIT_DEFENCE)
            fighters[i]->defenceBonus -= value;
        else if (types[i] == ORDER_BONUS)
            fighters[i]->plan[time]->fightInfluence -= value;
        else if (types[i] == UNIT_HEALTH)
        {
            QSet <GameUnit *> authors;
            foreach (GameUnit * u, fighters)
                if (u != fighters[i])
                    authors << u;

            Damage(fighters[i], value, authors);
        }
        else if (types[i] == DISTANT)
            fighters[i]->distantAttack -= value;
    }
}

void GameFight::UnitsFight(QList <GameUnit *> fighters, QList <FightStatus> fs)
{
    while (fs.size() < fighters.size())
        fs << getFightStatus(fighters[fs.size()]);

    while (fighters.size() > 1)
    {
        int val;
        QList <FightBrid> fb = getStrikeTypes(fighters, fs, val);

        if (fb.contains(STOP_FIGHT))
        {
            return;
        }

        QList <GameHex *> positions;
        for (int i = 0; i < fighters.size(); ++i)
        {
            checkIfGetToFight(fighters[i], fb[i]);
            positions << hex(fighters[i]->position);
        }

        AddEvent()->UnitsFight(fighters, fb, positions, val);
        CountStrike(fighters, fb, val);

        for (int i = 0; i < fighters.size(); ++i)
        {
            if (fighters[i]->health <= 0)
            {
                fighters.removeAt(i);
                fs.removeAt(i);
                --i;
            }
        }
    }
}


