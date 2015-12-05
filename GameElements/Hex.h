#ifndef HEX
#define HEX

#include "Technical/Headers.h"
#include "Game/GameHex.h"
#include "BasicElements/StateObject.h"
#include "Order.h"
#include "UnitWay.h"

class Hex : public StateObject
{
    Q_OBJECT

private:
    GameHex * prototype;
    Game * game;

    GraphicObject * HexPicture;
    QList <ResourcePic *> table;

    SpriteObject * Lighting;
    Object * UnitHomePicture;

    QSet <int> ids;
    QMap <int, QPointF> points;  // точки для юнитов и их атрибутов на гексе
    enum POSITION_STATE {STAY, ENTERING};
    QMap <int, POSITION_STATE> pointPositionState;
    QMap <int, WAY> pointsWay;

    friend GameWindow;
    friend Unit;  // <undone>

    QMap <int, QList <Order *> > orders;  // оставленные юнитом приказы
    QMap <Order *, QPointF> orders_pos;  // положение оставленных приказов

public:
    explicit Hex(GraphicObject * parent, Game * game, GameHex * prototype);
    explicit Hex(Hex * another);
    virtual void Delete();

public:
    void resizeChildren(qreal W, qreal H);
private:
    QList<QPointF> countTableCoordinates(qreal W, qreal H, int n);
    QMap <WAY, QPointF> EnterPoint;
    void recountPoints();
    void recountOrdersPosition();

    void resizeOrders();

public:
    void reconfigureOrders();

private:
    void startedAnimation()
    {
        this->setEnabled(false);
    }
    void finishedAnimation()
    {
        this->setEnabled(true);
    }

public:
    int createPoint(POSITION_STATE state, WAY way);
    void removePoint(int id);

    void setNewResourcesPosition(QList<OrderPic *> resources);  // <undone> в GameWindow

    void setState(QString state, bool isImmediate = false);  // TODO стэйты! привести в порядок
    void select(bool enable = true);
    void light(bool enable = true);
    void forbidToSelect(bool forbid = true);
    void changeColor();

    void addOrder(Order * order);
    void removeOrder(Order * order);

    void showUnitHome(QString color);
    void hideUnitHome();

    void leftClick() { emit clicked(prototype->x, prototype->y); }
signals:
    void clicked(int i, int j);
};

#endif
