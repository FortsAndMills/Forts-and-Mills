#include "GameWindow.h"

void GameWindow::initialConnections()
{
    connect(this, SIGNAL(writeToOpponent(QByteArray)), client, SLOT(sendToOpponent(QByteArray)));
    connect(client, SIGNAL(MessageToGame(QByteArray)), SLOT(readFromOpponent(QByteArray)));
    connect(client, SIGNAL(OpponentDisconnected(qint8)), SLOT(opponentDisconnected(qint8)));
    connect(client, SIGNAL(OpponentReconnected(qint8)), SLOT(opponentReconnected(qint8)));
    connect(client, SIGNAL(disconnected()), SLOT(serverDisconnected()));

    for (int i = 0; i < game->rules->fieldH; ++i)
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            connect(hexes[game->hexes[i][j]], SIGNAL(clicked(Coord)), SLOT(hexClicked(Coord)));
            connect(hexes[game->hexes[i][j]], SIGNAL(entered(Coord)), SLOT(hexEntered(Coord)));
            connect(hexes[game->hexes[i][j]], SIGNAL(left(Coord)), SLOT(hexLeft(Coord)));
        }
    }

    foreach (Unit * u, units)
        unitConnections(u);

    for (int i = 0; i < game->rules->dayTimes; ++i)
        connect(dayTimeTable->DayTimePictures[i], SIGNAL(leftClicked(int)), SLOT(dayTimeClicked(int)));

    for (int i = 0; i < game->rules->unitsInGame.size(); ++i)
        connect(startUnitsChoice->units[i], SIGNAL(whenClicked(bool,QString)), SLOT(startUnitTypeClicked(bool, QString)));


    connect(go, SIGNAL(leftClicked()), SLOT(GoButtonPushed()));
    connect(next, SIGNAL(leftClickStarted()), SLOT(NextButtonClicked()));
    connect(whiteFlag, SIGNAL(leftClicked()), SLOT(whiteFlagClicked()));
    connect(homeButton, SIGNAL(leftClicked()), SLOT(homeButtonClicked()));

    connect(fieldControl->up, SIGNAL(leftClicked()), SLOT(hexPointsChanged()));
    connect(fieldControl->down, SIGNAL(leftClicked()), SLOT(hexPointsChanged()));
    connect(fieldControl->right, SIGNAL(leftClicked()), SLOT(hexPointsChanged()));
    connect(fieldControl->left, SIGNAL(leftClicked()), SLOT(hexPointsChanged()));

    connect(dialog, SIGNAL(returned(bool, QString)), SLOT(DialogReturned(bool, QString)));
}
void GameWindow::unitConnections(Unit * unit)
{
    connect(unit, SIGNAL(entered(GameUnit*)), SLOT(unitHoverEntered(GameUnit*)));
    connect(unit, SIGNAL(left(GameUnit*)), SLOT(unitHoverLeft(GameUnit*)));
    connect(unit, SIGNAL(leftClicked(GameUnit*)), SLOT(unitLeftClicked(GameUnit*)));
    connect(unit, SIGNAL(orderVariantChosen(QString)), SLOT(orderVariantClicked(QString)));
    connect(unit, SIGNAL(unitTypeChosen(QString)), SLOT(unitTypeChosen(QString)));
}
void GameWindow::orderPicsConnections(OrderPic *pic)
{
    connect(pic, SIGNAL(orderPicEntered(QString,QString)), SLOT(orderPicEntered(QString,QString)));
    connect(pic, SIGNAL(orderPicLeft(QString,QString)), SLOT(orderPicLeft(QString,QString)));
}

