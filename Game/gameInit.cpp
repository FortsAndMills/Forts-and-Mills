#include "Game.h"
#include "GameUnit.h"
#include "GameRules.h"
#include "GamePlayer.h"

Game::Game(GameRules *rules, QList<int> random) : QObject()
{
    this->rules = rules;
    this->rand = new Random(random);
    generateField();

    foreach (PlayerColor color, rules->players)
        players[color] = new GamePlayer(this, rules, color);

    foreach (GamePlayer * player, players)
    {
        player->getStartUnits(startPositionVariants / 2);
    }
}

void Game::generateField()
{
    field.clear();  // <undone>

    QList <GameHex *> all;
    for (int x = 0; x < rules->fieldH; ++x)
    {
        field << QList <GameHex *> ();
        for (int y = 0; y < rules->fieldW; ++y)
        {
            field[x] << new GameHex(this, "Simple", x, y);
            all << field[x][y];
        }
    }

    for (QMap <HexType, Range>::iterator it = rules->numOfHexes.begin();
                                                                     it != rules->numOfHexes.end(); ++it)
    {
        int amount = it->get_rand(rand);

        for (int k = 0; k < amount; ++k)
        {
            int i = rand->next() % all.size();
            all[i]->setType(it.key());
            all.removeAt(i);
        }
    }

    for (QMap <Resource, Range>::iterator it = rules->numOfResources.begin();
                                                                     it != rules->numOfResources.end(); ++it)
    {
        int amount = it->get_rand(rand);
        for (int k = 0; k < amount; ++k)
        {
            int i = rand->next() % all.size();
            all[i]->resources << it.key();
        }
    }

    this->startPositionVariants = 0;
    for (int x = 0; x < rules->fieldH; ++x)
    {
        for (int y = 0; y < rules->fieldW; ++y)
        {
            if (field[x][y]->canBeChosenAsStartPoint)
                ++this->startPositionVariants;
        }
    }

    if (startPositionVariants < 2)
        generateField();  // <undone>
}
