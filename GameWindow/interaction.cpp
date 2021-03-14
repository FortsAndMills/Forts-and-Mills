#include "GameWindow.h"
#include "Technical/dialogtext.h"

void GameWindow::initialConnections()
{
    connect(this, SIGNAL(writeToOpponent(QByteArray)), client, SLOT(sendToOpponent(QByteArray)));
    connect(client, SIGNAL(MessageToGame(QByteArray)), SLOT(readFromOpponent(QByteArray)));
    connect(client, SIGNAL(OpponentDisconnected(qint8)), SLOT(opponentDisconnected(qint8)));
    connect(client, SIGNAL(OpponentReconnected(qint8)), SLOT(opponentReconnected(qint8)));
    connect(client, SIGNAL(disconnected()), SLOT(serverDisconnected()));
    connect(client, SIGNAL(SuccesfullyReconnected()), SLOT(succesfullyReconnected()));

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
    connect(question, SIGNAL(leftClicked()), SLOT(questionClicked()));
    connect(timer, SIGNAL(expired()), SLOT(timerExpired()));

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
    connect(unit, SIGNAL(orderVariantChosen(Game::PossibleChoice)), SLOT(orderVariantClicked(Game::PossibleChoice)));
    connect(unit, SIGNAL(unitTypeChosen(Game::PossibleChoice)), SLOT(unitTypeChosen(Game::PossibleChoice)));
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
            debug << "ERROR: choosen forbidden hex!\n";
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

    foreach (Unit * unit, units)
        if (unit->prototype->home == c)
            unit->light();
}
void GameWindow::hexLeft(Coord c)
{
    if (hexCopy != NULL &&
            hexCopy->prototype->coord == c)
    {
        hexCopy->Delete();
        hexCopy = NULL;
    }

    foreach (Unit * unit, units)
        if (unit->prototype->home == c)
            unit->light(false);
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

void GameWindow::orderVariantClicked(Game::PossibleChoice pos_order)
{
    if (selectedUnit == NULL)
        debug << "FatalError: no selected unit, but there is order variant!!!\n";

    // если выбран невозможный приказ, для которого нужно вывести пояснение
    if (pos_order.status != Game::POSSIBLE)
    {
        if (pos_order.status == Game::CANT_RECRUIT_ALREADY_PROVIDED)
        {
           emit help->HelpAsked("cantrecruit_already_provided");
        }
        else if (pos_order.status == Game::CANT_RECRUIT_NOT_CAPTURED)
        {
           emit help->HelpAsked("cantrecruit_not_captured");
        }
        else if (pos_order.status == Game::CANT_RECRUIT_NOT_CONNECTED)
        {
           emit help->HelpAsked("cantrecruit_not_connected");
        }
        else if (pos_order.status == Game::CANT_CAPTURE_NOT_LIBERATED)
        {
           emit help->HelpAsked("cantcapture_not_liberated");
        }
        return;
    }

    OrderType type = pos_order.variant;

    // если выбран "удалить последний приказ"
    if (type == DeleteLastOrder)
    {
        state = PLANNING;
        selectedUnit->hidePanel();
        setPreviousTime();  // откатываемся на прошлый шаг во времени

        // sanity check
        if (selectedUnit->prototype->plan[dayTime] == NULL)
        {
            debug << "FATAL ERROR: canceled order, but it... does not exist!!!\n";
            return;
        }

        // отменяем последний приказ
        deplanOrder(selectedUnit->prototype, dayTime);

        // снова показываем панель
        selectedUnit->showOrdersPanel(game->whatCanUse(selectedUnit->prototype));
        return;
    }

    state = CHOOSE_ORDER_PARAMETER;

    if (selectedUnit->prototype->plan[dayTime] != NULL)
    {
        debug << "ERROR: order on order!!!\n";
        delete selectedUnit->prototype->plan[dayTime];
    }

    // создаём новый приказ выбранного типа
    GameOrder * order = new GameOrder(game->rules, selectedUnit->prototype->type, type);
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
void GameWindow::unitTypeChosen(Game::PossibleChoice pos_unit)
{
    selectedUnit->prototype->plan[dayTime]->setParameter(pos_unit.variant);
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
        else if (hex(unit->position)->is_clickable())
        {
            hexClicked(unit->position);
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
void GameWindow::startUnitTypeClicked(bool on, UnitType type)
{
    if (state == CHOOSING_HEX && on)
    {
        // TODO: а есть же функция getUnitTypebyIndex или что-то такое?
        int i = 0;
        while (startUnitsChoice->units[i]->type != type) { ++i; }

        int to_turn_off = 0;
        while (startUnitsChoice->units[to_turn_off]->type != game->chosenUnitType[mainPlayerColor]) { ++to_turn_off; }

        startUnitsChoice->units[to_turn_off]->turnOn(false);
        startUnitsChoice->units[i]->turnOn(true);

        game->chosenUnitType[mainPlayerColor] = type;
    }
}

void GameWindow::timerExpired()
{
    if (game->players[mainPlayerColor]->status != GamePlayer::ALIVE)
        return;

    bool forced_plan = false;

    if (state == CHOOSING_HEX)
    {
        // принудительная отправка плана
        forced_plan = true;

        // придётся выбрать случайную клетку
        if (game->chosenHex[mainPlayerColor] == NOWHERE)
        {
            QList <Coord> vars;
            for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
            {
                for (int j = 0; j < game->rules->fieldW; ++j)
                {
                    if (hex(i, j)->is_clickable())
                    {
                        vars << Coord(i, j);
                    }
                }
            }
            game->chosenHex[mainPlayerColor] = vars[game->rand->next() % vars.size()];
        }

        this->disableWholeField();
    }

    if (state == CHOOSE_ORDER_PARAMETER)
        breakChoosingOrderParameter();

    if (state == ARE_YOU_SURE_DIALOG)
    {
        dialog->cancelClicked();
    }

    if (state == PLANNING)
    {
        forced_plan = true;

        if (selectedUnit != NULL)
        {
            selectedUnit->deselect();
            selectedUnit->hidePanel();
            selectedUnit = NULL;
        }
    }

    if (state == REALIZATION_PHASE)
    {
        forced_plan = true;
    }

    if (forced_plan)
    {
        sendPlan();

        if (game->players[mainPlayerColor]->status == GamePlayer::ALIVE)
        {
            dialog->set(mainPlayerColor, dialogtext->get("timeisup"), false, true, false, false, "", "timerexpired");
            resizeDialog(width(), height());
        }
    }
}

void GameWindow::CheckNextPhase()
{
    if (game->isEveryoneReady())
    {
        // равен true если мы просматривали исход предыдущего раунда,
        // а уже нужно обсчитать следующий раунд потому что время вышло
        bool trigger_next_phase = game->events.size() == 1;

        // переход из режима планирования в режим просмотра
        if (state == PLANNING || state == CHOOSE_ORDER_PARAMETER ||
            state == WAIT_FOR_ENEMY_PLAN || state == ARE_YOU_SURE_DIALOG)
        {
            getReadyToRealization();
        }

        // логгируем игру в файлик
        if (state == REALIZATION_PHASE)
            log->logPlan();
        else
            log->logMillChoice();

        // запускаем следующий этап игры
        int time = game->NextStage();

        // оповещение в трее
        qApp->alert(dynamic_cast<QWidget *>(this->parent()));

        // запускаем таймер
        timer->launch(time);

        // триггерим обработку следующего события
        if (trigger_next_phase)
            NextPhase();
    }
}

void GameWindow::GoButtonPushed()
{
    if (state == CHOOSE_ORDER_PARAMETER)
        breakChoosingOrderParameter();

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
        dialog->set(mainPlayerColor, dialogtext->get("areyousure"), false, true, true, false, "", "plan");
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

    if (isOk)
    {
        if (sig_mes == "GiveUp")
        {
            giveup();
        }
        else if (sig_mes == "GoHome")
        {
            giveup();
            emit GoHome();
        }
        else if (sig_mes == "Quit")
        {
            giveup();
            emit DecidedToQuit();
        }
    }
}

// общение с другими игроками
// на вход и выход передаются сообщения чисто местного протокола

#define GIVE_UP (qint8)1
#define START_COORDINATES_MESSAGE (qint8)2
#define PLAN (qint8)3
void GameWindow::sendPlan()
{
    go->enable(false);

    if (state == CHOOSING_HEX)
    {
        state = WAIT_FOR_ENEMY_START_POINT;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << START_COORDINATES_MESSAGE
                  << PlayerIndex
                  << game->chosenHex[mainPlayerColor]
                  << game->rules->unitTypeIndex(game->chosenUnitType[mainPlayerColor]);
        emit writeToOpponent(message);

        // запускаем таймер по готовности оппонента.
        timer->activate_on_opponent_plan_received(game);

        game->ready[mainPlayerColor] = true;        
        CheckNextPhase();
    }
    else if (state == PLANNING || state == REALIZATION_PHASE)
    {
        if (state == PLANNING)
            state = WAIT_FOR_ENEMY_PLAN;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << PLAN << PlayerIndex;

        foreach (GameUnit * unit, game->players[mainPlayerColor]->units)
        {
            for (int time = 0; time < game->rules->dayTimes; ++time)
            {
                // забиваем пустые места приказами безделья
                if (unit->plan[time] == NULL)
                {
                    unit->plan[time] = new GameOrder(game->rules, unit->type, DefaultOrder);
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

        // запускаем таймер по готовности оппонента.
        timer->activate_on_opponent_plan_received(game);

        game->ready[mainPlayerColor] = true;
        CheckNextPhase();
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
        dialog->set(mainPlayerColor, dialogtext->playerGaveUp(game->rules->players[sender_index]), false, false, true);
        resizeDialog(width(), height());

        game->playerGiveUp(sender_index);
    }

    if (Command == START_COORDINATES_MESSAGE)
    {        
        PlayerColor sender = game->rules->players[sender_index];

        in >> game->chosenHex[sender];

        qint8 unitTypeIndex;
        in >> unitTypeIndex;
        game->chosenUnitType[sender] = game->rules->unitsInGame[unitTypeIndex];

        // запускаем таймер по готовности оппонента.
        timer->activate_on_opponent_plan_received(game);
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

        // запускаем таймер по готовности оппонента.
        timer->activate_on_opponent_plan_received(game);
    }

    game->ready[game->rules->players[sender_index]] = true;
    CheckNextPhase();
}
void GameWindow::NextButtonClicked()
{
    NextPhase();
}

// оповещения о событиях
void GameWindow::serverDisconnected()
{
    dialog->set(mainPlayerColor, dialogtext->get("connectionerror"), false, false, true);
    resizeDialog(width(), height());

    qApp->alert(dynamic_cast<QWidget *>(this->parent()));
}
void GameWindow::succesfullyReconnected()
{
    dialog->set(mainPlayerColor, dialogtext->get("Ireconnected"), false, false, true);
    resizeDialog(width(), height());

    qApp->alert(dynamic_cast<QWidget *>(this->parent()));
}
void GameWindow::opponentDisconnected(qint8 index)
{
    if (game->players[game->rules->players[index]]->status == GamePlayer::ALIVE)
    {
        dialog->set(mainPlayerColor, dialogtext->lostConnection(game->rules->players[index]), false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}
void GameWindow::opponentReconnected(qint8 index)
{
    if (game->players[game->rules->players[index]]->status == GamePlayer::ALIVE)
    {
        dialog->set(mainPlayerColor, dialogtext->otherPlayerReconnected(game->rules->players[index]), false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}

void GameWindow::giveup()
{
    // если уже сдался или игра закончилась
    if (game->players[mainPlayerColor]->status != GamePlayer::ALIVE ||
        game->winner != "Neutral")
        return;

    if (state == CHOOSING_HEX || state == WAIT_FOR_ENEMY_START_POINT)
    {
        // отключаем выбранный гекс
        if (game->chosenHex[mainPlayerColor] != NOWHERE)
        {
            hex(game->chosenHex[mainPlayerColor])->select(false);
            game->chosenHex[mainPlayerColor] = NOWHERE;
        }

        // выключаем возможность кликать на поле
        disableWholeField();

        // убираем панели
        go->AnimationStart(OPACITY, 0, constants->goButtonAppearTime);
        startUnitsChoice->AnimationStart(OPACITY, 0);
    }

    // отправка сообщения
    if (game->players[mainPlayerColor]->status == GamePlayer::ALIVE)
    {
        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << GIVE_UP << PlayerIndex;
        emit writeToOpponent(message);

        game->playerGiveUp(PlayerIndex);
        CheckNextPhase();
    }
}

void GameWindow::ask_for_close()
{
    if (game->players[mainPlayerColor]->status != GamePlayer::ALIVE ||
        game->winner != "Neutral")
    {
        emit DecidedToQuit();
        return;
    }

    dialog->set(mainPlayerColor, dialogtext->get("askgiveup"), false, true, true, false, "", "Quit");
    resizeDialog(width(), height());
}
void GameWindow::whiteFlagClicked()
{
    if (game->players[mainPlayerColor]->status != GamePlayer::ALIVE)
        return;

    dialog->set(mainPlayerColor, dialogtext->get("askgiveup"), false, true, true, false, "", "GiveUp");
    resizeDialog(width(), height());
}
void GameWindow::homeButtonClicked()
{
    if (game->players[mainPlayerColor]->status != GamePlayer::ALIVE ||
        game->winner != "Neutral")
    {
        emit GoHome();
        return;
    }

    dialog->set(mainPlayerColor, dialogtext->get("askgiveup"), false, true, true, false, "", "GoHome");
    resizeDialog(width(), height());
}
void GameWindow::questionClicked()
{
    help->HelpAsked("HowToAskHelp");
}
