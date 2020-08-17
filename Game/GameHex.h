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
    bool isTall;
    bool forbidsToCaptureAdjacentHexesWhenCaptured;
    int increasesResourceLimitWhenCaptured;
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

    QSet<PlayerColor> agitated;
    QSet<PlayerColor> recruited;
    
    PlayerColor color = "Neutral";

    Coord coord;
    QMap <WAY, bool> rivers;

    QList <Resource> resources;

    //UnitType livingNation = "";    // для старта игры
    //Coord region_center = NOWHERE; // регион

    explicit GameHex(HexType type, Coord Pos) : GameHexParameters(type)
    {
        coord = Pos;
    }
};

#endif
