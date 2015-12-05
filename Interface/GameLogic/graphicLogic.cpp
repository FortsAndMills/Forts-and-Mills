#include "GameWindow.h"

void GameWindow::startNextPhase()
{
    if (game->nextPhase == &Game::startPointsChosen)
    {
        getReadyToChooseStartPoint();
    }
    else if (game->nextPhase == &Game::getStartResources)
    {
        state = WAITING_FOR_LAST_UNITS_ON_START_POINTS;

        dayTime = -1;

        if (settings->PROGRAMM_VERSION == REAL_CLIENT)
        {
            disableWholeField();
            delightWholeField();
        }

        animations->checkIfMainAnimationFinished();
    }
    else if (game->nextPhase == &Game::planRealisation)
    {
        state = GETTING_RESOURCES;

        ArtificialPanel->startGame();

        if (settings->PROGRAMM_VERSION == REAL_CLIENT)
            DayTimeTable->appear();

        this->reconfigureResources();
        animations->checkIfMainAnimationFinished();
    }
}

void GameWindow::getReadyToChooseStartPoint()
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        state = CHOOSE_START_POINT;

        foreach (GamePlayer * player, game->players)
        {
            player->selected_x = UNDEFINED;
            player->selected_y = UNDEFINED;
        }

        foreach (Hex * hex, hexes)
        {
            if (hex->prototype->canBeChosenAsStartPoint)
            {
                if (!hex->prototype->wasChosenAsStartPoint)
                {
                    hex->light();
                    hex->forbidToSelect(false);
                }
                else
                {
                    hex->light(false);
                    hex->forbidToSelect();
                }
            }
            else
                hex->forbidToSelect();
        }

        go->enable(false);
    }
}

void GameWindow::getReadyToPlanning()
{
    state = PLANNING;

    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        go->enable();
    }
}

bool GameWindow::setNextTime()
{
    if (dayTime + 1 == game->rules->dayTimes.size())
        return false;

    if (dayTime != -1)
    {
        DayTimeTable->select(dayTime, false);

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            finishPlannedOrder(unit);
    }

    ++dayTime;

    DayTimeTable->select(dayTime, true);

    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        showPlannedOrder(unit);

    return true;
}
void GameWindow::setPreviousTime()
{
    DayTimeTable->select(dayTime, false);

    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        deshowPlannedOrder(unit);

    --dayTime;

    if (dayTime != -1)
    {
        DayTimeTable->select(dayTime, true);

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            definishPlannedOrder(unit);
    }
}
void GameWindow::setTime(DayTime time)
{
    if (time == dayTime)
        return;

    if (state == CHOOSE_ORDER_PARAMETER)
        breakChoosingOrderParameter();

    if (selectedUnit != NULL)
    {
        selectedUnit->deselect();
        selectedUnit->hidePanel();
        selectedUnit = NULL;
    }

    if (dayTime == time - 1)
        setNextTime();
    else if (dayTime == time + 1)
        setPreviousTime();
    else if (dayTime < time)
        setLateTime(time);
    else
        setEarlyTime(time);
}
void GameWindow::setEarlyTime(DayTime time)
{
    DayTimeTable->select(dayTime, false);
    while (dayTime != time)
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            deshowPlannedOrder(unit, true);

        dayTime--;
    }

    if (dayTime != -1)
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            definishPlannedOrder(unit);

        DayTimeTable->select(dayTime, true);
    }
}
void GameWindow::setLateTime(DayTime time)
{
    if (dayTime != -1)
        DayTimeTable->select(dayTime, false);

    while (dayTime != time)
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            showPlannedOrder(unit);
            finishPlannedOrder(unit);
        }

        dayTime++;
    }

    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        showPlannedOrder(unit);

    DayTimeTable->select(dayTime, true);
}


