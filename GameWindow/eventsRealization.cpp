#include "EventsRealization.h"

EventsRealization::EventsRealization(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    GameLog(game, PlayerIndex, parent)
{

}

EventsRealization::WaitingType EventsRealization::RealizeEvent()
{
    GameMessage * e = game->events[0];
    if (e->type == CHOOSE_HEX)
    {
        if (Substate == 0)
        {
            getReadyToChooseHex(e->variants);

            Substate = 1;
            if (game->isEveryoneReady())
                return WaitingType();
            return WaitingType(WAIT_FOR_INTERACTION, 0, false);
        }
        else
        {
            logMillChoice();
            game->HexChosen();
            return WaitingType();
        }
    }
    else if (e->type == PLAN)
    {
        if (Substate == 0)
        {
            getReadyToPlanning();

            Substate = 1;
            if (game->isEveryoneReady())
                return WaitingType();
            return WaitingType(WAIT_FOR_INTERACTION, 0, false);
        }
        else
        {
            getReadyToRealization();

            logPlan();
            game->PlanRealisation();

            return WaitingType();
        }
    }
    else if (e->type == HEX_CAPTURED)
    {
        hexes[e->hex]->setState(e->color);
        return WaitingType();
    }
    else if (e->type == NEW_UNIT_APPEAR)
    {
        newUnit(e->unit, e->hex->coord);
        hex(e->unit->home)->hideInformation();

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
            disappearAllTurnedOffOrders();
            reconfigureResources();
        }

        return WaitingType();
    }
    else if (e->type == HEX_DEFENCE_FILLED)
    {
        hexes[e->hex]->defenceTurn(e->amount);

        return WaitingType();
    }
    else if (e->type == UNIT_DEFENCE_FILLED)
    {
        units[e->unit]->defenceTurn(e->amount);

        return WaitingType();
    }
    else if (e->type == TIME_STARTED)
    {
        if (dayTime != -1)
            DayTimeTable->select(dayTime, false);
        dayTime = e->time;
        DayTimeTable->select(dayTime, true);

        foreach (Unit * unit, units)
                unit->mainOrderAppear(e->plan[unit->prototype]);

        return WaitingType(BUTTON, constants->mainOrderAppearTime);
    }
    else if (e->type == DEFENCE_BONUS_APPEARS)
    {
        hexes[e->hex]->defenceAppear(e->amount, e->color);

        return WaitingType();
    }
    else if (e->type == ORDER_REALIZATION_STARTED)
    {
        player_windows[e->unit->color]->lightResource(e->R, true);

        if (!units[e->unit]->isMainOrderOpenned())
        {
            units[e->unit]->openMainOrder();
            return WaitingType(NO_WAIT, constants->mainOrderOpenTime);
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
        hexes[e->hex]->showInformation(e->unitType, "UnitIsGoingToRecruit");

        return WaitingType(BUTTON, 0);
    }
    else if (e->type == CAPTURE_FAILS_BECAUSE_OF_CASTLE)
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
        // здесь какая-то перестановка должна быть

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
        QList <QPointF> p = UnitsFightObject(e->fighters, e->fb, e->positions);

        bool imm = UnitsFight(e->fighters, e->fb, e->positions, e->amount, p);

        if (!imm)
        {
            return WaitingType(BUTTON, constants->unitReconfigureTime);
        }
        return WaitingType();
    }
    else if (e->type == HEX_IS_NOT_A_HOME_ANYMORE)
    {
        hexes[e->hex]->showInformation("Not" + e->color + "UnitHome", "NotUnitHome");
        return WaitingType();
    }
    else if (e->type == UNIT_DIES)
    {
        if (Substate < e->resources.size())
        {
            if (Substate == 0 && e->isActiveOrderBurns)
            {
                player_windows[e->unit->color]->burnLighten(e->resources[Substate]);
                //if (Substate + 1 < e->resources.size())
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
            blowUnit(e->unit);
            //hex(e->x2, e->y2)->showInformation("Not" + e->unit->color + "UnitHome");

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
        hexes[e->hex]->hideInformation();

        return WaitingType(BUTTON, constants->stateObjectChangeTime);
    }
    else if (e->type == DEFENCE_BONUS_DISAPPEARS)
    {
        hexes[e->hex]->defenceDisappear();

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
        return WaitingType();
    }
    else if (e->type == BURN_RESOURCE)
    {
        player_windows[e->color]->burn(e->R);

        return WaitingType();
    }
    else if (e->type == DAY_TIMES_CHANGED)
    {
        DayTimeTable->disappear();

        DayTimeTable = new DayTimePanel(this, game->rules->dayTimes);
        resizeDayTimeTable(width(), height());
        for (int i = 0; i < game->rules->dayTimes; ++i)
            connect(DayTimeTable->DayTimePictures[i], SIGNAL(leftClicked(int)), SLOT(dayTimeClicked(int)));
        DayTimeTable->appear();

        return WaitingType();
    }
    else if (e->type == WIN)
    {
        if (e->color == mainPlayerColor)
        {
            dialog->set(mainPlayerColor, "ПОБЕДА!!!", false, false, true);
        }
        else
        {
            dialog->set(mainPlayerColor, "ПОРАЖЕНИЕ!!!", false, false, true);
        }
        resizeDialog(width(), height());

        next->AnimationStart(OPACITY, 0);
        whiteFlag->AnimationStart(OPACITY, 0);
        go->AnimationStart(OPACITY, 0);

        return WaitingType(WAIT_FOR_INTERACTION, 0, false);
    }

    qDebug() << e->type << " unproccesed!";
    return WaitingType();
}

QList <QPointF> EventsRealization::UnitsFightObject(QList <GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions)
{
    QList <QPointF> ans;

    for (int i = 0; i < fighters.size(); ++i)
    {
        if (fb[i] == ENEMY_ORDER_PENALTY)
        {
            int j = 0;
            while (j < fighters.size() && fb[j] == ENEMY_ORDER_PENALTY)
                ++j;

            if (j < fighters.size())
                ans << units[fighters[j]]->mainOrderCenter(this);
            ans << units[fighters[i]]->mainOrderCenter(this);
        }
        else if (fb[i] == FORTIFICATION)
        {
            ans << hexes[positions[i]]->point(fortifications[hexes[positions[i]]][0]->point, this);
        }
        else if (fb[i] == HEX_DEFENCE)
        {
            ans << hexes[positions[i]]->shieldPoint(this);
        }
        else if (fb[i] == ORDER_BONUS)
        {
            ans << units[fighters[i]]->mainOrderCenter(this);
        }
        else if (fb[i] == UNIT_DEFENCE)
        {
            ans << units[fighters[i]]->shieldPoint(this);
        }
        else if (fb[i] == DISTANT)
        {
            ans << units[fighters[i]]->mainOrderCenter(this);
        }
        else if (fb[i] == UNIT_HEALTH)
        {
            ans << units[fighters[i]]->center(this);
        }
        else
            ans << QPointF(UNDEFINED, UNDEFINED);
    }

    return ans;
}
bool EventsRealization::UnitsFight(QList <GameUnit *> fighters, QList<FightBrid> fb, QList<GameHex *> positions, int value, QList <QPointF> points)
{
    int pens = 0;
    for (int i = 0; i < fighters.size(); ++i)
    {
        if (fb[i] == ENEMY_ORDER_PENALTY)
            ++pens;
    }

    for (int i = 0; i < fighters.size(); ++i)
    {
        if ((fb[i] != ENEMY_ORDER_PENALTY && pens > 0) || pens == fighters.size())
        {
            if (units[fighters[i]]->mainOrder != NULL)
                units[fighters[i]]->mainOrder->light();
        }

        if (fb[i] == FORTIFICATION)
        {
            fortify(hexes[positions[i]], -value);
        }
        else if (fb[i] == HEX_DEFENCE)
        {
            hexes[positions[i]]->defenceTurn(value, false);
        }
        else if (fb[i] == ORDER_BONUS)
        {
            for (int j = 0; j < fighters.size(); ++j)
            {
                if (fb[j] != DISTANT && j != i)
                    shoot(fighters[i]->color, points[i], points[j], value, Rocket::BLAST);
            }
        }
        else if (fb[i] == UNIT_DEFENCE)
        {
            units[fighters[i]]->defenceTurn(value, false);
        }
        else if (fb[i] == DISTANT)
        {
            for (int j = 0; j < fighters.size(); ++j)
            {
                if (i != j)
                    shoot(fighters[i]->color, points[i], points[j], value);
            }
        }
        else if (fb[i] == UNIT_HEALTH)
        {
            units[fighters[i]]->healthChanged(-value);
        }
    }

    return pens == fighters.size();
}

