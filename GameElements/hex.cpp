#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Game/GameUnit.h"
#include "BasicElements/SpriteObject.h"
#include "GameElements/Order.h"
#include "Game/GameHex.h"
#include "Unit.h"
#include "Hex.h"

Hex::Hex(GraphicObject *parent, Game *game, GameHex *prototype) :
    StateObject(parent, "Neutral", prototype->type + "Hex", CLICKABLE | HOVER | RIGHT_CLICKABLE, prototype->type + "HexFrame", "SimpleLayer")
{
    foreach (GamePlayer * player, game->players)
        addPicture(player->color, player->color + "Hex");
    addPicture("selected", "SelectedHex");
    addPicture("lighted", "LightedHex");

    this->prototype = prototype;
    this->game = game;

    Lighting = NULL;

    if (prototype->type != "Simple")
        HexPicture = new GraphicObject(this, 0, prototype->type);
    else
        HexPicture = NULL;

    foreach (WAY way, WAYS)
    {
        if (prototype->rivers[way])
            rivers[way] = new River(this);
        else
            rivers[way] = NULL;
    }

    foreach (OrderType R, prototype->resources)
    {
        table.append(new ResourcePic(this, R));
    }

    UnitHomePicture = new Object(this, "");
    UnitHomePicture->setVisible(false);

    information = NULL;
}
// создание копии-призрака для анимации сдвига поля
Hex::Hex(Hex *another, GraphicObject * newParent) : Hex(newParent, another->game, another->prototype)
{
    this->setX(another->x());
    this->setY(another->y());

    for (int i = 0; i < another->shields.size(); ++i)
    {
        shields << new Shield(another->shields[i], this);
    }

    if (another->information != NULL)
        this->information = new MergingObject(this, another->information->name);

    this->setState(another->cur_picture, true);
    this->resize(another->width(), another->height());

    this->setProperties(0);
    this->setEnabled(false);
    foreach (GraphicObject * pic, this->pictures)
        pic->setEnabled(false);
}
Hex::Hex(Hex *another) : Hex(another, dynamic_cast<GraphicObject *>(another->parentItem()))
{

}
void Hex::Delete()
{
    UnitHomePicture->Delete();
    if (Lighting != NULL)
        Lighting->Delete();
    if (HexPicture != NULL)
        HexPicture->Delete();
    foreach (WAY way, WAYS)
        if (rivers[way] != NULL)
            rivers[way]->Delete();
    foreach (ResourcePic * pic, table)
        pic->Delete();
    if (information != NULL)
        information->Delete();
    foreach (Shield * s, shields)
        s->Delete();
    foreach (MergingObject * mo, plannedCapturing)
        mo->Delete();

    StateObject::Delete();
}

