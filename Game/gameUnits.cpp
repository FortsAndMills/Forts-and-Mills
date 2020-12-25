#include "GameUnit.h"
#include "GameRules.h"

GameUnitParameters::GameUnitParameters(GameRules *, UnitType type)
{
    this->type = type;

    max_health = 5;
    init_health = 5;
    max_defenceBonus = 0;
    doubles = false;

    if (type == "Pig")
    {
        max_health = 4;
        init_health = 4;
    }
    if (type == "Karkun")
    {
        max_health = 4;
        init_health = 4;
    }
    if (type == "Hippo")
    {
        max_health = 4;
        init_health = 4;
        max_defenceBonus = 2;
    }
    if (type == "Mumusha")
    {
        max_health = 7;
        init_health = 3;
    }
    if (type == "Rabbit")
    {
        max_health = 3;
        init_health = 3;
        doubles = true;
    }
}
