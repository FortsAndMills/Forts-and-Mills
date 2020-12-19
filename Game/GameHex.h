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
    bool canGoHere;
    bool isTall;
    int increasesResourceLimitWhenCaptured;
    int defenceBonusWhenCaptured;

    GameHexParameters(HexType type);
};

// Класс игрового гекса

class GameHex : public GameHexParameters
{
public:
    int defence = 0;
    int fortification = 0;
    PlayerColor fortificationColor = "Neutral";

    bool canBeChosenAsStartPoint;

    QSet<PlayerColor> agitated;
    bool provides_unit = true;
    enum HexStatus {TO_BE_CONQUERED,   // нейтральна, есть юнит
                    NOT_CONNECTED,     // захвачена, нет соединения с мельницей
                    NOT_A_HOME,        // захвачена, можно рекрутировать
                    RECRUITING,        // проведено рекрутирование
                    HOME,              // является чьим-то домом
                    TOMBSTONE}         // юнит мёртв
                   status = TO_BE_CONQUERED;
    
    PlayerColor color = "Neutral";

    Coord coord;
    QMap <WAY, bool> rivers;

    QList <Resource> resources;

    explicit GameHex(HexType type, Coord Pos) : GameHexParameters(type)
    {
        coord = Pos;
        canBeChosenAsStartPoint = canGoHere;
    }
};

#endif
