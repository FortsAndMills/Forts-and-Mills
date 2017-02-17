#ifndef GAMEFIELD_H
#define GAMEFIELD_H

#include "GameTurns.h"

class GameField : public GameTurns
{
public:
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

    bool isHexAHome(Coord which, QString color);

    Coord canBeCaptured(Coord which, PlayerColor color);

    int resourcesLimit(PlayerColor color);

    QString isGameFinished();
};

#endif // GAMEFIELD_H
