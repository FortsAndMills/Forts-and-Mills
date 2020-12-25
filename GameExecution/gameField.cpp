#include "Game.h"

void Game::GenerateField()
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

    // генерим форты, мельницы, озёра и горы
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

            if (hexes[x][y]->type == "Mill" && rules->ordersInGame.contains("Recruit"))
            {
                hexes[x][y]->resources << "Recruit";
            }
            if (hexes[x][y]->type == "Fort")
            {
                hexes[x][y]->resources << "Capture";
            }
        }
    }

    // реки
    int num_rivers = rules->numOfRivers.get_rand(rand);
    int incorrect_attempts = 10;
    for (int i = 0; i < num_rivers; ++i)
    {
        // выбираем случайную стартовую гору
        GameHex * start = mountains[rand->next() % mountains.size()];

        // выбираем случайное направление от горы
        WAY way = WAY(rand->next() % 6);

        // hex1 - гекс выше ребра
        // hex2 - гекс ниже ребра
        // hex3 - гекс, в которое входит ребро
        GameHex * hex1, * hex2, * hex3;

        // длина реки
        int river_length = 0;

        do
        {
            // определеям стороны реки
            hex1 = hex(adjacentHex(start->coord, way));
            hex2 = hex(adjacentHex(start->coord, WAY((int(way) + 1) % 6)));

            // если по одну сторону гора, то эта река бессмысленна
            if (!hex1->canGoHere || !hex2->canGoHere)
                break;

            // если эта река уже существует, то выходим
            if (hex1->rivers[WAY((int(way) + 2) % 6)] || hex2->rivers[WAY((int(way) + 5) % 6)])
                break;

            // создаём реку
            river_length++;
            hex1->rivers[WAY((int(way) + 2) % 6)] = true;
            hex2->rivers[WAY((int(way) + 5) % 6)] = true;

            // определяем следующее направление
            hex3 = hex(adjacentHex(hex1->coord, WAY(int(way) + 1)));

            if (rand->next() % rules->river_interrupt_probability == 0)
                break;

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
        while (hex3->type != "Mountain" && hex3->type != "Lake");

        // нулевая речка не в счёт.
        if (river_length == 0 && incorrect_attempts > 0)
        {
            --i;
            --incorrect_attempts;
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
}
