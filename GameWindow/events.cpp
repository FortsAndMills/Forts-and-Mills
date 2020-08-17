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
    for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hex(i, j)->light(false);
        }
    }
}

//void Events::wantToCaptureRegion(Coord r, bool turn_on)
//{
//    for (int i = 0; i < game->rules->fieldH; ++i)
//    {
//        for (int j = 0; j < game->rules->fieldW; ++j)
//        {
//            if (game->hexes[i][j]->region_center == r || r == ANY)
//            {
//                if (turn_on)
//                    hex(i, j)->planCapturing(mainPlayerColor);
//                else
//                    hex(i, j)->deplanCapturing(mainPlayerColor);
//            }
//        }
//    }
//}

void Events::newUnit(GameUnit * unit, Coord where)
{
    units[unit] = new Unit(unit, game, this, unit->color == mainPlayerColor);
    // привязываем сигналы от нового юнита к обработчикам!
    unitConnections(units[unit]);

    // добавляем его текущую позицию в список его позиций
    units[unit]->stack << Position(unit->position, hex(where)->createPoint());
    units[unit]->anchorTo(hex(units[unit]));  // привязываем к текущему гексу

    // организуем появление
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

    // убираем все пути для данного юнита
    while (!ways_to[units[unit]].empty())
    {
        ways_to[units[unit]].pop()->disappear();
        ways_from[units[unit]].pop()->disappear();
    }

    // убираем все точки для этого юнита
    while (units[unit]->stack.size() > 0)
    {
        hex(units[unit])->removePoint(units[unit]->point());
        units[unit]->stack.removeLast();
    }

    // взрываем и забываем
    units[unit]->blow();
    units.remove(unit);

    // точки пересчитываем
    hexPointsChanged();
}

// перемещаем юнит в гекс coord с выбранным состоянием
bool Events::moveUnit(Unit * unit, Coord cord, Hex::POSITION_STATE position_type)
{
    // этот флаг показывает, что перемещение уже началось
    // и в исходном гексе у юнита уже есть аж две точки и, значит, готовая стрелка
    bool wasLeaving = hex(unit)->pointPositionState[unit->point()] == Hex::LEAVING;

    // открепляемся
    Coord from = unit->where();
    unit->deanchorFrom(hex(unit));

    if (wasLeaving)  // убираем лишнюю промежуточную точку, если надо
    {
        hex(unit)->removePoint(unit->point());
        unit->stack.removeLast();
    }

    // создаём новую точку!
    unit->stack << Position(cord, hex(cord)->createPoint(position_type, game->whereIs(cord, from)));
    unit->anchorTo(hex(unit));

    // изменяем уже имеющуюся стрелку, если таковая есть
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

    // возвращаем true, если со стрелкой работа не велась
    return !wasLeaving || ways_from[unit].isEmpty();
}
void Events::moveUnitBack(Unit * unit)
{
    // отмотка движения юнита по истории
    unit->deanchorFrom(hex(unit));

    hex(unit)->removePoint(unit->point());
    unit->stack.removeLast();

    unit->anchorTo(hex(unit));

    hexPointsChanged();
}
void Events::moveUnitForward(Unit * unit)
{
    // "реализуем" движение, то есть предполагается,
    // что юнит уже сдвинут, но ещё есть точка, из которой он пришёл,
    // и в интерфейса отображается, будто юнит в процессе перемещения

    // эта точка - точка, в которой находится юнит. Её потом вернём обратно.
    Position rem = unit->stack.last();
    unit->stack.removeLast();

    // на верхушке стэка теперь старая точка, подлежащая удалению
    hex(unit)->removePoint(unit->point());
    hex(unit)->recountPoints();
    unit->stack.removeLast();

    // возвращаем первую точку
    unit->stack << rem;

    // забываем, что мы откуда-то пришли - окончательно встаём в центр гекса
    hex(unit)->pointPositionState[unit->point()] = Hex::STAY;
    hex(unit)->pointsWay[unit->point()] = NO_WAY;
    hex(unit)->recountPoints();

    hexPointsChanged();
}
void Events::moveToWayUnit(Unit * unit, Coord to)
{
    // юнит сдвигается в промежуточную точку в стартовом гексе в направлении нового гекса
    unit->stack << Position(unit->where(), hex(unit)->createPoint(Hex::LEAVING, game->whereIs(to, unit->where())));
    hexPointsChanged();
}

// перевязываем приказ от юнита к гексу
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
// наоборот
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
        fp = hex->createPoint();  // если точки для укреплений нет, создаём
    else
        fp = fortifications[hex][0]->point;

    for (int i = 0; i < amount; ++i)  // создаём укрепления
    {
        fortifications[hex] << new Fortification(this, color, fp);
        fortifications[hex].last()->anchorTo(hex);

        // заготавливаем появление
        fortifications[hex].last()->setGeometry(origin.x(), origin.y(), 0, 0);
    }
    for (int i = 0; i > amount; --i)  // убираем укрепления
    {
        fortifications[hex].last()->disappear(constants->unitReconfigureTime);
        fortifications[hex].removeLast();
    }

    if (fortifications[hex].size() == 0)  // если все укрепления исчезли, точку для них стоит убрать
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

// создание стрелки по последнему перемещению юнита
void Events::createWay(Unit *unit)
{
    Position to = unit->stack.last();
    Position from = unit->stack[unit->stack.size() - 2];

    // для правильного отображения стрелки, проходящей через разрез поля
    // нужно отдельно создавать "начало и конец"
    // обработка их одинакова, они лишь "клипуются" разными гексами
    // и имееют привязку соответственно к начальному или конечному гексу

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
    sh->reconfigure(base, top);  // FAIL неудачное название функции, так как она ничего не анимирует
    sh->setGeometry(sh->geometry);  // почему это ручками приходится устанавливать старт и запускать?
    sh->setRotation(sh->rotation);
    sh->setStartPosition();
    sh->animate();
}

void Events::newResources(GameHex * hex, PlayerColor color, QList<Resource> resources, QList<bool> burn)
{
    // делаем массив burn корректным
    while (burn.size() < resources.size())
        burn << false;

    // составляем список графических объектов новых ресурсов
    QList<OrderPic *> new_resources;
    for (int i = 0; i < resources.size(); ++i)
    {
        OrderPic * pic = new OrderPic(this, color, resources[i]);
        new_resources << pic;

        orderPicsConnections(pic);
    }

    // стартовое положение
    hexes[hex]->setNewResourcesPosition(new_resources);

    // отправляем в полёт, привязывая всё к панели игрока
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


