#include "Game.h"

QList<OrderType> Game::whatCanUse(GameUnit * unit, QMap<Resource, int> spend)
{
    QList <OrderType> ans;
    foreach (OrderType order, rules->ordersInGame)
    {
        if (players[unit->color]->resources[order] > spend[order])
            ans << order;
    }
    ans << "Wait";
    return ans;
}

WAY Game::oppositeWay(WAY way)
{
    if (way == "UP")
        return "DOWN";
    if (way == "DOWN")
        return "UP";
    if (way == "RIGHT_UP")
        return "LEFT_DOWN";
    if (way == "RIGHT_DOWN")
        return "LEFT_UP";
    if (way == "LEFT_UP")
        return "RIGHT_DOWN";
    if (way == "LEFT_DOWN")
        return "RIGHT_UP";

    qDebug() << "ERROR: opposite way error!";
    return "";
}
WAY Game::whereIs(int x, int y, int from_x, int from_y)
{
    QMap<WAY, QPair<int, int> > adjacent = adjacentHexesMap(from_x, from_y);

    for (QMap<WAY, QPair<int, int> >::iterator it = adjacent.begin(); it != adjacent.end(); ++it)
    {
        if (it.value() == QPair<int, int>(x, y))
            return it.key();
    }

    qDebug() << "FATAL: WAY determination error!";
    return "";
}
QPair<int, int> Game::adjacentHex(int x, int y, QString way)
{
    if (way == "UP" ||
        (way == "RIGHT_UP" && y % 2 == 0) ||
        (way == "LEFT_UP" && y % 2 == 0))
       x  = (x - 1 + rules->fieldH) % rules->fieldH;
    else if (way == "DOWN" ||
             (way == "RIGHT_DOWN" && y % 2) ||
             (way == "LEFT_DOWN" && y % 2))
       x = (x + 1) % rules->fieldH;

    if (way == "LEFT_DOWN" || way == "LEFT_UP")
       y = (y - 1 + rules->fieldW) % rules->fieldW;
    else if (way == "RIGHT_DOWN" || way == "RIGHT_UP")
       y = (y + 1) % rules->fieldW;

    return QPair<int, int>(x, y);
}
QMap<WAY, QPair<int, int> > Game::adjacentHexesMap(int x, int y)
{
    QMap <WAY, QPair <int, int> > ans;

    foreach (QString way, WAYS)
    {
        ans[way] = adjacentHex(x, y, way);
    }

    return ans;
}
QSet<QPair<int, int> > Game::adjacentHexes(int x, int y)
{
    QSet <QPair <int, int> > ans;

    foreach (QString way, WAYS)
    {
        ans << adjacentHex(x, y, way);
    }

    return ans;
}
QSet<QPair<int, int> > Game::visible_hexes(int my_x, int my_y, int radius)
{
    QSet <QPair <int, int> > ans;

    foreach (QString way, WAYS)
    {
        int k = 0;
        QPair<int, int> coord = QPair<int, int>(my_x, my_y);

        do
        {
            coord = adjacentHex(coord, way);
            ans << coord;
            ++k;
        }
        while (k < radius && !field[coord.first][coord.second]->isBuilding);
    }

    return ans;
}

void Game::destroyUnit(GameUnit *unit)
{
    players[unit->color]->units.removeAll(unit);
    emit blowUnit(unit);
}

void Game::captureHex(int i, int j, PlayerColor color)
{
    // ПРОВЕРКА НА КОРРЕКТНОСТЬ ИГРОВОГО СОБЫТИЯ-------------------------------------------
    if (field[i][j]->color == color)
    {
        qDebug() << "NOTE: Tried to capture captured hex";
        return;
    }
    else if (color == "Neutral")
    {
        qDebug() << "ERROR: Tried to capture hex... with neutral color";
        decaptureHex(i, j);
        return;
    }
    else if (field[i][j]->color != "Neutral")
    {
        decaptureHex(i, j);
    }

    // ИСПОЛНЕНИЕ СОБЫТИЯ--------------------------------------------------------------------------------------
    if (!field[i][j]->canBeCaptured)
    {
        qWarning() << "ERROR: Mountain (?) captured - ?!?";
    }
    else
    {
        field[i][j]->color = color;
        emit hexColorChanged(field[i][j]);

        if (field[i][j]->type == "Mill")
            ++players[color]->numOfMills;
    }
}
void Game::decaptureHex(int i, int j)
{
    QString old_owner = field[i][j]->color;
    if (old_owner == "Neutral")
    {
        qDebug() << "NOTE: Tried to decapture decaptured hex";
        return;
    }

    if (!field[i][j]->canBeCaptured)
    {
        qWarning() << "ERROR: Mountain (?) decaptured - ?!?";
    }
    else
    {
        PlayerColor previousOwner = field[i][j]->color;
        field[i][j]->color = "Neutral";
        emit hexColorChanged(field[i][j]);

        if (field[i][j]->type == "Mill")
            --players[previousOwner]->numOfMills;
    }
}
