#include "Events.h"

Events::Events(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    InterfaceOrganization(game, PlayerIndex, parent)
{
}

void Events::disableWholeField()
{
    for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hex(i, j)->forbidToSelect(true);
        }
    }
}
void Events::delightWholeField()
{
    for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hex(i, j)->light(false);
        }
    }
}

void Events::newUnit(GameUnit * unit, Coord where)
{
    units[unit] = new Unit(unit, game, this, unit->color == mainPlayerColor);
    unitConnections(units[unit]);

    units[unit]->stack << Position(unit->position, hex(where)->createPoint());
    units[unit]->anchorTo(hex(units[unit]));

    units[unit]->setPos(unitCoordinate(units[unit]) +
                        QPointF(constants->unitsSize, constants->unitsSize) / 2);
    units[unit]->resize(0, 0);

    hexPointsChanged();
    units[unit]->AnimationStart(WIDTH, constants->unitsSize, constants->unitReconfigureTime);
    units[unit]->AnimationStart(HEIGHT, constants->unitsSize, constants->unitReconfigureTime);
}
void Events::blowUnit(GameUnit * unit)
{
    if (!units.contains(unit))
        debug << "FATAL ERROR: blowing very strange unit!\n";

    while (!ways_to[units[unit]].empty())
    {
        ways_to[units[unit]].pop()->disappear();
        ways_from[units[unit]].pop()->disappear();
    }

    while (units[unit]->stack.size() > 0)
    {
        hex(units[unit])->removePoint(units[unit]->point());
        units[unit]->stack.removeLast();
    }

    units[unit]->blow();
    units.remove(unit);

    hexPointsChanged();
}

bool Events::moveUnit(Unit * unit, Coord cord, Hex::POSITION_STATE position_type)
{
    bool wasLeaving = hex(unit)->pointPositionState[unit->point()] == Hex::LEAVING;

    Coord from = unit->where();
    unit->deanchorFrom(hex(unit));

    if (wasLeaving)
    {
        hex(unit)->removePoint(unit->point());
        unit->stack.removeLast();
    }

    unit->stack << Position(cord, hex(cord)->createPoint(position_type, game->whereIs(cord, from)));
    unit->anchorTo(hex(unit));

    if (wasLeaving && !ways_from[unit].isEmpty())
    {
        ways_from[unit].last()->to_point = unit->point();
        ways_from[unit].last()->to = unit->where();

        ways_to[unit].last()->UnclipWithItem(hex(from));
        ways_to[unit].last()->to_point = unit->point();
        ways_to[unit].last()->to = unit->where();
        ways_to[unit].last()->ClipWithItem(hex(unit));
    }

    hexPointsChanged();

    return !wasLeaving || ways_from[unit].isEmpty();
}
void Events::moveUnitBack(Unit * unit)
{
    unit->deanchorFrom(hex(unit));

    hex(unit)->removePoint(unit->point());
    unit->stack.removeLast();

    unit->anchorTo(hex(unit));

    hexPointsChanged();
}
void Events::moveUnitForward(Unit * unit)
{
    Position rem = unit->stack.last();
    unit->stack.removeLast();

    hex(unit)->removePoint(unit->point());
    hex(unit)->recountPoints();
    unit->stack.removeLast();

    unit->stack << rem;

    hex(unit)->pointPositionState[unit->point()] = Hex::STAY;
    hex(unit)->pointsWay[unit->point()] = "";
    hex(unit)->recountPoints();

    hexPointsChanged();
}
void Events::moveToWayUnit(Unit * unit, Coord to)
{
    unit->stack << Position(unit->where(), hex(unit)->createPoint(Hex::LEAVING, game->whereIs(to, unit->where())));
    hexPointsChanged();
}

void Events::anchorOrderFromUnit(Unit * unit, Order * order)
{
    if (order == NULL)
        return;

    order->deanchorFrom(unit);
    unit->orders_stack.removeAll(order);
    unit->reconfigureOrders();

    order->anchor = unit->where();
    order->anchor_point = unit->point();

    hex(unit->where())->addOrder(order);
}
void Events::anchorOrderToUnit(Unit *unit, Order *order)
{
    hex(order->anchor)->removeOrder(order);

    order->anchorTo(unit);
    unit->orders_stack.push_front(order);
    unit->reconfigureOrders();
}

void Events::fortify(Hex * hex, int amount, PlayerColor color, QPointF origin)
{
    int fp;
    if (fortifications[hex].size() == 0)
        fp = hex->createPoint();
    else
        fp = fortifications[hex][0]->point;

    for (int i = 0; i < amount; ++i)
    {
        fortifications[hex] << new Fortification(this, color, fp);
        fortifications[hex].last()->anchorTo(hex);
        fortifications[hex].last()->setGeometry(origin.x(), origin.y(), 0, 0);
    }
    for (int i = 0; i > amount; --i)
    {
        fortifications[hex].last()->disappear(constants->unitReconfigureTime);
        fortifications[hex].removeLast();
    }

    if (fortifications[hex].size() == 0)
    {
        hex->removePoint(fp);
    }
    hexPointsChanged();
}
void Events::deleteAllFortification(Hex * hex)
{
    if (fortifications[hex].size() > 0)
    {
        fortify(hex, -fortifications[hex].size());
    }
}

void Events::createWay(Unit *unit)
{
    Position to = unit->stack.last();
    Position from = unit->stack[unit->stack.size() - 2];

    UnitWay * NewTo = new UnitWay(this, unit,
                             from.position, from.point,
                             to.position, to.point);

    NewTo->setStartPosition(wayToStartCoordinate(NewTo),
                                           wayToEndCoordinate(NewTo));
    NewTo->anchorTo(hex(to.position));
    NewTo->ClipWithItem(hex(to.position));
    ways_to[unit] << NewTo;

    UnitWay * NewFrom = new UnitWay(this, unit,
                                    from.position, from.point,
                                    to.position, to.point);

    NewFrom->setStartPosition(wayFromStartCoordinate(NewTo),
                                           wayFromEndCoordinate(NewTo));
    NewFrom->anchorTo(hex(from.position));
    NewFrom->ClipWithItem(hex(from.position));
    ways_from[unit] << NewFrom;

    reconfigureWays();
}
void Events::shoot(PlayerColor color, QPointF base, QPointF top, int, Rocket::RocketType type)
{
    Rocket * sh = new Rocket(this, color, true, type);
    sh->reconfigure(base, top);
    sh->setGeometry(sh->geometry);
    sh->setRotation(sh->rotation);

    sh->setStartPosition();
    sh->animate();
}

void Events::newResources(GameHex * hex, PlayerColor color, QList<Resource> resources, QList<bool> burn)
{
    while (burn.size() < resources.size())
        burn << false;

    QList<OrderPic *> new_resources;
    for (int i = 0; i < resources.size(); ++i)
    {
        OrderPic * pic = new OrderPic(this, color, resources[i]);
        new_resources << pic;

        orderPicsConnections(pic);
    }

    hexes[hex]->setNewResourcesPosition(new_resources);

    player_windows[color]->animateNewResources(new_resources, burn);
    foreach (OrderPic * op, new_resources)
        op->anchorTo(player_windows[color]);
}
void Events::disappearAllTurnedOffOrders()
{
    foreach (PlayerColor player, game->rules->players)
        player_windows[player]->disappearAllTurnedOffOrders();
}
void Events::reconfigureResources()
{
    foreach (PlayerResources * player, player_windows)
    {
        player->reconfigureResources();
    }
}


