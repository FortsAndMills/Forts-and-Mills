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

// Картинка речки
// Отличие от обычной картинки в том, что по правой кнопке мыши
// она должна выдавать подсказку
class River : public GraphicObject
{
    Q_OBJECT

public:
    River(GraphicObject * parent) :
        GraphicObject(parent, RIGHT_CLICKABLE, "River") {}

    void rightClick() { emit help->HelpAsked("River"); }
};

// Какая-либо мигающая информация вверху гекса
// Такая может быть только одна единовременно
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

// ГЕКС
class Hex : public StateObject
{
    Q_OBJECT

public:
    GameHex * prototype;
    Game * game;

    GraphicObject * HexPicture;  // мельница, форт, гора, озеро или ничего
    QMap <WAY, River *> rivers;  // реки
    QList <ResourcePic *> table;  // таблица ресурсов гекса

    SpriteObject * Lighting;  // подсветка при выделении перед стартом
    Object * UnitHomePicture;  // домик, который отображается при наведении на соотв. юнита

    MergingObject * information;  // мерцающая вверху информация (не является домом или рекрутирующийся юнит)

    // точки для юнитов и их атрибутов на гексе
    // всё настолько хитро, что вынесено в отдельный .cpp файл
    // у каждой точки есть id, координаты на гексе, состояние ("входит, выходит, стоит")
    // и, если входит-выходит, то в каком направлении.
    // все объекты на гексе привязаны к той или иной точке (хранят просто id)
    // гекс по набору имеющихся точек определяет их координаты
    QSet <int> ids;
    QMap <int, QPointF> points;
    enum POSITION_STATE {STAY, ENTERING, LEAVING};
    QMap <int, POSITION_STATE> pointPositionState;
    QMap <int, WAY> pointsWay;

    QMap <int, QList <Object *> > orders;  // оставленные юнитом приказы
    QMap <Object *, QPointF> orders_pos;  // положение оставленных приказов

    QList <Shield *> shields;  // тоже самое для щитов
    QMap <Shield *, QPointF> shield_pos;

private:
    // при планировании, захват отображается как мигание соответствующим цветом
    QMap <PlayerColor, MergingObject *> plannedCapturing;
public:
    explicit Hex(GraphicObject * parent, Game * game, GameHex * prototype);
    explicit Hex(Hex * another, GraphicObject *newParent);
    explicit Hex(Hex * another);
    virtual void Delete();

public:
    void resizeChildren(qreal W, qreal H);
    // пересчёт координат таблицы ресурсов
    QList<QPointF> countTableCoordinates(qreal W, qreal H, int n);

    // точки входа и выхода из гекса в каждом направлении
    QMap <WAY, QPointF> EnterPoint;
    QMap <WAY, QPointF> LeavePoint;
    // у гекса есть семь "якорей": по шести направлениям и центр
    // если в якоре нужно расположить несколько объектов, они расбрасываются
    // согласно указанному в данной карте
    QMap <int, QVector <QPoint> > ShiftMap;

    // Перерасчёт позиций точек, а через них - позиций объектов
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

    // resize мгновенно меняет размер, reconfigure делает это через анимацию
private:
    void resizeOrders();
    void resizeShields();
public:
    void reconfigureOrders();
    void reconfigureShields();

private:
    // полезная оптимизация
    void startedAnimation()
    {
        this->setEnabled(false);
    }
    void finishedAnimation()
    {
        this->setEnabled(true);
    }

public:
    // создание и удаление новых точек
    int createPoint(POSITION_STATE state = STAY, WAY way = NO_WAY);
    void removePoint(int id);

    // размещает переданные ресурсы также, как и ресурсы в таблице на гексе
    // нужно для анимации сбора ресурсов
    void setNewResourcesPosition(QList<OrderPic *> resources);

    // смена цвета, выделения и подсветки
    void setState(QString state, bool isImmediate = false);
    void select(bool enable = true, bool immediate = false);
    void light(bool enable = true, bool immediate = false);
    void forbidToSelect(bool forbid = true);

    void planCapturing(PlayerColor color);
    void deplanCapturing(PlayerColor color);

    void showUnitHome(QString color);
    void hideUnitHome();

    void highlight(OrderType type, bool light = true);

    void showInformation(QString pic_name, QString name, bool merge = true);
    void hideInformation();

    // добавление и удаление объектов
    void addOrder(Order * order);
    void removeOrder(Order * order);

    void defenceAppear(int amount, PlayerColor color);
    void defenceDisappear();
    void defenceTurn(int amount, bool on = true);

    // интерактивность
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
