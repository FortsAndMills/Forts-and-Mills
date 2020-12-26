#include "GameOrder.h"
#include "GameRules.h"

// ИНИЦИАЛИЗАЦИЯ ИГРОВЫХ ПРИКАЗОВ
// каждое игровое действие имеет "приоритет" (число)
// сначала будут исполняться действия с маленьким приоритетом
// поэтому приоритет действия "захватить" должно быть меньше приоритета "рекрутировать"
// тогда эти действия можно исполнять двумя юнитами одновременно

GameOrderParameters::GameOrderParameters(GameRules * rules, UnitType owner, OrderType type)
{
    this->type = type;

    if (type == "Attack")
    {
        burnsWhenFight = false;
        fightInfluence = 0;

        AddAction(100)->LeaveHex();  // выходим из клетки
        AddAction(200)->Cross();     // пересекаем границу (может случиться бой, если шли
                                     // нам навстречу)
        AddAction(300)->EnterHexWithFight();  // сражаемся с защитниками клетки
        AddAction(400)->FinishEntering();     // вошли в клетку (и не умерли в бою)
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction(450)->Liberate();
    }
    if (type == "Go")
    {
        burnsWhenFight = false;
        fightInfluence = -1;

        AddAction(100)->LeaveHex();
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(400)->FinishEntering();
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction(450)->Liberate();
    }
    if (type == "Retreat")
    {
        burnsWhenFight = false;
        fightInfluence = -2;

        // [сначала с приоритетом 100 выполнятся выходы обычных перемещений]
        AddAction(140)->Ambush(1);  // мы стреляем во всех, кто вышел в нашу клетку
        AddAction(160)->LeaveHex(); // выходим сами; далее стандартно
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(400)->FinishEntering();
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction(450)->Liberate();
    }
    if (type == "Pursue")
    {
        burnsWhenFight = false;

        // [сначала с приоритетом 100 выполнятся выходы обычных перемещений]
        AddAction(120)->Pursue();  // выходим в клетку, где находится цель
        // [с приоритетом 140 происходит засада приказа Retreat]
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(320)->Return();  // вместо входа в клетку разворачиваемся и
                                   // идём в обратном направлении
        AddAction(330)->EnterHexWithFight();  // в покинутой клетке мог появиться враг!
        AddAction(400)->FinishEntering();
    }
    if (type == "Siege")
    {
        burnsWhenFight = false;
        fightInfluence = 2;

        if (owner == "Karkun")
        {
            AddAction(100)->LeaveHex();
            AddAction(200)->Cross();
            AddAction(300)->EnterHexWithFight();
            AddAction(400)->FinishEntering();
            if (rules->doesEnteringEnemyHexLiberates)
                AddAction(450)->Liberate();
        }
        else
            canUse = false;
    }
    if (type == "Fire")
    {
        burnsWhenFight = false;
        fightInfluence = -rules->fire_has_penalty;

        if (owner == "Pig")
        {
            // юниты уже вышли из клеток (100) и сразились с теми,
            // кто вышел им навстречу (200) ("бой между клетками")
            AddAction(250)->Shoot(2);
            // бой с защитниками клетки (300)
        }
        else
            canUse = false;
    }
    if (type == "Liberate")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(450)->Liberate();
    }
    if (type == "Cure")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(500)->Cure(1);
    }
    if (type == "Fortify")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(600)->Fortificate(1);
    }
    if (type == "Agite")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(650)->Agite();
    }
    if (type == "Capture")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(700)->CaptureHex();
        if (rules->doesCaptureRecruits)
            AddAction(800)->Recruit();
    }
    if (type == "Recruit")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(800)->Recruit();
    }

    burnsWhenFight = burnsWhenFight && rules->peacefullOrdersBurns;
}


QTextStream &operator <<(QTextStream &stream, const GameOrder *r)
{
    stream << r->type;
    foreach (GameAction a, r->actions)
    {
        if (a.whatParameter() == GameAction::PT_UNIT_TYPE)
            stream << " (" << a.unitType << ")";
        else if (a.whatParameter() == GameAction::PT_UNIT)
            stream << " " << a.unit->id;
        else if (a.whatParameter() == GameAction::PT_HEX)
            stream << " " << a.target;
    }
    stream << ";\n";
    return stream;
}
QTextStream &operator >>(QTextStream &stream, GameOrder *)
{
    // NOT IMPLEMENTED
    return stream;
}
