#include "GameTurns.h"

GameTurns::GameTurns(GameRules *rules, Random * rand) :
    GameHelp(rules, rand)
{
    foreach (PlayerColor color, rules->players)
        chosenUnitType[color] = rules->unitsInGame[0];
}

void GameTurns::playerGiveUp(int index)
{
    players[rules->players[index]]->GiveUp = true;
}

bool GameTurns::CanUse(GameUnit *unit, OrderType order)
{
    // если этот приказ технически не может использоваться юнитом, сразу нет
    if (!GameOrderParameters(rules, unit->type, order).canUse)
        return false;

    int spend = 0;  // подсчёт ресурсов
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
    // перебираем все варианты и запускаем CanUse
    QList <OrderType> ans;
    foreach (OrderType order, rules->ordersInGame)
    {
        if (CanUse(unit, order))
            ans << order;
    }
    ans << DefaultOrder;
    return ans;
}
bool GameTurns::must_be_last(GameUnit * unit, DayTime time)
{
    if (time == -1)
        return false;

    Coord where = unit->position;
    for (DayTime t = 0; t <= time; ++t)
    {
        foreach (GameAction action, unit->plan[t]->actions)
        {
            if (action.type == GameAction::LEAVE_HEX)
            {
                if (t == time)
                {
                    Coord target = action.target;
                    WAY way = whereIs(target, where);
                    if (hex(where)->rivers[way])
                        return true;
                }
                else
                    where = action.target;
            }
        }
    }
    return false;
}

bool GameTurns::ChooseOneOfTheStartHexes()
{
    if (rules->start_choices <= 0) return false;
    --rules->start_choices;

    // составляем список вариантов
    QList <Coord> variants;
    for (int x = 0; x < rules->fieldH; ++x)
        for (int y = 0; y < rules->fieldW; ++y)
            if (hexes[x][y]->canBeChosenAsStartPoint)
                variants << Coord(x, y);

    if (variants.size() < players.size())  // если выбор закончился
        return false;

    // очищаем от предыдущих ходов
    chosenHex.clear();
    ready.clear();

    // шлём запрос пользователю выбрать гекс
    AddEvent()->ChooseHex(variants);
    return true;
}

void GameTurns::StartPlanning()
{
    // очистка от предыдущих планов
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
