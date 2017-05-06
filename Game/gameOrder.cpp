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

        AddAction()->CaptureHex(700);
        if (rules->doesCaptureRecruits)
            AddAction()->Recruit(800);
    }
    if (type == "Recruit")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction()->Recruit(800);
    }
    if (type == "Liberate")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction()->Liberate(450);
    }
    if (type == "Attack")
    {
        burnsWhenFight = false;
        fightInfluence = 0;

        AddAction()->LeaveHex(100);
        AddAction()->EnterHexWithFight(300);
        AddAction()->FinishEntering(400);
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction()->Liberate(450);
    }
    if (type == "Go")
    {
        burnsWhenFight = false;
        fightInfluence = -1;

        AddAction()->LeaveHex(100);
        AddAction()->EnterHexWithFight(300);
        AddAction()->FinishEntering(400);
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction()->Liberate(450);
    }
    if (type == "Retreat")
    {
        burnsWhenFight = false;
        fightInfluence = -2;

        AddAction()->Ambush(140, 1);
        AddAction()->LeaveHex(160);
        AddAction()->EnterHexWithFight(300);
        AddAction()->FinishEntering(400);
        if (rules->doesEnteringEnemyHexLiberates)
            AddAction()->Liberate(450);
    }
    if (type == "Siege")
    {
        burnsWhenFight = false;
        fightInfluence = 2;

        if (owner == "Karkun")
        {
            AddAction()->LeaveHex(100);
            AddAction()->EnterHexWithFight(300);
            AddAction()->FinishEntering(400);
            if (rules->doesEnteringEnemyHexLiberates)
                AddAction()->Liberate(450);
        }
        else
            canUse = false;
    }
    if (type == "Fire")
    {
        burnsWhenFight = false;
        fightInfluence = 0;

        if (owner == "Pig")
        {
            AddAction()->Shoot(200, 2);
        }
        else
            canUse = false;
    }
    if (type == "Cure")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction()->Cure(500, 1);
    }
    if (type == "Fortify")
    {
        burnsWhenFight = true;
        fightInfluence = -1;

        AddAction()->Fortificate(600, 1);
    }
}


QTextStream &operator <<(QTextStream &stream, const GameOrder *r)
{
    stream << r->type;
    foreach (GameAction a, r->actions)
    {
        if (a.whatParameter() == 1)
            stream << " (" << a.unitType << ")";
        else if (a.whatParameter() == 2)
            stream << " " << a.target;
    }
    stream << "; " << r->priority << "\n";
    return stream;
}
QTextStream &operator >>(QTextStream &stream, GameOrder *r)
{

}
