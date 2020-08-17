#ifndef STATESCONTROL_H
#define STATESCONTROL_H

#include "Events.h"

// Контроль текущего состояния интерфейса и переключения между ними
// То есть что включено, что интерактивно, какие действия валидны в текущий момент, и т.д.

class StatesControl : public Events
{
    Q_OBJECT

public:
    enum State {CHOOSING_HEX,                                // выбор гекса
                        WAIT_FOR_ENEMY_START_POINT,  // ожидание выбора гекса оппонентов
                         PLANNING,                                          // планирование
                         CHOOSE_ORDER_PARAMETER,       // выбор параметров приказа
                         ARE_YOU_SURE_DIALOG,                // диалог "вы уверены?"
                         WAIT_FOR_ENEMY_PLAN,               // ожидание планов оппонентов
                         REALIZATION_PHASE}                     // реализация планов
                   state = CHOOSING_HEX;
    State prev_state;

protected:
    bool isSelectingEnemyUnit = false;

public:
    explicit StatesControl(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    void getReadyToChooseHex(QList<Coord> variants);  // подготовить интерфейс к выбору стартовой позиции
    void getReadyToPlanning();  // подготовиться к планированию
    void getReadyToRealization();  // подготовиться к реализации

    bool setNextTime();  // установка времени: следующего, предыдущего, конкретного...
    void setPreviousTime();
    void setTime(DayTime time);
    void setEarlyTime(DayTime time);
    void setLateTime(DayTime time);

    void getReadyToChooseOrderParameter();  // начать выбирать параметр приказа
    void breakChoosingOrderParameter();  // прервать выбор параметра
    void finishedChoosingOrderParameter();  // завершить выбор параметра

    void showPlannedOrder(GameUnit * unit);  // показать исполнение приказа как запланированный в данный момент
    void deshowPlannedOrder(GameUnit * unit, bool several = false);  // обратная операция
    void finishPlannedOrder(GameUnit * unit);  // считать исполнение приказа завершённым
    void definishPlannedOrder(GameUnit * unit);  // обратная операция

    void deplanOrder(GameUnit *unit, int time);  // убрать запланированный план
};

#endif // STATESCONTROL_H
