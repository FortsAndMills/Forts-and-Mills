#include "Game.h"

void Game::playerGiveUp(int index)
{
    players[rules->players[index]]->GiveUp = true;
}

bool Game::CanUse(GameUnit *unit, OrderType order)
{
    // если этот приказ технически не может использоваться юнитом, сразу нет
    if (!GameOrderParameters(rules, unit->type, order).canUse)
        return false;

    // находим текущую позицию юнита
    Coord where = unit->position;
    DayTime plan_time = 0;
    while (plan_time < rules->dayTimes && unit->plan[plan_time] != NULL)
    {
        where = unit->plan[plan_time]->moves(where);
        ++plan_time;
    }

    QSet<Coord> captured_today;
    QSet<Coord> liberated_today;

    int spend = 0;  // подсчёт ресурсов
    foreach (GameUnit * u, players[unit->color]->units)
    {
        Coord where = u->position;
        for (int t = 0; t < rules->dayTimes; ++t)
        {
            if (u->plan[t] != NULL)
            {
                // увеличиваем счётчик использованных ресурсов
                if (u->plan[t]->type == order)
                    ++spend;

                // сохраняем список клеток, которые собираемся захватить
                if (t <= plan_time && u->plan[t]->containsAction(GameAction::CAPTURE_HEX))
                    captured_today << where;
                // сохраняем список клеток, которые собираемся освободить
                if (t <= plan_time && u->plan[t]->containsAction(GameAction::LIBERATE))
                    liberated_today << where;

                // следим за предполагаемой позицией
                foreach (GameAction action, u->plan[t]->actions)
                {
                    if (action.type == GameAction::LEAVE_HEX)
                    {
                        where = action.target;
                    }
                }
            }
        }
    }

    // нехватка ресурсов
    if (players[unit->color]->resources[order] <= spend)
        return false;

    // если это рекрутирование, то нужно проверить, даёт ли вообще эта клетка юнита
    if (order == "Recruit")
    {
        // клетка уже дала юнита
        if (!hex(where)->provides_unit())
            return false;

        // не подсоединено
        if (!Connected(unit->color, false, captured_today).contains(hex(where)))
            return false;

        // клетка не была захвачена
        if (hex(where)->color != unit->color && !captured_today.contains(where))
            return false;
    }

    // если это захват, то нужно проверить, была ли клетка освобождена
    if (order == "Capture" && rules->ordersInGame.contains("Liberate"))
    {
        // клетка не была освобождена
        if (hex(where)->color != unit->color && hex(where)->color != "Neutral" && !liberated_today.contains(where))
            return false;
    }

    return true;
}
QList<OrderType> Game::whatCanUse(GameUnit * unit)
{
    // перебираем все варианты и запускаем CanUse
    QList <OrderType> ans;
    foreach (OrderType order, rules->ordersInGame)
    {
        if (CanUse(unit, order))
            ans << order;
    }
    ans << DefaultOrder;

    if (unit->plan[0] != NULL)
        ans << DeleteLastOrder;
    return ans;
}
bool Game::must_be_last(GameUnit * unit, DayTime time)
{
    if (time == -1)
        return false;

    // проверка пересечения речки
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


