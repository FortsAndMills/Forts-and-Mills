#ifndef GAMETURN_H
#define GAMETURN_H

#include "GameHelp.h"
#include "GameMessages.h"

// модуль для взаимодействия с графическим модулем
class GameTurns : public GameHelp
{
public:
    // список игровых событий для передачи графике
    QList <GameMessage *> events;

    // выбранные игроками гексы
    QMap <PlayerColor, Coord> chosenHex;

    // готовность игроков
    QMap <PlayerColor, bool> ready;
    bool isEveryoneReady()
    {
        foreach (PlayerColor pc, rules->players)
            if (!ready[pc] && !players[pc]->GiveUp)
                return false;
        return true;
    }

    explicit GameTurns(GameRules * rules, Random * rand);

    // удобная функция
    virtual GameMessage * AddEvent()
    {
        return (events << new GameMessage()).last();
    }

    void playerGiveUp(int index);

    // может ли юнит использовать приказ и какие у него есть варианты
    bool CanUse(GameUnit * unit, OrderType order);
    QList<OrderType> whatCanUse(GameUnit *);

    bool ChooseOneOfTheStartHexes();
    void StartPlanning();
};

#endif // GAMETURN_H
