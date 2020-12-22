#ifndef GAME_H
#define GAME_H

#include "GameMessages.h"

// класс игровой логики
class Game
{
public:
    explicit Game(GameRules * rules, Random * rand);
    virtual ~Game() {}

    // -------------------------------------------------------------------------------
    // функции внешнего вызова
    // реализации в Game.cpp
    virtual void StartGame();
    virtual void HexChosen();
    virtual void PlanRealisation();

protected:
    // -------------------------------------------------------------------------------
    // отдельные этапы игры
    // реализация в GameSteps.cpp
    void ProcessChosenHexes();

    void RealizePlan();

    void destroyHomelessUnits();
    void recruitNewUnits();
    void killAllies();
    void gatherResources();
    void burnExtraResources();
    void defenceFill();
    void agitationEnds();
    void addDayTime();

    // -------------------------------------------------------------------------------
    // реализация отдельных приказов
    // gameOrderExecution.cpp

    // оказалось полезным завести такую структуру:
    // кто от какого приказа какое действие исполняет,
    // чтобы например сортировать их
    class Action
    {
    public:
        GameUnit * unit;
        GameOrder * order;
        GameAction action;

        explicit Action(GameUnit * unit, DayTime time, int k)
        {
            this->unit = unit;
            order = unit->plan[time];
            action = order->actions[k];
        }
    };

    bool CheckIfActionBurns(Action a);
    void Realize(QList<Action> act);

    // -------------------------------------------------------------------------------
    // бой
    // GameFight.cpp
    QMap<GameUnit *, FightStatus> fs;

    Strike getStrike(QSet <GameUnit *> fighters);
    void CountStrike(QSet <GameUnit *> units, Strike strike);

    void UnitsFight(QSet<GameUnit *> fighters);

    // -------------------------------------------------------------------------------
    // микрособытия
    // реализация в GameEvents.cpp

    qint16 basicId = 0;
    QSet <GameUnit *> africa;  // складываем сюда всех погибших юнитов

    void NewDayTimeStarted(DayTime time);

    void CaptureHex(GameHex * hex, PlayerColor color);
    void DecaptureHex(GameHex * hex, GameUnit * who);

    void KillRecruited(GameHex * hex, GameUnit * who);
    void RecheckMillConnection(QString color, GameUnit * who);

    GameUnit * NewUnit(GamePlayer * player, UnitType type, Coord where);
    void DestroyUnit(GameUnit * unit);
    bool Damage(GameUnit * unit, int dam, QSet<GameUnit *> authors = QSet <GameUnit *>());

    void BurnOrder(GameUnit * unit, GameUnit *author = NULL);

    void GatherResources(GameHex * hex, PlayerColor color, QList <Resource> resources);
    void BurnExtraResources(PlayerColor color, Resource R);

    // -------------------------------------------------------------------------------
    // генерация поля
    // GameField.cpp
    DayTime time;

    class Recruited
    {
    public:
        Coord where;
        UnitType type;
        PlayerColor color;

        Recruited(Coord where, UnitType who, PlayerColor whos)
        {
            this->where = where;
            type = who;
            color = whos;
        }
    };
    QList <Recruited> recruitedUnits;  // юниты для генерации в конце хода
    virtual void GenerateField();

public:
    bool isHexAHome(Coord which, QString color);
    Coord canBeCaptured(Coord which, PlayerColor color);
    int resourcesLimit(PlayerColor color);

    enum SEARCH_TYPE {ENEMY, ALLY};
    QSet<GameUnit *> find(SEARCH_TYPE ST, GameUnit * for_whom,
                    Coord staying = ANY, Coord going_to = ANY);

    QString isGameFinished();

    // -------------------------------------------------------------------------------
    // взаимодействие с графическим модулем
    // GameTurns.cpp
public:
    // список игровых событий для передачи графике
    QList <GameMessage *> events;

    // выбранные игроками гексы
    QMap <PlayerColor, Coord> chosenHex;
    QMap <PlayerColor, UnitType> chosenUnitType;

    // готовность игроков
    QMap <PlayerColor, bool> ready;
    bool isEveryoneReady()
    {
        foreach (PlayerColor pc, rules->players)
            if (!ready[pc] && !players[pc]->GiveUp)
                return false;
        return true;
    }

protected:
    // удобная функция
    virtual GameMessage * AddEvent()
    {
        return (events << new GameMessage()).last();
    }

public:
    void playerGiveUp(int index);

    // может ли юнит использовать приказ и какие у него есть варианты
    bool CanUse(GameUnit * unit, OrderType order);
    QList<OrderType> whatCanUse(GameUnit *);
    bool must_be_last(GameUnit * unit, DayTime time);

    bool ChooseOneOfTheStartHexes();
    void StartPlanning();

    // -------------------------------------------------------------------------------
    // утилиты
    // GameHelp.cpp
protected:
    QSet <GameUnit *> alliesOnTheSameHex(GameUnit * tar);
    QSet <GameHex *> Connected(PlayerColor color, bool consider_occupied = false, const QSet<Coord> &additional_captures = QSet<Coord>());
    bool isOccupied(GameHex * hex);
    bool isAgitatedByEnemy(Coord which, PlayerColor me);

public:
    GameUnit * whoHasHomeAt(Coord c);
    GameUnit * getUnitById(qint16 id);

    // проверка на сдавшихся игроков
    QString lastPlayerInGame();

    // -------------------------------------------------------------------------------
    // утилиты
    // GameBase.cpp
public:
    Random * rand;
    GameRules * rules;

    QMap <PlayerColor, GamePlayer *> players;
    QList <QList <GameHex *> > hexes;
    GameHex * hex(Coord Pos)
    {
        return hexes[Pos.x][Pos.y];
    }

    // вспомогательные функции навигации по полю
    WAY oppositeWay(WAY way);
    Coord adjacentHex(Coord which, WAY way);
    QList<Coord> visible_hexes(Coord my, int radius = 2);

    QMap<WAY, Coord > adjacentHexesMap(Coord which);
    QList<Coord> adjacentHexes(Coord which);

    WAY whereIs(Coord which, Coord from);
    bool isAdjacent(Coord first, Coord second);
};

#endif
