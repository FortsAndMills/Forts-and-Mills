#include "GameHex.h"

// ИНИЦИАЛИЗАЦИЯ ИГРОВЫХ ГЕКСОВ

GameHexParameters::GameHexParameters(HexType type)
{
    this->type = type;

    canBeCaptured = true;
    canHaveResources = false;
    canGoHere = true;
    isTall = true;
    increasesResourceLimitWhenCaptured = 0;
    defenseBonusWhenCaptured = 0;

    if (type == "Simple")
    {
        canHaveResources = true;
        isTall = false;
    }
    if (type == "Fort")
    {
        defenseBonusWhenCaptured = 2;
        increasesResourceLimitWhenCaptured = 1;
    }
    if (type == "Mountain")
    {
        canBeCaptured = false;
        canGoHere = false;
    }
    if (type == "Lake")
    {
        canBeCaptured = false;
        canGoHere = false;
        isTall = false;
    }
}
