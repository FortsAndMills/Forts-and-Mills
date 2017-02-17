#ifndef EVENTS_H
#define EVENTS_H

#include "InterfaceOrganization.h"

class Events : public InterfaceOrganization
{
    Q_OBJECT

public:
    Unit * selectedUnit = NULL;  // выделенный юнит
    DayTime dayTime = -1;  // текущее время, -1 если не выбрано

    QMap <DayTime, int> priorities;  // приоритеты для создания приказов

    explicit Events(Game * game, qint8 PlayerIndex, GraphicObject * parent);

    void disableWholeField();
    void delightWholeField();

    virtual void unitConnections(Unit *) {}
    virtual void orderPicsConnections(OrderPic *) {}
    void newUnit(GameUnit * unit, Coord where);
    void blowUnit(GameUnit * unit);

    bool moveUnit(Unit * unit, Coord cord, Hex::POSITION_STATE position_type = Hex::STAY);
    void moveUnitBack(Unit * unit);
    void moveUnitForward(Unit * unit);
    void moveToWayUnit(Unit * unit, Coord to);

    void anchorOrderFromUnit(Unit *unit, Order *order);
    void anchorOrderToUnit(Unit *unit, Order *order);

    void fortify(Hex * hex, int amount, PlayerColor color = "Neutral", QPointF origin = QPointF(UNDEFINED, UNDEFINED));
    void deleteAllFortification(Hex * hex);

    void createWay(Unit * unit);
    void shoot(QString color, QPointF base, QPointF top, int value, Rocket::RocketType type = Rocket::ROCKET);

    void newResources(GameHex * hex, PlayerColor color, QList<Resource> resources, QList<bool> burn);
    void disappearAllTurnedOffOrders();  // все картинки использованных приказов исчезают
    void reconfigureResources();     // обновляет положение ресурсов на панелях игроков
};

#endif // EVENTS_H
