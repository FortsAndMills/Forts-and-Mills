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
    int defence = 0;  // бонус в обороне
    int fortification = 0;  // щиты и их цвет
    PlayerColor fortificationColor = "Neutral";

    bool canBeChosenAsStartPoint; // ещё может быть выбрана в качестве старта

    QSet<PlayerColor> agitated;  // игроки, которые агитировали эту клетку

    bool provides_unit = true;   // ещё может предоставить юнита
    enum HexStatus {TO_BE_CONQUERED,   // нейтральна, есть юнит
                    NOT_CONNECTED,     // захвачена, нет соединения с мельницей
                    NOT_A_HOME,        // захвачена, можно рекрутировать
                    RECRUITING,        // проведено рекрутирование
                    HOME,              // является чьим-то домом
                    TOMBSTONE}         // юнит мёртв
                   status = TO_BE_CONQUERED;
    
    PlayerColor color = "Neutral";  // владелец
    Coord coord;                    // координаты
    QMap <WAY, bool> rivers;        // с каких сторон есть речки
    QList <Resource> resources;     // какие ресурсы предоставляет

    explicit GameHex(HexType type, Coord Pos) : GameHexParameters(type)
    {
        coord = Pos;
        canBeChosenAsStartPoint = canGoHere;
    }
};

#endif
