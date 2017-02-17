#include "Game.h"

Game::Game(GameRules *rules, Random *rand) : GameSteps(rules, rand)
{
    GenerateField();
}

void Game::StartGame()
{
    ChooseOneOfTheStartHexes();
}

void Game::HexChosen()
{
    if (lastPlayerInGame() == "Neutral")
    {
        ProcessChosenHexes();

        if (!ChooseOneOfTheStartHexes())
        {
            StartPlanning();
        }
    }
    else
        AddEvent()->Win(lastPlayerInGame());
}

void Game::PlanRealisation()
{
    if (lastPlayerInGame() == "Neutral")
    {
        RealizePlan();

        destroyHomelessUnits();
        recruitNewUnits();
        killAllies();
        burnExtraResources();
        gatherResources();
        defenceFill();

        if (isGameFinished() == "Neutral")
        {
            addDayTime();
            StartPlanning();
        }
        else
        {
            AddEvent()->Win(isGameFinished());
        }
    }
    else
        AddEvent()->Win(lastPlayerInGame());
}
