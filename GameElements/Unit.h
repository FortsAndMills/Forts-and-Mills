#ifndef UNIT
#define UNIT

#include "Technical/Headers.h"
#include "BasicElements/GraphicObject.h"
#include "Order.h"

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

    SpriteObject * explosion; // взрыв при удалении

    UnitPanel * ordersPanel;  // панель с приказами!
    UnitPanel * unitTypePanel;  // панель с типами юнитов

public:  // FAIL из-за, блин, GameWindow
    QList <Order *> orders_stack;
    
    QStack <int> points_stack;  // храним инфу о передвижении юнита по полю
    QStack <int> x_stack;
    QStack <int> y_stack;

public:
    explicit Unit(GameUnit * prototype, Game * game, GraphicObject * parent, Field * field, bool isMain);
    explicit Unit(Unit * another);
    virtual void Delete();

private:
    QMap <Order *, QRectF> orderGeometry;
    void recountOrderGeometry();

    QPointF turn(QPointF start, QPointF axis, qreal degree);
    void resizeHealth(qreal W, qreal H);
    void resizeOrders();
    void resizeChildren(qreal W, qreal H);

public:
    void reconfigureOrders();

    void locateOrderLikeOnPanel(Order *order);  // расположить приказ как будто на своей панели

private slots:
    void turnFrame();  // для поворота рамки

public:
    void healthChanged();  // <undone> заменить на снижение и увеличеие здоровья
    void blow();        // взорвать юнит

private:
    Field * field;  // TODO для работы с якорем якобы необходима ссылка на поле...
public:
    void select();  // подсветить юнит
    void deselect();
    void showOrdersPanel(QList <OrderType> orders);  // панель
    void hidePanel();
    void showUnitTypePanel(QList <UnitType> types);  // панель
    void hideUnitTypePanel();

public:
    void wheel(int delta) { emit wheeled(this->prototype, delta); }
    void rightClick() { emit rightClicked(this->prototype); }
    void leftClick() { emit leftClicked(this->prototype); }
    void enter() { emit entered(this->prototype); }
    void leave() { emit left(this->prototype); }
    void dragStart() { emit dragStarted(this->prototype); }  // посылки сигналов
    void dragFinish();

signals:
    void wheeled(GameUnit *, int);
    void rightClicked(GameUnit *);
    void leftClicked(GameUnit *);
    void entered(GameUnit *);
    void left(GameUnit *);
    void dragStarted(GameUnit *);
    void dragFinished(GameUnit *, GameHex *);

    void orderVariantChosen(OrderType type);
    void unitTypeChosen(UnitType type);
};

#endif