void GameWindow::hexClicked(Coord c)
{
    if (state == CHOOSING_HEX)
    {
        if (game->hex(c)->canBeChosenAsStartPoint)
        {
            if (game->chosenHex[mainPlayerColor] == c)
            {
                game->chosenHex[mainPlayerColor] = NOWHERE;

                hex(c)->select(false);
                go->enable(false);
            }
            else
            {
                if (game->chosenHex[mainPlayerColor] != NOWHERE)
                {
                    hex(game->chosenHex[mainPlayerColor])->select(false);
                }

                game->chosenHex[mainPlayerColor] = c;

                hex(c)->select();
                go->enable();
            }
        }
        else
            debug << "ERROR: choosen not-mill hex when must choose mills!\n";
    }
    else if (state == CHOOSE_ORDER_PARAMETER)
    {
        selectedUnit->prototype->plan[dayTime]->setParameter(c);

        finishedChoosingOrderParameter();
    }
}
void GameWindow::hexEntered(Coord c)
{
    if (hexCopy != NULL)
    {
        hexCopy->Delete();
    }

    // при наведении на гекс стоит создать её копию в левом нижнем углу
    hexCopy = new Hex(hex(c), this);
    hexCopy->select(false, true);
    hexCopy->light(false, true);
    resizeHexCopy();

    // во время планирования также отображается юнит из этой клетки
    // TODO стоит эту инфу хранить динамически, чтоб можно было и во время реализации этим баловаться
    if (state != REALIZATION_PHASE)
    {
        GameUnit * unit = game->whoHasHomeAt(c);
        if (unit != NULL && units.contains(unit))
        {
            units[unit]->light();
        }
    }
}
void GameWindow::hexLeft(Coord c)
{
    if (hexCopy != NULL &&
            hexCopy->prototype->coord == c)
    {
        hexCopy->Delete();
        hexCopy = NULL;
    }

    if (state != REALIZATION_PHASE)
    {
        GameUnit * unit = game->whoHasHomeAt(c);
        if (unit != NULL && units.contains(unit))
        {
            units[unit]->light(false);
        }
    }
}

void GameWindow::orderPicEntered(OrderType type, PlayerColor)
{
    if (state != CHOOSE_ORDER_PARAMETER)
    {
        foreach (Hex * hex, hexes)
        {
            hex->highlight(type);
        }
    }
}
void GameWindow::orderPicLeft(OrderType type, PlayerColor)
{
    foreach (Hex * hex, hexes)
    {
        hex->highlight(type, false);
    }
}

void GameWindow::orderVariantClicked(OrderType type)
{
    if (selectedUnit == NULL)
        debug << "FatalError: no selected unit, but there is order variant!!!\n";

    state = CHOOSE_ORDER_PARAMETER;

    if (selectedUnit->prototype->plan[dayTime] != NULL)
    {
        debug << "ERROR: order on order!!!\n";
        delete selectedUnit->prototype->plan[dayTime];
    }

    // создаём новый приказ выбранного типа
    GameOrder * order = new GameOrder(game->rules, selectedUnit->prototype->type, type);
    ++priorities[dayTime];
    selectedUnit->prototype->plan[dayTime] = order;

    // инициализируем его и привязываем к текущему юниту
    orders[order] = new Order(this, order);
    orders[order]->anchorTo(selectedUnit);
    orders[order]->anchor = selectedUnit->where();

    selectedUnit->locateOrderLikeOnPanel(orders[order]);
    selectedUnit->reconfigureOrders();
    selectedUnit->hidePanel();

    // приказ отображается использованным
    if (type != DefaultOrder)
    {
        player_windows[mainPlayerColor]->turnResource(type, false);
    }

    // переходим в состояние выбора параметра приказа
    getReadyToChooseOrderParameter();
}

void GameWindow::unitTypeChosen(QString type)
{
    selectedUnit->prototype->plan[dayTime]->setParameter(type);
    finishedChoosingOrderParameter();
}

void GameWindow::unitHoverEntered(GameUnit *unit)
{
    if (unit->home != NOWHERE)
        hex(unit->home)->showUnitHome(unit->color);
}
void GameWindow::unitHoverLeft(GameUnit *unit)
{
    if (unit->home != NOWHERE)
        hex(unit->home)->hideUnitHome();
}
void GameWindow::unitLeftClicked(GameUnit * unit)
{
    if (state == PLANNING || state == CHOOSE_ORDER_PARAMETER)
    {
        if (this->mainPlayerColor == unit->color)
        {
            // если выбирался параметр приказа, то всё нужно откатывать назад
            // и "недовыбранный" приказ убрать
            if (state == CHOOSE_ORDER_PARAMETER)
                breakChoosingOrderParameter();

            // если кликнутый юнит совпадает с выделенным
            if (selectedUnit == units[unit])
            {
                if (state == CHOOSE_ORDER_PARAMETER)  // перезапуск выбора типа приказа
                {
                    selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype));
                }
                else  // он перестаёт быть выделенным
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

                if (game->must_be_last(units[unit]->prototype, time))
                {
                    if (time < dayTime)
                        setTime(time);
                }
                else
                {
                    if (time + 1 < dayTime)
                        setTime(time + 1);
                }

                selectedUnit = units[unit];  // новый выбранный юнит
                selectedUnit->select();

                time = dayTime;  // убираем все планы на сейчас и далее
                while (time != game->rules->dayTimes)
                {
                    deplanOrder(selectedUnit->prototype, time);
                    time++;
                }

                selectedUnit->showOrdersPanel(game->whatCanUse(unit));
            }
        }
        else if (isSelectingEnemyUnit)
        {
            selectedUnit->prototype->plan[dayTime]->setParameter(unit);

            finishedChoosingOrderParameter();
        }
    }
}