// геомееетрия!
QList<QPointF> Hex::countTableCoordinates(qreal W, qreal H, int n)
{
    QList<QPointF> answer;
    qreal margin = constants->resourceTableMargin * W;
    qreal pic_size = constants->resourceTablePicSize * W;
    qreal borders = constants->resourceTableBorders * W;

    int i = 0;
    qreal line = borders;
    while (i < n)
    {
        line += margin;

        if (line > H / 2)
            debug << "ERROR: table of resources overflow\n";

        // shift - координата по x нашего прямоугольника.
        qreal shift = (H - 2 * line) / H * constants->hexShift * W;
        int HowMuch = (W - 2 * shift - 2 * borders - margin) / (pic_size + margin);
        if (HowMuch > n - i)
        {
            HowMuch = n - i;
        }

        qreal dont_need = (W - 2 * shift - 2 * borders + margin) - HowMuch * (pic_size + margin);

        qreal x = shift + borders + dont_need / 2;
        line += pic_size;
        for (int j = 0; j < HowMuch; ++j)
        {
            answer << QPointF(x, H - line);

            x += pic_size + margin;
            ++i;
        }
    }
    return answer;
}
void Hex::recountOrdersPosition()
{
    for (int i = 0; i < orders.size(); ++i)
    {
        // для каждого якоря гекса центр приказа изначально совпадает с центром якоря
        QPointF center = points[i];
        for (int j = 0; j < orders[i].size(); ++j)
        {
            orders_pos[orders[i][j]] = (QPointF)mapToItem(orders[i][j]->parentItem(),
                        center.x() - constants->unitOrderWidth * constants->unitsSize / 2,
                        center.y() - constants->unitOrderHeight * constants->unitsSize / 2);

            // каждый следующий приказ смещается вправо и вниз
            center += QPointF(constants->orderStackShiftX * constants->unitOrderWidth * constants->unitsSize,  // TODO произведения констант сделать константой
                                           constants->orderStackShiftX * constants->unitOrderWidth * constants->unitsSize);
        }
    }
}
void Hex::recountShieldsPosition()
{
    for (int i = 0; i < shields.size(); ++i)
    {
        shield_pos[shields[i]] = QPointF(
                    width() * (1 / 2.0 + (i - shields.size() / 2.0) * constants->hexShieldsWidth +
                         (i - (shields.size() - 1) / 2.0) * constants->hexShieldsMergeX),
                    height() * (constants->hexShieldsLineY + constants->hexShieldsHeight / 2));
    }
}
QPointF Hex::shieldPoint()
{
    return QPointF(width() / 2, height() * (constants->hexShieldsLineY + constants->hexShieldsHeight / 2));
}

