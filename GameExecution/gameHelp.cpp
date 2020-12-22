#include "Game.h"

GameUnit *Game::whoHasHomeAt(Coord c)
{
    foreach (GamePlayer * p, players)
        foreach (GameUnit * u, p->units)
             if (u->home == c)
                 return u;
    return NULL;
}
bool Game::isOccupied(GameHex *hex)
{
    foreach (GamePlayer * p, players)
        if (p->color != hex->color)
            foreach (GameUnit * u, p->units)
                 if (u->position == hex->coord)
                     return true;
    return false;
}
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

    qDebug() << "ERROR: unit not found by id" << Qt::endl;
    return NULL;
}

QString Game::lastPlayerInGame()
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
