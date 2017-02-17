#include "InterfaceOrganization.h"

InterfaceOrganization::InterfaceOrganization(Game * game, qint8 PlayerIndex, GraphicObject * parent) :
    GameElementsOrganization(game, PlayerIndex, parent)
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

    DayTimeTable = new DayTimePanel(this, game->rules->dayTimes);
    DayTimeTable->setVisible(false);

    fieldControl = new FieldWindow(this, field);

    whiteFlag = new SpecialButton(this, "WhiteFlag");
    homeButton = new SpecialButton(this, "HomeButton");

    hexCopy = NULL;

    dialog = new Dialog(this, game->rules);
}
void InterfaceOrganization::Delete()
{
    foreach (PlayerResources * pr, player_windows)
        pr->Delete();
    foreach (LeftPanelSwitcher * lps, leftPanelSwitchers)
        lps->Delete();
    go->Delete();
    next->Delete();
    DayTimeTable->Delete();
    fieldControl->Delete();
    whiteFlag->Delete();
    homeButton->Delete();

    if (hexCopy != NULL)
        hexCopy->Delete();
    if (dialog != NULL)
        dialog->Delete();

    GameElementsOrganization::Delete();
}

QRectF InterfaceOrganization::leftPanelGeometry()
{
    return QRectF((1 - constants->playerPanelWidth) * width(),
                  constants->upperPanelSize * height(),
                  constants->playerPanelWidth * width(),
                  constants->playerPanelHeight * height());
}
QRectF InterfaceOrganization::closedLeftPanelGeometry()
{
    return QRectF(width(),
                   constants->upperPanelSize * height(),
                   constants->playerPanelWidth * width(),
                   constants->playerPanelHeight * height());
}
void InterfaceOrganization::resizeLeftPanelSwitchers(qreal W, qreal H)
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
void InterfaceOrganization::resizeDialog(qreal W, qreal H)
{
    dialog->setGeometry(W * (1 / 2.0 - constants->dialogWidth / 2),
                                       H * (1 / 2.0 - constants->dialogHeight / 2),
                                       W * constants->dialogWidth,
                                       H * constants->dialogHeight);
}

void InterfaceOrganization::resize(qreal W, qreal H)
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
                    W * constants->goSize,
                    H * constants->goSize);

    next->setGeometry(W * constants->goX,
                    H * constants->goY,
                    W * constants->goSize,
                    H * constants->goSize);

    resizeDayTimeTable(W, H);

    fieldControl->setGeometry(W * (constants->goX - constants->shiftBetweenGoAndFieldControl - constants->fieldControlWidth),
                                                 H * (1 - constants->downPanelSize / 2 - constants->downPanelSize * constants->fieldControlHeight / 2),
                                                 W * constants->fieldControlWidth,
                                                 H * constants->downPanelSize * constants->fieldControlHeight);

    whiteFlag->setGeometry(constants->whiteFlagX * W,
                                             (constants->upperPanelSize - constants->whiteFlagHeight) * H,
                                             constants->whiteFlagWidth * W,
                                             constants->whiteFlagHeight * H);
    homeButton->setGeometry(constants->homeButtonX * W,
                                             (constants->upperPanelSize - constants->homeButtonHeight) * H,
                                             constants->homeButtonWidth * W,
                                             constants->homeButtonHeight * H);

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
void InterfaceOrganization::resizeHexCopy()
{
    hexCopy->setGeometry(constants->hexCopyPointX,
                                           constants->hexCopyPointY,
                                           constants->hexCopyWidth,
                                           constants->hexCopyHeight);
}
void InterfaceOrganization::resizeDayTimeTable(qreal W, qreal H)
{
    int n = DayTimeTable->DayTimePictures.size();
    qreal width = constants->dayTimePictureWidth * (n + constants->dayTimeTableMargin * (n - 1));

    DayTimeTable->setGeometry(W * (1 - width) / 2,
                              H * (1 - constants->downPanelSize + constants->downPanelSize * constants->dayTimeTableUpSpace),
                              W * width,
                              H * constants->downPanelSize * (1 - 2 * constants->dayTimeTableUpSpace));
}

void InterfaceOrganization::leftPanelSwitcherClicked(QString name)
{
    if (name == mainPlayerColor)
        return;

    player_windows[leftPanel]->AnimationStart(closedLeftPanelGeometry(), constants->panelsChangeTime);
    leftPanel = name;
    player_windows[leftPanel]->AnimationStart(leftPanelGeometry(), constants->panelsChangeTime);
}
