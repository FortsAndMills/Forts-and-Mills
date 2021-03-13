#ifndef UNIT
#define UNIT

#include "Technical/Headers.h"
#include "Technical/HelpManager.h"
#include "BasicElements/GraphicObject.h"
#include "GameElements/Order.h"

// положение юнита характеризуется не только координатами гекса
// но и точкой в самом гексе
class Position
{
public:
    Coord position;
    int point;

    Position(Coord position, int point)
    {
        this->position = position;
        this->point = point;
    }
};

class Unit : public GraphicObject
{
    Q_OBJECT

    Game * game;
public:
    GameUnit * prototype;

private:
    GraphicObject * picture;
    QList <SpriteObject *> health;
    Object * unitFrame;
    bool isMain;

    SpriteObject * selection;  // дым при выделении
    SpriteObject * bigSelection = NULL;
    SpriteObject * lighting;

    //Object * brokenGlass = NULL;  // "разбитость" при потере дома.
    SpriteObject * explosion = NULL; // взрыв при удалении

    UnitPanel * ordersPanel;  // панель с приказами!
    UnitPanel * unitTypePanel;  // панель с типами юнитов

public:
    MainOrder * mainOrder;  // основной приказ юнита
    QList <Order *> orders_stack;

private:
    QList <Shield *> shields;
    
public:
    QList <Position> stack;  // храним инфу о передвижении юнита по полю
    Coord where() { return stack.last().position; }
    int point() { return stack.last().point; }

private:
    bool isSelected = false;
    bool isEntered = false;
    bool isLighten = false;
    void recheckZPos()  // следим за тем, какой юнит "выше" всех остальных
    {
        if (isSelected || isEntered || isLighten)
            this->setZValue(constants->selectedUnitZPos);
        else
            this->setZValue(constants->unitZPos);
    }

public:
    explicit Unit(GameUnit * prototype, Game * game, GraphicObject * parent, bool isMain);
    explicit Unit(GraphicObject * parent, QString color, UnitType type);
    explicit Unit(Unit * another);
    virtual void Delete();

    // геометрия
    QPointF mainOrderCenter(Object * obj);
private:
    QRectF mainOrderGeometry();

    QPair <QRectF, qreal> healthGeometry(int i);

    QPointF turn(QPointF start, QPointF axis, qreal degree);
    void resizeHealth();
    void resizeOrders();
    void resizeShields();
    void resizeChildren(qreal W, qreal H);

    QMap <Order *, QRectF> orderGeometry;
    QMap <Shield *, QRectF> shieldGeometry;
public:
    void recountOrderGeometry();
    void reconfigureOrders();
    void reconfigureHealth();
    void recountShieldGeometry();
    QPointF shieldPoint();
    QPointF shieldPoint(Object * obj)
    {
        return mapToItem(obj, shieldPoint());
    }

    void locateOrderLikeOnPanel(Order *order);  // расположить приказ как будто на своей панели

private slots:
    void turnFrame();  // для поворота рамки

public:
    void healthChanged(int change);
    void broke();  // разбить юнит
    void blow();   // взорвать юнит

    void mainOrderAppear(OrderType prototype);  // образовать основной приказ
    void mainOrderDisappear();
    bool isMainOrderOpenned();
    void openMainOrder();
    void mainOrderBurn();

    void select();  // подсветить юнит
    void deselect();
    void light(bool on = true);
    void showOrdersPanel(QList <OrderType> orders);  // панель приказов
    void hidePanel();
    void showUnitTypePanel(QList <UnitType> types);  // панель выбора типа юнитов
    void hideUnitTypePanel();

    void defenseTurn(int amount, bool on = true);

public:
    void wheel(int delta) { emit wheeled(this->prototype, delta); }
    void rightClick() { emit help->HelpAsked(this->prototype->type); }
    void leftClick() { emit leftClicked(this->prototype); }
    void enter()
    {
        isEntered = true;
        recheckZPos();
        emit entered(this->prototype);
    }
    void leave()
    {
        isEntered = false;
        recheckZPos();
        emit left(this->prototype);
    }
    void dragStart() { emit dragStarted(this->prototype); }  // посылки сигналов
    void dragFinish() { emit dragFinished(this->prototype); }

signals:
    void wheeled(GameUnit *, int);
    void leftClicked(GameUnit *);
    void entered(GameUnit *);
    void left(GameUnit *);
    void dragStarted(GameUnit *);
    void dragFinished(GameUnit *);

    void orderVariantChosen(OrderType type);
    void unitTypeChosen(UnitType type);
};

#endif



