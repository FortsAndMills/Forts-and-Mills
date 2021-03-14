#include "AI.h"

const qreal GO_RISK_PENALTY = 0.5;
const qreal I_AM_GONA_LOSE_PENALTY = 4;
const qreal I_AM_GONA_DRAW_PENALTY = 2.5;
const qreal ONE_FORT = 10;
const qreal FORT_COEFF = 0.5;
const qreal ATTACK_LIMIT_INCR = 1;
const qreal GO_LIMIT_INCR = 0.7;
const qreal ONE_MILL = 7;
const qreal MILL_COEFF = 0.3;
const qreal MILL_TERRITORY_COEFF = 0.5;
const qreal STAY_ON_HEX_COEFF = 0.7;
const qreal ENTER_EMPTY_HEX = 0.3;
const qreal HELP_SATISFIED = 10;
const qreal POTENTIAL_RECRUIT = 2.5;
const qreal FREE_HEX = 1;
const qreal SPENT_MOVING = 0.1;
const qreal ADJ_RIVER_HEX_WEIGHT = 0.1;
const qreal ADJ_HEX_WEIGHT = 0.2;
const QList<qreal> ATTACK_VAL = {4, 2.5, 1.5, 0.75, 0.5, 0.2, 0.1, 0.1, 0.1, 0.1};
const QList<qreal> GO_VAL = {3, 2, 1, 0.5, 0.2, 0.1, 0.03, 0.03, 0.03, 0.03};
const QList<qreal> LIBER_VAL = {6, 1, 0.2, 0.1, 0.01, 0.01, 0.01, 0.01, 0.01, 0.01};

AI::AI(Game *game)
{
    this->game = game;

    forces_precompute();
    hex_precompute();

    for (int i = 0; i < 2; ++i)
        hex_propagate();
}

