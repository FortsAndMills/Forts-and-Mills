#include "GameOrder.h"
#include "GameRules.h"

// ИНИЦИАЛИЗАЦИЯ ИГРОВЫХ ПРИКАЗОВ

GameOrderParameters::GameOrderParameters(GameRules * rules, UnitType owner, OrderType type)
{
    this->type = type;

    if (type == "Capture")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(700)->CaptureHex();
        if (rules->doesCaptureRecruits)
            AddAction(800)->Recruit();
    }
    if (type == "Agite")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(650)->Agite();
    }
    if (type == "Recruit")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(800)->Recruit();
    }
    if (type == "Liberate")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction(450)->Liberate();
    }
    if (type == "Attack")
    {
        burnsWhenFight = false;
        fightInfluence = 0;

        AddAction(100)->LeaveHex();
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(400)->FinishEntering();
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

        AddAction(140)->Ambush(1);
        AddAction(160)->LeaveHex();
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(400)->FinishEntering();
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction(450)->Liberate();
    }
    if (type == "Pursue")
    {
        burnsWhenFight = false;

        AddAction(120)->Pursue();
        AddAction(200)->Cross();
        AddAction(300)->EnterHexWithFight();
        AddAction(320)->Return();
        AddAction(330)->EnterHexWithFight();
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
            AddAction(250)->Shoot(2);
        }
        else
            canUse = false;
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
