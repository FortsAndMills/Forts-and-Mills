#ifndef GAMEUNITSMOVING_H
#define GAMEUNITSMOVING_H

#include "GameField.h"

// вспомогательные функции для поиска юнитов в тех же клетках
// или идущих в конкретном направлении...
// союзников и врагов...
class GameUnitsMoving : public GameField
{
public:
    DayTime time;

    explicit GameUnitsMoving(GameRules *rules, Random *rand);

    enum SEARCH_TYPE {ENEMY, ALLY};
    enum PRIORITY_ENDIAN {LOW, HIGH};
    QList<GameUnit *> find(SEARCH_TYPE ST, GameUnit * for_whom,
                    Coord staying = ANY, Coord going_to = ANY, PRIORITY_ENDIAN PE = LOW);
    QSet <GameUnit *> findAll(SEARCH_TYPE ST, GameUnit * for_whom,
                    Coord staying = ANY, Coord going_to = ANY);
};

#endif // GAMEUNITSMOVING_H
