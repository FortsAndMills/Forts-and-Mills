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
    QList <GameHex *> mountains;
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

            if (hexes[x][y]->type == "Mountain")
            {
                mountains << hexes[x][y];
            }

            if (hexes[x][y]->type == "Mill" && rules->recruitsInMills &&
                     rules->ordersInGame.contains("Recruit"))
            {
                hexes[x][y]->resources << "Recruit";
            }

            if (hexes[x][y]->type == "Fort" && rules->capturesInForts &&
                     rules->ordersInGame.contains("Capture"))
            {
                hexes[x][y]->resources << "Capture";
            }

            if (hexes[x][y]->canGoHere && rules->everything_is_starting)
            {
                hexes[x][y]->canBeChosenAsStartPoint = true;
            }
        }
    }

    // реки
    int num_rivers = 1 + rand->next() % 4;
    for (int i = 0; i < num_rivers; ++i)
    {
        GameHex * start = mountains[rand->next() % mountains.size()];
        WAY way = WAY(rand->next() % 6);

        GameHex * hex1, * hex2, * hex3;

        do
        {
            hex1 = hex(adjacentHex(start->coord, way));
            hex2 = hex(adjacentHex(start->coord, WAY((int(way) + 1) % 6)));

            if (!hex1->canGoHere || !hex2->canGoHere)
                break;

            if (hex1->rivers[WAY((int(way) + 2) % 6)] || hex2->rivers[WAY((int(way) + 5) % 6)])
                break;
            hex1->rivers[WAY((int(way) + 2) % 6)] = true;
            hex2->rivers[WAY((int(way) + 5) % 6)] = true;

            hex3 = hex(adjacentHex(hex1->coord, WAY(int(way) + 1)));

            if (rand->next() % 2)
            {
                start = hex1;
                way = WAY((int(way) + 1) % 6);
            }
            else
            {
                start = hex2;
                way = WAY((int(way) + 5) % 6);
            }
        }
        while (hex3->type != "Mountain");
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
//    if (rules->livingNations)
//    {
//        for (int x = 0; x < rules->fieldH; ++x)
//        {
//            for (int y = 0; y < rules->fieldW; ++y)
//            {
//                if (hexes[x][y]->canBeChosenAsStartPoint)
//                {
//                    hexes[x][y]->livingNation = rules->unitsInGame[rand->next() % rules->unitsInGame.size()];
//                }
//            }
//        }
//    }

    // регионы
//    if (rules->regions_start)
//    {
//        QList< QList<GameHex*> > regions;
//        QList< Coord > centers;
//        QList<int> resources_count;
//        for (int x = 0; x < rules->fieldH; ++x)
//        {
//            for (int y = 0; y < rules->fieldW; ++y)
//            {
//                if (hexes[x][y]->canBeChosenAsStartPoint)
//                {
//                    regions << (QList<GameHex*>() << hexes[x][y]);
//                    centers << hexes[x][y]->coord;
//                    hexes[x][y]->region_center = hexes[x][y]->coord;
//                    resources_count << hexes[x][y]->resources.size();
//                }
//            }
//        }

//        while (!regions.empty())
//        {
//            int r;
//            if (rules->equalize_regions)
//            {
//                int sum = 0;
//                foreach (int res_cnt, resources_count)
//                    sum += res_cnt;

//                QVector<int> probabilities;
//                int prob_sum = 0;
//                foreach (int res_cnt, resources_count)
//                {
//                    prob_sum += sum - res_cnt;
//                    probabilities << prob_sum;
//                }

//                if (prob_sum != 0)
//                {
//                    int a = rand->next() % prob_sum;
//                    r = 0;
//                    while (a >= probabilities[r])
//                        r++;
//                }
//                else
//                    r = rand->next() % regions.size();
//            }
//            else
//                r = rand->next() % regions.size();

//            QVector <Coord> candidates;
//            foreach (GameHex* RegionHex, regions[r])
//            {
//                foreach (Coord adj_coord, adjacentHexes(RegionHex->coord))
//                {
//                    if (hex(adj_coord)->region_center == NOWHERE &&
//                         hex(adj_coord)->canBeCaptured)
//                        candidates << adj_coord;
//                }
//            }

//            if (candidates.size() > 0)
//            {
//                Coord chosen = candidates[rand->next() % candidates.size()];
//                hex(chosen)->region_center = centers[r];
//                regions[r] << hex(chosen);
//            }
//            else
//            {
//                regions.removeAt(r);
//                centers.removeAt(r);
//                resources_count.removeAt(r);
//            }
//        }
//    }
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
            (rules->fortsBurnCaptures || hex(p)->type != "Fort") &&
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
            if (hexes[i][j]->increasesResourceLimitWhenCaptured &&
                hexes[i][j]->color == color && !isOccupied(hexes[i][j]))
                ++count;
        }
    }

    return count;
}

QSet <GameUnit *> GameField::find(SEARCH_TYPE ST, GameUnit * for_whom,
                          Coord staying, Coord going_to)
{
    QSet <GameUnit *> ans;
    for (int i = 0; i < rules->players.size(); ++i)
    {
        if ((ST == ENEMY && rules->players[i] != for_whom->color) ||
            (ST == ALLY && rules->players[i] == for_whom->color))
        {
            foreach (GameUnit * unit, players[rules->players[i]]->units)
            {
                if (unit->health > 0 && unit != for_whom &&
                     (staying == ANY || unit->position == staying) &&
                     (going_to == ANY || unit->going_to == going_to))
                {
                    ans << unit;
                }
            }
        }
    }

    return ans;
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
