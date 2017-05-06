#include "GameField.h"

GameField::GameField(GameRules *rules, Random *rand) :
    GameTurns(rules, rand)
{

}

void GameField::GenerateField()
{
    // Сначала всё наше поле - "пустое".
    QList <HexType *> vars;
    HexType types[rules->fieldH][rules->fieldW];
    for (int x = 0; x < rules->fieldH; ++x)
    {
        for (int y = 0; y < rules->fieldW; ++y)
        {
            types[x][y] = "Simple";
            vars << &types[x][y];
        }
    }

    // генерим форты, мельницы и горы
    for (QMap <HexType, Range>::iterator it = rules->numOfHexTypes.begin();
                                                                     it != rules->numOfHexTypes.end(); ++it)
    {
        int amount = it->get_rand(rand);

        if (it.key() == "Mill" && amount < 2)
            amount = 2;
        else if (it.key() == "Fort" && amount < 1)
            amount = 1;

        for (int k = 0; k < amount; ++k)
        {
            int i = rand->next() % vars.size();
            *vars[i] = it.key();
            vars.removeAt(i);
        }
    }

    // создаём, собственно, гексы и список тех, которые участвуют в розыгрыше ресурсов
    QList <GameHex *> all;
    for (int x = 0; x < rules->fieldH; ++x)
    {
        hexes << QList <GameHex *> ();
        for (int y = 0; y < rules->fieldW; ++y)
        {
            hexes[x] << new GameHex(types[x][y], Coord(x, y));
            if (hexes[x][y]->canHaveResources)
            {
                all << hexes[x][y];
            }
        }
    }

    // расставляем ресурсы
    for (int i = 0; i < rules->ordersInGame.size(); ++i)
    {
        int amount = rules->numOfResourcesOnField[rules->ordersInGame[i]].get_rand(rand);
        for (int k = 0; k < amount; ++k)
        {
            int j = rand->next() % all.size();
            all[j]->resources << rules->ordersInGame[i];
        }
    }

    // "живущии нации" - для старта игры
    for (int x = 0; x < rules->fieldH; ++x)
    {
        for (int y = 0; y < rules->fieldW; ++y)
        {
            if (hexes[x][y]->canBeChosenAsStartPoint)
            {
                hexes[x][y]->livingNation = rules->unitsInGame[rand->next() % rules->unitsInGame.size()];
            }
        }
    }
}

bool GameField::isHexAHome(Coord which, PlayerColor color)
{
    foreach (GameUnit * unit, players[color]->units)
        if (unit->home == which)
            return true;

    foreach (Recruited r, recruitedUnits)
        if (r.where == which && r.color == color)
            return true;

    return false;
}
Coord GameField::canBeCaptured(Coord which, PlayerColor color)
{
    foreach (Coord p, adjacentHexes(which))
    {
        if (hex(p)->forbidsToCaptureAdjacentHexesWhenCaptured &&
            hex(p)->color != "Neutral" &&
            hex(p)->color != color)
        {
            return p;
        }
    }
    return NOWHERE;
}
int GameField::resourcesLimit(PlayerColor color)
{
    int count = 0;
    for (int i = 0; i < rules->fieldH; ++i)
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            if (hexes[i][j]->highingResourceLimitWhenCaptured &&
                hexes[i][j]->color == color)
                ++count;
        }
    }

    return count;
}

// проверка на окончание игры!
PlayerColor GameField::isGameFinished()
{
    PlayerColor winner = "Neutral";
    for (int i = 0; i < rules->fieldH; ++i)
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            if (hexes[i][j]->mustBeCapturedToWin)
            {
                if (hexes[i][j]->color == "Neutral")
                    return "Neutral";
                if (winner != "Neutral" && hexes[i][j]->color != winner)
                    return "Neutral";

                winner = hexes[i][j]->color;
            }
        }
    }

    return winner;
}
