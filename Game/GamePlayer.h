#ifndef PLAYER
#define PLAYER

#include "Technical/Headers.h"
#include "GameUnit.h"

// Игровой класс игрока

class GamePlayer
{
public:
    PlayerColor color;

    QMap <Resource, int> resources;
    QList <GameUnit *> units;

    enum PlayerStatus {ALIVE, GAVE_UP, NO_UNITS, NO_FORTS} status = ALIVE;
    enum PlayerType {HUMAN, AI} playerType = HUMAN;

    explicit GamePlayer(PlayerColor color)
    {
        this->color = color;
    }
    GamePlayer(GamePlayer * other)
    {
        this->color = other->color;
        this->resources = other->resources;
        this->status = other->status;

        foreach (GameUnit * unit, other->units)
            this->units << new GameUnit(*unit);
    }
    ~GamePlayer()
    {
        foreach (GameUnit * unit, units)
            delete unit;
    }
};

#endif
