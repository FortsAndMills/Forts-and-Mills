#ifndef GAMESTEPS_H
#define GAMESTEPS_H

#include "GameOrderExecution.h"

class GameSteps : public GameOrderExecution
{
protected:
    explicit GameSteps(GameRules * rules, Random * rand);

    void ProcessChosenHexes();

    void RealizePlan();

    void destroyHomelessUnits();
    void recruitNewUnits();
    void killAllies();
    void gatherResources();
    void burnExtraResources();
    void defenceFill();
    void addDayTime();
};

#endif // GAMESTEPS_H
