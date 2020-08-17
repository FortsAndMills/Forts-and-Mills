#ifndef INTERACTION_H
#define INTERACTION_H

#include "StatesControl.h"

// Обработка сигналов пользователя
class Interaction : public StatesControl
{    
    Q_OBJECT

public:
    void unitConnections(Unit * unit);
    void orderPicsConnections(OrderPic * pic);

    explicit Interaction(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    virtual void NextPhase() {}

private:
    bool planned_to_go_home = false;

public slots:
    virtual void hexClicked(Coord c);
    void hexLeft(Coord c);
    virtual void hexEntered(Coord c);

    void orderPicEntered(OrderType type, PlayerColor color);
    void orderPicLeft(OrderType type, PlayerColor color);

    virtual void orderVariantClicked(OrderType type);  // выбран приказ с панели
    virtual void unitTypeChosen(UnitType type);

    virtual void unitHoverEntered(GameUnit * unit);
    virtual void unitHoverLeft(GameUnit * unit);
    virtual void unitLeftClicked(GameUnit *unit);

    void dayTimeClicked(DayTime time);
    void startUnitTypeClicked(bool on, QString type);

    void readFromOpponent(QByteArray in); // пришла посылка от противника
    void sendPlan();
    virtual void GoButtonPushed();
    void DialogReturned(bool, QString sig_mes);
    void NextButtonClicked();

    void serverDisconnected();
    void opponentDisconnected(qint8 index);
    void opponentReconnected(qint8 index);
    void giveup();

    void whiteFlagClicked();
    virtual void homeButtonClicked();

signals:
    void writeToOpponent(QByteArray);
    void GoHome();
};

#endif // INTERACTION_H
