#ifndef GAMEEVENTS_H
#define GAMEEVENTS_H

#include "GameField.h"

class GameEvents : public GameField
{
protected:
    qint16 basicId = 0;

    QSet <GameUnit *> africa;  // складываем сюда всех погибших юнитов

    explicit GameEvents(GameRules *rules, Random *rand);

    void NewDayTimeStarted(DayTime time);

    void CaptureHex(GameHex * hex, PlayerColor color);
    void KillRecruited(GameHex * hex, GameUnit * who);
    void DecaptureHex(GameHex * hex, GameUnit * who);

    GameUnit * NewUnit(GamePlayer * player, UnitType type, Coord where);
    void DestroyUnit(GameUnit * unit);
    bool Damage(GameUnit * unit, int dam, QSet<GameUnit *> authors = QSet <GameUnit *>());

    void BurnOrder(GameUnit * unit, GameUnit *author = NULL);

    void GatherResources(GameHex * hex, PlayerColor color, QList <Resource> resources);
    void BurnExtraResources(PlayerColor color, Resource R);
};

#endif // GAMEEVENTS_H
