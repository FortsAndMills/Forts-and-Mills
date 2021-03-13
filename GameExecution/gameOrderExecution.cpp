#include "Game.h"

// проверка на легитимность действия
bool Game::CheckIfActionBurns(Action a)
{
    // если юнит умер или его приказ был сожжён
    if (africa.contains(a.unit))
        return true;
    else if (a.order->realizationFinished)
        return true;

    if (a.action.type == GameAction::RECRUIT)
    {
        // рекрут на агитированной территории не прокатывает
        if (isAgitatedByEnemy(a.unit->position, a.unit->color))
        {
            players[a.unit->color]->resources[a.order->type]--;
            a.order->realizationFinished = true;
            AddEvent()->RecruitFailsBecauseOfAgite(a.unit, a.order->type, hex(a.unit->position));
            return true;
        }

        // рекрут на отсоединённой территории
        if (!Connected(a.unit->color, true).contains(hex(a.unit->position)))
        {
            players[a.unit->color]->resources[a.order->type]--;
            a.order->realizationFinished = true;

            // TODO: нет, причина сгорания приказа другая
            AddEvent()->RecruitFailsBecauseOfAgite(a.unit, a.order->type, hex(a.unit->position));
            return true;
        }
    }

    return false;
}

// одинаковые действия выполняются "одновременно"
// важно для, например, покидания гекса юнитом
void Game::Realize(QList<Action> act)
{
    if (act.size() == 0)
        return;

    GameAction::GameActionType T = act[0].action.type;
    if (T == GameAction::START_REALIZATION)
    {
        foreach (Action a, act)
        {
            a.order->realizationStarted = true;
            AddEvent()->OrderRealizationStarted(a.unit, a.unit->color, a.order->type);
        }
    }
    else if (T == GameAction::LEAVE_HEX)
    {
        foreach (Action a, act)
        {
            // "выход из клетки"
            a.unit->going_to = a.action.target;
            AddEvent()->UnitLeaves(a.unit, hex(a.action.target));
        }
    }
    else if (T == GameAction::PURSUE)
    {
        QMap<GameUnit *, Coord> triggered;
        foreach (Action a, act)
        {
            if (!africa.contains(a.action.unit))
            {
                // считаем клетку, в которую нужно идти, чтобы поймать целевого юнита
                Coord target_pos = (a.action.unit->going_to == NOWHERE ? a.action.unit->position : a.action.unit->going_to);

                // если целевой юнит не мёртв и он находится в соседней клетке
                if (isAdjacent(a.unit->position, target_pos))
                {
                    triggered[a.unit] = target_pos;
                }
            }
        }

        foreach (Action a, act)
        {
            if (triggered.contains(a.unit))
            {
                Coord target_pos = triggered[a.unit];

                // выходим из клетки (аналогично LEAVE_HEX)
                a.unit->going_to = target_pos;
                AddEvent()->UnitLeaves(a.unit, hex(target_pos));
            }
            else
            {
                AddEvent()->PursueNotTriggered(a.unit, a.action.unit);
            }
        }
    }
    else if (T == GameAction::AMBUSH)
    {
        foreach (Action a, act)
        {
            a.unit->distantAttack = a.action.amount;

            if (a.unit->distantAttack > 0)
            {
                // стреляем во врагов, которые вышли откуда угодно
                // в направлении нашей текущей позиции
                QSet <GameUnit *> targets = find(ENEMY, a.unit, ANY, a.unit->position);
                if (targets.size() > 0)
                {
                    UnitsFight(targets << a.unit);
                }
            }

            a.unit->distantAttack = 0;
        }
    }
    else if (T == GameAction::SHOOT)
    {
        foreach (Action a, act)
        {
            a.unit->distantAttack = a.action.amount;

            while (a.unit->distantAttack > 0)
            {
                QSet <GameUnit *> targets;
                // сначала стреляем в тех, кто входит в нашу клетку с правильной стороны
                // "выстрел в лицо" (правило может быть отменено)
                if (rules->pig_can_shoot_attacker && adjacentHexes(a.unit->position).contains(a.action.target))
                {
                    targets = find(ENEMY, a.unit, a.action.target, a.unit->position);
                }

                // иначе в тех, кто стоит в целевой клетке или входит в неё
                // решили, что во всю толпу сразу
                if (targets.size() == 0)
                {
                    targets = find(ENEMY, a.unit, a.action.target, NOWHERE);
                    targets += find(ENEMY, a.unit, ANY, a.action.target);
                }

                if (targets.size() == 0)
                {
                    break;
                }

                UnitsFight(targets << a.unit);
            }

            // оставшиеся выстрелы должны отправиться в пустоту в визуализации
            if (a.unit->distantAttack == a.action.amount)
            {
                AddEvent()->ShootLeft(a.unit, hex(a.action.target), a.unit->distantAttack);
            }

            a.unit->distantAttack = 0;
        }
    }
    else if (T == GameAction::CROSS)
    {
        foreach (Action a, act)
        {
            // не триггернутые перемещения (например, от PURSUE)
            if (a.unit->going_to == NOWHERE)
                continue;

            // если кто-то вышел навстречу, то они должны встретится и сразиться
            if (a.unit->health > 0)
            {
                QSet <GameUnit *> enemies = find(ENEMY, a.unit, a.unit->going_to, a.unit->position);

                if (enemies.size() > 0)
                {
                    // возможно, мы не одни, кто пересекает эту клетку
                    QSet <GameUnit *> allies = find(ALLY, a.unit, a.unit->position, a.unit->going_to);
                    allies << a.unit;

                    // сражение
                    QSet <GameUnit *> fighters = enemies + allies;
                    AddEvent()->UnitsAreGoingToFight(fighters);
                    UnitsFight(fighters);
                }
            }
        }

        // успешно вышедшие входят в целевую клетку
        foreach (Action a, act)
        {
            if (a.unit->health > 0 && a.unit->going_to != NOWHERE)
                AddEvent()->UnitEnters(a.unit, hex(a.unit->going_to), find(ENEMY, a.unit, a.unit->going_to, ANY));
        }
    }
    else if (T == GameAction::ENTER_HEX_WITH_FIGHT)
    {
        foreach (Action a, act)
        {
            // не триггернутые перемещения (например, от PURSUE)
            if (a.unit->going_to == NOWHERE)
                continue;

            if (a.unit->health > 0)
            {
                // ищем врагов в клетке
                QSet <GameUnit *> enemies = find(ENEMY, a.unit, a.unit->going_to, NOWHERE);
                // добавляем врагов, пришедших на подмогу
                enemies += find(ENEMY, a.unit, ANY, a.unit->going_to);

                if (enemies.size() > 0)
                {
                    // ищем союзников в клетке (такое может быть при возвращении в родную клетку от PURSUE)
                    QSet <GameUnit *> allies = find(ALLY, a.unit, a.unit->going_to, NOWHERE);
                    // ищем союзников, пришедших на подмогу
                    allies += find(ALLY, a.unit, ANY, a.unit->going_to);
                    allies << a.unit;

                    // сражение
                    QSet <GameUnit *> fighters = enemies + allies;
                    AddEvent()->UnitsAreGoingToFight(fighters);
                    UnitsFight(fighters);
                }
            }
        }
    }
    else if (T == GameAction::RETURN)
    {
        foreach (Action a, act)
        {
            if (a.unit->going_to != NOWHERE)
            {
                // меняем местами позицию и целевую клетку
                Coord pos = a.unit->position;
                a.unit->position = a.unit->going_to;
                a.unit->going_to = pos;

                // оказалось удобнее, чем отдельное действие
                AddEvent()->UnitFinishesEnter(a.unit, hex(a.unit->position));
                AddEvent()->UnitLeaves(a.unit, hex(a.unit->going_to));
                AddEvent()->UnitEnters(a.unit, hex(a.unit->going_to), find(ENEMY, a.unit, a.unit->going_to, ANY));
            }
        }
    }
    else if (T == GameAction::FINISH_ENTERING)
    {
        foreach (Action a, act)
        {
            if (a.unit->going_to != NOWHERE)
            {
                // заканчиваем вход в клетку
                a.unit->position = a.unit->going_to;
                a.unit->going_to = NOWHERE;

                GameHex * Hex = hex(a.unit->position);

                // в этот момент убиваем "рекрутируемых" юнитов
                KillRecruited(Hex, a.unit);

                // если есть укрепление, они исчезают
                if (Hex->fortificationColor != "Neutral" &&
                     Hex->fortificationColor != a.unit->color)
                {
                    if (Hex->fortification != 0)
                    {
                        Hex->fortification = 0;
                        AddEvent()->FortificationDisappears(a.unit, Hex);
                    }

                    Hex->fortificationColor = "Neutral";
                }

                AddEvent()->UnitFinishesEnter(a.unit, Hex);
            }
        }
    }
    else if (T == GameAction::AGITE)
    {
        foreach (Action a, act)
        {
            GameHex * Hex = hex(a.action.target);

            KillRecruited(Hex, a.unit);

            Hex->agitated.insert(a.unit->color);
            AddEvent()->Agitated(a.unit, hex(a.action.target));
        }
    }
    else if (T == GameAction::LIBERATE)
    {
        foreach (Action a, act)
        {
            GameHex * Hex = hex(a.unit->position);
            PlayerColor color = Hex->color;

            // нейтрализуем цвет гекса
            if (Hex->color != "Neutral" &&
                 Hex->color != a.unit->color)
            {
                DecaptureHex(Hex, a.unit);
            }

            // может быть потеряно соединение с мельницей
            // требуется смена иконок (статусов гексов)
            RecheckMillConnection(color, a.unit);
        }
    }
    else if (T == GameAction::CAPTURE_HEX)
    {
        foreach (Action a, act)
        {
            GameHex * Hex = hex(a.unit->position);

            if (Hex->canBeCaptured && Hex->color == "Neutral")
            {
                // появление бонуса в обороне при захвате
                if (Hex->defenseBonusWhenCaptured != 0)
                {
                    Hex->defense = Hex->defenseBonusWhenCaptured;
                    AddEvent()->DefenseBonusAppears(a.unit, Hex, a.unit->color, Hex->defense);
                }

                // перекрашиваем
                Hex->color = a.unit->color;

                // требуется смена иконок (статусов гексов)
                RecheckMillConnection(a.unit->color, a.unit);

                AddEvent()->UnitCapturesHex(a.unit, Hex, a.unit->color);
            }
        }
    }
    else if (T == GameAction::RECRUIT)
    {
        foreach (Action a, act)
        {
            // если клетка захвачена и к ней никто не привязан
            if (hex(a.unit->position)->color == a.unit->color)
            {
                if (hex(a.unit->position)->status == GameHex::NOT_A_HOME)
                {
                    AddEvent()->UnitIsGoingToRecruit(a.unit, hex(a.unit->position), a.action.unitType);
                    hex(a.unit->position)->status = GameHex::RECRUITING;
                    recruitedUnits << Recruited(a.unit->position, a.action.unitType, a.unit->color);
                }
            }
        }
    }
    else if (T == GameAction::CURE)
    {
        // лечим, если можно
        foreach (Action a, act)
        {
            if (a.unit->health < a.unit->max_health)
            {
                int val = qMin(a.unit->max_health - a.unit->health, a.action.amount);

                a.unit->health += val;
                AddEvent()->UnitCures(a.unit, val);
            }
        }
    }
    else if (T == GameAction::FORTIFICATE)
    {
        // добавляем укрепление
        foreach (Action a, act)
        {
            hex(a.unit->position)->fortificationColor = a.unit->color;
            hex(a.unit->position)->fortification += a.action.amount;
            AddEvent()->Fortified(a.unit, hex(a.unit->position), a.action.amount);
        }
    }
    else if (T == GameAction::FINISH_REALIZATION)
    {
        // в этот момент удаляем единицу соотв. ресурса
        foreach (Action a, act)
        {
            --players[a.unit->color]->resources[a.order->type];
            a.order->realizationFinished = true;
            AddEvent()->OrderRealizationFinished(a.unit, a.unit->color, a.order->type);
        }
    }
}
