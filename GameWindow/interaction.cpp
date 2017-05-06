#include "Interaction.h"

Interaction::Interaction(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    StatesControl(game, PlayerIndex, parent)
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
        connect(DayTimeTable->DayTimePictures[i], SIGNAL(leftClicked(int)), SLOT(dayTimeClicked(int)));

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
void Interaction::unitConnections(Unit * unit)
{
    connect(unit, SIGNAL(entered(GameUnit*)), SLOT(unitHoverEntered(GameUnit*)));
    connect(unit, SIGNAL(left(GameUnit*)), SLOT(unitHoverLeft(GameUnit*)));
    connect(unit, SIGNAL(leftClicked(GameUnit*)), SLOT(unitLeftClicked(GameUnit*)));
    connect(unit, SIGNAL(orderVariantChosen(QString)), SLOT(orderVariantClicked(QString)));
    connect(unit, SIGNAL(unitTypeChosen(QString)), SLOT(unitTypeChosen(QString)));
}
void Interaction::orderPicsConnections(OrderPic *pic)
{
    connect(pic, SIGNAL(orderPicEntered(QString,QString)), SLOT(orderPicEntered(QString,QString)));
    connect(pic, SIGNAL(orderPicLeft(QString,QString)), SLOT(orderPicLeft(QString,QString)));
}

void Interaction::hexClicked(Coord c)
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
                    hex(game->chosenHex[mainPlayerColor])->select(false);

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
void Interaction::hexEntered(Coord c)
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

void Interaction::hexLeft(Coord c)
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

void Interaction::orderPicEntered(OrderType type, PlayerColor)
{
    foreach (Hex * hex, hexes)
    {
        hex->highlight(type);
    }
}
void Interaction::orderPicLeft(OrderType type, PlayerColor)
{
    foreach (Hex * hex, hexes)
    {
        hex->highlight(type, false);
    }
}

void Interaction::orderVariantClicked(OrderType type)
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
    GameOrder * order = new GameOrder(game->rules, selectedUnit->prototype->type, type, priorities[dayTime]);
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

void Interaction::unitTypeChosen(QString type)
{
    selectedUnit->prototype->plan[dayTime]->setParameter(type);
    finishedChoosingOrderParameter();
}

void Interaction::unitHoverEntered(GameUnit *unit)
{
    if (unit->home != NOWHERE)
        hex(unit->home)->showUnitHome(unit->color);
}
void Interaction::unitHoverLeft(GameUnit *unit)
{
    if (unit->home != NOWHERE)
        hex(unit->home)->hideUnitHome();
}
void Interaction::unitRightClicked(GameUnit *unit)
{
    return;
}
void Interaction::unitLeftClicked(GameUnit * unit)
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
                if (time < dayTime - 1)
                    setTime(time + 1);

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
    }
}

void Interaction::dayTimeClicked(DayTime time)
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

void Interaction::GoButtonPushed()
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
void Interaction::DialogReturned(bool isOk, QString sig_mes)
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
void Interaction::sendPlan()
{
    if (state == CHOOSING_HEX)
    {
        state = WAIT_FOR_ENEMY_START_POINT;

        QByteArray message;
        QDataStream write(&message, QIODevice::WriteOnly);
        write << MILLS_COORDINATES_MESSAGE
                  << PlayerIndex
                  << game->chosenHex[mainPlayerColor];
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

        for (int i = 0; i < game->players[mainPlayerColor]->units.size(); ++i)
        {
            for (int time = 0; time < game->rules->dayTimes; ++time)
            {
                // забиваем пустые места приказами безделья
                if (game->players[mainPlayerColor]->units[i]->plan[time] == NULL)
                {
                    game->players[mainPlayerColor]->units[i]->plan[time] = new GameOrder(game->rules, game->players[mainPlayerColor]->units[i]->type, DefaultOrder, priorities[time]);
                    ++priorities[time];
                }

                // отправляем план
                GameOrder * order = game->players[mainPlayerColor]->units[i]->plan[time];
                write << game->rules->orderIndex(order->type) << order->priority;

                foreach (GameAction a, order->actions)
                {
                    if (a.whatParameter() == 1)
                        write << game->rules->unitTypeIndex(a.unitType);
                    else if (a.whatParameter() == 2)
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
void Interaction::readFromOpponent(QByteArray message)
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
    }
    else if (Command == PLAN)
    {
        GamePlayer * sender = game->players[game->rules->players[sender_index]];

        for (int i = 0; i < sender->units.size(); ++i)
        {
            for (int time = 0; time < game->rules->dayTimes; ++time)
            {
                qint8 orderTypeIndex;
                qint16 priority;
                in >> orderTypeIndex >> priority;

                sender->units[i]->plan[time] = new GameOrder(game->rules, sender->units[i]->type, game->rules->getOrderByIndex(orderTypeIndex), priority);

                for (int k = 0; k < sender->units[i]->plan[time]->actions.size(); ++k)
                {
                    GameAction * a = &sender->units[i]->plan[time]->actions[k];

                    int additionalInfo = a->whatParameter();
                    if (additionalInfo == 1)
                    {
                        qint8 unitTypeIndex;
                        in >> unitTypeIndex;
                        a->unitType = game->rules->unitsInGame[unitTypeIndex];
                    }
                    else if (additionalInfo == 2)
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
void Interaction::NextButtonClicked()
{
    NextPhase();
}

// оповещения о событиях
void Interaction::serverDisconnected()
{
    dialog->set(mainPlayerColor, "ОШИБКА!<br>Соединение с сервером разорвано! Пытаемся переподключиться...", false, false, true);
    resizeDialog(width(), height());

    qApp->alert(dynamic_cast<QWidget *>(this->parent()));
}
void Interaction::opponentDisconnected(qint8 index)
{
    if (!game->players[game->rules->players[index]]->GiveUp)
    {
        dialog->set(mainPlayerColor, "Потеряно соединение с игроком " + game->rules->players[index] + "! Пытаемся переподключиться.", false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}
void Interaction::opponentReconnected(qint8 index)
{
    if (!game->players[game->rules->players[index]]->GiveUp)
    {
        dialog->set(mainPlayerColor, "Игрок " + game->rules->players[index] + " успешно переподключился!", false, false, true);
        resizeDialog(width(), height());

        qApp->alert(dynamic_cast<QWidget *>(this->parent()));
    }
}

void Interaction::giveup()
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

void Interaction::whiteFlagClicked()
{
    if (game->players[mainPlayerColor]->GiveUp)
        return;

    prev_state = state;
    go->enable(false);
    planned_to_go_home = false;

    dialog->set(mainPlayerColor, "Сдаётесь?", false, true, true, false, "", "GiveUp");
    resizeDialog(width(), height());
}
void Interaction::homeButtonClicked()
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
