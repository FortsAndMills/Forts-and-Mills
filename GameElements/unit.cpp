#include "Unit.h"
#include "BasicElements/SpriteObject.h"
#include "Game/Game.h"
#include "Game/GameUnit.h"
#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Field.h"
#include "Hex.h"
#include "UnitPanel.h"

Unit::Unit(GameUnit *prototype, Game *game, GraphicObject *parent, Field *field, bool isMain) :
    GraphicObject(parent, (settings->PROGRAMM_VERSION == POWER_POINT_VERSION) *
                  (DRAGABLE | RIGHT_CLICKABLE | WHEEL) +
                   (settings->PROGRAMM_VERSION == REAL_CLIENT) *
                  (CLICKABLE))
{
    this->setZValue(constants->unitZPos);

    this->prototype = prototype;
    this->game = game;
    this->isMain = isMain;
    this->field = field;

    picture = new GraphicObject(this, CHILD, prototype->color + prototype->type);

    unitFrame = new Object(this, prototype->color + "UnitFrame");

    for (int i = 0; i < prototype->health; ++i)
    {
        health.append(new SpriteObject(this, 0, prototype->color + "UnitHealth"));
    }

    explosion = NULL;

    selection = NULL;

    ordersPanel = new UnitPanel(parent, game);
    ordersPanel->anchorTo(this);
    unitTypePanel = new UnitPanel(parent, game);
    unitTypePanel->anchorTo(this);
}
Unit::Unit(Unit *another) :
    Unit(another->prototype, another->game, dynamic_cast<GraphicObject *>(another->parentItem()), another->field, another->isMain)
{
    this->setX(another->x());
    this->setY(another->y());
    this->resize(another->width(), another->height());

    this->setEnabled(false);
}
void Unit::Delete()
{
    picture->Delete();
    unitFrame->Delete();
    foreach (SpriteObject * h, health)
        h->Delete();
    if (explosion != NULL)
        explosion->Delete();
    if (selection != NULL)
        selection->Delete();
    ordersPanel->Delete();
    unitTypePanel->Delete();

    GraphicObject::Delete();
}

