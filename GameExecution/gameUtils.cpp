#include "Game.h"

// проверяет, оккупирована ли клетка врагом
bool Game::isOccupied(GameHex *hex)
{
    foreach (GamePlayer * p, players)
        if (p->color != hex->color)
            foreach (GameUnit * u, p->units)
                 if (u->position == hex->coord)
                     return true;
    return false;
}

// поиск прочих юнитов по критериям
QSet <GameUnit *> Game::alliesOnTheSameHex(GameUnit * tar)
{
    QSet <GameUnit *> ans;
    foreach (GameUnit * unit, players[tar->color]->units)
    {
        if (unit != tar && unit->position == tar->position)
        {
            ans << unit;
        }
    }
    return ans;
}
QSet <GameUnit *> Game::find(SEARCH_TYPE ST, GameUnit * for_whom,
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

// подсчёт лимита ресурсов игрока
int Game::resourcesLimit(PlayerColor color, bool consider_occupied)
{
    int count = 0;
    for (int i = 0; i < rules->fieldH; ++i)
    {
        for (int j = 0; j < rules->fieldW; ++j)
        {
            if (hexes[i][j]->increasesResourceLimitWhenCaptured &&
                hexes[i][j]->color == color &&
                (!consider_occupied || !isOccupied(hexes[i][j])))
                ++count;
        }
    }

    return count;
}

// найти все соединённые с мельницей территории
QSet<GameHex *> Game::Connected(QString color, bool consider_occupied, const QSet<Coord> & additional_captures)
{
    QSet<GameHex *> visited;
    QVector<GameHex*> to_visit;

    foreach (QList <GameHex *> hex_row, hexes)
    {
        foreach (GameHex * hex, hex_row)
        {
            if ((hex->color == color || additional_captures.contains(hex->coord)) &&
                hex->type == "Mill" &&
                (!consider_occupied || !isOccupied(hex)))
            {
                to_visit << hex;
            }
        }
    }

    for (int i = 0; i < to_visit.size(); ++i)
    {
        visited << to_visit[i];

        QList<Coord> adj = adjacentHexes(to_visit[i]->coord);
        foreach (Coord c, adj)
        {
            if ((hex(c)->color == color || additional_captures.contains(c)) &&
                !visited.contains(hex(c)) &&
                (!consider_occupied || !isOccupied(hex(c))))
            {
                to_visit << hex(c);
            }
        }
    }

    return visited;
}

// проверить, была ли клетка агитирована врагом
bool Game::isAgitatedByEnemy(Coord which, QString me)
{
    foreach (PlayerColor color, hex(which)->agitated)
        if (color != me)
            return true;
    return false;
}

GameUnit *Game::getUnitById(qint16 id)
{
    foreach (GamePlayer * player, players)
    {
        foreach (GameUnit * unit, player->units)
        {
            if (unit->id == id)
                return unit;
        }
    }

    debug << "ERROR: unit not found by id" << Qt::endl;
    return NULL;
}

// проверка на то, что сдались или проиграли все игроки кроме одного
QString Game::lastPlayerInGame()
{
    QString ans = "Neutral";
    bool flag = true;
    foreach (GamePlayer * player, players)
    {
        if (player->status == GamePlayer::ALIVE)
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

    // TODO все мертвы: это ничья.
    if (flag)
        return "Draw";
    return ans;
}