// предпосчёты
void AI::forces_precompute()
{
    foreach (GamePlayer * player, game->players)
    {
        foreach (GameUnit * unit, player->units)
        {
            int res = player->resources["Go"] + player->resources["Attack"];
            int max_dt = qMin(game->rules->dayTimes, res);

            QSet <Coord> reachable_without_river_crossing;
            reachable_without_river_crossing << unit->position;

            QSet <Coord> reachable;
            reachable << unit->position;

            forces[player->color][unit->position][-1] += unit->health;

            for (int dt = 0; dt < game->rules->dayTimes; ++dt)
            {
                if (dt < max_dt)
                {
                    QSet <Coord> rwrc_copy = reachable_without_river_crossing;
                    foreach (Coord c, rwrc_copy)
                    {
                        QMap <WAY, Coord> options = game->adjacentHexesMap(c);
                        for (QMap <WAY, Coord>::iterator it = options.begin(); it != options.end(); ++it)
                        {
                            if (game->hex(it.value())->canGoHere)
                            {
                                reachable << it.value();

                                if (!game->hex(c)->rivers[it.key()])
                                    reachable_without_river_crossing << it.value();
                            }
                        }
                    }
                }

                foreach (Coord c, reachable)
                    forces[player->color][c][dt] += unit->health;
            }
        }
    }

    // учитываем бонус фортов
    foreach (QList <GameHex *> hex_row, game->hexes)
    {
        foreach (GameHex * hex, hex_row)
        {
            if (hex->color != "Neutral" && hex->defense > 0)
            {
                int dt = -1;
                while (dt < game->rules->dayTimes && forces[hex->color][hex->coord][dt] == 0)
                    ++dt;

                ++dt;
                while (dt < game->rules->dayTimes)
                {
                    forces[hex->color][hex->coord][dt] += hex->defense;
                    ++dt;
                }
            }
        }
    }
}
void AI::hex_precompute()
{
    foreach (GamePlayer * player, game->players)
    {
        int forts_limits = game->resourcesLimit(player->color);
        int num_of_mills = 0;
        foreach (QList <GameHex *> hex_row, game->hexes)
            foreach (GameHex * hex, hex_row)
                if (hex->color == player->color && hex->type == "Mill")
                    ++num_of_mills;

        int current_size = game->Connected(player->color).size();

        foreach (QList<GameHex*> hex_row, game->hexes)
        {
            foreach (GameHex * hex, hex_row)
            {
                qreal eval = 0;

                // бонус за подсоединение территорий
                if (hex->color == player->color)
                {
                    hex->color = "Neutral";
                    int new_size = game->Connected(player->color).size();
                    hex->color = player->color;

                    eval += (current_size - new_size) * MILL_TERRITORY_COEFF;
                }
                else
                {
                    QSet<Coord> wbc;
                    int new_size = game->Connected(player->color, false, wbc << hex->coord).size();
                    eval += (new_size - current_size) * MILL_TERRITORY_COEFF;
                }

                // отдельные бонусы за количество фортов и мельниц
                if (hex->type == "Fort")
                {
                    qreal fort_bonus = ONE_FORT;
                    for (int i = 1; i < forts_limits; ++i)
                        fort_bonus *= FORT_COEFF;
                    eval += fort_bonus;

                    if (player->resources["Attack"] == forts_limits)
                        eval += ATTACK_LIMIT_INCR;
                    if (player->resources["Go"] == forts_limits)
                        eval += GO_LIMIT_INCR;
                }
                if (hex->type == "Mill")
                {
                    qreal mill_bonus = ONE_MILL;
                    for (int i = 1; i < num_of_mills; ++i)
                        mill_bonus *= MILL_COEFF;
                    eval += mill_bonus;
                }

                // бонусы за ресурсы
                if (hex->type == "Simple")
                {
                    int attacks = player->resources["Attack"];
                    int gos = player->resources["Go"];
                    int libers = player->resources["Liberate"];

                    foreach (Resource R, hex->resources)
                    {
                        if (hex->color == player->color)
                        {
                            if (R == "Attack")
                            {
                                --attacks;
                                eval += ATTACK_VAL[qMin(9, qMax(0, attacks))];
                            }
                            if (R == "Gos")
                            {
                                --gos;
                                eval += GO_VAL[qMin(9, qMax(0, gos))];
                            }
                            if (R == "Liberate")
                            {
                                --libers;
                                eval += LIBER_VAL[qMin(9, qMax(0, libers))];
                            }
                        }
                        else
                        {
                            forts_limits = qMin(forts_limits, 9);
                            if (R == "Attack")
                            {
                                ++attacks;
                                eval += ATTACK_VAL[qMin(forts_limits, attacks)];
                            }
                            if (R == "Gos")
                            {
                                ++gos;
                                eval += GO_VAL[qMin(forts_limits, gos)];
                            }
                            if (R == "Liberate")
                            {
                                ++libers;
                                eval += LIBER_VAL[qMin(forts_limits, libers)];
                            }
                        }
                    }
                }

                // бонус за то, что в этой клетке ещё не создан юнит
                // или за то, что он является домом
                if (hex->status == GameHex::HOME)
                {
                    foreach (GameUnit * enemy_unit, game->players[hex->color]->units)
                        if (enemy_unit->home == hex->coord)
                            eval += enemy_unit->health;
                }
                else if (hex->status == GameHex::NOT_A_HOME)
                {
                    eval += POTENTIAL_RECRUIT;
                }
                else if (hex->status != GameHex::TOMBSTONE)
                {
                    eval += FREE_HEX;
                }

                hex_value[hex->coord][player->color] = eval;
            }
        }
    }
}
void AI::hex_propagate()
{
    QMap<Coord, QMap<PlayerColor, qreal> > new_hex_value;

    foreach (QList<GameHex*> hex_row, game->hexes)
    {
        foreach (GameHex * hex, hex_row)
        {
            QMap<WAY, Coord> nb = game->adjacentHexesMap(hex->coord);
            foreach (GamePlayer * player, game->players)
            {
                qreal new_val = hex_value[hex->coord][player->color];
                qreal weight = 1;

                for (QMap<WAY, Coord>::iterator it = nb.begin(); it != nb.end(); ++it)
                {
                    if (game->hex(it.value())->canGoHere)
                    {
                        qreal w = hex->rivers[it.key()] ? ADJ_RIVER_HEX_WEIGHT : ADJ_HEX_WEIGHT;
                        weight += w;
                        new_val += w * hex_value[it.value()][player->color];
                    }
                }

                new_val /= weight;
                new_hex_value[hex->coord][player->color] = new_val;
            }
        }
    }

    hex_value = new_hex_value;
}