void Unit::turnFrame()
{
    if (unitFrame->isDeleted)
        return;

    unitFrame->setRotation(0);

    Animation * a;
    if (isMain)
        a = unitFrame->AnimationStart(ROTATION, 360);
    else
        a = unitFrame->AnimationStart(ROTATION, -360);
    connect(a, SIGNAL(finished()), SLOT(turnFrame()));
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
QPointF Unit::turn(QPointF start, QPointF axis, qreal degree)
{
    degree *=  3.1415 / 180;
    return QPointF((start.x() - axis.x()) * cos(degree) - (start.y() - axis.y()) * sin(degree) + axis.x(),
                   (start.x() - axis.x()) * sin(degree) + (start.y() - axis.y()) * cos(degree) + axis.y());
}
void Unit::resizeHealth(qreal W, qreal H)
{
    int N = health.size();  // TODO возможно, тут теперь не нужна адская геометрия
    for (int i = 0; i < N; ++i)
    {
        qreal HealthIconWidth = constants->healthIconWidth * W;  // "входные данные"
        qreal HealthIconHeight = constants->healthIconHeight * H;
        QPointF rotation_center = QPointF(W / 2, H / 2);
        qreal rotation = (i - (N - 1) / 2.0) * constants->degreeBetweenUnitHealth;

        // ЛИНЕЙНАЯ АЛГЕБРА!
        QPointF start = QPointF(W / 2 - HealthIconWidth / 2,
                                -constants->healthPictureOffset * H);
        start = turn(start, rotation_center, rotation);

        QPointF center = QPointF(W / 2,
                                 -constants->healthPictureOffset * H + HealthIconHeight / 2);
        center = turn(center, rotation_center, rotation);

        start = turn(start, center, -rotation);

        health[i]->setGeometry(start.x(), start.y(), HealthIconWidth, HealthIconHeight);
        health[i]->setRotation(rotation);
    }
}
void Unit::resizeOrders()
{
    recountOrderGeometry();
    foreach (Order * order, orders_stack)
        order->setGeometry(orderGeometry[order]);
}
void Unit::resizeChildren(qreal W, qreal H)
{
    picture->setGeometry(0, 0, W, H);
    unitFrame->setGeometry(0, 0, W, H);
    turnFrame();
    if (selection != NULL)
    {
        selection->setGeometry(W * constants->unitSelectionPointX,
                                                  H * constants->unitSelectionPointY,
                                                  W * constants->unitSelectionWidth,
                                                  H * constants->unitSelectionHeight);
    }

    // панель ресайзится по своим правилам!
    ordersPanel->setPos(x() + W * constants->unitPanelPointX, y() + H * constants->unitPanelPointY);
    ordersPanel->reconfigure();
    unitTypePanel->setPos(x() + W * constants->unitPanelPointX, y() + H * constants->unitPanelPointY);
    unitTypePanel->reconfigure();

    resizeHealth(W, H);
    resizeOrders();
}

void Unit::reconfigureOrders()
{
    recountOrderGeometry();
    foreach (Order * order, orders_stack)
        order->AnimationStart(orderGeometry[order], constants->unitReconfigureTime);
}


// ИГРОВЫЕ ДЕЙСТВИЯ------------------------------------------------------------------------
void Unit::blow()
{
    this->setEnabled(false);
    if (prototype->x != UNDEFINED)  // спасаем гексы от заваливания программы
        qDebug() << "ERROR: not undefined unit is blown";

    this->AnimationStart(WIDTH, 0, constants->unitDisappearTime, true);
    this->AnimationStart(HEIGHT, 0, constants->unitDisappearTime, true);
    this->AnimationStart(X_POS,   x() + width() / 2, constants->unitDisappearTime, true);
    this->AnimationStart(Y_POS, y() + height() / 2, constants->unitDisappearTime, true);

    // FAIL rand() % 3 и по-другому никак
    explosion = new SpriteObject(dynamic_cast<GraphicObject *>(this->parentItem()), 0, "Explosion" + QString::number(rand() % 3 + 1));
    explosion->setGeometry(x() + constants->unitExplosionPointX * width(),
                                              y() + constants->unitExplosionPointY * height(),
                                              constants->unitExplosionWidth * width(),
                                              constants->unitExplosionHeight * height());
    connect(explosion, SIGNAL(looped()), this, SLOT(Delete()));
}

void Unit::healthChanged()
{
    foreach (SpriteObject * heal, health)  // TODO лишние удаления здоровья юнитов
    {
        heal->Delete();
    }
    health.clear();

    for (int i = 0; i < prototype->health; ++i)
    {
        health.append(new SpriteObject(this, 0, prototype->color + "UnitHealth"));
    }
    resizeHealth(width(), height());
}

void Unit::dragFinish()
{
    Hex * hex = field->HexAt(field->mapFromItem(parentItem(),
                                                x() + width() / 2, y() + height() / 2));
    if (hex == NULL)
        emit dragFinished(this->prototype, NULL);
    else
        emit dragFinished(this->prototype, hex->prototype);
}

void Unit::select()
{
    if (selection == NULL)
    {
        selection = new SpriteObject(this, 0, prototype->color + "UnitSelectionLayer");
        selection->setZValue(constants->selectionZpos);
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
    this->setZValue(constants->unitZPos);
}
void Unit::showOrdersPanel(QList<OrderType> orders)
{
    ordersPanel->setVariants(orders);
    ordersPanel->appear();

    foreach (SpecialButton * var, ordersPanel->variants)
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

    foreach (SpecialButton * var, unitTypePanel->variants)
        connect(var, SIGNAL(leftClicked(QString)), this, SIGNAL(unitTypeChosen(QString)));
}
void Unit::hideUnitTypePanel()
{
    unitTypePanel->disappear();
}

void Unit::locateOrderLikeOnPanel(Order * order)
{
    orders_stack.append(order);

    order->setPos(ordersPanel->pos() + ordersPanel->variantPos(order->prototype->type));
    order->resize(constants->orderVariantSize, constants->orderVariantSize);

    reconfigureOrders();
}

