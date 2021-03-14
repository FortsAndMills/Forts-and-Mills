#include "GameWindow.h"
#include "Technical/dialogtext.h"

GameWindow::WaitingType GameWindow::RealizeEvent()
{
    GameMessage * e = game->events[0];
    if (e->type == CHOOSE_HEX)
    {
        // делаем доступными к нажатию определённые гексы
        if (game->events.size() == 1)
        {
            if (game->players[mainPlayerColor]->GiveUp)
                state = WAIT_FOR_ENEMY_START_POINT; // мы сдались, но планы врагов ещё не пришли
            else
                getReadyToChooseHex(e->variants); // мы не сдались, и у нас ещё есть время

            return WaitingType(WAIT_FOR_INTERACTION);
        }
        else
        {
            // игра уже перешла к следующему раунду
            return WaitingType();
        }
    }
    else if (e->type == PLAN)
    {
        if (game->events.size() == 1)
        {
            getReadyToPlanning();
            return WaitingType(WAIT_FOR_INTERACTION);
        }
        else
        {
            // игра уже перешла к следующему раунду
            return WaitingType();
        }
    }
    else if (e->type == START_CHOICE_MADE)
    {
        // увеличиваем прогресс-бар
        startChoiceProgress->expand();
        return WaitingType();
    }
    else if (e->type == HEX_CAPTURED)
    {
        hexes[e->hex]->setState(e->color);
        return WaitingType();
    }
    else if (e->type == NEW_UNIT_APPEAR)
    {
        newUnit(e->unit, e->hex->coord);
        hex(e->unit->home)->hideInformation();  // предполагается, что там была картинка этого юнита как рекрутирующегося

        player_windows[e->unit->color]->forces->increase(e->unit->health);

        if (game->events[1]->type == NEW_UNIT_APPEAR ||
                state != REALIZATION_PHASE)
            return WaitingType();
        else
            return WaitingType(BUTTON, constants->unitReconfigureTime);
    }
    else if (e->type == GATHER_RESOURCES)
    {
        newResources(e->hex, e->color, e->resources, e->burn);

        if (game->events[1]->type != GATHER_RESOURCES)
        {
            // анимация запускается единовременно для всех таких идущих подряд событий
            disappearAllTurnedOffOrders();
            reconfigureResources();
        }

        return WaitingType();
    }
    else if (e->type == HEX_DEFENSE_FILLED)
    {
        hexes[e->hex]->defenseTurn(e->amount);

        return WaitingType();
    }
    else if (e->type == UNIT_DEFENSE_FILLED)
    {
        units[e->unit]->defenseTurn(e->amount);

        return WaitingType();
    }
    else if (e->type == TIME_STARTED)
    {
        // переключаем время дня на табличке
        if (dayTime != -1)
            dayTimeTable->select(dayTime, false);
        dayTime = e->time;
        dayTimeTable->select(dayTime, true);

        // появляются все приказы
        foreach (Unit * unit, units)
                unit->mainOrderAppear(e->plan[unit->prototype]);

        return WaitingType(BUTTON, constants->mainOrderAppearTime);
    }
    else if (e->type == DEFENSE_BONUS_APPEARS)
    {
        hexes[e->hex]->defenseAppear(e->amount, e->color);

        return WaitingType();
    }
    else if (e->type == ORDER_REALIZATION_STARTED)
    {
        player_windows[e->unit->color]->lightResource(e->R, true);

        // при необходимости открывается приказ исполнителя
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime);
        }

        return WaitingType();
    }
    else if (e->type == AGITATED)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime, false);
        }

        return WaitingType();
    }
    else if (e->type == UNIT_CAPTURES_HEX)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime, false);
        }

        hexes[e->hex]->setState(e->color);

        if (game->events[1]->type == UNIT_IS_GOING_TO_RECRUIT)
            return WaitingType();
        else
            return WaitingType(BUTTON, constants->stateObjectChangeTime);
    }
    else if (e->type == UNIT_IS_GOING_TO_RECRUIT)
    {
        hexes[e->hex]->hideInformation();
        hexes[e->hex]->showInformation(e->unitType, "UnitIsGoingToRecruit");

        return WaitingType(BUTTON, 0);
    }
    else if (e->type == RECRUIT_FAILS_BECAUSE_OF_AGITE)
    {
        units[e->unit]->mainOrderBurn();
        player_windows[e->unit->color]->burnLighten(e->R);
        leftPanelSwitcherClicked(e->unit->color);

        return WaitingType(BUTTON, constants->stateObjectChangeTime);
    }
    else if (e->type == UNIT_LEAVES)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime, false);
        }

        moveToWayUnit(units[e->unit], e->hex->coord);
        createWay(units[e->unit]);

        if (game->events[1]->type == UNIT_ENTERS)
            return WaitingType();
        return WaitingType(BUTTON, constants->unitReconfigureTime);
    }
    else if (e->type == PURSUE_NOT_TRIGGERED)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(BUTTON, constants->mainOrderOpenTime);
        }
        return WaitingType();
    }
    else if (e->type == UNIT_ENTERS)
    {
        moveUnit(units[e->unit], e->hex->coord, Hex::ENTERING);

        return WaitingType(BUTTON, constants->unitReconfigureTime);
    }
    else if (e->type == UNIT_FINISHES_ENTER)
    {
        ways_to[units[e->unit]].pop()->disappear();  // или shrink()?
        ways_from[units[e->unit]].pop()->disappear();  // или shrink()?
        moveUnitForward(units[e->unit]);

        return WaitingType();
    }
    else if (e->type == UNITS_ARE_GOING_TO_FIGHT)
    {
        // TODO здесь какая-то перестановка должна быть?

        foreach (GameUnit * u, e->fighters)
        {
            if (!units[u]->isMainOrderOpenned())
            {
                units[u]->openMainOrder();
                return WaitingType(BUTTON, constants->mainOrderOpenTime, false);
            }
        }

        return WaitingType();
    }
    else if (e->type == ORDER_BURNS_IN_FIGHT)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(BUTTON, constants->mainOrderOpenTime, false);
        }

        if (units[e->unit]->mainOrder != NULL)
        {
            units[e->unit]->mainOrderBurn();
            player_windows[e->unit->color]->burn(e->R);
        }
        return WaitingType();
    }
    else if (e->type == UNITS_FIGHT)
    {
        QMap <GameUnit *, QPointF > p;
        QMap <PlayerColor, QSet<QPointF> > team_p;

        UnitsFightObject(e->fighters, e->strike, p, team_p);
        UnitsFight(e->fighters, e->strike, p, team_p);

        return WaitingType(BUTTON, constants->unitReconfigureTime);
    }
    else if (e->type == HEX_STATUS_CHANGE)
    {
        hexes[e->hex]->hideInformation();
        if (e->hexStatus == GameHex::NOT_CONNECTED)
            hexes[e->hex]->showInformation("NotConnected", "NotConnected");
        else if (e->hexStatus == GameHex::NOT_A_HOME)
            hexes[e->hex]->showInformation("Not" + e->color + "UnitHome", "NotUnitHome");
        else if (e->hexStatus == GameHex::TOMBSTONE)
            hexes[e->hex]->showInformation("Tombstone", "Tombstone", false);
        return WaitingType();
    }
    else if (e->type == UNIT_DIES)
    {
        if (Substate < e->resources.size())
        {
            if (Substate == 0 && e->isActiveOrderBurns)
            {
                player_windows[e->unit->color]->burnLighten(e->resources[Substate]);
                // переключение на панель того игрока, у которого сгорают ресурсы
                //if (Substate + 1 < e->resources.size())   так-так, почему это не работает?
                    //leftPanelSwitcherClicked(e->unit->color);
            }
            else
            {
                player_windows[e->unit->color]->burn(e->resources[Substate]);
                //leftPanelSwitcherClicked(e->unit->color);
            }
            ++Substate;
            return WaitingType(NO_WAIT, constants->orderBurnTime, false);
        }
        else
        {
            player_windows[e->unit->color]->forces->decrease(e->unit->health);
            blowUnit(e->unit);

            return WaitingType(BUTTON, constants->unitReconfigureTime);
        }
    }
    else if (e->type == RECRUITED_KILLED)
    {
        hexes[e->hex]->hideInformation();

        return WaitingType();
    }
    else if (e->type == UNIT_LOOSES_HOME)
    {
        units[e->unit]->broke();

        return WaitingType();
    }
    else if (e->type == UNIT_DECAPTURES_HEX)
    {
        hexes[e->hex]->setState("Neutral");
        if (hexes[e->hex]->information != NULL && hexes[e->hex]->information->name != "Tombstone")
            hexes[e->hex]->hideInformation();

        return WaitingType(BUTTON, constants->stateObjectChangeTime);
    }
    else if (e->type == DEFENSE_BONUS_DISAPPEARS)
    {
        hexes[e->hex]->defenseDisappear();

        return WaitingType();
    }
    else if (e->type == FORTIFICATION_DISAPPEARS)
    {
        deleteAllFortification(hexes[e->hex]);

        return WaitingType();
    }
    else if (e->type == SHOOT_LEFT)
    {
        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime, false);
        }

        shoot(e->unit->color, units[e->unit]->mainOrder->center(this), hexes[e->hex]->center(this), e->amount);

        return WaitingType(BUTTON, constants->rocketFlyTime);
    }
    else if (e->type == UNIT_CURES)
    {
        Unit * u = units[e->unit];
        u->healthChanged(e->amount);

        player_windows[e->unit->color]->forces->increase(e->amount);

        // TODO надо бы это в event-функцию оформить...
        // это просто создание крестика, инициализация и старт анимации
        CureIcon * cure = new CureIcon(this);
        cure->setPos(mapFromItem(units[e->unit], units[e->unit]->center()) - QPointF(constants->cureIconWidth, constants->cureIconHeight) * constants->unitsSize / 2);
        cure->resize(constants->cureIconWidth * constants->unitsSize, constants->cureIconHeight * constants->unitsSize);
        cure->anchorTo(units[e->unit]);
        cure->startAnimation();

        return WaitingType(BUTTON);
    }
    else if (e->type == FORTIFIED)
    {
        fortify(hexes[e->hex], e->amount, e->unit->color, units[e->unit]->center(this));

        return WaitingType(NO_WAIT, constants->unitReconfigureTime);
    }
    else if (e->type == ORDER_REALIZATION_FINISHED)
    {
        units[e->unit]->mainOrderDisappear();
        player_windows[e->unit->color]->turnOffLightenResource(e->R);

        return WaitingType();
    }
    else if (e->type == TIME_FINISHED)
    {
        // открываем все оставшиеся приказы, которые являются бездельем
        bool wait = false;
        foreach (Unit * u, units)
        {
            if (!u->isMainOrderOpenned())
            {
                wait = true;
                u->openMainOrder();
            }
        }

        if (wait)
            return WaitingType(BUTTON, constants->mainOrderOpenTime, false);

        // все приказы безделья исчезают
        foreach (Unit * u, units)
        {
            if (u->mainOrder != NULL)
            {
                wait = true;
                u->mainOrderDisappear();
            }
        }

        if (wait)
            return WaitingType(NO_WAIT, constants->orderDisappearTime);
        return WaitingType();
    }
    else if (e->type == PLAN_REALIZATION_FINISHED)
    {
        return WaitingType();
    }
    else if (e->type == HOMELESS_UNIT_DAMAGED)
    {
        units[e->unit]->healthChanged(-e->amount);
        player_windows[e->unit->color]->forces->decrease(e->amount);
        return WaitingType();
    }
    else if (e->type == AGITATION_ENDS)
    {
        return WaitingType();
    }
    else if (e->type == BURN_RESOURCE)
    {
        player_windows[e->color]->burn(e->R);

        return WaitingType();
    }
    else if (e->type == DAY_TIMES_CHANGED)
    {
        dayTimeTable->disappear();

        // для изменения количества времён дня, табличку приходится пересоздавать
        dayTimeTable = new DayTimePanel(this, game->rules->dayTimes);
        resizeDayTimeTable(width(), height());
        for (int i = 0; i < game->rules->dayTimes; ++i)
            connect(dayTimeTable->DayTimePictures[i], SIGNAL(leftClicked(int)), SLOT(dayTimeClicked(int)));
        dayTimeTable->appear();

        return WaitingType();
    }
    else if (e->type == WIN)
    {
        if (e->color == mainPlayerColor)
        {
            dialog->set(mainPlayerColor, dialogtext->get("victory"), false, false, true);
        }
        else
        {
            dialog->set(mainPlayerColor, dialogtext->get("defeat"), false, false, true);
        }
        resizeDialog(width(), height());

        // все кнопки исчезают
        next->AnimationStart(OPACITY, 0);
        whiteFlag->AnimationStart(OPACITY, 0);
        go->AnimationStart(OPACITY, 0);

        return WaitingType(WAIT_FOR_INTERACTION, 0, false);
    }

    debug << e->type << " unproccesed!";
    return WaitingType();
}

