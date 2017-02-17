#ifndef GAMEHELP_H
#define GAMEHELP_H

#include "GameBase.h"

class GameHelp : public GameBase
{
public:
    explicit GameHelp(GameRules * rules, Random * rand);

    GameUnit * whoHasHomeAt(Coord c);

    QSet <GameUnit *> alliesOnTheSameHex(GameUnit * tar);

    void deleteAllies(QList<GameUnit *> &e);

    QString lastPlayerInGame()
    {
        QString ans = "Neutral";
        bool flag = true;
        foreach (GamePlayer * player, players)
        {
            if (!player->GiveUp)
            {
                if (flag)
                {
                    flag = false;
                    ans = player->color;
                }
                else
                    ans = "Neutral";
            }
        }
        return ans;
    }
};

#endif // GAMEHELP_H
