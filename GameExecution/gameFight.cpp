#include "Game.h"

int fb_priority(FightBrid fb)
{
    if (fb == FORTIFICATION || fb == HEX_DEFENCE)
        return 100;
    if (fb == DISTANT)
        return 10;
    if (fb == ORDER_BONUS)
        return 5;
    return 1;
}

Strike Game::getStrike(QSet <GameUnit*> fighters, const QMap<GameUnit *, FightStatus>& fs)
{
    // определяем, есть ли игрок, который пока сражается укреплениями
    PlayerColor defender = "none";
    foreach (GameUnit * u, fighters)
    {
        if (fs[u] == FS_DEFEND &&
            ((hex(u->position)->fortification > 0 && hex(u->position)->fortificationColor == u->color) ||
            (hex(u->position)->defence > 0 && hex(u->position)->color == u->color)))
        {
            defender = u->color;
            break;
        }
    }

    Strike strike;

    // сначала разберёмся со штрафами
    bool was_penalty = false;
    foreach (GameUnit * u, fighters)
    {
        // нужно проверить, что за игрока не сражаются щиты или бонусы гекса
        // в таком случае не считается, что юнит сражается сам, и штраф "не работает"
        if (defender != u->color)
        {
            // источником штрафа может являться только приказ
            // вторая проверка актуальна для Ambush-приказа
            if (u->plan[time]->fightInfluence < 0 && fs[u] != FS_DISTANT)
            {
                was_penalty = true;

                // сжигание мирных приказов
                if (!u->plan[time]->wasInFight)
                {
                    u->plan[time]->wasInFight = true;

                    if (u->plan[time]->burnsWhenFight)
                    {
                        players[u->color]->resources[u->plan[time]->type]--;
                        u->plan[time]->realizationFinished = true;
                        AddEvent()->OrderBurnsInFight(u, u->color, u->plan[time]->type);
                    }
                }

                if (fs[u] == FS_DEFEND && u->defenceBonus > 0)
                {
                    strike.fb[u] = UNIT_DEFENCE;
                    strike.amount[u] = qMin(u->defenceBonus, -u->plan[time]->fightInfluence);
                }
                else
                {
                    strike.fb[u] = UNIT_HEALTH;
                    strike.amount[u] = -u->plan[time]->fightInfluence;
                }
                strike.penalty[u] = strike.amount[u];
            }
        }
    }

    if (was_penalty)
    {
        // остальные юниты ничего не делают
        return strike;
    }

    // что делает каждый юнит
    QMap<PlayerColor, int> priority;
    foreach (GameUnit * u, fighters)
    {
        if (u->color == defender && fs[u] == FS_DEFEND &&
            hex(u->position)->fortification > 0 && hex(u->position)->fortificationColor == u->color)
        {
            // сражающиеся щитами
            strike.fb[u] = FORTIFICATION;
            strike.team_amount[u->color] = hex(u->position)->fortification;
            strike.granularity[u->color] = 1;
        }
        else if (u->color == defender && fs[u] == FS_DEFEND &&
                 hex(u->position)->defence > 0 && hex(u->position)->color == u->color)
        {
            // сражающиеся защитой на гексе
            strike.fb[u] = HEX_DEFENCE;
            strike.team_amount[u->color] = hex(u->position)->defence;
            strike.granularity[u->color] = 1;
        }
        else if (fs[u] == FS_DISTANT && u->distantAttack > 0)
        {
            // дистанционная атака
            strike.fb[u] = DISTANT;
            strike.amount[u] = u->distantAttack;
        }
        else if (fs[u] != FS_DISTANT && u->plan[time]->fightInfluence > 0)
        {
            // бонус приказа
            strike.fb[u] = ORDER_BONUS;
            strike.amount[u] = u->plan[time]->fightInfluence;
        }
        else if (fs[u] == FS_DEFEND && u->defenceBonus > 0)
        {
            // защита юнита (бегемота)
            strike.fb[u] = UNIT_DEFENCE;
            strike.amount[u] = u->defenceBonus;
        }
        else if (fs[u] != FS_DISTANT)
        {
            // собственно, юнит
            strike.fb[u] = UNIT_HEALTH;
            strike.amount[u] = u->health;
        }

        priority[u->color] = qMax(priority[u->color], fb_priority(strike.fb[u]));
    }

    // менее приоритетные действия пока не выполняем
    QMap<PlayerColor, int> amount_per_unit;
    foreach (GameUnit * u, fighters)
    {
        if (fb_priority(strike.fb[u]) < priority[u->color])
        {
            strike.fb[u] = NOTHING;
            strike.amount[u] = 0;
        }
        else
        {
            // если два юнита одной команды выполняют одинаковое действие
            // нужно, чтобы объём тоже был одинаковый
            if (amount_per_unit.count(u->color) > 0)
                amount_per_unit[u->color] = qMin(amount_per_unit[u->color], strike.amount[u]);
            else
                amount_per_unit[u->color] = strike.amount[u];
        }
    }

    // заодно считаем суммарную мощь и гранулярность
    foreach (GameUnit * u, fighters)
    {
        strike.amount[u] = qMin(amount_per_unit[u->color], strike.amount[u]);
        strike.team_amount[u->color] += strike.amount[u];
        if (strike.amount[u] > 0)
            strike.granularity[u->color] += 1;
    }

    // итого сколько в ходе этого обмена ударами будет получено урона
    int val = INF;
    foreach (PlayerColor color, strike.team_amount.keys())
    {
        val = qMin(strike.team_amount[color], val);
    }

    // завершение боя (например, дистанционного)
    if (val == 0)
    {
        strike.fight_finished = true;
        return strike;
    }

    // нормализуем, чтобы было более-менее равенство
    foreach (PlayerColor color, strike.team_amount.keys())
    {
        while (strike.team_amount[color] - strike.granularity[color] >= val)
        {
            foreach (GameUnit * u, fighters)
            {
                if (u->color == color && strike.amount[u] > 0)
                {
                    strike.amount[u]--;
                }
            }

            strike.team_amount[color] -= strike.granularity[color];
        }
    }

    return strike;
}
void Game::CountStrike(QSet<GameUnit *> fighters, Strike strike)
{
    // Реализация боя (обмена ударами)
    foreach (GameUnit * u, fighters)
    {
        // восполнение штрафов
        if (strike.penalty[u] > 0)
            u->plan[time]->fightInfluence += strike.penalty[u];

        // юзание различных бонусов
        if (strike.fb[u] == FORTIFICATION)
        {
            hex(u->position)->fortification -= strike.team_amount[u->color];
            strike.team_amount[u->color] = 0;
        }
        else if (strike.fb[u] == HEX_DEFENCE)
        {
           hex(u->position)->defence -= strike.team_amount[u->color];
           strike.team_amount[u->color] = 0;
        }
        else if (strike.fb[u] == UNIT_DEFENCE)
            u->defenceBonus -= strike.amount[u];
        else if (strike.fb[u] == ORDER_BONUS)
            u->plan[time]->fightInfluence -= strike.amount[u];
        else if (strike.fb[u] == UNIT_HEALTH)
        {
            // для анимации требуется хранить список тех, кто "убил" данного юнита
            QSet <GameUnit *> authors;
            foreach (GameUnit * u2, fighters)
                if (u2->color != u->color)
                    authors << u;

            // этот список требуется функции нанесения юниту урона
            Damage(u, strike.amount[u], authors);
        }
        else if (strike.fb[u] == DISTANT)
            u->distantAttack -= strike.amount[u];
    }
}