void Hex::resizeChildren(qreal W, qreal H)
{
    UnitHomePicture->setGeometry(W * constants->unitHomePointX,
                                                            H * constants->unitHomePointY,
                                                            W * constants->unitHomeWidth,
                                                            H * constants->unitHomeHeight);
    if (Lighting != NULL)
        Lighting->setGeometry(0, 0, W, H);
    if (HexPicture != NULL)
    {
        HexPicture->setGeometry(W * constants->hexPictureOffsetW,
                                                    H * constants->hexPictureOffsetH,
                                                    W * (1 - 2 * constants->hexPictureOffsetW),
                                                    H * (1 - 2 * constants->hexPictureOffsetH));
    }

    if (information != NULL)
    {
        information->setGeometry(W * constants->informationPointX,
                                                     H * constants->informationPointY,
                                                     W * constants->informationWidth,
                                                     H * constants->informationHeight);
    }

    QMap<WAY, QPointF> river_centers;
    river_centers[UP] = QPointF(W / 2, 0);
    river_centers[RIGHT_UP] = QPointF(W * (1 - constants->hexShift / 2), H / 4);
    river_centers[RIGHT_DOWN] = QPointF(W * (1 - constants->hexShift / 2), 3 * H / 4);
    river_centers[DOWN] = QPointF(W / 2, H);
    river_centers[LEFT_DOWN] = QPointF(W * constants->hexShift / 2, 3 * H / 4);
    river_centers[LEFT_UP] = QPointF(W * constants->hexShift / 2, H / 4);

    // TODO: всё должно быть кратно 60, но из-за странностей картинки гекса...
    QMap<WAY, int> river_rotation;
    river_rotation[UP] = -1;
    river_rotation[RIGHT_UP] = 58;
    river_rotation[RIGHT_DOWN] = 121;
    river_rotation[DOWN] = 179;
    river_rotation[LEFT_DOWN] = 238;
    river_rotation[LEFT_UP] = 301;

    foreach (WAY way, WAYS)
    {
        if (rivers[way] != NULL)
        {
            float width = W * (1 - 2 * constants->hexShift);
            float height = H * constants->riverHeight;
            rivers[way]->setGeometry(river_centers[way].x() - width / 2,
                                     river_centers[way].y() - height / 2,
                                     width,
                                     height);
            rivers[way]->setRotation(river_rotation[way]);
        }
    }

    QList <QPointF> table_pos = countTableCoordinates(W, H, table.size());
    for (int i = 0; i < table.size(); ++i)
    {
        table[i]->setPos(table_pos[i]);
        table[i]->resize(constants->resourceTablePicSize * W,
                                  constants->resourceTablePicSize * W);
    }

    foreach (MergingObject * mo, plannedCapturing)
        mo->setGeometry(0, 0, W, H);

    // ЗАПОЛНЕНИЕ КОНСТАНТ ЯКОРЕЙ

    EnterPoint[UP] = QPointF(W / 2, 3 * H / 4);  // TODO придумать что-то более адекватное!
    EnterPoint[DOWN] = QPointF(W / 2, H / 4);
    EnterPoint[LEFT_DOWN] = QPointF(W * (3 - constants->hexShift) / 4, 3 * H / 8);
    EnterPoint[LEFT_UP] = QPointF(W * (3 - constants->hexShift) / 4, 5 * H / 8);
    EnterPoint[RIGHT_DOWN] = QPointF(W * (1 + constants->hexShift) / 4, 3 * H / 8);
    EnterPoint[RIGHT_UP] = QPointF(W * (1 + constants->hexShift) / 4, 5 * H / 8);

    LeavePoint[UP] = QPointF(W / 2, H / 4 - H / 8);
    LeavePoint[DOWN] = QPointF(W / 2, 3 * H / 4 + H / 8);
    LeavePoint[RIGHT_UP] = QPointF(W * (3 - constants->hexShift) / 4, 3 * H / 8);
    LeavePoint[RIGHT_DOWN] = QPointF(W * (3 - constants->hexShift) / 4, 5 * H / 8);
    LeavePoint[LEFT_UP] = QPointF(W * (1 + constants->hexShift) / 4, 3 * H / 8);
    LeavePoint[LEFT_DOWN] = QPointF(W * (1 + constants->hexShift) / 4, 5 * H / 8);

    ShiftMap.clear();
    ShiftMap[1] << QPoint(0, 0);
    ShiftMap[2] << QPoint(W / 6, 0) << QPoint(-W / 6, 0);
    ShiftMap[3] << QPoint(0, -H / 6) << QPoint(W / 6 / 1.7, H / 6 / 1.7) << QPoint(-W / 6 / 1.7, H / 6 / 1.7);
    ShiftMap[4] << QPoint(W / 6 / 1.7, -H / 6 / 1.7) << QPoint(W / 6 / 1.7, H / 6 / 1.7) << QPoint(-W / 6 / 1.7, H / 6 / 1.7) << QPoint(-W / 6 / 1.7, -H / 6 / 1.7);
    ShiftMap[5] << QPoint(W / 6 / 1.7, -H / 6 / 1.7) << QPoint(W / 6 / 1.7, H / 6 / 1.7) << QPoint(-W / 6 / 1.7, H / 6 / 1.7) << QPoint(-W / 6 / 1.7, -H / 6 / 1.7) << QPoint(0, 0);

    recountPoints();
    resizeOrders();
    resizeShields();
}
void Hex::resizeOrders()
{
    recountOrdersPosition();
    for (int i = 0; i < orders.size(); ++i)
    {
        for (int j = 0; j < orders[i].size(); ++j)
        {
            orders[i][j]->setPos(orders_pos[orders[i][j]]);
            orders[i][j]->resize(constants->unitOrderWidth * constants->unitsSize,
                                   constants->unitOrderHeight * constants->unitsSize);
        }
    }
}
void Hex::resizeShields()
{
    recountShieldsPosition();
    for (int i = 0; i < shields.size(); ++i)
    {
        shields[i]->setPos(shield_pos[shields[i]]);
        shields[i]->resize(constants->hexShieldsWidth * width(),
                                     constants->hexShieldsHeight * height());
    }
}

