#include "GameWindow.h"

void GameWindow::mainAnimationFinished()
{
    if (state == WAITING_FOR_LAST_UNITS_ON_START_POINTS)
    {
        (game->*(game->nextPhase))();
        startNextPhase();
    }
    else if (state == GETTING_RESOURCES)
    {
        getReadyToPlanning();  // FAIL тотальное нарушение логики
    }
    else if (state == MOVING_CENTRAL_UNIT)
    {
        state = PLANNING;
        selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype, spended));
    }
}

void GameWindow::hexClicked(int i, int j)
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        if (state == CHOOSE_START_POINT)
        {
            if (game->field[i][j]->canBeChosenAsStartPoint)
            {
                if (game->players[mainPlayerColor]->selected_x == i &&
                     game->players[mainPlayerColor]->selected_y == j)
                {
                    game->players[mainPlayerColor]->selected_x = UNDEFINED;
                    game->players[mainPlayerColor]->selected_y = UNDEFINED;

                    hex(i, j)->select(false);
                    go->setEnabled(false);
                }
                else
                {
                    if (game->players[mainPlayerColor]->selected_x != UNDEFINED)
                        hex(game->players[mainPlayerColor]->selected_x, game->players[mainPlayerColor]->selected_y)->select(false);

                    game->players[mainPlayerColor]->selected_x = i;
                    game->players[mainPlayerColor]->selected_y = j;

                    hex(i, j)->select();
                    go->enable();
                }
            }
            else
                qDebug() << "ERROR: choosen not-mill hex when must choose mills!";
        }
        else if (state == CHOOSE_ORDER_PARAMETER)
        {
            selectedUnit->prototype->plan[dayTime]->parameter_x = i;
            selectedUnit->prototype->plan[dayTime]->parameter_y = j;

            finishedChoosingOrderParameter();
        }
    }
    else if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (ToBlue)
        {
            game->captureHex(i, j, "Blue");
        }
        else if (ToRed)
        {
            game->captureHex(i, j, "Red");
        }
        else if (ToNeutral)
        {
            game->decaptureHex(i, j);
        }
    }
}

void GameWindow::orderPicPushed(Resource type, PlayerColor owner, bool state)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (state)
        {
            --game->players[owner]->resources[type];
            player_windows[owner]->turnResource(type, false);
        }
        else
        {
            ++game->players[owner]->resources[type];
            player_windows[owner]->turnResource(type, true);
        }

        this->createFreeResource(new GameOrder(game, type));
    }
}
void GameWindow::OrderRightClicked(GameOrder * order)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        orders[order]->Delete();
        orders.remove(order);
    }
}
void GameWindow::orderVariantClicked(OrderType type)
{
    if (selectedUnit == NULL)
        qDebug() << "FatalError: no selected unit, but there is order variant!!!";

    state = CHOOSE_ORDER_PARAMETER;

    if (selectedUnit->prototype->plan[dayTime] != NULL)
    {
        qDebug() << "ERROR: order on order!!!";
        delete selectedUnit->prototype->plan[dayTime];
    }

    GameOrder * order = new GameOrder(game, type);
    selectedUnit->prototype->plan[dayTime] = order;
    orders[order] = new Order(this, order);
    orders[order]->anchorTo(selectedUnit);

    selectedUnit->locateOrderLikeOnPanel(orders[order]);
    selectedUnit->reconfigureOrders();
    selectedUnit->hidePanel();

    if (type != "Wait")
    {
        ++spended[type];
        player_windows[mainPlayerColor]->turnResource(type, false);
    }

    getReadyToChooseOrderParameter();
}

void GameWindow::unitTypeChosen(QString type)
{
    selectedUnit->prototype->plan[dayTime]->parameter_unit_type = type;
    finishedChoosingOrderParameter();
}

