#include "Game.h"
#include "GameUnit.h"
#include "GameRules.h"
#include "GamePlayer.h"
#include "GameOrder.h"
#include "Technical/ProgrammSettings.h"

void Game::getReadyToChooseStartPoint()
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        state = CHOOSE_START_POINT;

        foreach (GamePlayer * player, players)
        {
            player->selected_x = UNDEFINED;
            player->selected_y = UNDEFINED;
        }

        for (int i = 0; i < rules->fieldH; ++i)
        {
            for (int j = 0; j < rules->fieldW; ++j)
            {
                if (field[i][j]->canBeChosenAsStartPoint)
                {
                    if (!field[i][j]->wasChosenAsStartPoint)
                    {
                        emit lightHex(field[i][j]);
                        emit enableToSelectHex(field[i][j]);
                    }
                    else
                    {
                        emit delightHex(field[i][j]);
                        emit forbidToSelectHex(field[i][j]);
                    }
                }
                else
                    emit forbidToSelectHex(field[i][j]);
            }
        }

        emit goButtonStateChanged(false);
    }
}
void Game::CheckIfEveryoneChoosedStartPoint()
{
    if (state == WAIT_FOR_ENEMY_START_POINT)
    {
        foreach (GamePlayer * player, players)
        {
            if (player->selected_x == UNDEFINED)
                return;
        }

        startPointsChosen();
    }
}

void Game::makeAllHexesUnaviable()
{
    for (int i = 0; i < rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            emit delightHex(field[i][j]);
            emit forbidToSelectHex(field[i][j]);
        }
    }
}

void Game::getReadyToPlanning()
{
    state = PLANNING;

    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        emit goButtonStateChanged(true);
    }
}

void Game::dayTimeClicked(DayTime time)
{
    if (time != dayTime)
        setTime(time);
    else
        setTime("");
}
bool Game::setNextTime()
{
    DayTime prevDayTime = dayTime;
    if (nextTime(dayTime) == "")
        return false;
    dayTime = nextTime(dayTime);

    if (prevDayTime != "")
    {
        foreach (GameUnit * unit, players[mainPlayerColor]->units)
            finishPlannedOrder(unit, prevDayTime);
    }

    emit selectDayTime(dayTime, true);
    if (prevDayTime != "")
    {
        emit selectDayTime(prevDayTime, false);
    }

    foreach (GameUnit * unit, players[mainPlayerColor]->units)
        showPlannedOrder(unit, dayTime);

    return true;
}
void Game::setPreviousTime()
{
    setEarlyTime(prevTime(dayTime));
}
void Game::setTime(DayTime time)
{
    if (time == dayTime)
        return;

    if (selectedUnit != NULL)
    {
        emit deselectUnit(selectedUnit);
        emit hidePanel(selectedUnit);
    }

    if (dayTime == "")
    {
        if (time == rules->dayTimes[0])
            setNextTime();
        else
            setLateTime(time);
        return;
    }
    else if (time == "")
    {
        if (dayTime == rules->dayTimes[0])
            setPreviousTime();
        else
            setEarlyTime(time);
        return;
    }

    if (time == prevTime(dayTime))
    {
        setPreviousTime();
        return;
    }
    if (time == nextTime(dayTime))
    {
        setNextTime();
        return;
    }

    int i = 0;
    bool found = false;
    while (rules->dayTimes[i] != dayTime)
    {
        if (rules->dayTimes[i] == time)
            found = true;
        ++i;
    }

    if (found)
        setEarlyTime(time);
    else
        setLateTime(time);
    return;
}
void Game::setEarlyTime(DayTime time)
{
    emit selectDayTime(dayTime, false);
    while (dayTime != time)
    {
        foreach (GameUnit * unit, players[mainPlayerColor]->units)
            hidePlannedOrder(unit, dayTime);

        dayTime = prevTime(dayTime);
    }

    foreach (GameUnit * unit, players[mainPlayerColor]->units)
        planPlannedOrder(unit, dayTime);

    if (dayTime != "")
        emit selectDayTime(dayTime, true);
}
void Game::setLateTime(DayTime time)
{

}

