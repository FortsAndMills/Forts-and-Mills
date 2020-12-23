#include "GameWindow.h"

// ищет координаты соседнего гекса - удобная функция
QPointF GameWindow::adjacentHexCoord(Hex *hex, WAY way)
{
    QPointF p = mapFromItem(hex->parentItem(), hex->pos());
    if (way == UP)
        return p + QPointF(0, -constants->hexHeight);
    if (way == DOWN)
        return p + QPointF(0, constants->hexHeight);
    if (way == RIGHT_UP)
        return p + QPointF(constants->hexWidth * (1 - constants->hexShift), -constants->hexHeight / 2);
    if (way == RIGHT_DOWN)
        return p + QPointF(constants->hexWidth * (1 - constants->hexShift), constants->hexHeight / 2);
    if (way == LEFT_UP)
        return p + QPointF(-constants->hexWidth * (1 - constants->hexShift), -constants->hexHeight / 2);
    if (way == LEFT_DOWN)
        return p + QPointF(-constants->hexWidth * (1 - constants->hexShift), constants->hexHeight / 2);

    debug << "ERROR: AdjacentHexCoordinates wrong way!\n";
    return p;
}

// координаты объектов обычно ищутся через точки на гексах, к которым они привязаны
// поэтому для объектов хранится не только гекс, к которым они привязаны, но и id якоря внутри гекса
QPointF GameWindow::unitCoordinate(Unit * u)
{
    return hex(u)->point(u->point(), this) - QPointF(constants->unitsSize / 2, constants->unitsSize / 2);
}
void GameWindow::resizeUnits()
{
    foreach (Unit * u, units)
    {
        if (u->prototype->position != NOWHERE)
        {
            u->setPos(unitCoordinate(u));
            u->resize(constants->unitsSize, constants->unitsSize);
        }
    }
}
QPointF GameWindow::wayToStartCoordinate(UnitWay *way)
{
    return adjacentHexCoord(hex(way->to),
          game->whereIs(way->from, way->to)) +
            hex(way->from)->points[way->from_point];
}
QPointF GameWindow::wayToEndCoordinate(UnitWay *way)
{
    return mapFromItem(hex(way->to),
            hex(way->to)->points[way->to_point]);
}
QPointF GameWindow::wayFromStartCoordinate(UnitWay *way)
{
    return mapFromItem(hex(way->from),
            hex(way->from)->points[way->from_point]);
}
QPointF GameWindow::wayFromEndCoordinate(UnitWay *way)
{
    return adjacentHexCoord(hex(way->from),
          game->whereIs(way->to, way->from)) +
            hex(way->to)->points[way->to_point];
}
void GameWindow::resizeWays()
{
    foreach (QStack <UnitWay *> list, ways_to)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayToStartCoordinate(way),
                                          wayToEndCoordinate(way));

            way->setGeometry(way->geometry);
            way->setRotation(way->rotation);
        }
    }

    foreach (QStack <UnitWay *> list, ways_from)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayFromStartCoordinate(way),
                                          wayFromEndCoordinate(way));

            way->setGeometry(way->geometry);
            way->setRotation(way->rotation);
        }
    }
}
void GameWindow::resizeRockets()
{
    foreach (Unit * u, units)
    {
        foreach (Rocket * r, rockets[u])
        {
            QPointF start = unitCoordinate(u);
            start += QPointF(constants->unitsSize / 2, constants->unitsSize / 2);

            QPointF finish = mapFromItem(hex(r->target)->parentItem(), hex(r->target)->pos());
            finish += QPointF(constants->hexWidth / 2, constants->hexHeight / 2);

            r->reconfigure(start, finish);
            r->setGeometry(r->geometry);
            r->setRotation(r->rotation);
        }
    }
}
QRectF GameWindow::fortificationGeometry(Hex *hex, int index)
{
    qreal W = constants->fortificationWidth * constants->unitsSize;
    qreal H = constants->fortificationHeight * constants->unitsSize;

    QPointF cen = mapFromItem(hex, hex->points[fortifications[hex][index]->point]);
    cen += index * QPointF(constants->fortificationStackShiftX * W,
                                            constants->fortificationStackShiftY * H);
    return QRectF(cen.x() - W / 2, cen.y() - H / 2, W, H);
}
void GameWindow::resizeFortifications()
{
    foreach (Hex * hex, hexes)
        for (int i = 0; i < fortifications[hex].size(); ++i)
            fortifications[hex][i]->setGeometry(fortificationGeometry(hex, i));
}

// все объекты отправляются в полёт к своим назначенным точкам
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
        if (unit->prototype->position != NOWHERE)
        {
            QPointF point = unitCoordinate(unit);
            unit->AnimationStart(X_POS, point.x(), constants->unitReconfigureTime);
            unit->AnimationStart(Y_POS, point.y(), constants->unitReconfigureTime);
        }
    }
}
void GameWindow::reconfigureWays()
{
    foreach (QStack <UnitWay *> list, ways_to)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayToStartCoordinate(way),
                                          wayToEndCoordinate(way));

            way->AnimationStart(way->geometry, constants->unitReconfigureTime);
            way->AnimationStart(ROTATION, way->rotation, constants->unitReconfigureTime);
        }
    }

    foreach (QStack <UnitWay *> list, ways_from)
    {
        foreach (UnitWay * way, list)
        {
            way->reconfigure(wayFromStartCoordinate(way),
                                          wayFromEndCoordinate(way));

            way->AnimationStart(way->geometry, constants->unitReconfigureTime);
            way->AnimationStart(ROTATION, way->rotation, constants->unitReconfigureTime);
        }
    }
}
void GameWindow::reconfigureRockets()
{
    foreach (Unit * u, units)
    {
        foreach (Rocket * r, rockets[u])
        {
            QPointF start = unitCoordinate(u);
            start += QPointF(constants->unitsSize / 2, constants->unitsSize / 2);

            QPointF finish = mapFromItem(hex(r->target)->parentItem(), hex(r->target)->pos());
            finish += QPointF(constants->hexWidth / 2, constants->hexHeight / 2);

            r->reconfigure(start, finish);
            r->AnimationStart(r->geometry, constants->unitReconfigureTime);
            r->AnimationStart(ROTATION, r->rotation, constants->unitReconfigureTime);
        }
    }
}
void GameWindow::reconfigureFortifications()
{
    foreach (Hex * hex, hexes)
        for (int i = 0; i < fortifications[hex].size(); ++i)
            fortifications[hex][i]->AnimationStart(fortificationGeometry(hex, i), constants->unitReconfigureTime);
}

void GameWindow::hexPointsChanged()
{
    reconfigureUnits();
    reconfigureWays();
    reconfigureRockets();
    reconfigureFortifications();
}

