#include "Game.h"

Game::Game(GameRules *rules, Random *rand)
{
    rules->formPlayersList(rand);
    this->rules = rules;
    this->rand = rand;

    for (int i = 0; i < rules->players.size(); ++i)
        players[rules->players[i]] = new GamePlayer(rules->players[i]);

    foreach (PlayerColor color, rules->players)
        chosenUnitType[color] = rules->unitsInGame[0];

    GenerateField();
}
Game::Game(const Game *other) : Game(*other)
{
    this->africa.clear();
    this->events.clear();

    // копируем игроков и их юнитов
    this->players.clear();
    foreach (GamePlayer * player, other->players)
        this->players[player->color] = new GamePlayer(*player);

    // копируем поле
    this->hexes.clear();
    foreach (QList<GameHex *> hex_row, other->hexes)
    {
        this->hexes << QList<GameHex*>();
        foreach (GameHex * hex, hex_row)
            this->hexes.last() << new GameHex(*hex);
    }
}
Game::~Game()
{
    foreach (GameUnit * unit, africa)
        delete unit;
    foreach (GameMessage * message, events)
        delete message;
    foreach (GamePlayer * player, players)
        delete player;
    foreach (QList<GameHex *> hex_row, hexes)
        foreach (GameHex * hex, hex_row)
            delete hex;
}

void Game::StartGame()
{
    ChooseOneOfTheStartHexes();
}

int Game::NextStage()
{
    ++round;

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
                return rules->timer_per_plan[rules->timer];
            }
            else
            {
                // запускаем таймер
                return rules->timer_per_choice[rules->timer];
            }
        }
        else
        {
            RealizePlan();

            destroyHomelessUnits();
            recruitNewUnits();
            burnExtraResources();
            gatherResources();
            defenseFill();
            agitationEnds();

            PlayerColor winner = isGameFinished();
            if (winner == "Neutral")
            {
                addDayTime();
                StartPlanning();

                // запускаем таймер
                return rules->timer_per_plan[rules->timer];
            }
            else
            {
                win(winner);
            }
        }

    }
    else
        win(lastPlayerInGame());

    return 0;
}
