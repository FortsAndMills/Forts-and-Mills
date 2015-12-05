#ifndef GAMEUNIT
#define GAMEUNIT

#include "Technical/Headers.h"

class GameUnit : public QObject
{
    Q_OBJECT

public:
    PlayerColor color;

    int health;
    int max_health;

    int x, y;

    UnitType type;

    int home_x, home_y;

    // ПЛАН!!!
    QMap <DayTime, GameOrder *> plan;

    explicit GameUnit(Game * game, UnitType type, PlayerColor color) : QObject((QObject *)game)
    {
        this->color = color;
        x = UNDEFINED;
        y = UNDEFINED;
        home_x = UNDEFINED;
        home_y = UNDEFINED;

        this->setType(type);
        this->health = this->max_health;
    }
    void setType(UnitType type);
};

#endif
