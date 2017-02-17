#include "GameBase.h"

GameBase::GameBase(GameRules *rules, Random * rand)
{
    this->rules = rules;
    this->rand = rand;
    rules->FormPlayersList(rand);

    for (int i = 0; i < rules->players.size(); ++i)
        players[rules->players[i]] = new GamePlayer(rules->players[i]);
}

WAY GameBase::oppositeWay(WAY way)
{
    if (way == UP)
        return DOWN;
    if (way == DOWN)
        return UP;
    if (way == RIGHT_UP)
        return LEFT_DOWN;
    if (way == RIGHT_DOWN)
        return LEFT_UP;
    if (way == LEFT_UP)
        return RIGHT_DOWN;
    if (way == LEFT_DOWN)
        return RIGHT_UP;

    debug << "ERROR: opposite way error!\n";
    return "";
}
Coord GameBase::adjacentHex(Coord which, WAY way)
{
    if (way == UP ||
        (way == RIGHT_UP && which.y % 2 == 0) ||
        (way == LEFT_UP && which.y % 2 == 0))
       which.x  = (which.x - 1 + rules->fieldH) % rules->fieldH;
    else if (way == DOWN ||
             (way == RIGHT_DOWN && which.y % 2) ||
             (way == LEFT_DOWN && which.y % 2))
       which.x = (which.x + 1) % rules->fieldH;

    if (way == LEFT_DOWN || way == LEFT_UP)
       which.y = (which.y - 1 + rules->fieldW) % rules->fieldW;
    else if (way == RIGHT_DOWN || way == RIGHT_UP)
       which.y = (which.y + 1) % rules->fieldW;

    return which;
}
QList<Coord> GameBase::visible_hexes(Coord my, int radius)
{
    QList <Coord> ans;

    foreach (WAY way, WAYS)
    {
        int k = 0;
        Coord coord = my;

        do
        {
            coord = adjacentHex(coord, way);
            ans << coord;
            ++k;
        }
        while (k < radius && !hex(coord)->isBuilding);
    }

    return ans;
}

QMap<WAY, Coord> GameBase::adjacentHexesMap(Coord which)
{
    QMap <WAY, Coord > ans;
    foreach (QString way, WAYS)
    {
        ans[way] = adjacentHex(which, way);
    }
    return ans;
}
QList<Coord> GameBase::adjacentHexes(Coord which)
{
    QList <Coord> ans;
    foreach (QString way, WAYS)
    {
        ans << adjacentHex(which, way);
    }
    return ans;
}

WAY GameBase::whereIs(Coord which, Coord from)
{
    QMap<WAY, Coord > adjacent = adjacentHexesMap(from);

    for (QMap<WAY, Coord >::iterator it = adjacent.begin(); it != adjacent.end(); ++it)
    {
        if (it.value() == which)
            return it.key();
    }

    debug << "FATAL: WAY determination error!\n";
    return "";
}
