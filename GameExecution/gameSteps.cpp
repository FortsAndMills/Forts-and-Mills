#include "Game.h"
#include "AI/AI.h"

// обработка выбора стартовых клеток в начале игры
void Game::ProcessChosenHexes()
{
    AddEvent()->StartChoiceMade();

    QSet <GameHex *> ch;
    int giveupers = 0;
    foreach (GamePlayer * player, players)
    {
        if (player->status != GamePlayer::ALIVE)
            ++giveupers;
        else if (ch.contains(hex(chosenHex[player->color])))
        {
            // один гекс был выбран несколькими игроками; он выкидывается из вариантов
            hex(chosenHex[player->color])->canBeChosenAsStartPoint = false;            
        }
        else
        {
            ch << hex(chosenHex[player->color]);
        }
    }

    // если количество различных выбранных гексов равно количеству игроков
    if (ch.size() == players.size() - giveupers)
    {
        foreach (GamePlayer * player, players)
        {
            if (player->status == GamePlayer::ALIVE)
            {
                // удаляем гекс из вариантов
                hex(chosenHex[player->color])->canBeChosenAsStartPoint = false;

                // перекрашиваем и даём юнита
                GameHex * Hex = hex(chosenHex[player->color]);
                NewUnit(player, chosenUnitType[player->color], Hex->coord);
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
            if (players[rules->players[i]]->status == GamePlayer::ALIVE)
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
void Game::defenseFill()
{
    // восполнение оборонительного бонуса для юнитов
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            if (unit->defenseBonus != unit->max_defenseBonus)
            {
                AddEvent()->UnitDefenseFilled(unit, unit->max_defenseBonus - unit->defenseBonus);
                unit->defenseBonus = unit->max_defenseBonus;
            }
        }
    }

    // для гексов
    for (int i = 0; i < rules->fieldH; ++i)
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            if (hexes[i][j]->color != "Neutral" &&
                 hexes[i][j]->defense < hexes[i][j]->defenseBonusWhenCaptured)
            {
                AddEvent()->HexDefenseFilled(hexes[i][j], hexes[i][j]->defenseBonusWhenCaptured - hexes[i][j]->defense);
                hexes[i][j]->defense = hexes[i][j]->defenseBonusWhenCaptured;
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

// проверка на окончание игры!
bool Game::checkForLosers()
{
    QSet<PlayerColor> lost;
    int alive_players = 0;

    foreach (GamePlayer * player, players)
    {
        // у игрока есть юниты и лимит ресурсов > 0
        if (player->status == GamePlayer::ALIVE)
        {
            if (player->units.size() == 0)
            {
                player->status = GamePlayer::NO_UNITS;
                lost << player->color;
            }
            else if (resourcesLimit(player->color, false) == 0)
            {
                player->status = GamePlayer::NO_FORTS;
                lost << player->color;
            }
            else
                alive_players += 1;
        }
    }

    if (alive_players > 1)
    {
        foreach (PlayerColor color, lost)
            AddEvent()->PlayerLoses(color);
        return true;
    }

    // игра окончена
    return false;
}
void Game::win(PlayerColor winner)
{
    this->winner = winner;
    AddEvent()->Win(winner);
}

// Подготовки к новому раунду (очистки предыдущих планов)
// возвращает false если выбор закончился
bool Game::ChooseOneOfTheStartHexes()
{
    if (round >= rules->start_choices) return false;

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

    foreach (GamePlayer * player, players)
    {
        if (player->playerType == GamePlayer::AI)
        {
            AI(this).plan(player->color);
            ready[player->color] = true;
        }
    }

    AddEvent()->Plan();
}
