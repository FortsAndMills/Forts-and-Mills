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

    // TODO log here
}

void Game::StartGame()
{
    ChooseOneOfTheStartHexes();
}

int Game::NextStage()
{
    if (lastPlayerInGame() == "Neutral")
    {
        if (state == GS_CHOOSE_HEX)
        {
            ProcessChosenHexes();

            if (!ChooseOneOfTheStartHexes())
            {
                state = GS_PLAN;
                StartPlanning();

                // запускаем таймер
                if (rules->timer_per_round)
                {
                    return rules->timer_per_plan;
                }
            }
            else
            {
                // запускаем таймер
                if (rules->timer_per_round)
                {
                    return rules->timer_per_choice;
                }
            }
        }
        else
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

                // запускаем таймер
                if (rules->timer_per_round)
                {
                    return rules->timer_per_plan;
                }
            }
            else
            {
                AddEvent()->Win(isGameFinished());
            }
        }

    }
    else
        AddEvent()->Win(lastPlayerInGame());

    return 0;
}
