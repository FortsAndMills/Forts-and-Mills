#include "GameWindow.h"

bool GameWindow::DoesEveryoneChoosedStartPoint()
{
    if (state == WAIT_FOR_ENEMY_START_POINT)
    {
        foreach (GamePlayer * player, game->players)
        {
            if (player->selected_x == UNDEFINED)
                return false;
        }

        return true;
    }
}

void GameWindow::disableWholeField()
{
    for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            //hex(i, j)->light(false);
            hex(i, j)->forbidToSelect(true);
        }
    }
}
void GameWindow::delightWholeField()
{
    for (int i = 0; i < game->rules->fieldH; ++i)  // делаем гексы невыделенными и недоступными
    {
        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hex(i, j)->light(false);
        }
    }
}

void GameWindow::moveUnit(Unit * unit, int x, int y, Hex::POSITION_STATE position_type)
{
    int from_x = unit->x_stack.top();
    int from_y = unit->y_stack.top();
    unit->deanchorFrom(hex(from_x, from_y));

    unit->x_stack << x;
    unit->y_stack << y;
    unit->points_stack << hex(x, y)->createPoint(position_type, game->whereIs(x, y, from_x, from_y));

    unit->anchorTo(hex(x, y));
    reconfigureUnits();
    reconfigureWays();
}
void GameWindow::moveUnitBack(Unit * unit)
{
    int from_x = unit->x_stack.pop();
    int from_y = unit->y_stack.pop();
    unit->deanchorFrom(hex(from_x, from_y));

    hex(from_x, from_y)->removePoint(unit->points_stack.pop());

    unit->anchorTo(hex(unit->x_stack.top(), unit->y_stack.top()));
    reconfigureUnits();
    reconfigureWays();
}

void GameWindow::anchorOrderFromUnit(Unit * unit, Order * order)
{
    order->deanchorFrom(unit);
    unit->orders_stack.removeAll(order);
    unit->reconfigureOrders();

    order->anchor_x = unit->x_stack.top();
    order->anchor_y = unit->y_stack.top();
    order->anchor_point = unit->points_stack.top();

    hex(unit->x_stack.top(), unit->y_stack.top())->addOrder(order);
}
void GameWindow::anchorOrderToUnit(Unit *unit, Order *order)
{
    hex(order->anchor_x, order->anchor_y)->removeOrder(order);

    order->anchorTo(unit);
    unit->orders_stack.push_front(order);
    unit->reconfigureOrders();
}

void GameWindow::newUnit(GameUnit * unit)  // создаёт нового юнита
{
    units[unit] = new Unit(unit, game, this, field->field, unit->color == mainPlayerColor);
    resizeUnits();
}
void GameWindow::blowUnit(GameUnit * unit)  // взорвать юнит
{
    units[unit]->blow();
    units.remove(unit);
}

void GameWindow::createWay(Unit *unit)
{
    int to_x = unit->x_stack.pop();
    int to_y = unit->y_stack.pop();
    int to_point = unit->points_stack.pop();

    UnitWay * New = new UnitWay(this, unit,
            unit->x_stack.top(), unit->y_stack.top(), unit->points_stack.top(),
                             to_x, to_y, to_point);

    New->setStartPosition(wayStartCoordinate(New),
                                           wayEndCoordinate(New));
    New->anchorTo(hex(unit->x_stack.top(), unit->y_stack.top()));
    ways[unit] << New;

    unit->x_stack << to_x;
    unit->y_stack << to_y;
    unit->points_stack << to_point;

    reconfigureWays();
}

void GameWindow::createFreeResource(GameOrder * order)  // создаёт иконку безвольного ресурса
{
    orders[order] = new Order(this, order);  // <undone>
    orders[order]->setGeometry(field->x(), field->y(), constants->unitsSize / 1.5, constants->unitsSize / 1.5);
}
void GameWindow::unitGetOutOfField(GameUnit * unit, int x, int y)  // юнит откалывается от поля
{
}
