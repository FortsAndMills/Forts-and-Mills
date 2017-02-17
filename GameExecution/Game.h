#ifndef GAME_H
#define GAME_H

#include "GameSteps.h"

class Game : public GameSteps
{
public:
    explicit Game(GameRules * rules, Random * rand);

    virtual void StartGame();

    virtual void HexChosen();

    virtual void PlanRealisation();
};

#endif
