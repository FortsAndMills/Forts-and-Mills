#include "Unit.h"
#include "BasicElements/SpriteObject.h"
#include "GameExecution/Game.h"
#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Field.h"
#include "Hex.h"
#include "UnitPanel.h"

Unit::Unit(GameUnit *prototype, Game *game, GraphicObject *parent, bool isMain) :
    GraphicObject(parent, CLICKABLE | HOVER | RIGHT_CLICKABLE)
{
    this->setZValue(constants->unitZPos);

    this->prototype = prototype;
    this->game = game;
    this->isMain = isMain;

    picture = new GraphicObject(this, CHILD, prototype->color + prototype->type);

    unitFrame = new Object(this, prototype->color + "UnitFrame");

    for (int i = 0; i < prototype->health; ++i)
    {
        health.append(new SpriteObject(this, 0, prototype->color + "UnitHealth"));
    }

    for (int i = 0; i < prototype->defenceBonus; ++i)
    {
        shields << new Shield(this, prototype->color);
    }

    explosion = NULL;
    selection = NULL;
    mainOrder = NULL;
    bigSelection = NULL;
    lighting = NULL;

    ordersPanel = new UnitPanel(parent, game);
    ordersPanel->anchorTo(this);
    unitTypePanel = new UnitPanel(parent, game);
    unitTypePanel->anchorTo(this);
}
Unit::Unit(GraphicObject *parent, QString color, QString type) :
    GraphicObject(parent, CHILD)
{
    GameUnitParameters prototype(settings->rules, type);

    picture = new GraphicObject(this, CHILD, color + type);

    if (color != "Neutral")
        unitFrame = new Object(this, color + "UnitFrame");
    else
        unitFrame = NULL;

    for (int i = 0; i < prototype.init_health; ++i)
    {
        health.append(new SpriteObject(this, 0, color + "UnitHealth"));
    }

    for (int i = 0; i < prototype.max_defenceBonus; ++i)
    {
        shields << new Shield(this, color);
    }

    explosion = NULL;
    selection = NULL;
    mainOrder = NULL;
    bigSelection = NULL;
    lighting = NULL;

    ordersPanel = NULL;
    unitTypePanel = NULL;
}
Unit::Unit(Unit *another) :
    Unit(another->prototype, another->game, dynamic_cast<GraphicObject *>(another->parentItem()), another->isMain)
{
    this->setX(another->x());
    this->setY(another->y());
    this->resize(another->width(), another->height());

    if (another->brokenGlass != NULL)
        broke();

    this->setEnabled(false);
}
void Unit::Delete()
{
    picture->Delete();
    if (unitFrame != NULL)
        unitFrame->Delete();
    foreach (SpriteObject * h, health)
        h->Delete();
    if (explosion != NULL)
        explosion->Delete();
    if (selection != NULL)
        selection->Delete();
    if (bigSelection != NULL)
        bigSelection->Delete();
    if (lighting != NULL)
        lighting->Delete();
    if (ordersPanel != NULL)
        ordersPanel->Delete();
    if (unitTypePanel != NULL)
        unitTypePanel->Delete();
    if (mainOrder != NULL)
        mainOrder->Delete();
    if (brokenGlass != NULL)
        brokenGlass->Delete();
    foreach (Shield * s, shields)
        s->Delete();

    GraphicObject::Delete();
}

// вращение обода юнита
void Unit::turnFrame()
{
    if (unitFrame == NULL || unitFrame->isDeleted)
        return;

    unitFrame->setRotation(0);

    Animation * a;
    if (isMain)
        a = unitFrame->AnimationStart(ROTATION, 360);
    else
        a = unitFrame->AnimationStart(ROTATION, -360);
    connect(a, SIGNAL(finished()), SLOT(turnFrame()));
}

