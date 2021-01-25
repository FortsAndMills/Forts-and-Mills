#include "GameWindow.h"

void GameWindow::getReadyToChooseHex(QList <Coord> variants)
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
void GameWindow::getReadyToPlanning()
{
    if (game->players[mainPlayerColor]->GiveUp)
        state = WAIT_FOR_ENEMY_PLAN; // мы сдались, но планы врагов ещё не пришли
    else
    {
        state = PLANNING;  // мы не сдались и у нас ещё есть время

        go->enable();
        go->AnimationStart(OPACITY, 1, constants->goButtonAppearTime);
    }

    next->AnimationStart(OPACITY, 0, constants->goButtonAppearTime);
    next->enable(false);

    delightWholeField();
    disableWholeField();

    if (!dayTimeTable->isVisible())
        dayTimeTable->appear();

    startUnitsChoice->AnimationStart(OPACITY, 0);
    startChoiceProgress->AnimationStart(OPACITY, 0);

    dayTimeTable->deselectAll();
    dayTime = -1;
}
void GameWindow::getReadyToRealization()
{
    state = REALIZATION_PHASE;

    setTime(-1);

    // удаление приказов возле юнитов
    orders.remove(NULL);
    foreach (Order * order, orders)
        order->disappear();

    orders.clear();
    foreach (Unit * unit, units)
        unit->orders_stack.clear();

    // включение обратно "использованных" в фазе планирования приказов
    foreach (GameUnit * u, game->players[mainPlayerColor]->units)
        for (DayTime t = 0; t < game->rules->dayTimes; ++t)
            if (u->plan[t] != NULL && u->plan[t]->type != DefaultOrder)
                player_windows[mainPlayerColor]->turnResource(u->plan[t]->type, true);

    go->AnimationStart(OPACITY, 0, constants->goButtonAppearTime);
    next->AnimationStart(OPACITY, 1, constants->goButtonAppearTime);
    next->enable(false);
}

bool GameWindow::setNextTime()
{
    // проверка на корректность запроса
    if (dayTime + 1 == game->rules->dayTimes)
        return false;

    // выключение выделения текущего времени дня
    if (dayTime != -1)
    {
        dayTimeTable->select(dayTime, false);

        // "завершение" того, что отображается в промежуточном состоянии
        // например, если юнит идёёёёт, то он должен прийти
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            finishPlannedOrder(unit);
    }

    ++dayTime;

    // выделение нового текущего времени дня
    dayTimeTable->select(dayTime, true);

    // отображение приказов в это время дня
    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        showPlannedOrder(unit);

    return true;
}
void GameWindow::setPreviousTime()
{
    dayTimeTable->select(dayTime, false);

    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        deshowPlannedOrder(unit);

    --dayTime;

    if (dayTime != -1)
    {
        dayTimeTable->select(dayTime, true);

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
    dayTimeTable->select(dayTime, false);
    while (dayTime != time)  // откатываем приказы полностью
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            deshowPlannedOrder(unit, true);

        dayTime--;
    }

    if (dayTime != -1)  // откатываем только до промежуточного состояния
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
            definishPlannedOrder(unit);

        dayTimeTable->select(dayTime, true);
    }
}
void GameWindow::setLateTime(DayTime time)
{
    if (dayTime != -1)  // завершаем текущие планы из "промежуточных" состояний
    {
        dayTimeTable->select(dayTime, false);

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            finishPlannedOrder(unit);
        }
    }

    ++dayTime;
    while (dayTime != time)  // проходимся по планам полностью
    {
        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            showPlannedOrder(unit);
            finishPlannedOrder(unit);
        }

        dayTime++;
    }

    // показываем текущие планы
    foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        showPlannedOrder(unit);

    dayTimeTable->select(dayTime, true);
}

void GameWindow::getReadyToChooseOrderParameter()
{
    GameOrder * order = selectedUnit->prototype->plan[dayTime];

    QList <Coord> variants;

    // получаем, какой параметр нужно выбрать
    GameAction::GameActionParameter PT = order->nextParameterType();
    if (PT == GameAction::ADJACENT_HEX_WHERE_CAN_GO)
    {
        variants = game->adjacentHexes(selectedUnit->stack.last().position);
    }
    else if (PT == GameAction::VISIBLE_HEX_IN_RADIUS_2)
    {
        variants = game->visible_hexes(selectedUnit->stack.last().position, 2);
    }
    else if (PT == GameAction::ENEMY_UNIT)
    {
        isSelectingEnemyUnit = true;
        foreach (Hex* hex, hexes)
        {
            hex->highlight("");
        }
    }
    else if (PT == GameAction::TYPE_OF_UNIT)
    {
        selectedUnit->showUnitTypePanel(game->rules->unitsInGame);
    }
    else if (PT == GameAction::NONE)
        finishedChoosingOrderParameter();

    // делаем доступными для выбора указанные гексы
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
void GameWindow::breakChoosingOrderParameter()
{
    deplanOrder(selectedUnit->prototype, dayTime);  // убираем планирующийся приказ

    state = PLANNING;

    // TODO этот код дважды повторяется
    delightWholeField();  // убираем следы выбора параметра
    disableWholeField();
    selectedUnit->hideUnitTypePanel();
    isSelectingEnemyUnit = false;
    foreach (Hex* hex, hexes)
    {
        hex->highlight("", false);
    }
}
void GameWindow::finishedChoosingOrderParameter()
{
    delightWholeField();  // убираем следы выбора параметров приказа
    disableWholeField();
    selectedUnit->hideUnitTypePanel();
    isSelectingEnemyUnit = false;
    foreach (Hex* hex, hexes)
    {
        hex->highlight("", false);
    }

    // ищем следующий параметр, который нужно выбрать
    selectedUnit->prototype->plan[dayTime]->findNextActionWithNoParameter();

    if (selectedUnit->prototype->plan[dayTime]->nextParameterType() != GameAction::NONE)
    {
        getReadyToChooseOrderParameter();
    }
    else  // если такого нет, то отображаем новоявленный приказ и переходим к след. времени
    {
        showPlannedOrder(selectedUnit->prototype);

        // првоерка пересечения реки
        bool must_be_last = game->must_be_last(selectedUnit->prototype, dayTime);

        if (must_be_last) riverCrossedInPlan(); // нужно для обучения

        if (!must_be_last && setNextTime())  // если это было не последнее время, сразу же суём панельку
        {
            selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype));
        }
        else  // иначе можно девыделить юнита
        {
            state = PLANNING;
            selectedUnit->deselect();
            selectedUnit = NULL;
        }
    }
}

