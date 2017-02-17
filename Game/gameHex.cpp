#include "GameHex.h"

GameHexParameters::GameHexParameters(HexType type)
{
    this->type = type;

    canBeCaptured = true;
    canHaveResources = false;
    canBeChosenAsStartPoint = false;
    canGoHere = true;
    isBuilding = true;
    forbidsToCaptureAdjacentHexesWhenCaptured = false;
    highingResourceLimitWhenCaptured = 0;
    defenceBonusWhenCaptured = 0;
    mustBeCapturedToWin = false;

    if (type == "Simple")
    {
        canHaveResources = true;
        isBuilding = false;
    }
    if (type == "Mill")
    {
        canBeChosenAsStartPoint = true;
        highingResourceLimitWhenCaptured = 1;
    }
    if (type == "Fort")
    {
        forbidsToCaptureAdjacentHexesWhenCaptured = true;
        defenceBonusWhenCaptured = 2;
        mustBeCapturedToWin = true;
    }
    if (type == "Mountain")
    {
        canBeCaptured = false;
        canGoHere = false;
    }
}
