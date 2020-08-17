#ifndef GAMEFIGHT_H
#define GAMEFIGHT_H

#include "GameEvents.h"

class GameFight : public GameEvents
{
protected:
    GameFight(GameRules *rules, Random *rand);

private:
    QMap<GameUnit *, FightStatus> fs;

    Strike getStrike(QSet <GameUnit *> fighters);
    void CountStrike(QSet <GameUnit *> units, Strike strike);

protected:
    void UnitsFight(QSet<GameUnit *> fighters);
};

#endif // GAMEFIGHT_H