void Unit::recountShieldGeometry()
{
    for (int i = 0; i < shields.size(); ++i)
    {
        shieldGeometry[shields[i]] = QRectF(
                    width() * (1 / 2.0 + (i - shields.size() / 2.0) * constants->unitShieldsWidth +
                         (i - (shields.size() - 1) / 2.0) * constants->unitShieldsMergeX),
                    height() * constants->unitShieldsLineY,
                    constants->unitShieldsWidth * width(), constants->unitShieldsHeight * height());
    }
}
QPointF Unit::shieldPoint()
{
    return QPointF(width() / 2, height() * (constants->unitShieldsLineY + constants->unitShieldsHeight / 2));
}
void Unit::recountOrderGeometry()
{
    qreal X = x() + constants->unitsSize * constants->unitOrderPointX;
    qreal Y = y() + constants->unitsSize * constants->unitOrderPointY;

    for (int i = 0; i < orders_stack.size(); ++i)
    {
        orderGeometry[orders_stack[i]] = QRectF(X, Y,
                                                constants->unitsSize * constants->unitOrderWidth,
                                                constants->unitsSize * constants->unitOrderHeight);

        X += constants->orderStackShiftX * constants->unitsSize * constants->unitOrderWidth;
        Y += constants->orderStackShiftY * constants->unitsSize * constants->unitOrderHeight;
    }
}
QRectF Unit::mainOrderGeometry()
{
    return QRectF(width() * constants->mainOrderX,
                             height() * constants->mainOrderY,
                             width() * constants->mainOrderWidth,
                  height() * constants->mainOrderHeight);
}
QPointF Unit::mainOrderCenter(Object *obj)
{
    return mapToItem(obj, mainOrderGeometry().center());
}

QPair<QRectF, qreal> Unit::healthGeometry(int i)
{
    qreal HealthIconWidth = constants->healthIconWidth * width();  // "входные данные"
    qreal HealthIconHeight = constants->healthIconHeight * height();
    QPointF rotation_center = QPointF(width() / 2, height() / 2);
    qreal rotation = (i - (health.size() - 1) / 2.0) * constants->degreeBetweenUnitHealth;

    // ЛИНЕЙНАЯ АЛГЕБРА!
    QPointF start = QPointF(width() / 2 - HealthIconWidth / 2,
                            -constants->healthPictureOffset * height());
    start = turn(start, rotation_center, rotation);

    QPointF center = QPointF(width() / 2,
                             -constants->healthPictureOffset * height() + HealthIconHeight / 2);
    center = turn(center, rotation_center, rotation);

    start = turn(start, center, -rotation);

    return QPair<QRectF, qreal> (QRectF(start.x(), start.y(),
                                        HealthIconWidth, HealthIconHeight),
                                                       rotation);
}
QPointF Unit::turn(QPointF start, QPointF axis, qreal degree)
{
    degree *=  3.1415 / 180;
    return QPointF((start.x() - axis.x()) * cos(degree) - (start.y() - axis.y()) * sin(degree) + axis.x(),
                   (start.x() - axis.x()) * sin(degree) + (start.y() - axis.y()) * cos(degree) + axis.y());
}
void Unit::resizeHealth()
{
    for (int i = 0; i < health.size(); ++i)
    {
        QPair <QRectF, qreal> geom = healthGeometry(i);
        health[i]->setGeometry(geom.first);
        health[i]->setRotation(geom.second);
        health[i]->setVisible(true);
    }
}