// ОТОБРАЖЕНИЕ ПЛАНИРУЮЩИХСЯ ПРИКАЗОВ
// В текущее время дня приказы отображаются как "выполняющиеся", т.е. в процессе
// show отображает как раз такое промежуточное, текущее состояние
// finish завершает приказ, т.е. переводит из промежуточного в конечное
// плюс у обеих операций должны быть обратные для отката
void GameWindow::showPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            // ищем те приказы, которые совершаются на покидаемом гексе
            DayTime time = dayTime - 1;
            while (time > 0 && orders[unit->plan[time - 1]]->anchor == units[unit]->where())
                --time;
            while (time < dayTime)  // эти приказы перевязываются к гексу
            {
                this->anchorOrderFromUnit(units[unit], orders[unit->plan[time]]);
                ++time;
            }

            // перемещаем юнита и создаём стрелку
            moveUnit(units[unit], a.target, Hex::ENTERING);
            createWay(units[unit]);

            // помечаем, что приказ при этом привязан к новой позиции юнита
            // и, если что, он будет оставлен на целевом гексе
            orders[unit->plan[dayTime]]->anchor = units[unit]->where();
        }
        else if (a.type == GameAction::SHOOT)
        {
            // отображаем анимацию стрельбы в цикле
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
            hex(units[unit])->hideInformation();
            hex(units[unit])->showInformation(a.unitType, "UnitIsGoingToRecruit");
        }
    }
}
void GameWindow::deshowPlannedOrder(GameUnit *unit, bool several)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            if (several)  // если нужно убрать сразу много стрелок, то они просто исчезают
            {
                ways_to[units[unit]].pop()->disappear();
                ways_from[units[unit]].pop()->disappear();
            }
            else  // если только одну, то красиво сжимается вместе с "возвращением" юнита
            {
                ways_to[units[unit]].pop()->hide();
                ways_from[units[unit]].pop()->hide();
            }

            // само возвращение юнита!
            moveUnitBack(units[unit]);

            // возвращаем ему приказы с гекса, на который вернулся
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

            // проверка на то, что в этом гексе должна быть инфа об отсутствии дома
            if (hex(units[unit])->prototype->color == mainPlayerColor &&
                hex(units[unit])->prototype->status == GameHex::NOT_A_HOME)
            {
                hex(units[unit])->showInformation("Not" + mainPlayerColor + "UnitHome", "NotUnitHome");
            }
        }
    }
}
void GameWindow::finishPlannedOrder(GameUnit * unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            // завершаем перемещение, изменив статус последней точки
            hex(units[unit])->pointPositionState[units[unit]->point()] = Hex::STAY;
            hex(units[unit])->recountPoints();

            hexPointsChanged();
        }
        else if (a.type == GameAction::SHOOT)
        {
            // ракеты стреляются только в "текущем" состоянии
            foreach (Rocket * r, rockets[units[unit]])
                r->disappear();
            rockets[units[unit]].clear();
        }
    }
}
void GameWindow::definishPlannedOrder(GameUnit *unit)
{
    if (unit->plan[dayTime] == NULL)
        return;

    foreach (GameAction a, unit->plan[dayTime]->actions)
    {
        if (a.type == GameAction::LEAVE_HEX)
        {
            // возвращаем последнюю точку в состояние "входит"
            hex(units[unit])->pointPositionState[units[unit]->stack.last().point] = Hex::ENTERING;
            hex(units[unit])->recountPoints();
            hex(units[unit])->reconfigureOrders();

            hexPointsChanged();
        }
        else if (a.type == GameAction::SHOOT)
        {
            // возвращаем ракеты
            rockets[units[unit]] << new Rocket(this, mainPlayerColor, false, Rocket::ROCKET, a.target);
            rockets[units[unit]].last()->anchorTo(hex(units[unit]));
            resizeRockets();
            rockets[units[unit]].last()->setStartPosition();
            rockets[units[unit]].last()->animate();
        }
    }
}

void GameWindow::deplanOrder(GameUnit * unit, DayTime time)
{
    GameOrder * order = unit->plan[time];
    if (order == NULL)
    {
        return;
    }

    // перестаём отображать приказ, требующий удаления
    if (time == dayTime && state != CHOOSE_ORDER_PARAMETER)
        deshowPlannedOrder(unit, time);

    // включаем обратно ресурс
    if (order->type != DefaultOrder)
    {
        player_windows[mainPlayerColor]->turnResource(order->type, true);
    }

    // удаляем графическое представление приказа из списка приказов юнита
    units[unit]->orders_stack.removeAll(orders[order]);
    // и удаляем его!
    orders[order]->disappear();
    orders.remove(order);

    // удаляем также и игровое представление приказа
    delete order;
    unit->plan[time] = NULL;
}


