#ifndef STATESCONTROL_H
#define STATESCONTROL_H

#include "Events.h"

class StatesControl : public Events
{
    Q_OBJECT

public:
    enum State {CHOOSING_HEX,
                        WAIT_FOR_ENEMY_START_POINT,
                         PLANNING,
                         CHOOSE_ORDER_PARAMETER,
                         ARE_YOU_SURE_DIALOG,
                         WAIT_FOR_ENEMY_PLAN,
                         REALIZATION_PHASE}
                   state = CHOOSING_HEX;
    State prev_state;

    explicit StatesControl(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    void getReadyToChooseHex(QList<Coord> variants);  // подготовить интерфейс к выбору стартовой позиции
    void getReadyToPlanning();  // подготовиться к планированию
    void getReadyToRealization();

    bool setNextTime();  // установка времени
    void setPreviousTime();
    void setTime(DayTime time);
    void setEarlyTime(DayTime time);
    void setLateTime(DayTime time);

    void getReadyToChooseOrderParameter();  // начать выбирать параметр приказа
    void breakChoosingOrderParameter();
    void finishedChoosingOrderParameter();

    void showPlannedOrder(GameUnit * unit);  // показать исполнение приказа как запланированный в данный момент
    void deshowPlannedOrder(GameUnit * unit, bool several = false);  // обратная операция
    void finishPlannedOrder(GameUnit * unit);  // считать исполнение приказа завершённым
    void definishPlannedOrder(GameUnit * unit);  // обратная операция

    void deplanOrder(GameUnit *unit, int time);  // убрать запланированный план
};

#endif // STATESCONTROL_H