void Game::UnitsFight(QSet <GameUnit *> fighters)
{
    // заполняем FightStatus: защищается ли юнит, обороняется или он стреляет.
    QMap<GameUnit *, FightStatus> fs;
    foreach (GameUnit * u, fighters)
    {
        if (u->distantAttack > 0)
            fs[u] = FS_DISTANT;
        else if (u->going_to != NOWHERE)
            fs[u] = FS_ATTACK;
        else
            fs[u] = FS_DEFEND;
    }

    // пока бой не закончен явно
    QSet <PlayerColor> teams_alive;

    do
    {
        Strike strike = getStrike(fighters, fs);

        // неявное завершение боя (напр. при дистанционной атаке)
        if (strike.fight_finished)
            return;

        // составляем список гексов, в которых находятся юниты, на случай бонуса в гексе (?)
        foreach (GameUnit * u, fighters)
        {
            strike.positions[u] = hex(u->position);
        }

        // реализация итерации боя
        AddEvent()->UnitsFight(fighters, strike);
        CountStrike(fighters, strike);

        // убираем мёртвых из списка бойцов
        teams_alive.clear();
        QSet <GameUnit *> alive;
        foreach (GameUnit * u, fighters)
        {
            if (u->health > 0)
            {
                alive << u;
                teams_alive << u->color;
            }
        }
        fighters = alive;
    }
    while (teams_alive.size() > 1);
}


