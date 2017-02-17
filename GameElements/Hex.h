#ifndef HEX
#define HEX

#include "Technical/Headers.h"
#include "Technical/HelpManager.h"
#include "Game/GameHex.h"
#include "BasicElements/StateObject.h"
#include "BasicElements/MergingObject.h"
#include "Order.h"
#include "UnitWay.h"
#include "Shield.h"

class LivingNationPicture : public GraphicObject
{
    Q_OBJECT

    UnitType type;

public:
    LivingNationPicture(GraphicObject * parent, UnitType type) :
        GraphicObject(parent, RIGHT_CLICKABLE, type)
    {
        this->type = type;
    }

    void rightClick() { emit help->HelpAsked(type); }
};
class Information : public MergingObject
{
    Q_OBJECT

    QString info_name;
public:
    Information(GraphicObject * parent, QString pic_name, QString name) :
        MergingObject(parent, pic_name, RIGHT_CLICKABLE)
    {
        this->info_name = name;
    }

    void rightClick() { emit help->HelpAsked(info_name); }
};

class Hex : public StateObject
{
    Q_OBJECT

public:
    GameHex * prototype;
    Game * game;

    GraphicObject * HexPicture;
    QList <ResourcePic *> table;

    SpriteObject * Lighting;
    Object * UnitHomePicture;

    Object * livingNationPicture;
    MergingObject * information;

    QSet <int> ids;
public:
    QMap <int, QPointF> points;  // точки для юнитов и их атрибутов на гексе
    enum POSITION_STATE {STAY, ENTERING, LEAVING};
    QMap <int, POSITION_STATE> pointPositionState;
    QMap <int, WAY> pointsWay;

    QMap <int, QList <Object *> > orders;  // оставленные юнитом приказы
    QMap <Object *, QPointF> orders_pos;  // положение оставленных приказов

    QList <Shield *> shields;
    QMap <Shield *, QPointF> shield_pos;

    QMap <PlayerColor, MergingObject *> plannedCapturing;

public:
    explicit Hex(GraphicObject * parent, Game * game, GameHex * prototype);
    explicit Hex(Hex * another, GraphicObject *newParent);
    explicit Hex(Hex * another);
    virtual void Delete();

public:
    void resizeChildren(qreal W, qreal H);
    QList<QPointF> countTableCoordinates(qreal W, qreal H, int n);
    QMap <WAY, QPointF> EnterPoint;
    QMap <WAY, QPointF> LeavePoint;
    QMap <int, QVector <QPoint> > ShiftMap;
    void recountPoints();
    void recountOrdersPosition();
    void recountShieldsPosition();
public:
    QPointF shieldPoint();
    QPointF shieldPoint(Object * obj)
    {
        return mapToItem(obj, shieldPoint());
    }
    QPointF point(int p, Object * obj)
    {
        return mapToItem(obj, points[p]);
    }

private:
    void resizeOrders();
    void resizeShields();

public:
    void reconfigureOrders();
    void reconfigureShields();

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
    int createPoint(POSITION_STATE state = STAY, WAY way = "");
    void removePoint(int id);

    void setNewResourcesPosition(QList<OrderPic *> resources);

    void setState(QString state, bool isImmediate = false);
    void select(bool enable = true, bool immediate = false);
    void light(bool enable = true, bool immediate = false);
    void forbidToSelect(bool forbid = true);

    void planCapturing(PlayerColor color);
    void deplanCapturing(PlayerColor color);

    void addOrder(Order * order);
    void removeOrder(Order * order);

    void showUnitHome(QString color);
    void hideUnitHome();

    void hideLivingNation();    
    void showInformation(QString pic_name, QString name);
    void hideInformation();

    void highlight(OrderType type, bool light = true);

    void defenceAppear(int amount, PlayerColor color);
    void defenceDisappear();
    void defenceTurn(int amount, bool on = true);

    void leftClick() { emit clicked(prototype->coord); }
    void rightClick() { emit help->HelpAsked(prototype->type); }
    void enter() { emit entered(prototype->coord); }
    void leave() { emit left(prototype->coord); }
signals:
    void clicked(Coord p);
    void entered(Coord p);
    void left(Coord p);
};

#endif
