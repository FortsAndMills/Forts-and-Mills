#include "GameHex.h"
#include "Game.h"
#include "GamePlayer.h"

void GameHex::setType(HexType type)
{
    this->type = type;

    this->canBeCaptured = true;      // по умолчанию
    this->canHaveResources = false;
    this->canBeChosenAsStartPoint = false;
    this->canGoHere = true;
    this->isBuilding = true;

    if (type == "Simple")  // особенности типа
    {
        this->canHaveResources = true;
        this->isBuilding = false;
    }
    if (type == "Mill")
    {
        canBeChosenAsStartPoint = true;
    }
    if (type == "Fort")
    {

    }
    if (type == "Mountain")
    {
        this->canBeCaptured = false;
        this->canGoHere = false;
    }
}