// оценка плана
qreal AI::Eval(GameUnit * unit, const Plan & plan, const QSet<Coord> & help_required, QSet<Coord> & help_will_be_needed)
{
    qreal eval = 0;
    Coord unit_c = unit->position;
    bool help_satisfied = false;
    //qreal health_K = unit->health / 5;

    for (DayTime dt = 0; dt < game->rules->dayTimes; ++dt)
    {
        Coord unit_new_c = plan[dt].moves(unit_c);

        int ef = enemy_force(unit->color, unit_new_c, dt);

        // штраф за риск попасть в бой со штрафом
        if (ef > 0)
            eval += GO_RISK_PENALTY * plan[dt].fightInfluence;

        int my_force = forces[unit->color][unit_new_c][dt];
        if (ef > my_force)
            eval -= I_AM_GONA_LOSE_PENALTY;
        else if (ef == my_force)
            eval -= I_AM_GONA_DRAW_PENALTY;

        if (ef >= cur_plan_forces[unit_new_c][dt] + (unit->position == unit_new_c ? 0 : unit->health))
        {
            help_will_be_needed << unit_new_c;
        }

        if (unit_new_c != unit_c)
        {
            eval -= SPENT_MOVING;

            if (help_required.contains(unit_c) && ef >= cur_plan_forces[unit_new_c][dt])
                help_satisfied = true;

            int cur_hex_force = cur_plan_forces[unit_c][dt] + (unit->position == unit_c ? 0 : unit->health);

            int leave_ef = enemy_force(unit->color, unit_c, dt);
            if (game->hex(unit_c)->color == unit->color)
            {
                if (cur_hex_force - unit->health < leave_ef)
                    eval -= hex_value[unit_c][unit->color] * STAY_ON_HEX_COEFF;
            }
            else if (game->hex(unit_c)->color == "Neutral")
            {
                eval -= hex_value[unit_c][unit->color] * ENTER_EMPTY_HEX;
            }
            else
            {
                if (cur_hex_force - unit->health <= leave_ef || cur_hex_force - unit->health <= leave_ef == 0)
                    eval -= hex_value[unit_c][game->hex(unit_c)->color] * STAY_ON_HEX_COEFF;
            }

            unit_c = unit_new_c;
            cur_hex_force = cur_plan_forces[unit_c][dt] + (unit->position == unit_c ? 0 : unit->health);

            if (game->hex(unit_c)->color == unit->color)
            {
                if (cur_hex_force - unit->health < ef)
                    eval += hex_value[unit_c][unit->color] * STAY_ON_HEX_COEFF;
            }
            else if (game->hex(unit_c)->color == "Neutral")
            {
                eval += hex_value[unit_c][unit->color] * ENTER_EMPTY_HEX;
            }
            else
            {
                if (cur_hex_force - unit->health <= ef)
                    eval += hex_value[unit_c][game->hex(unit_c)->color] * STAY_ON_HEX_COEFF;
            }
        }

        if (plan[dt].type == "Capture")
        {
            eval += hex_value[unit_c][unit->color];
        }
        if (plan[dt].type == "Recruit")
        {
            eval += 5;
        }
        if (plan[dt].type == "Liberate")
        {
            eval += hex_value[unit_c][game->hex(unit_c)->color];
        }
    }

    if (help_satisfied)
        eval += HELP_SATISFIED;

    return eval;
}

