#include "GameHex.h"

// ИНИЦИАЛИЗАЦИЯ ИГРОВЫХ ГЕКСОВ

GameHexParameters::GameHexParameters(HexType type)
{
    this->type = type;

    canBeCaptured = true;
    canHaveResources = false;
    canBeChosenAsStartPoint = false;
    canGoHere = true;
    isTall = true;
    forbidsToCaptureAdjacentHexesWhenCaptured = false;
    increasesResourceLimitWhenCaptured = 0;
    defenceBonusWhenCaptured = 0;
    mustBeCapturedToWin = false;

    if (type == "Simple")
    {
        canHaveResources = true;
        isTall = false;
    }
    if (type == "Mill")
    {
        //canBeChosenAsStartPoint = true;
        //increasesResourceLimitWhenCaptured = 1;
    }
    if (type == "Fort")
    {
        forbidsToCaptureAdjacentHexesWhenCaptured = true;
        defenceBonusWhenCaptured = 2;
        canBeChosenAsStartPoint = true;
        increasesResourceLimitWhenCaptured = 1;
        //mustBeCapturedToWin = true;
    }
    if (type == "Mountain")
    {
        canBeCaptured = false;
        canGoHere = false;
    }
}
