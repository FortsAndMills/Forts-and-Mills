#ifndef PLAYER
#define PLAYER

#include "Technical/Headers.h"

// Игровой класс игрока

class GamePlayer
{
public:
    PlayerColor color;

    QMap <Resource, int> resources;
    QList <GameUnit *> units;

    bool GiveUp = false;

    explicit GamePlayer(PlayerColor color)
    {
        this->color = color;
    }
};

#endif
