#include "GameWindow.h"

void GameWindow::initInterface()
{
    setOpacity(0);
    leftPanel = "";
    foreach (GamePlayer * player, game->players)
    {
        player_windows[player->color] = new PlayerResources(game, this, player, player->color == mainPlayerColor);
        if (leftPanel == "" && player->color != mainPlayerColor)
            leftPanel = player->color;

        if (player->color != mainPlayerColor)
        {
            leftPanelSwitchers << new LeftPanelSwitcher(this, player->color);
            connect(leftPanelSwitchers.last(), SIGNAL(leftPanelSwitcherClicked(QString)), SLOT(leftPanelSwitcherClicked(QString)));
        }

        foreach (GameUnit* unit, player->units)
        {
            units[unit] = new Unit(unit, game, this, player->color == mainPlayerColor);
        }
    }
    if (leftPanelSwitchers.size() < 2)
        foreach (LeftPanelSwitcher * lps, leftPanelSwitchers)
            lps->setVisible(false);

    go = new GoButton(this, mainPlayerColor);

    next = new NextButton(this, mainPlayerColor);
    next->setOpacity(0);

    dayTimeTable = new DayTimePanel(this, game->rules->dayTimes);
    dayTimeTable->setVisible(false);

    startUnitsChoice = new UnitsTune(this, game->rules, mainPlayerColor, game->chosenUnitType[mainPlayerColor]);

    timer = new Timer(this);
    timer->setVisible(false);

    fieldControl = new FieldWindow(this, field);

    startChoiceProgress = new StartChoiceProgress(this, mainPlayerColor, game->rules->start_choices);

    whiteFlag = new EnlargingButton(this, "WhiteFlag");
    homeButton = new EnlargingButton(this, "HomeButton");
    question = new EnlargingButton(this, "Question");

    hexCopy = NULL;

    dialog = new Dialog(this, game->rules);
}
void GameWindow::deleteInterface()
{
    foreach (PlayerResources * pr, player_windows)
        pr->Delete();
    foreach (LeftPanelSwitcher * lps, leftPanelSwitchers)
        lps->Delete();
    go->Delete();
    next->Delete();
    timer->Delete();
    dayTimeTable->Delete();
    startUnitsChoice->Delete();
    fieldControl->Delete();
    whiteFlag->Delete();
    homeButton->Delete();
    question->Delete();
    startChoiceProgress->Delete();

    if (hexCopy != NULL)
        hexCopy->Delete();
    if (dialog != NULL)
        dialog->Delete();
}

QRectF GameWindow::leftPanelGeometry()
{
    return QRectF((1 - constants->playerPanelWidth) * width(),
                  constants->upperPanelSize * height(),
                  constants->playerPanelWidth * width(),
                  constants->playerPanelHeight * height());
}
QRectF GameWindow::closedLeftPanelGeometry()
{
    return QRectF(width(),
                   constants->upperPanelSize * height(),
                   constants->playerPanelWidth * width(),
                   constants->playerPanelHeight * height());
}
void GameWindow::resizeLeftPanelSwitchers(qreal W, qreal H)
{
    int n = leftPanelSwitchers.size();
    qreal wid = constants->playerPanelWidth * W / (n + (n + 1) * constants->leftPanelSwitchersMargin);

    qreal X = (1 - constants->playerPanelWidth) * W + wid * constants->leftPanelSwitchersMargin;
    for (int i = 0; i < n; ++i)
    {
        leftPanelSwitchers[i]->setGeometry(X, constants->leftPanelSwitchersY * H,
                                                                    wid, constants->leftPanelSwitchersHeight * H);
        X += wid * (1 + constants->leftPanelSwitchersMargin);
    }
}
void GameWindow::resizeDialog(qreal W, qreal H)
{
    dialog->setGeometry(W * (1 / 2.0 - constants->dialogWidth / 2),
                                       H * (1 / 2.0 - constants->dialogHeight / 2),
                                       W * constants->dialogWidth,
                                       H * constants->dialogHeight);
}

