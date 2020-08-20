#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "GameTurns.h"

class GameField : public GameTurns
{
protected:
    DayTime time;

    class Recruited
    {
    public:
        Coord where;
        UnitType type;
        PlayerColor color;

        Recruited(Coord where, UnitType who, PlayerColor whos)
        {
            this->where = where;
            type = who;
            color = whos;
        }
    };
    QList <Recruited> recruitedUnits;  // юниты для генерации в конце хода

    explicit GameField(GameRules * rules, Random * rand);

    virtual void GenerateField();

public:
    bool isHexAHome(Coord which, QString color);
    Coord canBeCaptured(Coord which, PlayerColor color);
    int resourcesLimit(PlayerColor color);

    enum SEARCH_TYPE {ENEMY, ALLY};
    QSet<GameUnit *> find(SEARCH_TYPE ST, GameUnit * for_whom,
                    Coord staying = ANY, Coord going_to = ANY);

    QString isGameFinished();
};

#endif // GAMEFIELD_H