void GameWindow::getReadyToChooseOrderParameter()
{
    GameOrder * order = selectedUnit->prototype->plan[dayTime];

    QSet <QPair <int, int> > variants;

    if (order->parameterType == GameOrder::ADJACENT_HEX_WHERE_CAN_GO)
    {
        variants = game->adjacentHexes(selectedUnit->x_stack.top(), selectedUnit->y_stack.top());
    }
    else if (order->parameterType == GameOrder::VISIBLE_HEX_IN_RADIUS_2)
    {
        variants = game->visible_hexes(selectedUnit->x_stack.top(), selectedUnit->y_stack.top(), 2);
    }
    else if (order->parameterType == GameOrder::TYPE_OF_UNIT)
    {
        selectedUnit->showUnitTypePanel(game->rules->unitsInGame);
    }
    else if (order->parameterType == GameOrder::NONE)
        finishedChoosingOrderParameter();

    for (int i = 0; i < game->rules->fieldH; ++i)  // выделяем нужные гексики
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            if (variants.contains(QPair<int, int>(i, j)) && game->field[i][j]->canGoHere)
            {
                hex(i, j)->light();
                hex(i, j)->forbidToSelect(false);
            }
            else
            {
                hex(i, j)->light(false);
                hex(i, j)->forbidToSelect();
            }
        }
    }
}
void GameWindow::breakChoosingOrderParameter()
{
    state = PLANNING;

    deplanOrder(selectedUnit->prototype, dayTime);  // убираем планирующийся приказ

    disableWholeField();  // убираем следы выбора параметра
    selectedUnit->hideUnitTypePanel();
}
void GameWindow::finishedChoosingOrderParameter()
{
    disableWholeField();
    delightWholeField();
    selectedUnit->hideUnitTypePanel();

    showPlannedOrder(selectedUnit->prototype);
    if (setNextTime())
    {
        state = MOVING_CENTRAL_UNIT;
        animations->checkIfMainAnimationFinished();
    }
    else
    {
        state = PLANNING;
        selectedUnit->deselect();
        selectedUnit = NULL;
    }
}

void GameWindow::showPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
    {

    }
    else if (unit->plan[dayTime]->isMovingUnit)
    {
        DayTime time = dayTime - 1;
        while (time >= 0 && !unit->plan[time]->isMovingUnit)
            --time;
        if (time < 0) time = 0;
        while (time < dayTime)
        {
            this->anchorOrderFromUnit(units[unit], orders[unit->plan[time]]);
            ++time;
        }

        moveUnit(units[unit], unit->plan[dayTime]->parameter_x, unit->plan[dayTime]->parameter_y, Hex::ENTERING);
        createWay(units[unit]);
    }
}
void GameWindow::deshowPlannedOrder(GameUnit *unit, bool several)
{
    if (unit->plan[dayTime] == NULL)
    {

    }
    else if (unit->plan[dayTime]->isMovingUnit)
    {
        if (several)
        {
            ways[units[unit]].pop()->disappear();
        }
        else
        {
            ways[units[unit]].pop()->hide();
        }

        moveUnitBack(units[unit]);

        DayTime time = dayTime - 1;
        while (time >= 0)
        {
            this->anchorOrderToUnit(units[unit], orders[unit->plan[time]]);

            if (unit->plan[time]->isMovingUnit)
                break;

            --time;
        }
    }
}
void GameWindow::finishPlannedOrder(GameUnit * unit)
{
    if (unit->plan[dayTime] == NULL)
    {

    }
    else if (unit->plan[dayTime]->isMovingUnit)
    {
        hex(units[unit])->pointPositionState[units[unit]->points_stack.top()] = Hex::STAY;
        hex(units[unit])->recountPoints();

        reconfigureUnits();
        reconfigureWays();
        hex(units[unit])->reconfigureOrders();
    }
}
void GameWindow::definishPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
    {

    }
    else if (unit->plan[dayTime]->isMovingUnit)
    {
        hex(units[unit])->pointPositionState[units[unit]->points_stack.top()] = Hex::ENTERING;
        hex(units[unit])->recountPoints();

        reconfigureUnits();
        reconfigureWays();
        hex(units[unit])->reconfigureOrders();
    }
}

void GameWindow::deplanOrder(GameUnit * unit, DayTime time)
{
    GameOrder * order = unit->plan[time];
    if (order == NULL)
    {
        qDebug() << "ERROR: deleting... no order?";
        return;
    }

    if (time == dayTime)
        deshowPlannedOrder(unit, time);

    if (order->type != "Wait")
    {
        --spended[order->type];
        player_windows[mainPlayerColor]->turnResource(order->type, true);
    }

    units[unit]->orders_stack.removeAll(orders[order]);
    orders[order]->disappear();
    delete order;
    unit->plan[time] = NULL;
}

void GameWindow::roundEnd()
{
    this->disappearAllTurnedOffOrders();
    game->burnExtraResources();
    game->gatherResources();
    this->reconfigureResources();
}
void GameWindow::disappearAllTurnedOffOrders()
{
    foreach (PlayerColor player, game->rules->players)
        player_windows[player]->disappearAllTurnedOffOrders();
}
void GameWindow::reconfigureResources()
{
    foreach (PlayerResources * player, player_windows)
    {
        player->reconfigureResources();
    }
}

