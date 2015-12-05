#include "Game.h"

void Game::startPointsChosen()
{
    foreach (GamePlayer * player, players)  // отмечаем, что выбранные клетки были выбраны
    {
        // Проверка на случай, если два игрока прямо сейчас выбрали одну и ту же клетку
        if (!field[player->selected_x][player->selected_y]->wasChosenAsStartPoint)
        {
            --this->startPositionVariants;
            field[player->selected_x][player->selected_y]->wasChosenAsStartPoint = true;
        }
    }

    if (players[rules->players[0]]->selected_x != players[rules->players[1]]->selected_x ||
         players[rules->players[0]]->selected_y != players[rules->players[1]]->selected_y)
    {  // если два игрока выбрали разные, захватываем их.
        foreach (GamePlayer * player, players)
        {
            captureHex(player->selected_x, player->selected_y, player->color);

            GameUnit * toMove = player->firstFreeUnit();
            toMove->home_x = player->selected_x;
            toMove->home_y = player->selected_y;
            toMove->x = player->selected_x;
            toMove->y = player->selected_y;
            emit unitSetToField(toMove);
        }
    }

    if (this->startPositionVariants / 2 < players[rules->players[0]]->numOfFreeUnits())
    {
        foreach (GamePlayer * player, players)
        {
            destroyUnit(player->lastFreeUnit());
        }
    }

    if (this->startPositionVariants <= 1)  // Проверка на окончание выбора мельниц
    {
        nextPhase = &Game::getStartResources;
    }
    else
    {
        nextPhase = &Game::startPointsChosen;
    }
}

void Game::getStartResources()
{
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            if (unit->x != UNDEFINED)  // иначе даём за него стартовые ресурсы
            {
                player->addResources(rules->startResources[unit->type]);

                QList <bool> burn;  // никого не сжигаем
                for (int i = 0; i < rules->startResources[unit->type].size(); ++i)
                    burn << 0;

                emit newResources(field[unit->x][unit->y], unit->color,
                                  rules->startResources[unit->type], burn);
            }
        }
    }
    nextPhase = &Game::planRealisation;
}

void Game::planRealisation()
{

}

void Game::burnExtraResources()
{
    foreach (GamePlayer * player, players)
    {
        foreach (Resource R, rules->ordersInGame)
        {
            while (player->resources[R] > player->limit())
            {
                emit burnResource(player->color, R);
                --player->resources[R];
            }
        }
    }

    nextPhase = &Game::gatherResources;
}
void Game::gatherResources()
{
    foreach (QList <GameHex *> hex_row, field)
    {
        foreach (GameHex * hex, hex_row)
        {
            if (hex->color != "Neutral")
            {
                QList <Resource> res = hex->resources;
                QList <bool> burns;

                for (int i = 0; i < res.size(); ++i)
                {
                    if (players[hex->color]->resources[res[i]] < players[hex->color]->limit())
                    {
                        burns << false;
                        players[hex->color]->resources[res[i]]++;
                    }
                    else
                        burns << true;
                }

                emit newResources(hex, hex->color, res, burns);
            }
        }
    }

    nextPhase = &Game::planRealisation;
}