void GameWindow::unitHoverEntered(GameUnit *unit)
{
    if (unit->home_x != UNDEFINED)
        hex(unit->home_x, unit->home_y)->showUnitHome(unit->color);

    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
        units[unit]->select();
}
void GameWindow::unitHoverLeft(GameUnit *unit)
{
    if (unit->home_x != UNDEFINED)
        hex(unit->home_x, unit->home_y)->hideUnitHome();

    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
        units[unit]->deselect();
}
void GameWindow::unitRightClicked(GameUnit *unit)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (unit->home_x != UNDEFINED)
        {
            hex(unit->home_x, unit->home_y)->hideUnitHome();
            unit->home_x = UNDEFINED;
            unit->home_y = UNDEFINED;
        }
        game->destroyUnit(unit);
    }
}
void GameWindow::unitLeftClicked(GameUnit * unit)
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        if (state == PLANNING || state == CHOOSE_ORDER_PARAMETER)
        {
            if (this->mainPlayerColor == unit->color)
            {
                if (state == CHOOSE_ORDER_PARAMETER)
                    breakChoosingOrderParameter();

                if (selectedUnit == units[unit])
                {
                    if (state == CHOOSE_ORDER_PARAMETER)
                    {
                        selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype, spended));
                    }
                    else
                    {
                        units[unit]->deselect();
                        units[unit]->hidePanel();
                        selectedUnit = NULL;
                    }
                }
                else
                {
                    if (dayTime == -1)  // если был режим созерцания, переходим к завтраку
                        setNextTime();

                    if (selectedUnit != NULL)  // если был выбран другой юнит... ну вы поняли
                    {
                        selectedUnit->deselect();
                        selectedUnit->hidePanel();
                    }

                    DayTime time = dayTime;  // ищем для него подходящее время
                    while (time != -1 && units[unit]->prototype->plan[time] == NULL)
                        time--;
                    if (time != game->rules->dayTimes.size() - 1 && time != dayTime)
                        setTime(time + 1);

                    selectedUnit = units[unit];  // новый выбранный юнит
                    selectedUnit->select();

                    time = dayTime;  // убираем все планы на сейчас и далее
                    while (time != game->rules->dayTimes.size())
                    {
                        deplanOrder(selectedUnit->prototype, time);
                        time++;
                    }

                    selectedUnit->showOrdersPanel(game->whatCanUse(unit, spended));
                }
            }
        }
    }
}

void GameWindow::dayTimeClicked(DayTime time)
{
    if (time != dayTime)
        setTime(time);
    else
        setTime(-1);
}

#define MILLS_COORDINATES_MESSAGE (qint8)1
void GameWindow::GoButtonPushed(bool)
{
    if (state == CHOOSE_START_POINT)
    {
        this->disableWholeField();
        state = WAIT_FOR_ENEMY_START_POINT;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << MILLS_COORDINATES_MESSAGE
                  << PlayerIndex
                  << (qint32)game->players[mainPlayerColor]->selected_x
                  << (qint32)game->players[mainPlayerColor]->selected_y;
        emit writeToOpponent(message);

        if (DoesEveryoneChoosedStartPoint())
        {
            game->startPointsChosen();
            startNextPhase();
        }
    }
}
void GameWindow::readFromOpponent(QByteArray message)
{
    QDataStream in(&message, QIODevice::ReadOnly);
    qint8 Command;
    in >> Command;

    if (Command == MILLS_COORDINATES_MESSAGE)
    {
        quint8 sender_index;
        in >> sender_index;
        PlayerColor sender = game->rules->players[sender_index];

        in >> game->players[sender]->selected_x >> game->players[sender]->selected_y;

        if (DoesEveryoneChoosedStartPoint())
        {
            game->startPointsChosen();
            startNextPhase();
        }
    }
}


// ДЛЯ ИСКУССТВЕННОГО КОНТРОЛЯ PP-ВЕРСИИ
void GameWindow::unitWheeled(GameUnit * unit, int delta)
{
    if (delta > 0 && unit->health != unit->max_health)
    {
        ++unit->health;
        units[unit]->healthChanged();
    }
    else if (delta < 0 && unit->health > 0)
    {
        --unit->health;
        units[unit]->healthChanged();
    }
}
void GameWindow::unitDragStarted(GameUnit *unit)
{
    WasUnitHomeless = (unit->home_x == UNDEFINED);
    if (unit->x != UNDEFINED)
    {
        this->unitGetOutOfField(unit, unit->x, unit->y);
        unit->x = UNDEFINED;
        unit->y = UNDEFINED;
    }
}
void GameWindow::unitDragFinished(GameUnit *unit, GameHex *hex)
{
    if (WasUnitHomeless && hex != NULL)
    {
        unit->home_x = hex->x;
        unit->home_y = hex->y;
    }
    else if (!WasUnitHomeless && hex == NULL)
    {
        this->hex(unit->home_x, unit->home_y)->hideUnitHome();
        unit->home_x = UNDEFINED;
        unit->home_y = UNDEFINED;
    }

    if (hex != NULL && (unit->x != hex->x || unit->y != hex->y))
    {
        unit->x = hex->x;
        unit->y = hex->y;
        units[unit]->anchorTo(hexes[hex]);
    }
}

void GameWindow::toBluePushed(bool state)
{
    ToBlue = state;
}
void GameWindow::toRedPushed(bool state)
{
    ToRed = state;
}
void GameWindow::toNeutralPushed(bool state)
{
    ToNeutral = state;
}

void GameWindow::unitCreatorClicked(PlayerColor color, UnitType type)
{
    this->newUnit(game->players[color]->newUnit(type));
}

void GameWindow::startGamePressed(bool)
{
    // TODO ???
}