void Hex::reconfigureOrders()
{
    recountOrdersPosition();
    for (int i = 0; i < orders.size(); ++i)
    {
        for (int j = 0; j < orders[i].size(); ++j)
        {
            orders[i][j]->AnimationStart(X_POS, orders_pos[orders[i][j]].x(), constants->orderReconfigureTime);
            orders[i][j]->AnimationStart(Y_POS, orders_pos[orders[i][j]].y(), constants->orderReconfigureTime);
            orders[i][j]->AnimationStart(WIDTH, constants->unitsSize * constants->unitOrderWidth, constants->orderReconfigureTime);
            orders[i][j]->AnimationStart(HEIGHT, constants->unitsSize * constants->unitOrderHeight, constants->orderReconfigureTime);
        }
    }
}
void Hex::reconfigureShields()
{
    recountShieldsPosition();
    for (int i = 0; i < shields.size(); ++i)
    {
        shields[i]->AnimationStart(X_POS, shield_pos[shields[i]].x(), constants->shieldReconfigureTime);
        shields[i]->AnimationStart(Y_POS, shield_pos[shields[i]].y(), constants->shieldReconfigureTime);
    }
}


// ИГРОВЫЕ СОБЫТИЯ-------------------------------------------------------------------------------

int Hex::createPoint(POSITION_STATE state, WAY way)
{
    int id = 0;
    while (points.contains(id)) { ++id; }

    // TODO в принципе, эти три значения можно и в структуру как-то оформить
    ids << id;
    pointPositionState[id] = state;
    pointsWay[id] = way;

    recountPoints();
    reconfigureOrders();

    // дальнейшие работы с точкой идут через id
    return id;
}
void Hex::removePoint(int id)
{
    ids.remove(id);
    points.remove(id);
    pointPositionState.remove(id);
    pointsWay.remove(id);

    recountPoints();
    reconfigureOrders();
}

void Hex::setNewResourcesPosition(QList<OrderPic *> resources)
{
    // делаем из переданных объектов "копию" таблички ресурсов
    // для анимации сбора ресурсов
    QList <QPointF> coord = countTableCoordinates(width(), height(), resources.size());
    for (int i = 0; i < resources.size(); ++i)
    {
        resources[i]->setPos(mapToItem(resources[i]->parentItem(), coord[i]));
        resources[i]->resize(constants->resourceTablePicSize * width(),
                                         constants->resourceTablePicSize * width());
    }
}

// Переключения между состояниями
void Hex::setState(QString state, bool isImmediate)
{
    setPictureState(state, isImmediate);
    if (state != "selected")
    {
        if (Lighting != NULL)
        {
            Lighting->Delete();
            Lighting = NULL;
        }
    }
    else
    {
        if (Lighting == NULL)
        {
            Lighting = new SpriteObject(this, 0, "MillSelection");
            Lighting->setZValue(constants->HexSelectionZpos);
            Lighting->setGeometry(0, 0, width(), height());
        }
    }
}
void Hex::select(bool enable, bool immediate)
{
    if (enable)
    {
        setState("selected", immediate);
    }
    else
    {
        setState("lighted", immediate);
    }
}
void Hex::light(bool enable, bool immediate)
{
    if (enable)
        setState("lighted", immediate);
    else
    {
        setState(prototype->color, immediate);
    }
}
void Hex::forbidToSelect(bool forbid)
{
    if (forbid)
    {
        setFrame("ForbiddenSelectionHexFrame");
        setProperties(HOVER | RIGHT_CLICKABLE);
    }
    else
    {
        setFrame(prototype->type + "HexFrame");
        setProperties(CLICKABLE | HOVER | RIGHT_CLICKABLE);
    }
}
bool Hex::is_clickable()
{
    return (this->flags & CLICKABLE) != 0;
}

void Hex::planCapturing(PlayerColor color)
{
    if (!plannedCapturing.contains(color))
    {
        plannedCapturing[color] = new MergingObject(this, color + "Hex");
        plannedCapturing[color]->setZValue(-0.1);
        plannedCapturing[color]->setGeometry(0, 0, width(), height());
    }
}
void Hex::deplanCapturing(PlayerColor color)
{
    if (plannedCapturing.contains(color))
    {
        plannedCapturing[color]->DeleteOnMerging();
        plannedCapturing.remove(color);
    }
}

