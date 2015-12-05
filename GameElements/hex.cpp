#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Game/GameUnit.h"
#include "BasicElements/SpriteObject.h"
#include "GameElements/Order.h"
#include "Game/GameHex.h"
#include "Unit.h"
#include "Hex.h"

Hex::Hex(GraphicObject *parent, Game *game, GameHex *prototype) :
    StateObject(parent, "Neutral", CLICKABLE, prototype->type + "Hex", prototype->type + "HexFrame", "SimpleLayer")
{
    addState("Blue", "BlueHex", CLICKABLE, prototype->type + "HexFrame");
    addState("Red", "RedHex", CLICKABLE, prototype->type + "HexFrame");
    addState("selected", "SelectedHex", CLICKABLE, prototype->type + "HexFrame");
    addState("lighted", "LightedHex", CLICKABLE, prototype->type + "HexFrame");
    frames["forbidden"] = "ForbiddenSelectionHexFrame";
    properties["forbidden"] = 0;

    this->prototype = prototype;
    this->game = game;

    Lighting = NULL;

    if (prototype->type != "Simple")
        HexPicture = new GraphicObject(this, 0, prototype->type);
    else
        HexPicture = NULL;

    if (prototype->canHaveResources)
    {
        foreach (OrderType R, prototype->resources)
        {
            table.append(new ResourcePic(this, R));
        }
    }

    UnitHomePicture = new Object(this, "");
    UnitHomePicture->setVisible(false);
}
Hex::Hex(Hex *another) : Hex(dynamic_cast<GraphicObject *>(another->parentItem()), another->game, another->prototype)
{
    this->setX(another->x());
    this->setY(another->y());
    this->resize(another->width(), another->height());
    this->setState(another->cur_picture, true);

    this->setEnabled(false);
}
void Hex::Delete()
{
    UnitHomePicture->Delete();
    if (Lighting != NULL)
        Lighting->Delete();
    if (HexPicture != NULL)
        HexPicture->Delete();
    foreach (ResourcePic * pic, table)
        pic->Delete();

    StateObject::Delete();
}

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
            qDebug() << "ERROR: table of resources overflow";

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
void Hex::recountPoints()
{
    WAY where_entered = "";
    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)  // TODO алгоритм вычисления точек
    {
        int i = (*it);
        if (pointPositionState[i] == ENTERING)
        {
            points[i] = EnterPoint[pointsWay[i]];
            where_entered = pointsWay[i];
        }
    }

    if (where_entered == "" && ids.size() > 1)
    {
        for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
        {
            int i = (*it);
            if (pointPositionState[i] == STAY && pointsWay[i] != "")
            {
                where_entered = pointsWay[i];
            }
        }
    }

    for (QSet<int>::iterator it = ids.begin(); it != ids.end(); ++it)
    {
        int i = (*it);
        if (pointPositionState[i] == STAY)
        {
            if (where_entered == "")
            {
                points[i] = QPointF(width() / 2, height() / 2);
            }
            else if (pointsWay[i] == "")
            {
                points[i] = EnterPoint[game->oppositeWay(where_entered)];
            }
            else
            {
                points[i] = EnterPoint[where_entered];
            }
        }
    }
}
void Hex::recountOrdersPosition()
{
    for (int i = 0; i < orders.size(); ++i)
    {
        QPointF center = points[i];
        for (int j = 0; j < orders[i].size(); ++j)
        {
            orders_pos[orders[i][j]] = (QPointF)mapToItem(orders[i][j]->parentItem(),
                        center.x() - constants->unitOrderWidth * constants->unitsSize / 2,
                        center.y() - constants->unitOrderHeight * constants->unitsSize / 2);

            center += QPointF(constants->orderStackShiftX * constants->unitOrderWidth * constants->unitsSize,  // <undone>
                                           constants->orderStackShiftX * constants->unitOrderWidth * constants->unitsSize);
        }
    }
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

    QList <QPointF> table_pos = countTableCoordinates(W, H, table.size());
    for (int i = 0; i < table.size(); ++i)
    {
        table[i]->setPos(table_pos[i]);
        table[i]->resize(constants->resourceTablePicSize * W,
                                  constants->resourceTablePicSize * W);
    }

    EnterPoint["UP"] = QPointF(W / 2, 3 * H / 4);  // <undone>
    EnterPoint["DOWN"] = QPointF(W / 2, H / 4);
    EnterPoint["LEFT_DOWN"] = QPointF(W * (3 - constants->hexShift) / 4, 3 * H / 8);
    EnterPoint["LEFT_UP"] = QPointF(W * (3 - constants->hexShift) / 4, 5 * H / 8);
    EnterPoint["RIGHT_DOWN"] = QPointF(W * (1 + constants->hexShift) / 4, 3 * H / 8);
    EnterPoint["RIGHT_UP"] = QPointF(W * (1 + constants->hexShift) / 4, 5 * H / 8);

    recountPoints();
    resizeOrders();
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

void Hex::reconfigureOrders()
{
    recountOrdersPosition();
    for (int i = 0; i < orders.size(); ++i)
    {
        for (int j = 0; j < orders[i].size(); ++j)
        {
            orders[i][j]->AnimationStart(X_POS, orders_pos[orders[i][j]].x(), constants->orderReconfigureTime);
            orders[i][j]->AnimationStart(Y_POS, orders_pos[orders[i][j]].y(), constants->orderReconfigureTime);
        }
    }
}


// ИГРОВЫЕ СОБЫТИЯ-------------------------------------------------------------------------------

int Hex::createPoint(POSITION_STATE state, WAY way)
{
    int id = 0;
    while (points.contains(id)) { ++id; }

    ids << id;
    pointPositionState[id] = state;
    pointsWay[id] = way;

    recountPoints();
    reconfigureOrders();

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
    QList <QPointF> coord = countTableCoordinates(width(), height(), resources.size());
    for (int i = 0; i < resources.size(); ++i)
    {
        resources[i]->setPos(mapToItem(resources[i]->parentItem(), coord[i]));
        resources[i]->resize(constants->resourceTablePicSize * width(),
                                         constants->resourceTablePicSize * width());
    }

    if (prototype->resources.size() < resources.size())  // сбрасываем до нуля
    {
        foreach (OrderPic * order, resources)
        {
            order->moveBy(order->width() / 2, order->height() / 2);
            order->resize(0, 0);
        }
    }
}

void Hex::setState(QString state, bool isImmediate)
{    
    StateObject::setState(state, isImmediate);

    if (state == "forbidden")
        return;

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
            resizeChildren(width(), height());
        }
    }
}
void Hex::select(bool enable)
{
    if (enable)
    {
        setState("selected");
    }
    else
    {
        setState("lighted");
    }
}
void Hex::light(bool enable)
{
    if (enable)
        setState("lighted");
    else
    {
        setState(prototype->color);
    }
}
void Hex::forbidToSelect(bool forbid)
{
    if (forbid)
    {
        setState("forbidden");
    }
    else
    {
        setState(cur_picture);
    }
}
void Hex::changeColor()
{
    this->setState(prototype->color);
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
    UnitHomePicture->setPixmap(pictures->get(color + "UnitHome"));
    UnitHomePicture->setVisible(true);
}
void Hex::hideUnitHome()
{
    UnitHomePicture->setVisible(false);
}
