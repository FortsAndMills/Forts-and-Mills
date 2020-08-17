#include "GameUnit.h"
#include "GameRules.h"

GameUnitParameters::GameUnitParameters(GameRules * rules, UnitType type)
{
    this->type = type;

    max_health = 5;
    init_health = 5;
    max_defenceBonus = 0;
    isDestroyingAllies = false;

    startResources << "Capture" << "Go" << "Attack";

    if (type == "Pig")
    {
        max_health = 4;
        init_health = 4;
        startResources[2] = "Fire";

        if (rules->ordersInGame.contains("Retreat"))
            startResources << "Retreat";
    }
    if (type == "Karkun")
    {
        max_health = 4;
        init_health = 4;
        startResources[2] = "Siege";
    }
    if (type == "Hippo")
    {
        max_health = 4;
        init_health = 4;
        max_defenceBonus = 2;
    }
    if (type == "Bruter")
    {
        max_health = 6;
        init_health = 6;
        isDestroyingAllies = true;
    }
    if (type == "Mumusha")
    {
        max_health = 6;
        init_health = 3;
        startResources << "Cure" << "Cure";
    }
}