void Unit::resizeOrders()
{
    recountOrderGeometry();
    foreach (Order * order, orders_stack)
        order->setGeometry(orderGeometry[order]);
}
void Unit::resizeShields()
{
    recountShieldGeometry();
    for (int i = 0; i < shields.size(); ++i)
    {
        shields[i]->setGeometry(shieldGeometry[shields[i]]);
    }
}
void Unit::resizeChildren(qreal W, qreal H)
{
    picture->setGeometry(0, 0, W, H);
    if (unitFrame != NULL)
        unitFrame->setGeometry(0, 0, W, H);
    turnFrame();
    if (selection != NULL)
    {
        selection->setGeometry(W * constants->unitSelectionPointX,
                                                  H * constants->unitSelectionPointY,
                                                  W * constants->unitSelectionWidth,
                                                  H * constants->unitSelectionHeight);
    }
    if (bigSelection != NULL)
    {
        bigSelection->setGeometry(W * constants->unitBigSelectionPointX,
                                                  H * constants->unitBigSelectionPointY,
                                                  W * constants->unitBigSelectionWidth,
                                                  H * constants->unitBigSelectionHeight);
    }
    if (lighting != NULL)
    {
        lighting->setGeometry(W * constants->unitLightingPointX,
                                                  H * constants->unitLightingPointY,
                                                  W * constants->unitLightingWidth,
                                                  H * constants->unitLightingHeight);
    }
    if (brokenGlass != NULL)
    {
        brokenGlass->setGeometry(0, 0, W, H);
    }

    // панель ресайзится по своим правилам!
    if (ordersPanel != NULL)
    {
        ordersPanel->setPos(x() + W * constants->unitPanelPointX, y() + H * constants->unitPanelPointY);
        ordersPanel->reconfigure();
    }
    if (unitTypePanel != NULL)
    {
        unitTypePanel->setPos(x() + W * constants->unitPanelPointX, y() + H * constants->unitPanelPointY);
        unitTypePanel->reconfigure();
    }

    if (mainOrder != NULL)
        mainOrder->setGeometry(mainOrderGeometry());

    resizeHealth();
    resizeOrders();
    resizeShields();
}

void Unit::reconfigureOrders()
{
    recountOrderGeometry();
    foreach (Order * order, orders_stack)
        order->AnimationStart(orderGeometry[order], constants->unitReconfigureTime);
}
void Unit::reconfigureHealth()
{
    for (int i = 0; i < health.size(); ++i)
    {
        QPair <QRectF, qreal> geom = healthGeometry(i);
        health[i]->AnimationStart(geom.first, constants->unitReconfigureTime);
        health[i]->AnimationStart(ROTATION, geom.second, constants->unitReconfigureTime);
        health[i]->setVisible(true);
    }
}

// ИГРОВЫЕ ДЕЙСТВИЯ------------------------------------------------------------------------
void Unit::blow()  // взрыв с последующим удалением
{
    this->setEnabled(false);

    this->AnimationStart(QRectF(x() + width() / 2, y() + height() / 2, 0, 0), constants->unitDisappearTime);

    // FAIL rand() % 3 и по-другому никак
    explosion = new SpriteObject(dynamic_cast<GraphicObject *>(this->parentItem()), 0, "Explosion" + QString::number(rand() % 3 + 1));
    explosion->setGeometry(x() + constants->unitExplosionPointX * width(),
                                              y() + constants->unitExplosionPointY * height(),
                                              constants->unitExplosionWidth * width(),
                                              constants->unitExplosionHeight * height());
    connect(explosion, SIGNAL(looped()), this, SLOT(Delete()));
}
void Unit::broke()
{
    brokenGlass = new Object(this, "BrokenGlass");
    brokenGlass->setGeometry(0, 0, width(), height());

    brokenGlass->setOpacity(0);
    brokenGlass->AnimationStart(OPACITY, 1);
}

void Unit::mainOrderAppear(OrderType prototype)
{
    mainOrder = new MainOrder(this, prototype, isMain);

    mainOrder->setGeometry(width() * constants->mainOrderX + width() * constants->mainOrderWidth / 2,
                           height() * constants->mainOrderY + height() * constants->mainOrderHeight / 2,
                                               0, 0);
    mainOrder->AnimationStart(mainOrderGeometry(), constants->mainOrderAppearTime);
}
void Unit::mainOrderDisappear()
{
    if (mainOrder == NULL)
    {
        debug << "FATAL ERROR: main order (for disappearing) is not found\n";
        return;
    }

    mainOrder->AnimationStart(QRectF(width() * constants->mainOrderX + width() * constants->mainOrderWidth / 2,
                           height() * constants->mainOrderY + height() * constants->mainOrderHeight / 2,
                                               0, 0), constants->mainOrderAppearTime);
    connect(mainOrder, SIGNAL(movieFinished()), mainOrder, SLOT(Delete()));

    mainOrder = NULL;
}
bool Unit::isMainOrderOpenned()
{
    return mainOrder == NULL || mainOrder->opened;
}
void Unit::openMainOrder()
{
    mainOrder->open();

    // пускаем дым в честь такого события
    bigSelection = new SpriteObject(this, 0, prototype->color + "UnitSelectionLayer", -1, true);
    bigSelection->setZValue(constants->selectionZpos);
    resizeChildren(width(), height());
    connect(bigSelection, SIGNAL(looped()), bigSelection, SLOT(Delete()));
    bigSelection = NULL;
}
void Unit::mainOrderBurn()
{
    if (mainOrder == NULL)
    {
        debug << "ERROR: burned null order\n";
        return;
    }

    mainOrder->burn();
    mainOrder = NULL;
}

