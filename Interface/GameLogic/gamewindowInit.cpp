#include "GameWindow.h"

GameWindow::GameWindow(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    GraphicObject(parent)
{
    this->game = game;
    this->PlayerIndex = PlayerIndex;
    this->mainPlayerColor = game->rules->players[PlayerIndex];
    connect(game, SIGNAL(unitSetToField(GameUnit*)), SLOT(anchorUnitToField(GameUnit*)));
    connect(game, SIGNAL(blowUnit(GameUnit*)), SLOT(blowUnit(GameUnit*)));
    connect(game, SIGNAL(newResources(GameHex*,QString,QList<QString>,QList<bool>)), SLOT(newResources(GameHex*,QString,QList<QString>,QList<bool>)));

    connect(this, SIGNAL(writeToOpponent(QByteArray)), client, SLOT(sendToOpponent(QByteArray)));
    connect(client, SIGNAL(MessageToGame(QByteArray)), SLOT(readFromOpponent(QByteArray)));

    connect(animations, SIGNAL(mainAnimationFinished()), SLOT(mainAnimationFinished()));

    field = new FieldWindow(game, this);
    for (int i = 0; i < game->rules->fieldH; ++i)
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hexes[game->field[i][j]] = field->field->hexes[i][j];
            connect(hexes[game->field[i][j]], SIGNAL(clicked(int,int)), SLOT(hexClicked(int,int)));
        }
    }

    foreach (GamePlayer * player, game->players)
    {
        player_windows[player->color] = new PlayerResources(game, this, player);

        foreach (GameUnit* unit, player->units)
        {
            units[unit] = new Unit(unit, game, this, field->field, player->color == mainPlayerColor);
            unitConnections(units[unit]);
        }
    }

    go = new GoButton(this);
    go->setVisible(settings->PROGRAMM_VERSION == REAL_CLIENT);
    connect(go, SIGNAL(pushed(bool)), SLOT(GoButtonPushed(bool)));

    DayTimeTable = new DayTimePanel(this, game);
    DayTimeTable->setVisible(false);
    for (int i = 0; i < game->rules->dayTimes.size(); ++i)
        connect(DayTimeTable->DayTimePictures[i], SIGNAL(leftClicked(int)), SLOT(dayTimeClicked(int)));

    ArtificialPanel = new ArtificialControlPanel(this);  // Искусственная панель
    ArtificialPanel->setVisible(settings->PROGRAMM_VERSION == POWER_POINT_VERSION);
    UnitCreator = new ArtificialUnitCreator(this, game);  // Создание юнитов
    UnitCreator->setVisible(settings->PROGRAMM_VERSION == POWER_POINT_VERSION);

    startNextPhase();
}
void GameWindow::unitConnections(Unit * unit)
{    
    connect(unit, SIGNAL(entered(GameUnit*)), SLOT(unitHoverEntered(GameUnit*)));
    connect(unit, SIGNAL(left(GameUnit*)), SLOT(unitHoverLeft(GameUnit*)));
    connect(unit, SIGNAL(leftClicked(GameUnit*)), SLOT(unitLeftClicked(GameUnit*)));
    connect(unit, SIGNAL(rightClicked(GameUnit*)), SLOT(unitRightClicked(GameUnit*)));
    connect(unit, SIGNAL(orderVariantChosen(QString)), SLOT(orderVariantClicked(QString)));
    connect(unit, SIGNAL(unitTypeChosen(QString)), SLOT(unitTypeChosen(QString)));
}

