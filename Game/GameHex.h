#ifndef GAMEHEX
#define GAMEHEX

#include "Technical/Headers.h"

// Параметры гекса, зависящие только от его типа

class GameHexParameters
{
public:
    HexType type;

    bool canBeCaptured;
    bool canHaveResources;
    bool canBeChosenAsStartPoint;
    bool canGoHere;
    bool isBuilding;
    bool forbidsToCaptureAdjacentHexesWhenCaptured;
    int highingResourceLimitWhenCaptured;
    int defenceBonusWhenCaptured;
    bool mustBeCapturedToWin;

    GameHexParameters(HexType type);
};

// Класс игрового гекса

class GameHex : public GameHexParameters
{
public:
    int defence = 0;
    int fortification = 0;
    PlayerColor fortificationColor = "Neutral";

    PlayerColor color = "Neutral";

    Coord coord;

    QList <Resource> resources;

    UnitType livingNation = "";    // для старта игры

    explicit GameHex(HexType type, Coord Pos) : GameHexParameters(type)
    {
        coord = Pos;
    }
};

#endif