void Game::showPlannedOrder(GameUnit *unit, DayTime time)
{
    if (unit->plan[time] == NULL)
    {

    }
    else if (unit->plan[time]->isMovingUnit)
    {
        if (prevTime(time) != "")
            emit anchorOrderFromUnit(unit, unit->plan[prevTime(time)], unit->x, unit->y);

        unit->x = unit->plan[time]->parameter_x;
        unit->y = unit->plan[time]->parameter_y;

        emit planUnitMove(unit, field[unit->plan[time]->x][unit->plan[time]->y],
                          whereIs(unit->x, unit->y, unit->plan[time]->x, unit->plan[time]->y));
    }
}
void Game::finishPlannedOrder(GameUnit * unit, DayTime time)
{
    if (unit->plan[time] == NULL)
    {

    }
    else if (unit->plan[time]->isMovingUnit)
    {
        emit finishEntering(unit, field[unit->x][unit->y]);
    }
}
void Game::planPlannedOrder(GameUnit * unit, DayTime time)
{
    if (unit->plan[time] == NULL)
    {

    }
    else if (unit->plan[time]->isMovingUnit)
    {
        emit backEntering(unit, field[unit->x][unit->y]);
    }
}
void Game::hidePlannedOrder(GameUnit *unit, DayTime time)
{
    if (unit->plan[time] == NULL)
    {

    }
    else if (unit->plan[time]->isMovingUnit)
    {
        unit->x = unit->plan[time]->x;
        unit->y = unit->plan[time]->y;

        if (prevTime(time) != "")
            emit anchorOrderToUnit(unit, unit->plan[prevTime(time)], field[unit->x][unit->y]);

        emit updateUnitHexPosition(unit);
        emit deleteWay(unit);
    }
}

void Game::orderVariantClicked(OrderType order)
{
    if (selectedUnit == NULL)
        qDebug() << "FatalError: no selected unit, but there is order variant!!!";

    planOrder(selectedUnit, dayTime, order);
}
void Game::getReadyToChooseOrderParameter(GameOrder * order)
{
    if (order->parameterType == GameOrder::ADJACENT_HEX_WHERE_CAN_GO)
    {
        QSet <QPair <int, int> > variants = adjacentHexes(order->owner->x, order->owner->y);
        for (int i = 0; i < rules->fieldH; ++i)
        {
            for (int j = 0; j < rules->fieldW; ++j)
            {
                if (variants.contains(QPair<int, int>(i, j)) && field[i][j]->canGoHere)
                {
                    emit lightHex(field[i][j]);
                    emit enableToSelectHex(field[i][j]);
                }
                else
                {
                    emit delightHex(field[i][j]);
                    emit forbidToSelectHex(field[i][j]);
                }
            }
        }
    }
    else if (order->parameterType == GameOrder::VISIBLE_HEX_IN_RADIUS_2)
    {
        QSet <QPair <int, int> > variants = visible_hexes(order->owner->x, order->owner->y, 2);
        for (int i = 0; i < rules->fieldH; ++i)
        {
            for (int j = 0; j < rules->fieldW; ++j)
            {
                if (variants.contains(QPair<int, int>(i, j)) && field[i][j]->canGoHere)
                {
                    emit lightHex(field[i][j]);
                    emit enableToSelectHex(field[i][j]);
                }
                else
                {
                    emit delightHex(field[i][j]);
                    emit forbidToSelectHex(field[i][j]);
                }
            }
        }
    }
}

