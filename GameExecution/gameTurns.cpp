#include "GameTurns.h"

GameTurns::GameTurns(GameRules *rules, Random * rand) :
    GameHelp(rules, rand)
{

}

void GameTurns::playerGiveUp(int index)
{
    players[rules->players[index]]->GiveUp = true;
}

bool GameTurns::CanUse(GameUnit *unit, OrderType order)
{
    if (!GameOrderParameters(rules, unit->type, order).canUse)
        return false;

    int spend = 0;
    foreach (GameUnit * u, players[unit->color]->units)
    {
        for (int t = 0; t < rules->dayTimes; ++t)
        {
            if (u->plan[t] != NULL && u->plan[t]->type == order)
                ++spend;
        }
    }

    return players[unit->color]->resources[order] > spend;
}
QList<OrderType> GameTurns::whatCanUse(GameUnit * unit)
{
    QList <OrderType> ans;
    foreach (OrderType order, rules->ordersInGame)
    {
        if (CanUse(unit, order))
            ans << order;
    }
    ans << DefaultOrder;
    return ans;
}

bool GameTurns::ChooseOneOfTheStartHexes()
{
    QList <Coord> variants;
    for (int x = 0; x < rules->fieldH; ++x)
        for (int y = 0; y < rules->fieldW; ++y)
            if (hexes[x][y]->canBeChosenAsStartPoint)
                variants << Coord(x, y);

    if (variants.size() < players.size())
        return false;

    chosenHex.clear();
    ready.clear();
    AddEvent()->ChooseHex(variants);
    return true;
}

void GameTurns::StartPlanning()
{
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            foreach (GameOrder * order, unit->plan)
            {
                delete order;
            }
            unit->plan.clear();
        }
    }

    ready.clear();
    AddEvent()->Plan();
}