void GameWindow::resize(qreal W, qreal H)
{
    constants->recountSizes(W, H, game->rules);  // Пересчёт всех констант!

    GraphicObject::resize(W, H);

    field->setGeometry(constants->fieldXPos,
                                     constants->fieldYPos,
                                     constants->fieldWidth,
                                     constants->fieldHeight);

    player_windows[mainPlayerColor]->setGeometry(0,
                                                       constants->upperPanelSize * H,
                                                       constants->playerPanelWidth * W,
                                                       constants->playerPanelHeight * H);

    foreach (PlayerResources * pr, player_windows)
    {
        if (pr->prototype->color == leftPanel)
        {
            pr->setGeometry(leftPanelGeometry());
        }
        else if (pr->prototype->color != mainPlayerColor)
        {
            pr->setGeometry(closedLeftPanelGeometry());
        }
    }
    resizeLeftPanelSwitchers(W, H);

    go->setGeometry(W * constants->goX,
                    H * constants->goY,
                    H * constants->goSize,
                    H * constants->goSize);

    next->setGeometry(W * constants->goX,
                      H * constants->goY,
                      H * constants->goSize,
                      H * constants->goSize);

    timer->setGeometry(W * constants->timerX,
                       H * constants->timerY,
                       W * constants->timerWidth,
                       H * constants->timerHeight);

    resizeDayTimeTable(W, H);

    fieldControl->setGeometry(W * (constants->goX - constants->shiftBetweenGoAndFieldControl - constants->fieldControlWidth),
                                                 H * (1 - constants->downPanelSize / 2 - constants->downPanelSize * constants->fieldControlHeight / 2),
                                                 W * constants->fieldControlWidth,
                                                 H * constants->downPanelSize * constants->fieldControlHeight);

    startChoiceProgress->setGeometry(W / 2 * (1 - constants->startChoiceProgressBarWidth),
                                     H * constants->startChoiceProgressBarY,
                                     W * constants->startChoiceProgressBarWidth,
                                     H * constants->startChoiceProgressBarHeight);

    whiteFlag->setGeometry(constants->whiteFlagX * W,
                          (constants->upperPanelSize - constants->whiteFlagHeight) * H,
                          constants->whiteFlagWidth * W,
                          constants->whiteFlagHeight * H);
    homeButton->setGeometry(constants->homeButtonX * W,
                          (constants->upperPanelSize - constants->homeButtonHeight) * H,
                          constants->homeButtonWidth * W,
                          constants->homeButtonHeight * H);
    question->setGeometry(constants->gameQuestionButtonX * W,
                          (constants->upperPanelSize - constants->gameQuestionButtonHeight) * H,
                          constants->gameQuestionButtonWidth * W,
                          constants->gameQuestionButtonHeight * H);

    if (hexCopy != NULL)
    {
        resizeHexCopy();
    }

    if (dialog != NULL)
    {
        resizeDialog(W, H);
    }

    resizeUnits();
    resizeWays();
    resizeRockets();
    resizeFortifications();
}
void GameWindow::resizeHexCopy()
{
    hexCopy->setGeometry(constants->hexCopyPointX,
                                           constants->hexCopyPointY,
                                           constants->hexCopyWidth,
                                           constants->hexCopyHeight);
}
void GameWindow::resizeDayTimeTable(qreal W, qreal H)
{
    int n = dayTimeTable->DayTimePictures.size();
    qreal width = constants->dayTimePictureWidth * (n + constants->dayTimeTableMargin * (n - 1));

    dayTimeTable->setGeometry(W * (1 - width) / 2,
                              H * (1 - constants->downPanelSize + constants->downPanelSize * constants->dayTimeTableUpSpace),
                              W * width,
                              H * constants->downPanelSize * (1 - 2 * constants->dayTimeTableUpSpace));

    // TODO нормально оформить
    n = game->rules->unitsInGame.size();
    width = constants->dayTimePictureWidth * (n + constants->dayTimeTableMargin * (n - 1));

    startUnitsChoice->setGeometry(W * (1 - width) / 2,
                              H * (1 - constants->downPanelSize + constants->downPanelSize * constants->dayTimeTableUpSpace),
                              W * width,
                              H * constants->downPanelSize * (1 - 2 * constants->dayTimeTableUpSpace));
}

void GameWindow::leftPanelSwitcherClicked(QString name)
{
    if (name == mainPlayerColor)
        return;

    // анимировать ширину и высоту здесь нельзя
    // от этого происходит ресайз ресурсов в таблице
    player_windows[leftPanel]->AnimationStart(X_POS, closedLeftPanelGeometry().x(), constants->panelsChangeTime);
    leftPanel = name;
    player_windows[leftPanel]->AnimationStart(X_POS, leftPanelGeometry().x(), constants->panelsChangeTime);
}
