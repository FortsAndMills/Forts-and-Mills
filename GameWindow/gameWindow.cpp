#include "GameWindow.h"

GameWindow::GameWindow(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    GraphicObject(parent)
{
    this->game = game;
    this->PlayerIndex = PlayerIndex;
    this->mainPlayerColor = game->rules->players[PlayerIndex];

    field = new Field(this, game);
    for (int i = 0; i < game->rules->fieldH; ++i)
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hexes[game->hexes[i][j]] = field->hexes[i][j];
        }
    }

    foreach (GamePlayer * player, game->players)
    {
        foreach (GameUnit* unit, player->units)
        {
            units[unit] = new Unit(unit, game, this, player->color == mainPlayerColor);
        }
    }

    initInterface();
    initialConnections();

    game->StartGame();

    // это чтобы NextPhase вызвался уже после инициализации...
    QTimer::singleShot(1, this, SLOT(NextPhase()));
}
void GameWindow::Delete()
{
    foreach (QList<Rocket*> list, rockets)
        foreach (Rocket * R, list)
            R->Delete();
    foreach (QStack <UnitWay *> stack, ways_to)
        foreach (UnitWay * way, stack)
            way->Delete();
    foreach (QStack <UnitWay *> stack, ways_from)
        foreach (UnitWay * way, stack)
            way->Delete();
    foreach (QList <Fortification *> list, fortifications)
        foreach (Fortification * F, list)
            F->Delete();
    foreach (Order * Or, orders)
        if (Or != NULL)
            Or->Delete();
    foreach (Unit * u, units)
        u->Delete();
    field->Delete();
    delete game;
    deleteInterface();
    GraphicObject::Delete();
}

void GameWindow::NextPhase()
{
    if (isRealizingFinished)  // завершение обработки очередного события
    {
        game->events.removeAt(0);
        Substate = 0;

        if (game->events.size() == 0)
        {
            debug << "FATAL ERROR: NO EVENTS!!!!!!\n";
            return;
        }

        MixEvents();
    }

    // реализуем очередное действие и делаем то, что попросили
    WaitingType WT = this->RealizeEvent();
    isRealizingFinished = WT.finished_this_event;

    if (WT.type == NO_WAIT)
        QTimer::singleShot(WT.time, this, SLOT(NextPhase()));
    else if (WT.type == BUTTON)
        QTimer::singleShot(WT.time, next, SLOT(enableWithCheck()));
}

void GameWindow::MixEvents()
{
    // перемешивание происходит только когда очередное событие - начало реализации очередного времени дня
    if (game->events[0]->type == TIME_STARTED)
    {
        QStack <GameUnit *> hero;  // чьи приказы реализовываются
        QStack <GameMessage *> heroEventLimit;  // до какого приказа нужно разыграть героя

        int base = 1;
        while (game->events[base]->involved().size() > 0)
        {
            bool Permission = false;
            int e;

            do
            {
                e = base;
                // ищем нового героя среди своих, если героев нет
                while (hero.isEmpty() && !game->events[e]->involved().isEmpty())
                {
                    QSet <GameUnit *> inv = game->events[e]->involved();
                    QSet <GameUnit *>::iterator it = inv.begin();
                    while (it != inv.end() && (*it)->color != mainPlayerColor)
                    {
                        ++it;
                    }

                    if (it != inv.end())
                    {
                        hero.push((*it));
                        heroEventLimit.push(lastDayTimeEvent(base));
                    }
                    else
                        ++e;
                }

                // иначе герой - первый попавшийся
                if (hero.isEmpty())
                {
                    e = base;
                    hero.push(game->events[e]->unit);
                    heroEventLimit.push(lastDayTimeEvent(base));

                    if (game->events[e]->unit == NULL)
                    {
                        qDebug() << "ERROR: Hero is NULL" << Qt::endl;
                    }
                }

                // ищем событие, связанное с нашим героем
                while (game->events[e] != heroEventLimit.top() &&
                           !game->events[e]->involved().contains(hero.top()))
                {
                    ++e;
                }

                // не нашли - с этим героем работа закончена, ищем заново
                Permission = true;
                if (game->events[e] == heroEventLimit.top())
                {
                    Permission = false;
                    hero.pop();
                    heroEventLimit.pop();
                }
            }
            while (!Permission);

            // всё, мы хотим выполнить event()
            // но, возможно, в нём участвуют ещё лица!
            Permission = false;
            while (!Permission)
            {
                Permission = true;
                foreach (GameUnit * part, game->events[e]->involved())
                {
                    if (part != hero.top())
                    {
                        int i = base;
                        while (i < e && !game->events[i]->involved().contains(part))
                            ++i;

                        if (i < e)
                        {
                            hero.push(part);
                            heroEventLimit.push(lastDayTimeEvent(base));

                            Permission = false;
                            e = i;
                            break;
                        }
                    }
                }
            }

            // event() найден, переставляем в начало списка!
            GameMessage * sh = game->events[e];
            for (int i = e; i > base; --i)
                game->events[i] = game->events[i - 1];
            game->events[base] = sh;

            ++base;
        }
    }
}

// вспомогательная функция нахождения последнего события раунда
GameMessage *GameWindow::lastDayTimeEvent(int base)
{
    int i = base;
    while (game->events[i]->type != TIME_FINISHED) //involved().isEmpty())
        ++i;
    return game->events[i];
}