void Unit::healthChanged(int change)
{
    while (change < 0)
    {
        health[health.size() - 1]->disappear(constants->unitDisappearTime);
        health.removeLast();
        ++change;
    }
    while (change > 0)
    {
        health.append(new SpriteObject(this, 0, prototype->color + "UnitHealth"));
        --change;
    }

    reconfigureHealth();
}

void Unit::select()
{
    if (selection == NULL)
    {
        selection = new SpriteObject(this, 0, prototype->color + "UnitSelectionLayer");
        selection->setZValue(constants->selectionZpos);
        isSelected = true;
        recheckZPos();
        resize(width(), height());
    }
    this->setZValue(constants->selectedUnitZPos);
}
void Unit::deselect()
{
    if (selection != NULL)
    {
        selection->Delete();
        selection = NULL;
    }
    isSelected = false;
    recheckZPos();
}
void Unit::light(bool on)
{
    if (on)
    {
        if (lighting == NULL)
        {
            lighting = new SpriteObject(this, 0, prototype->color + "Light");
            lighting->setZValue(constants->selectionZpos);
            isLighten = true;
            recheckZPos();
            resize(width(), height());
        }
    }
    else
    {
        if (lighting != NULL)
        {
            lighting->Delete();
            lighting = NULL;
        }

        isLighten = false;
        recheckZPos();
    }
}
void Unit::showOrdersPanel(QList<OrderType> orders)
{
    ordersPanel->setVariants(orders);
    ordersPanel->appear();

    // подключаем сигналы от панели
    // кнопки панели меняются при её включении, поэтому это нужно делать здесь
    foreach (OrderVariant * var, ordersPanel->variants)
        connect(var, SIGNAL(leftClicked(QString)), this, SIGNAL(orderVariantChosen(QString)));
}
void Unit::hidePanel()
{
    ordersPanel->disappear();
}
void Unit::showUnitTypePanel(QList<UnitType> types)
{
    unitTypePanel->setVariants(types);
    unitTypePanel->appear();

    foreach (OrderVariant * var, unitTypePanel->variants)
        connect(var, SIGNAL(leftClicked(QString)), this, SIGNAL(unitTypeChosen(QString)));
}
void Unit::hideUnitTypePanel()
{
    unitTypePanel->disappear();
}

// выбранный приказ появляется на панели приказов, и оттуда едет к юниту
void Unit::locateOrderLikeOnPanel(Order * order)
{
    orders_stack.append(order);

    order->setPos(ordersPanel->pos() + ordersPanel->variantPos(order->prototype->type));
    order->resize(constants->orderVariantSize, constants->orderVariantSize);

    reconfigureOrders();
}

// появление или исчезновение щитов
void Unit::defenceTurn(int amount, bool on)
{
    int i = shields.size() - 1;
    while (i >= 0 && !shields[i]->isOn)
        --i;

    if (on)
    {
        for (int j = 0; j < amount; ++j)
        {
            ++i;
            shields[i]->turnOn();
        }
    }
    else
    {
        for (int j = 0; j < amount; ++j)
        {
            shields[i]->turnOn(false);
            --i;
        }
    }
}

