#ifndef PLAYER
#define PLAYER

#include "Technical/Headers.h"

class GamePlayer : public QObject
{
    Q_OBJECT
private:
    GameRules * rules;
    Game * game;

public:
    PlayerColor color;

    QMap <Resource, int> resources;
    QList <GameUnit *> units;

    int selected_x = UNDEFINED;  // выбранная игроком клетка, т.е. его по сути ход
    int selected_y = UNDEFINED;

    explicit GamePlayer(Game *game, GameRules * rules, PlayerColor color);

    int numOfMills = 0;  // вспомогательные переменные
    int limit();  // возвращает лимит ресурсов

    GameUnit * firstFreeUnit();  // возвращает первого юнита с UNDEFINED-координатами
    GameUnit * lastFreeUnit();  // аналогично последнего
    int numOfFreeUnits();  // аналогично количество таковых

    void addResources(QList <Resource> newResources)  // добавление ресурсов без проверки на предел
    {
        foreach (Resource R, newResources)
            ++resources[R];
    }

    void getStartUnits(int numOfStartResources);  // получение стартовых юнитов
    GameUnit * newUnit(UnitType type);                // создание нового юнита (ссылка в сигнале)
    void deleteUnit(GameUnit * unit);                      // уничтожение юнита
};

#endif
