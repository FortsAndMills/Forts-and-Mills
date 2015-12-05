#include "GameUnit.h"

void GameUnit::setType(UnitType type)
{
    this->type = type;
    this->max_health = 4;  // по умолчанию

    if (type == "Scrat")       // особенности
        this->max_health = 5;
    else if (type == "Pig")
        this->max_health = 3;
    else if (type == "Hippo")
    {

    }
    else if (type == "Karkun")
    {

    }
}

