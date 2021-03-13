#ifndef GAMEUNIT
#define GAMEUNIT

#include "Technical/Headers.h"

// Параметры юнита, зависящие только от типа юнита

class GameUnitParameters
{
public:
    UnitType type;

    int max_health = 5;
    int init_health = 5;
    int max_defenseBonus = 0;
    bool doubles = false;

    GameUnitParameters(GameRules *, UnitType type);
};


// Игровой класс юнита
class GameUnit : public GameUnitParameters
{
public:
    PlayerColor color;
    int health;
    Coord position;
    UnitType type;
    Coord home;
    int defenseBonus;

    // план
    QMap <DayTime, GameOrder *> plan;

    // вспомогательные переменные
    qint16 id;
    Coord going_to;
    int distantAttack;
    QSet <GameUnit *> death_authors;  // "виновники в смерти" для корректной анимации

    explicit GameUnit(GameRules * rules, UnitType type, PlayerColor color, Coord where, qint16 id) :
        GameUnitParameters(rules, type)
    {
        this->color = color;
        this->health = init_health;
        this->position = where;
        this->type = type;
        this->home = where;
        this->defenseBonus = max_defenseBonus;
        this->distantAttack = 0;

        this->id = id;
    }
};

#endif