// ищем центр объекта, который представляет то, чем сражается юнит
void GameWindow::UnitsFightObject(QSet <GameUnit *> fighters, const Strike & strike,
                                         QMap <GameUnit *, QPointF > & ans, QMap <PlayerColor, QSet<QPointF> > & team_ans)
{
    foreach (GameUnit * u, fighters)
    {
        if (strike.fb[u] == FORTIFICATION)
        {
            Hex * hex = hexes[strike.positions[u]];
            ans[u] = hex->point(fortifications[hex][0]->point, this);
        }
        else if (strike.fb[u] == HEX_DEFENSE)
        {
            ans[u] = hexes[strike.positions[u]]->shieldPoint(this);
        }
        else if (strike.fb[u] == ORDER_BONUS)
        {
            ans[u] = units[u]->mainOrderCenter(this);
        }
        else if (strike.fb[u] == UNIT_DEFENSE)
        {
            ans[u] = units[u]->shieldPoint(this);
        }
        else if (strike.fb[u] == DISTANT)
        {
            ans[u] = units[u]->mainOrderCenter(this);
        }
        else if (strike.fb[u] == UNIT_HEALTH)
        {
            ans[u] = units[u]->center(this);
        }
        else
            ans[u] = QPointF(UNDEFINED, UNDEFINED);

        if (ans[u] != QPointF(UNDEFINED, UNDEFINED))
            team_ans[u->color] << ans[u];
    }
}
void GameWindow::UnitsFight(QSet <GameUnit *> fighters, const Strike & strike,
                                   const QMap <GameUnit *, QPointF > & points, const QMap <PlayerColor, QSet<QPointF> > & team_points)
{
    QSet <PlayerColor> global_dones;

    foreach (GameUnit * u, fighters)
    {
        // подсвечиваем красным штрафующие приказы
        if (strike.penalty[u] > 0)
        {
            if (units[u]->mainOrder != NULL)
                units[u]->mainOrder->light();
        }

        if (strike.fb[u] == FORTIFICATION && !global_dones.contains(u->color))  // убираем использованные щиты
        {
            fortify(hexes[strike.positions[u]], -strike.team_amount[u->color]);
            global_dones << u->color;
        }
        else if (strike.fb[u] == HEX_DEFENSE && !global_dones.contains(u->color))  // деактивируем бонус гекса
        {
            hexes[strike.positions[u]]->defenseTurn(strike.team_amount[u->color], false);
            global_dones << u->color;
        }
        else if (strike.fb[u] == ORDER_BONUS)  // стреляем бластом из приказа, дающего бонус, в точку, представляющую противника
        {
            foreach (PlayerColor enemy_color, team_points.keys())
            {
                if (u->color != enemy_color)
                {
                    foreach (QPointF p, team_points[enemy_color])
                    {
                        shoot(u->color, points[u], p, strike.amount[u], Rocket::BLAST);
                    }
                }
            }
        }
        else if (strike.fb[u] == UNIT_DEFENSE)
        {
            units[u]->defenseTurn(strike.amount[u], false);
        }
        else if (strike.fb[u] == DISTANT)  // смешно смотрится, если несколько участников боя
        {
            foreach (PlayerColor enemy_color, team_points.keys())
            {
                if (u->color != enemy_color)
                {
                    foreach (QPointF p, team_points[enemy_color])
                    {
                        shoot(u->color, points[u], p, strike.amount[u]);
                    }
                }
            }
        }
        else if (strike.fb[u] == UNIT_HEALTH)
        {
            units[u]->healthChanged(-strike.amount[u]);
            player_windows[u->color]->forces->decrease(strike.amount[u]);
        }
    }
}