void Game::hexClicked(int i, int j)
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        if (state == CHOOSE_START_POINT)
        {
            if (field[i][j]->type == "Mill")
            {
                if (players[mainPlayerColor]->selected_x == i &&
                     players[mainPlayerColor]->selected_y == j)
                {
                    players[mainPlayerColor]->selected_x = UNDEFINED;
                    players[mainPlayerColor]->selected_y = UNDEFINED;
                    emit deselectHex(field[i][j]);
                    emit goButtonStateChanged(false);
                }
                else
                {
                    if (players[mainPlayerColor]->selected_x != UNDEFINED)
                        emit deselectHex(field[players[mainPlayerColor]->selected_x][players[mainPlayerColor]->selected_y]);
                    players[mainPlayerColor]->selected_x = i;
                    players[mainPlayerColor]->selected_y = j;
                    emit selectHex(field[i][j]);
                    emit goButtonStateChanged(true);
                }
            }
            else
                qDebug() << "ERROR: choosen not-mill hex when must choose mills!";
        }
        else if (state == CHOOSE_ORDER_PARAMETER)
        {
            selectedUnit->plan[dayTime]->parameter_x = i;
            selectedUnit->plan[dayTime]->parameter_y = j;

            makeAllHexesUnaviable();

            showPlannedOrder(selectedUnit, dayTime);
            if (setNextTime())
            {
                state = MOVING_CENTRAL_UNIT;
                emit waitForAnimation();
            }
            else
            {
                state = PLANNING;
                emit deselectUnit(selectedUnit);
                selectedUnit = NULL;
            }
        }
    }
    else if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (ToBlue)
        {
            captureHex(i, j, "Blue");
        }
        else if (ToRed)
        {
            captureHex(i, j, "Red");
        }
        else if (ToNeutral)
        {
            decaptureHex(i, j);
        }
    }
}

void Game::orderPicPushed(Resource type, PlayerColor owner, bool state)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (state)
        {
            --players[owner]->resources[type];
            emit changePlayerResourceState(owner, type, false);
        }
        else
        {
            ++players[owner]->resources[type];
            emit changePlayerResourceState(owner, type, true);
        }

        emit createResource(new GameOrder(this, NULL, type));
    }
}
void Game::OrderRightClicked(GameOrder * order)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
        emit deleteOrder(order);
}

void Game::unitHoverEntered(GameUnit *unit)
{
    if (unit->home_x != UNDEFINED)
        emit showHome(field[unit->home_x][unit->home_y], unit->color);

    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
        emit selectUnit(unit);
}
void Game::unitHoverLeft(GameUnit *unit)
{
    if (unit->home_x != UNDEFINED)
        emit hideHome(field[unit->home_x][unit->home_y]);

    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
        emit deselectUnit(unit);
}
void Game::unitRightClicked(GameUnit *unit)
{
    if (settings->PROGRAMM_VERSION == POWER_POINT_VERSION)
    {
        if (unit->home_x != UNDEFINED)
        {
            emit hideHome(field[unit->home_x][unit->home_y]);
            unit->home_x = UNDEFINED;
            unit->home_y = UNDEFINED;
        }
        destroyUnit(unit);
    }
}
void Game::unitLeftClicked(GameUnit * unit)
{
    if (settings->PROGRAMM_VERSION == REAL_CLIENT)
    {
        if (state == PLANNING)
        {
            if (this->mainPlayerColor == unit->color)
            {
                if (selectedUnit == unit)
                {
                    emit deselectUnit(unit);
                    emit hidePanel(unit);
                    selectedUnit = NULL;
                }
                else
                {
                    if (dayTime == "")  // если был режим созерцания, переходим к завтраку
                        setNextTime();

                    if (selectedUnit != NULL)  // если был выбран другой юнит... ну вы поняли
                    {
                        emit deselectUnit(selectedUnit);
                        emit hidePanel(selectedUnit);
                    }

                    selectedUnit = unit;  // новый выбранный юнит

                    DayTime time = dayTime;  // ищем для него подходящее время
                    while (time != "" && selectedUnit->plan[time] == NULL)
                        time = prevTime(time);
                    if (nextTime(time) != "")
                        setTime(nextTime(time));

                    time = dayTime;  // убираем все планы на сейчас и далее
                    while (time != "")
                    {
                        deplanOrder(selectedUnit, time);
                        time = nextTime(time);
                    }

                    emit showUnitOrdersPanel(unit, whatCanUse(unit));
                    emit selectUnit(unit);
                }
            }
        }
        else if (state == CHOOSE_ORDER_PARAMETER)
        {
            if (unit->color == mainPlayerColor)
            {
                deplanOrder(selectedUnit, dayTime);  // убираем планирующийся приказ

                makeAllHexesUnaviable();  // убираем следы выбора параметра

                if (selectedUnit == unit)  // устраиваем выбор приказа
                {
                    emit showUnitOrdersPanel(unit, whatCanUse(unit));
                }
                else
                {
                    if (selectedUnit != NULL)
                    {
                        emit deselectUnit(selectedUnit);
                    }

                    selectedUnit = unit;
                    emit showUnitOrdersPanel(unit, whatCanUse(unit));
                    emit selectUnit(unit);
                }
            }
        }
    }
}

