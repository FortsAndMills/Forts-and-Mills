#ifndef GAMERULES
#define GAMERULES

#include "Technical/Headers.h"

// Класс настроек игры

class GameRules
{
public:
    // список всех потенциально возможных цветов игроков, приказов, юнитов
    // для проверок на корректность и переборов вариантов в меню
    static QList <QString> AllPlayers;
    static QList <QString> AllTimerTypes;
    static QList <QString> AllOrders;
    static QList <QString> AllUnits;

    // размер поля
    int fieldW;
    int fieldH;

    // игроки
    int numOfPlayers;
    QList <PlayerColor> players;
    void formPlayersList(Random * rand);

    // таймер
    TimerType timer;
    static QMap <TimerType, int> timer_per_plan;
    static QMap <TimerType, int> timer_per_choice;
    static QMap <TimerType, int> timer_per_choice_after_opponent;
    static QMap <TimerType, int> timer_per_plan_after_opponent;
    qint8 timerTypeIndex(TimerType type) const
    {
        qint8 i = 0;
        while (AllTimerTypes[i] != type) { ++i; }
        return i;
    }

    // включёные в игру приказы
    QList <OrderType> ordersInGame;
    // навигация по списку приказов
    OrderType getOrderByIndex(qint8 i)
    {
        if (i == -1)
            return DefaultOrder;
        else
            return ordersInGame[i];
    }
    qint8 orderIndex(OrderType order)
    {
        if (order == DefaultOrder)
            return -1;

        qint8 i = 0;
        while (ordersInGame[i] != order) { ++i; }
        return i;
    }

    QList <UnitType> unitsInGame;
    qint8 unitTypeIndex(UnitType type)
    {
        qint8 i = 0;
        while (unitsInGame[i] != type) { ++i; }
        return i;
    }

    // количество времён дня и увеличиваются ли они каждый раунд
    int dayTimes;
    bool changingDayTimes;

    // диапазон для генерации каждого из типов клеток и приказов
    QMap <HexType, Range> numOfHexTypes;
    QMap <OrderType, Range> numOfResourcesOnField;
    Range numOfRivers;

    // шансы реки прерваться при генерации
    int river_interrupt_probability;

    // эти флаги выставляются в зависимости от настроек игры
    bool doesCaptureRecruits;
    bool doesEnteringEnemyHexLiberates;

    // эти флаги нужны для режима обучения
    int limitatingResources = true;

    // тестовые настройки
    int damageToHomellesUnits;  // тестовая настройка: бездомные не умирают, а получают дамаг
    int peacefullOrdersBurns;   // тестовая настройка: мирные приказы сжигаются
    int start_choices;          // сколько раз выбираем стартовые клетки
    int pig_can_shoot_attacker; // тестовые настройки хрюшки
    int fire_has_penalty;
    int fort_gp_min, fort_gp_max;  // диапазоны генерации фортов, мельниц, рек
    int mill_gp_min, mill_gp_max;
    int river_gp_min, river_gp_max;
    double K_change;               // коэф. увеличения доли ресурсов на карте

    explicit GameRules();
    explicit GameRules(QList <qint32> hash);
    void setTestOptions();
    void recountGenerationParameters();
    friend QTextStream &operator << (QTextStream &stream, const GameRules *r);
    friend QTextStream &operator >>(QTextStream &stream, GameRules *r);
    friend QDataStream &operator << (QDataStream &stream, const GameRules *r);
    friend QDataStream &operator >>(QDataStream &stream, GameRules *r);

    // для передачи правил по сети приходится консервировать их в списки qint32.
    QList <qint32> get();
};

#endif
