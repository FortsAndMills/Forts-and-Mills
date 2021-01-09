#ifndef AI_H
#define AI_H

#include "Technical/Headers.h"
#include "GameExecution/Game.h"

typedef QMap<DayTime, GameOrder> Plan;

class AI
{
private:
    Game * game;

    // предподсчёты
    // суммарная мощь игрока, которая может добраться до клетки к моменту времени
    QMap<PlayerColor, QMap<Coord, QMap <DayTime, int> > > forces;
    void forces_precompute();
    int enemy_force(PlayerColor color, Coord c, DayTime dt)
    {
        int ef = 0;
        foreach (GamePlayer * player, game->players)
            if (player->color != color)
                ef = qMax(ef, forces[player->color][c][dt]);
        return ef;
    }

    QMap<Coord, QMap<PlayerColor, qreal> > hex_value;
    void hex_precompute();
    void hex_propagate();  // засовываем в ценность клеток ценность соседей чуть-чуть

    // информация о уже составленном плане
    QSet<Coord> already_captured;
    QSet<Coord> already_liberated;
    QSet<Coord> already_recruited;
    QMap<Coord, QMap <DayTime, int> > cur_plan_forces;

    QList<Plan> PossiblePlans(GameUnit * unit);
    qreal Eval(GameUnit * unit, const Plan & plan, const QSet<Coord> & help_required, QSet<Coord> & help_will_be_needed);

public:
    explicit AI(Game * game);

    void plan(PlayerColor color);
};




#endif // AI_H
