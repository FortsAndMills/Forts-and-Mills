#include "GameSteps.h"

GameSteps::GameSteps(GameRules *rules, Random *rand) :
    GameOrderExecution(rules, rand)
{

}

// обработка выбора мельниц в начале игры
void GameSteps::ProcessChosenHexes()
{
    QSet <GameHex *> ch;
    int giveupers = 0;
    foreach (GamePlayer * player, players)
    {
        if (player->GiveUp)
            ++giveupers;
        else if (ch.contains(hex(chosenHex[player->color])))
            hex(chosenHex[player->color])->canBeChosenAsStartPoint = false;
        else
            ch << hex(chosenHex[player->color]);
    }

    // если количество различных выбранных гексов не равно количеству игроков
    if (ch.size() == players.size() - giveupers)
    {
        foreach (GamePlayer * player, players)
        {
            if (!player->GiveUp)
            {
                // удаляем гекс из вариантов
                hex(chosenHex[player->color])->canBeChosenAsStartPoint = false;

                // перекрашиваем и даём юнита
                GameHex * Hex = hex(chosenHex[player->color]);
                GameUnit * New = NewUnit(player, Hex->livingNation, Hex->coord);
                CaptureHex(Hex, player->color);
                GatherResources(Hex, player->color, New->startResources);
            }
        }
    }
}

void GameSteps::RealizePlan()
{
    // заполняем пропуски приказами безделья
    for (time = 0; time < rules->dayTimes; ++time)
    {
        foreach (GamePlayer * player, players)
        {
            foreach (GameUnit * unit, player->units)
            {
                if (unit->plan[time] == NULL)
                    unit->plan[time] = new GameOrder(rules, unit->type, DefaultOrder, unit->id);
            }
        }
    }

    // для каждого времени дня
    for (time = 0; time < rules->dayTimes; ++time)
    {
        NewDayTimeStarted(time);

        // разбиваем действия по приоритетам; называем один пак таких действий актом
        typedef QList < Action > act;
        QMap <int, act> acts;
        for (int i = 0; i < rules->players.size(); ++i)
        {
            if (!players[rules->players[i]]->GiveUp)
            {
                foreach (GameUnit * unit, players[rules->players[i]]->units)
                {
                    for (int k = 0; k < unit->plan[time]->actions.size(); ++k)
                    {
                        int p = unit->plan[time]->actions[k].priority;

                        acts[p] << Action(unit, time, k);

                        // в рамках одного акта сортируем по приоритету приказа
                        int j = acts[p].size() - 1;
                        while (j > 0 && acts[p][j].order->priority < acts[p][j - 1].order->priority)
                        {
                            qSwap(acts[p][j], acts[p][j - 1]);
                            --j;
                        }
                    }
                }
            }
        }

        // разыгрываем каждый акт
        foreach (act actions, acts)
        {
            for (int i = 0; i < actions.size(); ++i)
            {
                // проверка на то, что приказ валиден
                if (CheckIfActionBurns(actions[i]))
                {
                    actions.removeAt(i);
                    --i;
                }
            }

            // реализация действия
            Realize(actions);

            // убиваем мёртвых юнитов
            foreach (GamePlayer * p, players)
            {
                QList <GameUnit *> units = p->units;
                foreach (GameUnit * u, units)
                    if (u->health <= 0)
                        DestroyUnit(u);
            }
        }

        AddEvent()->TimeFinished(time);
        killAllies();  // проверка на Брутера
    }
    AddEvent()->PlanRealizationFinished();
}

void GameSteps::destroyHomelessUnits()
{
    // разборки с бездомными юнитами в конце раунда
    foreach (GamePlayer * player, players)
    {
        QList <GameUnit *> units = player->units;
        foreach (GameUnit * unit, units)
        {
            if (hex(unit->home)->color != player->color)
            {
                if (rules->damageToHomellesUnits > 6)
                    DestroyUnit(unit);
                else
                {
                    int amount = qMin(rules->damageToHomellesUnits, unit->health);
                    AddEvent()->HomelessUnitDamaged(unit, amount);
                    Damage(unit, amount);
                }
            }
        }
    }
}
void GameSteps::recruitNewUnits()
{
    foreach (Recruited r, recruitedUnits)
    {
        NewUnit(players[r.color], r.type, r.where);
    }

    recruitedUnits.clear();
}
void GameSteps::killAllies()
{
    QSet <GameUnit *> toKill;
    foreach (GamePlayer * player, players)
    {
        for (int j = 0; j < player->units.size(); ++j)
        {
            if (player->units[j]->isDestroyingAllies)
            {
                QSet <GameUnit *> boat = alliesOnTheSameHex(player->units[j]);
                foreach (GameUnit * u, boat)
                {
                    //u->death_author = player->units[j];
                    toKill << u;
                }
            }
        }
    }

    foreach (GameUnit * unit, toKill)
        DestroyUnit(unit);
}
void GameSteps::burnExtraResources()
{
    if (!rules->limitatingResources)
        return;

    foreach (GamePlayer * player, players)
    {
        foreach (Resource R, rules->ordersInGame)
        {
            while (player->resources[R] > resourcesLimit(player->color))
            {
                --player->resources[R];
                AddEvent()->BurnResource(player->color, R);
            }
        }
    }
}
void GameSteps::gatherResources()
{
    foreach (QList <GameHex *> hex_row, hexes)
    {
        foreach (GameHex * Hex, hex_row)
        {
            if (Hex->color != "Neutral")
            {
                GatherResources(Hex, Hex->color, Hex->resources);
            }
        }
    }
}
void GameSteps::defenceFill()
{
    // восполнение оборонительного бонуса для юнитов
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            if (unit->defenceBonus != unit->max_defenceBonus)
            {
                AddEvent()->UnitDefenceFilled(unit, unit->max_defenceBonus - unit->defenceBonus);
                unit->defenceBonus = unit->max_defenceBonus;
            }
        }
    }

    // для гексов
    for (int i = 0; i < rules->fieldH; ++i)
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            if (hexes[i][j]->color != "Neutral" &&
                 hexes[i][j]->defence < hexes[i][j]->defenceBonusWhenCaptured)
            {
                AddEvent()->HexDefenceFilled(hexes[i][j], hexes[i][j]->defenceBonusWhenCaptured - hexes[i][j]->defence);
                hexes[i][j]->defence = hexes[i][j]->defenceBonusWhenCaptured;
            }
        }
    }
}
void GameSteps::addDayTime()
{
    if (rules->changingDayTimes)
    {
        rules->dayTimes = rules->dayTimes % 5 + 1;
        AddEvent()->DayTimesChanged();
    }
}
