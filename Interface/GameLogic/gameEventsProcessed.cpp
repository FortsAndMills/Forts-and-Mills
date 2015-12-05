#include "GameWindow.h"

void GameWindow::newResources(GameHex * hex, PlayerColor player_id,
                  QList<Resource> resources, QList<bool> burn)
{
    QList<OrderPic *> new_resources;
    for (int i = 0; i < resources.size(); ++i)
        new_resources << new OrderPic(this, game, player_id, resources[i]);

    hexes[hex]->setNewResourcesPosition(new_resources);

    player_windows[player_id]->animateNewResources(new_resources, burn);
}

void GameWindow::anchorUnitToField(GameUnit * unit)  // привязаться к гексу под ним
{
    units[unit]->points_stack << hex(unit->x, unit->y)->createPoint(Hex::STAY, "");
    units[unit]->anchorTo(hex(unit->x, unit->y));
    units[unit]->x_stack << unit->x;
    units[unit]->y_stack << unit->y;

    reconfigureUnits();
    reconfigureWays();
}