// перебор
// перебор всевозможных планов юнита
bool check_repeat(GameUnit * unit, DayTime dt, OrderType to_check)
{
    while (dt > 0)
    {
        --dt;
        if (unit->plan[dt]->type == "Go" || unit->plan[dt]->type == "Attack")
            return true;
        if (unit->plan[dt]->type == to_check)
            return false;
    }
    return true;
}
QList<Plan> AI::PossiblePlans(GameUnit * unit)
{
    QList<Plan> plans;
    plans << QMap<DayTime, GameOrder>();

    for (DayTime dt = 0; dt < game->rules->dayTimes; ++dt)
    {
        QList<Plan> new_plans;

        foreach (Plan plan, plans)
        {
            unit->plan.clear();

            Coord unit_c = unit->position;
            DayTime dayTime = 0;
            while (plan.contains(dayTime))
            {
                unit->plan[dayTime] = &plan[dayTime];
                unit_c = unit->plan[dayTime]->moves(unit_c);
                ++dayTime;
            }

            if (!game->must_be_last(unit, dayTime - 1))
            {
                QList< Game::PossibleChoice > options = game->whatCanUse(unit);
                foreach (Game::PossibleChoice pos_order, options)
                {
                    OrderType order = pos_order.variant;
                    if (pos_order.status == Game::POSSIBLE)
                    {
                        if (order == "Capture" && game->hex(unit_c)->color != unit->color &&
                            check_repeat(unit, dayTime, "Capture") &&
                            !already_captured.contains(unit_c))
                        {
                            Plan new_plan = plan;
                            new_plan[dayTime] = GameOrder(game->rules, unit->type, order);
                            new_plans << new_plan;
                        }
                        if (order == "Recruit" &&
                            check_repeat(unit, dayTime, "Recruit") &&
                            !already_recruited.contains(unit_c))
                        {
                            Plan new_plan = plan;
                            new_plan[dayTime] = GameOrder(game->rules, unit->type, order);
                            new_plan[dayTime].setParameter("Scrat");
                            new_plans << new_plan;
                        }
                        if (order == "Liberate" &&
                            check_repeat(unit, dayTime, "Liberate") &&
                            !already_liberated.contains(unit_c) &&
                            game->hex(unit_c)->color != "Neutral" &&
                            game->hex(unit_c)->color != unit->color)
                        {
                            Plan new_plan = plan;
                            new_plan[dayTime] = GameOrder(game->rules, unit->type, order);
                            new_plans << new_plan;
                        }
                        if (order == DefaultOrder)
                        {
                            Plan new_plan = plan;
                            new_plan[dayTime] = GameOrder(game->rules, unit->type, order);
                            new_plans << new_plan;
                        }
                        if (order == "Attack" || order == "Go")
                        {
                            foreach (Coord target, game->adjacentHexes(unit_c))
                            {
                                if (game->hex(target)->canGoHere)
                                {
                                    Plan new_plan = plan;
                                    new_plan[dayTime] = GameOrder(game->rules, unit->type, order);
                                    new_plan[dayTime].setParameter(target);
                                    new_plans << new_plan;
                                }
                            }
                        }
                    }
                }
            }
            else
                new_plans << plan;
        }

        plans = new_plans;
    }
    unit->plan.clear();

    return plans;
}
void AI::plan(PlayerColor color)
{
    // будем хранить свою текущую силу во всех клетках
    cur_plan_forces.clear();
    foreach (GameUnit * unit, game->players[color]->units)
    {
        for (int dt = 0; dt < game->rules->dayTimes; ++dt)
        {
            cur_plan_forces[unit->position][dt] += unit->health;
        }
    }

    // возможно, мы спланируем атаку туда, куда нужно будет отправить ещё юнитов.
    // тогда в будущем за посещение клеток, где нужна подмога, будем давать плюс много.
    QSet<Coord> help_required;

    // здесь запоминаем захватываемые, освобождаемые и рекрутируемые клетки
    // чтобы не проводить захваты по несколько раз
    already_captured.clear();
    already_liberated.clear();
    already_recruited.clear();

    bool nothing_happens = false;
    while (!nothing_happens)
    {
        // перебираем всевозможные планы и выбираем лучший
        GameUnit * best_unit = NULL;
        qreal best_eval = -INF;
        Plan best_plan;
        QSet<Coord> best_plan_help_will_be_needed;

        foreach (GameUnit* unit, game->players[color]->units)
        {
            if (unit->plan[0] == NULL)
            {
                foreach (Plan plan, PossiblePlans(unit))
                {
                    QSet<Coord> help_will_be_needed;
                    qreal eval = Eval(unit, plan, help_required, help_will_be_needed);
                    if (best_unit == NULL || eval > best_eval)
                    {
                        best_unit = unit;
                        best_eval = eval;
                        best_plan = plan;
                        best_plan_help_will_be_needed = help_will_be_needed;
                    }
                }
            }
        }

        nothing_happens = true;
        if (best_unit != NULL)
        {
            Coord unit_c = best_unit->position;
            for (int dt = 0; dt < game->rules->dayTimes; ++dt)
            {
                int penalty = 0;
                if (best_plan.contains(dt))
                {
                    best_unit->plan[dt] = new GameOrder(best_plan[dt]);

                    if (best_plan[dt].type != DefaultOrder)
                        nothing_happens = false;
                    if (best_plan[dt].type == "Capture")
                        already_captured << unit_c;
                    if (best_plan[dt].type == "Liberate")
                        already_liberated << unit_c;
                    if (best_plan[dt].type == "Recruit")
                        already_recruited << unit_c;

                    unit_c = best_unit->plan[dt]->moves(unit_c);
                    penalty = best_unit->plan[dt]->fightInfluence;
                }

                cur_plan_forces[best_unit->position][dt] -= best_unit->health;
                cur_plan_forces[unit_c][dt] += best_unit->health + penalty;
            }

            help_required |= best_plan_help_will_be_needed;
        }
    }
}
