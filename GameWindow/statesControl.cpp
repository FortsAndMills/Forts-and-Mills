#include "StatesControl.h"

StatesControl::StatesControl(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    Events(game, PlayerIndex, parent)
{

}

void StatesControl::getReadyToChooseHex(QList <Coord> variants)
{
    if (!game->players[mainPlayerColor]->GiveUp)
    {
        state = CHOOSING_HEX;

        foreach (Hex * hex, hexes)
        {
            if (variants.contains(hex->prototype->coord))
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
    }
    else
    {
        state = WAIT_FOR_ENEMY_START_POINT;
    }
}
void StatesControl::getReadyToPlanning()
{
    if (!game->players[mainPlayerColor]->GiveUp)
    {
        state = PLANNING;

        for (int i = 0; i < priorities.size(); ++i)
            priorities[i] = 0;

        go->enable();
        go->AnimationStart(OPACITY, 1, constants->goButtonAppearTime);
        next->AnimationStart(OPACITY, 0, constants->goButtonAppearTime);
        next->enable(false);
    }
    else
    {
        state = WAIT_FOR_ENEMY_PLAN;
    }

    delightWholeField();
    disableWholeField();

    if (!DayTimeTable->isVisible())
        DayTimeTable->appear();

    foreach (Hex * hex, hexes)
        hex->hideLivingNation();

    DayTimeTable->deselectAll();
    dayTime = -1;
}
void StatesControl::getReadyToRealization()
{
    state = REALIZATION_PHASE;

    setTime(-1);

    orders.remove(NULL);
    foreach (Order * order, orders)
        order->disappear();

    orders.clear();
    foreach (Unit * unit, units)
        unit->orders_stack.clear();

    foreach (GameUnit * u, game->players[mainPlayerColor]->units)
        for (DayTime t = 0; t < game->rules->dayTimes; ++t)
            if (u->plan[t] != NULL && u->plan[t]->type != DefaultOrder)
                player_windows[mainPlayerColor]->turnResource(u->plan[t]->type, true);

    go->AnimationStart(OPACITY, 0, constants->goButtonAppearTime);
    next->AnimationStart(OPACITY, 1, constants->goButtonAppearTime);
    next->enable(false);
}

bool StatesControl::setNextTime()
{
    if (dayTime + 1 == game->rules->dayTimes)
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
void StatesControl::setPreviousTime()
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
void StatesControl::setTime(DayTime time)
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
void StatesControl::setEarlyTime(DayTime time)
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
void StatesControl::setLateTime(DayTime time)
{
    if (dayTime != -1)
    {
        DayTimeTable->select(dayTime, false);

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            finishPlannedOrder(unit);
        }
    }

    ++dayTime;
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

void StatesControl::getReadyToChooseOrderParameter()
{
    GameOrder * order = selectedUnit->prototype->plan[dayTime];

    QList <Coord> variants;

    GameAction::GameActionParameterType PT = order->nextParameterType();
    if (PT == GameAction::ADJACENT_HEX_WHERE_CAN_GO)
    {
        variants = game->adjacentHexes(selectedUnit->stack.last().position);
    }
    else if (PT == GameAction::VISIBLE_HEX_IN_RADIUS_2)
    {
        variants = game->visible_hexes(selectedUnit->stack.last().position, 2);
    }
    else if (PT == GameAction::TYPE_OF_UNIT)
    {
        selectedUnit->showUnitTypePanel(game->rules->unitsInGame);
    }
    else if (PT == GameAction::NONE)
        finishedChoosingOrderParameter();

    foreach (Coord c, variants)
    {
        if (game->hex(c)->canGoHere)
        {
            hex(c)->light();
            hex(c)->forbidToSelect(false);
        }
        else
        {
            hex(c)->light(false);
            hex(c)->forbidToSelect();
        }
    }
}
void StatesControl::breakChoosingOrderParameter()
{
    deplanOrder(selectedUnit->prototype, dayTime);  // убираем планирующийся приказ

    state = PLANNING;

    delightWholeField();
    disableWholeField();  // убираем следы выбора параметра
    selectedUnit->hideUnitTypePanel();
}
void StatesControl::finishedChoosingOrderParameter()
{
    delightWholeField();
    disableWholeField();
    selectedUnit->hideUnitTypePanel();

    selectedUnit->prototype->plan[dayTime]->findNextActionWithNoParameter();

    if (selectedUnit->prototype->plan[dayTime]->nextParameterType() != GameAction::NONE)
    {
        getReadyToChooseOrderParameter();
    }
    else
    {
        showPlannedOrder(selectedUnit->prototype);
        if (setNextTime())
        {
            selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype));
        }
        else
        {
            state = PLANNING;
            selectedUnit->deselect();
            selectedUnit = NULL;
        }
    }
}

void StatesControl::showPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            DayTime time = dayTime - 1;
            while (time > 0 && orders[unit->plan[time - 1]]->anchor == units[unit]->where())
                --time;
            while (time < dayTime)
            {
                this->anchorOrderFromUnit(units[unit], orders[unit->plan[time]]);
                ++time;
            }

            moveUnit(units[unit], a.target, Hex::ENTERING);
            createWay(units[unit]);

            orders[unit->plan[dayTime]]->anchor = units[unit]->where();
        }
        else if (a.type == GameAction::SHOOT)
        {
            rockets[units[unit]] << new Rocket(this, mainPlayerColor, false, Rocket::ROCKET, a.target);
            rockets[units[unit]].last()->anchorTo(hex(units[unit]));
            resizeRockets();
            rockets[units[unit]].last()->setStartPosition();
            rockets[units[unit]].last()->animate();
        }
        else if (a.type == GameAction::CAPTURE_HEX)
        {
            hex(units[unit])->planCapturing(mainPlayerColor);
        }
        else if (a.type == GameAction::RECRUIT)
        {
            hex(units[unit])->showInformation(a.unitType, "UnitIsGoingToRecruit");
        }
    }
}
void StatesControl::deshowPlannedOrder(GameUnit *unit, bool several)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            if (several)
            {
                ways_to[units[unit]].pop()->disappear();
                ways_from[units[unit]].pop()->disappear();
            }
            else
            {
                ways_to[units[unit]].pop()->hide();
                ways_from[units[unit]].pop()->hide();
            }

            moveUnitBack(units[unit]);

            DayTime time = dayTime - 1;
            while (time >= 0 && orders[unit->plan[time]]->anchor == units[unit]->where())
            {
                this->anchorOrderToUnit(units[unit], orders[unit->plan[time]]);
                --time;
            }
        }
        else if (a.type == GameAction::SHOOT)
        {
            foreach (Rocket * r, rockets[units[unit]])
                r->disappear();
            rockets[units[unit]].clear();
        }
        else if (a.type == GameAction::CAPTURE_HEX)
        {
            hex(units[unit])->deplanCapturing(mainPlayerColor);
        }
        else if (a.type == GameAction::RECRUIT)
        {
            hex(units[unit])->hideInformation();

            if (hex(units[unit])->prototype->color == mainPlayerColor &&
                 !game->isHexAHome(units[unit]->where(), mainPlayerColor))
            {
                hex(units[unit])->showInformation("Not" + mainPlayerColor + "UnitHome", "NotUnitHome");
            }
        }
    }
}
void StatesControl::finishPlannedOrder(GameUnit * unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            hex(units[unit])->pointPositionState[units[unit]->point()] = Hex::STAY;
            hex(units[unit])->recountPoints();

            hexPointsChanged();
        }
        else if (a.type == GameAction::SHOOT)
        {
            foreach (Rocket * r, rockets[units[unit]])
                r->disappear();
            rockets[units[unit]].clear();
        }
    }
}
void StatesControl::definishPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            hex(units[unit])->pointPositionState[units[unit]->stack.last().point] = Hex::ENTERING;
            hex(units[unit])->recountPoints();
            hex(units[unit])->reconfigureOrders();

            hexPointsChanged();
        }
        else if (a.type == GameAction::SHOOT)
        {
            rockets[units[unit]] << new Rocket(this, mainPlayerColor, false, Rocket::ROCKET, a.target);
            rockets[units[unit]].last()->anchorTo(hex(units[unit]));
            resizeRockets();
            rockets[units[unit]].last()->setStartPosition();
            rockets[units[unit]].last()->animate();
        }
    }
}

void StatesControl::deplanOrder(GameUnit * unit, DayTime time)
{
    GameOrder * order = unit->plan[time];
    if (order == NULL)
    {
        return;
    }

    if (time == dayTime && state != CHOOSE_ORDER_PARAMETER)
        deshowPlannedOrder(unit, time);

    if (order->type != DefaultOrder)
    {
        player_windows[mainPlayerColor]->turnResource(order->type, true);
    }

    units[unit]->orders_stack.removeAll(orders[order]);

    orders[order]->disappear();
    orders.remove(order);

    delete order;
    unit->plan[time] = NULL;
}





