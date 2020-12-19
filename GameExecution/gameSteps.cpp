#include "GameSteps.h"

GameSteps::GameSteps(GameRules *rules, Random *rand) :
    GameOrderExecution(rules, rand)
{

}

// обработка выбора стартовых клеток в начале игры
void GameSteps::ProcessChosenHexes()
{
    QSet <GameHex *> ch;
    int giveupers = 0;
    foreach (GamePlayer * player, players)
    {
        if (player->GiveUp)
            ++giveupers;
        else
        {
            hex(chosenHex[player->color])->canBeChosenAsStartPoint = false;
            ch << hex(chosenHex[player->color]);
        }
    }

    // если количество различных выбранных гексов равно количеству игроков
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
                GameUnit * New = NewUnit(player, chosenUnitType[player->color], Hex->coord);
                CaptureHex(Hex, player->color);

                // даём ресурсы за юнита
                GatherResources(Hex, player->color, Hex->resources);
            }
        }

        // surround_start
//        if (!rules->waves_start && rules->surround_start)
//        {
//            // переменные для surround_start-а:
//            QMap<PlayerColor, QSet<GameHex*> > wanted;
//            QMap<GameHex*, int> wanted_by;

//            foreach (GamePlayer * player, players)
//            {
//                if (!player->GiveUp)
//                {
//                    GameHex * Hex = hex(chosenHex[player->color]);
//                    foreach (Coord adj_coord, adjacentHexes(Hex->coord))
//                    {
//                        GameHex * adj = hex(adj_coord);
//                        if (adj->color == "Neutral" && adj->canBeCaptured)
//                        {
//                            if (!wanted[player->color].contains(adj))
//                            {
//                                wanted[player->color].insert(adj);
//                                ++wanted_by[adj];
//                            }
//                        }
//                    }
//                }
//            }

//            foreach (PlayerColor color, rules->players)
//            {
//                foreach (GameHex* to_capture, wanted[color])
//                {
//                    if (wanted_by[to_capture] < 2)
//                    {
//                        CaptureHex(to_capture, color);
//                        AddEvent()->HexIsNotAHomeAnymore(to_capture, color, QSet<GameUnit*>());
//                        to_capture->canBeChosenAsStartPoint = false;
//                    }
//                }
//            }
//        }

//        if (rules->regions_start)
//        {
//            // TODO заменить на цвет
//            foreach (GamePlayer * player, players)
//            {
//                if (!player->GiveUp)
//                {
//                    Coord ChosenRegion = hex(chosenHex[player->color])->coord;
//                    foreach (QList<GameHex*> hex_row, hexes)
//                    {
//                        foreach (GameHex * Hex, hex_row)
//                        {
//                            if (Hex->region_center == ChosenRegion &&
//                                 Hex->coord != ChosenRegion)
//                            {
//                                CaptureHex(Hex, player->color);
//                                AddEvent()->HexIsNotAHomeAnymore(Hex, player->color, QSet<GameUnit*>());
//                                Hex->canBeChosenAsStartPoint = false;
//                            }
//                        }
//                    }
//                }
//            }
//        }
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
                    unit->plan[time] = new GameOrder(rules, unit->type, DefaultOrder);
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
//                        int j = acts[p].size() - 1;
//                        while (j > 0 && acts[p][j].order->priority < acts[p][j - 1].order->priority)
//                        {
//                            qSwap(acts[p][j], acts[p][j - 1]);
//                            --j;
//                        }
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
                if (rules->damageToHomellesUnits > 10)
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
        if (!rules->mill_connections || Connected(r.color, true).contains(hex(r.where)))
            NewUnit(players[r.color], r.type, r.where);
        else
        {
            GameHex * Hex = hex(r.where);
            KillRecruited(Hex, NULL);
            Hex->status = GameHex::NOT_CONNECTED;
            AddEvent()->HexStatusChanged(Hex, Hex->status, Hex->color, NULL);
        }
    }

    recruitedUnits.clear();
}
void GameSteps::killAllies()
{
//    QSet <GameUnit *> toKill;
//    foreach (GamePlayer * player, players)
//    {
//        foreach (GameUnit * unit, player->units)
//        {
//            if (unit->isDestroyingAllies)
//            {
//                QSet <GameUnit *> boat = alliesOnTheSameHex(unit);
//                foreach (GameUnit * u, boat)
//                {
//                    toKill << u;
//                }
//            }
//        }
//    }

//    foreach (GameUnit * unit, toKill)
//        DestroyUnit(unit);
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
            if (Hex->color != "Neutral" && !isAgitatedByEnemy(Hex->coord, Hex->color) && !isOccupied(Hex))
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
void GameSteps::agitationEnds()
{
    foreach (QList <GameHex *> hex_row, hexes)
    {
        foreach (GameHex * Hex, hex_row)
        {
            if (!Hex->agitated.empty())
            {
                foreach (PlayerColor color, Hex->agitated)
                    AddEvent()->AgitationEnds(Hex, color);
                Hex->agitated.clear();
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