void GameWindow::dayTimeClicked(DayTime time)
{
    if (state == PLANNING || state == CHOOSE_ORDER_PARAMETER ||
         state == WAIT_FOR_ENEMY_PLAN)
    {
        if (time != dayTime)
            setTime(time);
        else
            setTime(-1);
    }
}
void GameWindow::startUnitTypeClicked(bool on, QString type)
{
    if (state == CHOOSING_HEX && on)
    {
        int i = 0;
        while (startUnitsChoice->units[i]->type != type) { ++i; }

        int to_turn_off = 0;
        while (startUnitsChoice->units[to_turn_off]->type != game->chosenUnitType[mainPlayerColor]) { ++to_turn_off; }

        startUnitsChoice->units[to_turn_off]->turnOn(false);
        startUnitsChoice->units[i]->turnOn(true);

        game->chosenUnitType[mainPlayerColor] = type;
    }
}

void GameWindow::GoButtonPushed()
{
    if (state == CHOOSE_ORDER_PARAMETER)
        breakChoosingOrderParameter();

    go->enable(false);

    if (state == CHOOSING_HEX)
    {
        this->disableWholeField();

        sendPlan();
    }
    else if (state == PLANNING)
    {
        if (selectedUnit != NULL)
        {
            selectedUnit->deselect();
            selectedUnit->hidePanel();
            selectedUnit = NULL;
        }

        state = ARE_YOU_SURE_DIALOG;
        dialog->set(mainPlayerColor, "УВЕРЕНЫ?", false, true, true, false, "", "plan");
        resizeDialog(width(), height());
    }
}
void GameWindow::DialogReturned(bool isOk, QString sig_mes)
{
    if (ARE_YOU_SURE_DIALOG == state)
    {
        state = PLANNING;

        if (isOk)
        {
            sendPlan();
        }
        else
        {
            go->enable();
        }
    }
    if (sig_mes == "GiveUp" && isOk)
        giveup();
}

// общение с другими игроками
// на вход и выход передаются сообщения чисто местного протокола