#define MILLS_COORDINATES_MESSAGE (qint8)1
void Game::GoButtonPushed(bool)
{
    if (state == CHOOSE_START_POINT)
    {
        emit disableAllField();
        state = WAIT_FOR_ENEMY_START_POINT;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << MILLS_COORDINATES_MESSAGE
                  << PlayerIndex
                  << (qint32)players[mainPlayerColor]->selected_x
                  << (qint32)players[mainPlayerColor]->selected_y;
        emit writeToOpponent(message);

        CheckIfEveryoneChoosedStartPoint();
    }
}
void Game::readFromOpponent(QByteArray message)
{
    QDataStream in(&message, QIODevice::ReadOnly);
    qint8 Command;
    in >> Command;

    if (Command == MILLS_COORDINATES_MESSAGE)
    {
        quint8 sender_index;
        in >> sender_index;
        PlayerColor sender = rules->players[sender_index];

        in >> players[sender]->selected_x >> players[sender]->selected_y;

        CheckIfEveryoneChoosedStartPoint();
    }
}


// ДЛЯ ИСКУССТВЕННОГО КОНТРОЛЯ PP-ВЕРСИИ
void Game::unitWheeled(GameUnit * unit, int delta)
{
    if (delta > 0 && unit->health != unit->max_health)
    {
        ++unit->health;
        emit unitHealthChanged(unit);
    }
    else if (delta < 0 && unit->health > 0)
    {
        --unit->health;
        emit unitHealthChanged(unit);
    }
}
void Game::unitDragStarted(GameUnit *unit)
{
    WasUnitHomeless = (unit->home_x == UNDEFINED);
    if (unit->x != UNDEFINED)
    {
        emit unitGetOutOfField(unit, unit->x, unit->y);
        unit->x = UNDEFINED;
        unit->y = UNDEFINED;
    }
}
void Game::unitDragFinished(GameUnit *unit, GameHex *hex)
{
    if (WasUnitHomeless && hex != NULL)
    {
        unit->home_x = hex->x;
        unit->home_y = hex->y;
    }
    else if (!WasUnitHomeless && hex == NULL)
    {
        emit hideHome(field[unit->home_x][unit->home_y]);
        unit->home_x = UNDEFINED;
        unit->home_y = UNDEFINED;
    }

    if (hex != NULL && (unit->x != hex->x || unit->y != hex->y))
    {
        unit->x = hex->x;
        unit->y = hex->y;
        emit updateUnitHexPosition(unit);
    }
}

void Game::toBluePushed(bool state)
{
    ToBlue = state;
}
void Game::toRedPushed(bool state)
{
    ToRed = state;
}
void Game::toNeutralPushed(bool state)
{
    ToNeutral = state;
}

void Game::unitCreatorClicked(PlayerColor color, UnitType type)
{
    GameUnit * New = new GameUnit(this, type, color);
    players[color]->units << New;
    emit newUnit(New);
}

void Game::startGamePressed(bool)
{
    startMainPhaseGame();
}