void Hex::addOrder(Order *order)
{
    order->anchorTo(this);
    orders[order->anchor_point] << order;
    reconfigureOrders();
}
void Hex::removeOrder(Order *order)
{
    order->deanchorFrom(this);
    orders[order->anchor_point].removeAll(order);
    reconfigureOrders();
}

void Hex::showUnitHome(PlayerColor color)
{
    UnitHomePicture->setPixmap(images->get(color + "UnitHome"));
    UnitHomePicture->setVisible(true);
}
void Hex::hideUnitHome()
{
    UnitHomePicture->setVisible(false);
}

void Hex::showInformation(QString pic_name, QString name, bool merge)
{
    if (information != NULL)
    {
        debug << "PANIC! information on information!\n";

        // может, лучше плавно заставить её исчезнуть?
        // вроде эта "ошибка" встречается по разумным причинам!
        information->Delete();
    }

    information = new Information(this, pic_name, name);
    resizeChildren(width(), height());

    if (!merge)
        information->StopMerging();
}
void Hex::hideInformation()
{
    if (information != NULL)
    {
        information->DeleteOnMerging();
        information = NULL;
    }
}

// подсветка определённых ресурсов
void Hex::highlight(OrderType type, bool light)
{
    if (light)
    {
        // составляем список остающихся ресурсов
        QList <ResourcePic *> to_shift;
        foreach (ResourcePic * R, table)
        {
            if (R->R == type)
                to_shift.push_back(R);
            else  // остальные исчезают
                R->AnimationStart(OPACITY, 0, constants->hexResourcesShiftTime);
        }

        if (to_shift.size() == 0)  // если искомых ресурсов нет, исчезает весь гекс
        {
            AnimationStart(OPACITY, constants->hexOpaqueState, constants->hexResourcesShiftTime);
        }

        // рассчитываем таблицу для случая нового числа ресурсов и анимируем
        QList <QPointF> table_pos = countTableCoordinates(width(), height(), to_shift.size());
        for (int i = 0; i < to_shift.size(); ++i)
        {
            to_shift[i]->AnimationStart(X_POS, table_pos[i].x(), constants->hexResourcesShiftTime);
            to_shift[i]->AnimationStart(Y_POS, table_pos[i].y(), constants->hexResourcesShiftTime);
        }
    }
    else
    {
        // делаем все ресурсы видимыми и двигаем к исходной позиции

        QList <QPointF> table_pos = countTableCoordinates(width(), height(), table.size());
        for (int i = 0; i < table.size(); ++i)
        {
            table[i]->AnimationStart(X_POS, table_pos[i].x(), constants->hexResourcesShiftTime);
            table[i]->AnimationStart(Y_POS, table_pos[i].y(), constants->hexResourcesShiftTime);
            table[i]->AnimationStart(OPACITY, 1, constants->hexResourcesShiftTime);
        }

        AnimationStart(OPACITY, 1, constants->hexResourcesShiftTime);
    }
}

// появление оборонительного бонуса
void Hex::defenceAppear(int amount, QString color)
{
    for (int i = 0; i < amount; ++i)
        shields << new Shield(this, color);

    recountShieldsPosition();

    for (int i = 0; i < amount; ++i)
    {
        shields[i]->setGeometry(shield_pos[shields[i]].x() + width() * constants->hexShieldsWidth / 2,
                                                 shield_pos[shields[i]].y() + height() * constants->hexShieldsHeight / 2,
                                                 0, 0);

        shields[i]->AnimationStart(WIDTH, width() * constants->hexShieldsWidth, constants->shieldReconfigureTime);
        shields[i]->AnimationStart(HEIGHT, height() * constants->hexShieldsHeight, constants->shieldReconfigureTime);
    }

    reconfigureShields();
}
void Hex::defenceDisappear()
{
    for (int i = 0; i < shields.size(); ++i)
        shields[i]->disappear(constants->shieldReconfigureTime);
    shields.clear();
}
// turn означает анимацию использования оборонительного бонуса
// turn on=true, соответственно, означает его восполнение
void Hex::defenceTurn(int amount, bool on)
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
