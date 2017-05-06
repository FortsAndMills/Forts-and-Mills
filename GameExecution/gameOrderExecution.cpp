#include "GameOrderExecution.h"

GameOrderExecution::GameOrderExecution(GameRules *rules, Random *rand) :
    GameFight(rules, rand)
{
}

// проверка на легитимность действия
bool GameOrderExecution::CheckIfActionBurns(Action a)
{
    // если юнит умер или его приказ был сожжён
    if (africa.contains(a.unit))
        return true;
    else if (a.order->realizationFinished)
        return true;

    // если есть запрет на захват
    if (a.action.type == GameAction::CAPTURE_HEX)
    {
        Coord reason = canBeCaptured(a.unit->position, a.unit->color);
        if (reason != NOWHERE)
        {
            players[a.unit->color]->resources[a.order->type]--;
            a.order->realizationFinished = true;
            AddEvent()->CaptureFailsBecauseOfCastle(a.unit, a.order->type, hex(a.unit->position), hex(reason));
            return true;
        }
    }

    return false;
}

// одинаковые действия выполняются "одновременно"
// важно для, например, покидания гекса юнитом
void GameOrderExecution::Realize(QList<Action> act)
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

            // если кто-то вышел навстречу, то они должны встретится и сразиться
            while (a.unit->health > 0)
            {
                QList <GameUnit *> enemies = find(ENEMY, a.unit, a.action.target, a.unit->position);
                if (enemies.size() == 0)
                    break;

                // сражение
                enemies << a.unit;
                AddEvent()->UnitsAreGoingToFight(enemies);
                UnitsFight(enemies);
            }
        }

        // успешно вышедшие входят в целевую клетку
        foreach (Action a, act)
        {
            if (a.unit->health > 0)
                AddEvent()->UnitEnters(a.unit, hex(a.unit->going_to), findAll(ENEMY, a.unit, a.unit->going_to, ANY));
        }
    }
    else if (T == GameAction::AMBUSH)
    {
        foreach (Action a, act)
        {
            // стреляем во врагов, которые входят в клетку
            a.unit->distantAttack = a.action.amount;

            while (a.unit->distantAttack > 0)
            {
                QList <GameUnit *> targets;
                targets = find(ENEMY, a.unit, ANY, a.unit->position);
                if (targets.size() == 0)
                {
                    break;
                }

                QList <GameUnit *> enemies;
                enemies << a.unit << targets;
                UnitsFight(enemies, QList <FightStatus>() << FS_DISTANT);
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
                QList <GameUnit *> targets;
                // сначала стреляем в тех, кто входит в нашу клетку с правильной стороны
                if (adjacentHexes(a.unit->position).contains(a.action.target))
                {
                    targets = find(ENEMY, a.unit, a.action.target, a.unit->position);
                }
                if (targets.size() == 0)  // иначе в тех, кто стоит в целевой клетке
                {
                    targets = find(ENEMY, a.unit, a.action.target, NOWHERE);
                }
                if (targets.size() == 0)  // иначе в тех, кто в неё входит
                {
                    targets = find(ENEMY, a.unit, ANY, a.action.target);
                }
                if (targets.size() == 0)
                {
                    break;
                }

                QList <GameUnit *> enemies;
                enemies << a.unit << targets;
                UnitsFight(enemies, QList <FightStatus>() << FS_DISTANT);
            }

            // оставшиеся выстрелы должны отправиться в пустоту
            if (a.unit->distantAttack == a.action.amount)
            {
                AddEvent()->ShootLeft(a.unit, hex(a.action.target), a.unit->distantAttack);
            }

            a.unit->distantAttack = 0;
        }
    }
    else if (T == GameAction::ENTER_HEX_WITH_FIGHT)
    {
        foreach (Action a, act)
        {
            // проверка на то, что в клетке изначально нет союзников, которые будут сражаться раньше нас
            if (find(ALLY, a.unit, a.unit->going_to, NOWHERE).size() == 0)
            {
                while (a.unit->health > 0)
                {
                    // сражаемся с теми, кто уже находится в клетке
                    QList <GameUnit *> enemies = find(ENEMY, a.unit, a.unit->going_to, NOWHERE);
                    enemies << find(ENEMY, a.unit, ANY, a.unit->going_to);
                    deleteAllies(enemies);

                    if (enemies.size() == 0)
                        break;

                    enemies << a.unit;
                    AddEvent()->UnitsAreGoingToFight(enemies);
                    UnitsFight(enemies);
                }
            }
        }
    }
    else if (T == GameAction::FINISH_ENTERING)
    {
        foreach (Action a, act)
        {
            // заканчиваем вход в клетку
            a.unit->position = a.unit->going_to;
            a.unit->going_to = NOWHERE;

            // в этот момент убиваем "рекутируемых" юнитов
            KillRecruited(hex(a.unit->position), a.unit);

            AddEvent()->UnitFinishesEnter(a.unit, hex(a.unit->position));
        }
    }
    else if (T == GameAction::LIBERATE)
    {
        foreach (Action a, act)
        {
            GameHex * Hex = hex(a.unit->position);

            // если есть укрепление
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

            // нейтрализуем цвет гекса
            if (Hex->color != "Neutral" &&
                 Hex->color != a.unit->color)
            {
                DecaptureHex(Hex, a.unit);
            }
        }
    }
    else if (T == GameAction::CAPTURE_HEX)
    {
        foreach (Action a, act)
        {
            // если ещё можно захватить
            if (hex(a.unit->position)->canBeCaptured &&
                hex(a.unit->position)->color == "Neutral")
            {
                // появление бонуса в обороне при захвате
                if (hex(a.unit->position)->defenceBonusWhenCaptured != 0)
                {
                    hex(a.unit->position)->defence = hex(a.unit->position)->defenceBonusWhenCaptured;
                    AddEvent()->DefenceBonusAppears(a.unit, hex(a.unit->position), a.unit->color, hex(a.unit->position)->defence);
                }

                // перекрашиваем
                hex(a.unit->position)->color = a.unit->color;
                AddEvent()->HexIsNotAHomeAnymore(hex(a.unit->position), a.unit->color, QSet <GameUnit *>() << a.unit);
                AddEvent()->UnitCapturesHex(a.unit, hex(a.unit->position), a.unit->color);
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
                if (!isHexAHome(a.unit->position, a.unit->color))
                {
                    AddEvent()->UnitIsGoingToRecruit(a.unit, hex(a.unit->position), a.action.unitType);
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