void GameWindow::resize(qreal W, qreal H)
{
    constants->recountSizes(W, H, game->rules);  // Пересчёт всех констант!

    GraphicObject::resize(W, H);

    field->setGeometry(W * constants->playerPanelWidth,  // Поле
                       H * constants->upperPanelSize,
                       W * (1 - 2 * constants->playerPanelWidth),
                       H * (1 - constants->upperPanelSize - constants->downPanelSize));

    player_windows[mainPlayerColor]->setGeometry(0,  // Левая панель и её юниты
                                                       constants->upperPanelSize * H,
                                                       constants->playerPanelWidth * W,
                                                       constants->playerPanelHeight * H);

    int i = 0;  // Ищем правую панель
    while (game->rules->players[i] == mainPlayerColor) ++i;
    player_windows[game->rules->players[i]]->setGeometry(
                (1 - constants->playerPanelWidth) * W,
                constants->upperPanelSize * H,
                constants->playerPanelWidth * W,
                constants->playerPanelHeight * H);

    go->setGeometry(W * constants->artificialPanelX,  // кнопка "гоу!"
                    H * constants->artificialPanelY,
                    W * constants->artificialPanelSize,
                    H * constants->artificialPanelSize);

    DayTimeTable->setGeometry(W * (1 - constants->dayTimeTableWidth) / 2,
                              H * (1 - constants->downPanelSize + constants->downPanelSize * constants->dayTimeTableUpSpace),
                              W * constants->dayTimeTableWidth,
                              H * constants->downPanelSize * (1 - 2 * constants->dayTimeTableUpSpace));

    ArtificialPanel->setGeometry(W * constants->artificialPanelX,  // Искусственная панель
                                 H * constants->artificialPanelY,
                                 W * constants->artificialPanelSize,
                                 H * constants->artificialPanelSize);

    UnitCreator->setGeometry(W * constants->unitCreatorPanelX,
                             H * constants->unitCreatorPanelY,
                             W * constants->unitCreatorPanelSize,
                             H * constants->unitCreatorPanelSize);

    resizeUnits();
    resizeWays();
}

QPointF GameWindow::unitCoordinate(Unit * u)
{
    QPointF point = mapFromItem(hex(u), hex(u)->points[u->points_stack.top()]);
    return QPointF(point.x() - constants->unitsSize / 2,
                                point.y() - constants->unitsSize / 2);
}
void GameWindow::resizeUnits()
{
    foreach (PlayerColor color, game->rules->players)
        player_windows[color]->showUndefinedUnits(units);

    foreach (Unit * u, units)
    {
        if (u->prototype->x != UNDEFINED)
        {
            u->setPos(unitCoordinate(u));
            u->resize(constants->unitsSize, constants->unitsSize);
        }
    }
}

QPointF GameWindow::wayStartCoordinate(UnitWay *way)
{
    return mapFromItem(hex(way->from_x, way->from_y), 
            hex(way->from_x, way->from_y)->points[way->from_point]);
}
QPointF GameWindow::wayEndCoordinate(UnitWay *way)
{
    return mapFromItem(hex(way->to_x, way->to_y),
            hex(way->to_x, way->to_y)->points[way->to_point]);
}
void GameWindow::resizeWays()
{
    foreach (QStack <UnitWay *> list, ways)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayStartCoordinate(way),
                                          wayEndCoordinate(way));

            way->setGeometry(way->geometry);
            way->setRotation(way->rotation);
        }
    }
}

void GameWindow::allHexesToReconfigurePoints()
{
    foreach (Hex * hex, hexes)
    {
        hex->recountPoints();
    }
}
void GameWindow::reconfigureUnits()
{
    foreach (Unit * unit, units)
    {
        if (unit->prototype->x != UNDEFINED)
        {
            QPointF point = unitCoordinate(unit);
            unit->AnimationStart(X_POS, point.x(), constants->unitReconfigureTime, true);
            unit->AnimationStart(Y_POS, point.y(), constants->unitReconfigureTime, true);
        }
    }
}
void GameWindow::reconfigureWays()
{
    foreach (QStack <UnitWay *> list, ways)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayStartCoordinate(way),
                                          wayEndCoordinate(way));

            way->AnimationStart(way->geometry, constants->unitReconfigureTime);
            way->AnimationStart(ROTATION, way->rotation, constants->unitReconfigureTime);
        }
    }
}
