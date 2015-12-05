#ifndef GAMESTATE
#define GAMESTATE

#include "Technical/Headers.h"
#include "GameUnit.h"
#include "GameRules.h"
#include "GamePlayer.h"
#include "GameOrder.h"
#include "Technical/ProgrammSettings.h"

class Game : public QObject
{
    Q_OBJECT

public:
    Random * rand;
    GameRules * rules;

    QMap <PlayerColor, GamePlayer *> players;
    QList <QList <GameHex *> > field;

    // вспомогательные переменные
    int startPositionVariants = 0;


    // ИНИЦИАЛИЗАЦИЯ----------------------------------------------------------------------------------------------
   explicit Game(GameRules * rules, QList <int> random = QList <int> ());

    void generateField();

    QList <WAY> WAYS = QList <WAY>() << "UP" << "DOWN"
                                                                   << "RIGHT_UP" << "RIGHT_DOWN"
                                                                   << "LEFT_UP" << "LEFT_DOWN";


// ИГРА-----------------------------------------------------------------------------------------------------------------------
public:
    WAY oppositeWay(WAY way);
    WAY whereIs(int x, int y, int from_x, int from_y);
    QPair<int, int> adjacentHex(int x, int y, WAY way);
    QPair<int, int> adjacentHex(QPair<int, int> coord, WAY way)
    {
        return adjacentHex(coord.first, coord.second, way);
    }
    QMap<WAY, QPair<int, int> > adjacentHexesMap(int x, int y);
    QSet<QPair<int, int> > adjacentHexes(int x, int y);
    QSet<QPair<int, int> > visible_hexes(int my_x, int my_y, int radius = 2);

    // непосредственные фазы игры
public:
    void (Game::*nextPhase)() = &Game::startPointsChosen;
    void startPointsChosen();  // обрабатывает выбор мельниц игроков

    void getStartResources();  // получение стартовых ресурсов

    void planRealisation();  // пора реализовывать план!?

    void gatherResources();  // сбор ресурсов
    void burnExtraResources();  // сжечь лишние из уже имеющихся ресурсов


    // игровые события
    QList<OrderType> whatCanUse(GameUnit *, QMap<Resource, int> spend);  // Какие приказы может использовать

    void destroyUnit(GameUnit * unit);  // уничтожение объекта

    void captureHex(int i, int j, PlayerColor color);  // перекраски
    void decaptureHex(int i, int j);

    // ИГРОВЫЕ СОБЫТИЯ
signals:
    void unitSetToField(GameUnit * unit);  // юнит установлен на клетку

    void newResources(GameHex *, PlayerColor, QList <Resource>, QList <bool>);

    void blowUnit(GameUnit * unit);
    void hexColorChanged(GameHex * hex);

    void burnResource(PlayerColor, Resource);
};

#endif