#define GIVE_UP (qint8)1
#define MILLS_COORDINATES_MESSAGE (qint8)2
#define PLAN (qint8)3
void GameWindow::sendPlan()
{
    if (state == CHOOSING_HEX)
    {
        state = WAIT_FOR_ENEMY_START_POINT;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << MILLS_COORDINATES_MESSAGE
                  << PlayerIndex
                  << game->chosenHex[mainPlayerColor]
                  << game->rules->unitTypeIndex(game->chosenUnitType[mainPlayerColor]);
        emit writeToOpponent(message);

        game->ready[mainPlayerColor] = true;
        if (game->isEveryoneReady())
        {
            NextPhase();
        }
    }
    else if (state == PLANNING)
    {
        state = WAIT_FOR_ENEMY_PLAN;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << PLAN
                  << PlayerIndex;

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            for (int time = 0; time < game->rules->dayTimes; ++time)
            {
                // забиваем пустые места приказами безделья
                if (unit->plan[time] == NULL)
                {
                    unit->plan[time] = new GameOrder(game->rules, unit->type, DefaultOrder);
                    ++priorities[time];
                }

                // отправляем план
                GameOrder * order = unit->plan[time];
                write << game->rules->orderIndex(order->type);

                foreach (GameAction a, order->actions)
                {
                    if (a.whatParameter() == GameAction::PT_UNIT_TYPE)
                        write << game->rules->unitTypeIndex(a.unitType);
                    else if (a.whatParameter() == GameAction::PT_UNIT)
                        write << a.unit->id;
                    else if (a.whatParameter() == GameAction::PT_HEX)
                        write << a.target;
                }
            }
        }

        emit writeToOpponent(message);

        game->ready[mainPlayerColor] = true;
        if (game->isEveryoneReady())
        {
            NextPhase();
        }
    }
}
void GameWindow::readFromOpponent(QByteArray message)
{
    QDataStream in(&message, QIODevice::ReadOnly);
    qint8 Command;
    in >> Command;

    quint8 sender_index;
    in >> sender_index;

    if (Command == GIVE_UP)
    {
        dialog->set(mainPlayerColor, "Игрок " + game->rules->players[sender_index] + " сдался!", false, false, true);
        resizeDialog(width(), height());

        game->playerGiveUp(sender_index);
        if ((state == WAIT_FOR_ENEMY_PLAN ||
             state == WAIT_FOR_ENEMY_START_POINT) &&
                game->isEveryoneReady())
            NextPhase();
    }
    if (Command == MILLS_COORDINATES_MESSAGE)
    {        
        PlayerColor sender = game->rules->players[sender_index];

        in >> game->chosenHex[sender];

        qint8 unitTypeIndex;
        in >> unitTypeIndex;
        game->chosenUnitType[sender] = game->rules->unitsInGame[unitTypeIndex];
    }
    else if (Command == PLAN)
    {
        GamePlayer * sender = game->players[game->rules->players[sender_index]];

        for (int i = 0; i < sender->units.size(); ++i)
        {
            for (int time = 0; time < game->rules->dayTimes; ++time)
            {
                qint8 orderTypeIndex;
                in >> orderTypeIndex;

                sender->units[i]->plan[time] = new GameOrder(game->rules, sender->units[i]->type,
                                                             game->rules->getOrderByIndex(orderTypeIndex));

                for (int k = 0; k < sender->units[i]->plan[time]->actions.size(); ++k)
                {
                    GameAction * a = &sender->units[i]->plan[time]->actions[k];

                    GameAction::GameActionParameterType additionalInfo = a->whatParameter();
                    if (additionalInfo == GameAction::PT_UNIT_TYPE)
                    {
                        qint8 unitTypeIndex;
                        in >> unitTypeIndex;
                        a->unitType = game->rules->unitsInGame[unitTypeIndex];
                    }
                    else if (additionalInfo == GameAction::PT_UNIT)
                    {
                        qint16 unitId;
                        in >> unitId;
                        a->unit = game->getUnitById(unitId);
                    }
                    else if (additionalInfo == GameAction::PT_HEX)
                        in >> a->target;
                }
            }
        }
    }

    game->ready[game->rules->players[sender_index]] = true;
    if (game->isEveryoneReady())
    {
        NextPhase();
        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}
void GameWindow::NextButtonClicked()
{
    NextPhase();
}

// оповещения о событиях
void GameWindow::serverDisconnected()
{
    dialog->set(mainPlayerColor, "ОШИБКА!<br>Соединение с сервером разорвано! Пытаемся переподключиться...", false, false, true);
    resizeDialog(width(), height());

    qApp->alert(dynamic_cast<QWidget *>(this->parent()));
}
void GameWindow::opponentDisconnected(qint8 index)
{
    if (!game->players[game->rules->players[index]]->GiveUp)
    {
        dialog->set(mainPlayerColor, "Потеряно соединение с игроком " + game->rules->players[index] + "! Пытаемся переподключиться.", false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}
void GameWindow::opponentReconnected(qint8 index)
{
    if (!game->players[game->rules->players[index]]->GiveUp)
    {
        dialog->set(mainPlayerColor, "Игрок " + game->rules->players[index] + " успешно переподключился!", false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}

void GameWindow::giveup()
{
    if (!game->players[mainPlayerColor]->GiveUp)
    {
        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << GIVE_UP << PlayerIndex;
        emit writeToOpponent(message);

        game->playerGiveUp(PlayerIndex);
        if ((state == PLANNING || state == CHOOSING_HEX ||
             state == WAIT_FOR_ENEMY_PLAN ||
             state == WAIT_FOR_ENEMY_START_POINT) &&
             game->isEveryoneReady())
            NextPhase();
    }

    if (planned_to_go_home)  // что это?
        emit GoHome();
}

void GameWindow::whiteFlagClicked()
{
    if (game->players[mainPlayerColor]->GiveUp)
        return;

    prev_state = state;
    go->enable(false);
    planned_to_go_home = false;

    dialog->set(mainPlayerColor, "Сдаётесь?", false, true, true, false, "", "GiveUp");
    resizeDialog(width(), height());
}
void GameWindow::homeButtonClicked()
{
    if (game->players[mainPlayerColor]->GiveUp)
    {
        emit GoHome();
        return;
    }

    prev_state = state;
    go->enable(false);  // TODO возможно, тут сокрыта бага им. Козловцева
    planned_to_go_home = true;

    dialog->set(mainPlayerColor, "Сдаётесь?", false, true, true, false, "", "GiveUp");
    resizeDialog(width(), height());
}
