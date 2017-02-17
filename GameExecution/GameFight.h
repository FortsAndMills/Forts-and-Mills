#ifndef GAMEFIGHT_H
#define GAMEFIGHT_H

#include "GameEvents.h"

class GameFight : public GameEvents
{
public:
    GameFight(GameRules *rules, Random *rand);

    FightStatus getFightStatus(GameUnit * unit);
    QList <FightBrid> getStrikeTypes(QList <GameUnit *> unit, QList <FightStatus> fs, int &val);

    void checkIfGetToFight(GameUnit * unit, FightBrid fb);
    int EstimateDamage(GameUnit * unit, FightBrid type, GameUnit * enemy);
    void CountStrike(QList <GameUnit *> units, QList <FightBrid> types, int value);

    void UnitsFight(QList<GameUnit *> fighters, QList<FightStatus> fs = QList <FightStatus>());
};

#endif // GAMEFIGHT_H
