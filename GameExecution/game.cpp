#include "Game.h"

Game::Game(GameRules *rules, Random *rand)
{
    this->rules = rules;
    this->rand = rand;
    rules->FormPlayersList(rand);

    for (int i = 0; i < rules->players.size(); ++i)
        players[rules->players[i]] = new GamePlayer(rules->players[i]);

    foreach (PlayerColor color, rules->players)
        chosenUnitType[color] = rules->unitsInGame[0];

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
            //if (rules->waves_start && !rules->surround_start)
            //    WavesStart();
            //if ((rules->surround_start || rules->regions_start) && !rules->waves_start)
            //    gatherResources();

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
        agitationEnds();

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
