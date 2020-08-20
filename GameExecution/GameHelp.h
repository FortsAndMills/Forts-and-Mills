#ifndef GAMEHELP_H
#define GAMEHELP_H

#include "GameBase.h"

class GameHelp : public GameBase
{
protected:
    explicit GameHelp(GameRules * rules, Random * rand);

    QSet <GameUnit *> alliesOnTheSameHex(GameUnit * tar);
    QSet <GameHex *> Connected(PlayerColor color, bool consider_occupied = false, const QSet<Coord> &additional_captures = QSet<Coord>());
    bool isOccupied(GameHex * hex);
    bool isAgitatedByEnemy(Coord which, PlayerColor me);

public:
    GameUnit * whoHasHomeAt(Coord c);
    GameUnit * getUnitById(qint16 id);

    // проверка на сдавшихся игроков
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
