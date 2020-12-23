#include "Game.h"

// обработка выбора стартовых клеток в начале игры
void Game::ProcessChosenHexes()
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
    }
}

void Game::RealizePlan()
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

void Game::destroyHomelessUnits()
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
void Game::recruitNewUnits()
{
    foreach (Recruited r, recruitedUnits)
    {
        if (Connected(r.color, true).contains(hex(r.where)))
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
void Game::killAllies()
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
void Game::burnExtraResources()
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
void Game::gatherResources()
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
void Game::defenceFill()
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
void Game::agitationEnds()
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
void Game::addDayTime()
{
    if (rules->changingDayTimes)
    {
        rules->dayTimes = rules->dayTimes % 5 + 1;
        AddEvent()->DayTimesChanged();
    }
}

// Подготовки к новому раунду (очистки предыдущих планов)
bool Game::ChooseOneOfTheStartHexes()
{
    if (rules->start_choices <= 0) return false;
    --rules->start_choices;

    // составляем список вариантов
    QList <Coord> variants;
    for (int x = 0; x < rules->fieldH; ++x)
        for (int y = 0; y < rules->fieldW; ++y)
            if (hexes[x][y]->canBeChosenAsStartPoint)
                variants << Coord(x, y);

    if (variants.size() < players.size())  // если выбор закончился
        return false;

    // очищаем от предыдущих ходов
    chosenHex.clear();
    ready.clear();

    // шлём запрос пользователю выбрать гекс
    AddEvent()->ChooseHex(variants);
    return true;
}
void Game::StartPlanning()
{
    // очистка от предыдущих планов
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            foreach (GameOrder * order, unit->plan)
            {
                delete order;
            }
            unit->plan.clear();
        }
    }

    ready.clear();
    AddEvent()->Plan();
}
