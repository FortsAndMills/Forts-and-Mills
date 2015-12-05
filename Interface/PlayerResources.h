#include <Game/GamePlayer.h>
#include <GameElements/Unit.h>
#include <GameElements/Order.h>
#include <Game/GameRules.h>
#include <Game/Game.h>
#include <Technical/Pictures.h>
#include <Game/GameUnit.h>
#include <Technical/Constants.h>

class PlayerResources : public GraphicObject
{
    Q_OBJECT

    QGridLayout * layout;
    QMap <Resource, QList <OrderPic *> > table;

    Game * game;
    GamePlayer * prototype;

public:
    PlayerResources(Game * game, GraphicObject * parent, GamePlayer * prototype) :
        GraphicObject(parent, 0, prototype->color + "PlayerPanel")
    {
        this->game = game;
        this->prototype = prototype;
    }

private:
    QMap <Resource, QList <QPointF> > tableCoord;
    QMap <Resource, QList <qreal> > tableSize;
    qreal ResourceTableHeight;
    qreal BiggestOrderSize;
    void recountTableCoordinates(qreal W, qreal)
    {
        qreal X = constants->resourcesTableMargin * W;
        qreal Y = constants->resourcesTableMargin * W;

        foreach (Resource R, game->rules->ordersInGame)
        {
            tableCoord[R].clear();
            tableSize[R].clear();

            if (table[R].size() != 0)
            {
                int in_row = qMax(table[R].size(), constants->minOrdersInRow);
                qreal size = W / (in_row + (in_row + 1) * constants->resourcesTableMargin);

                qreal margin = (W - size * table[R].size()) / (table[R].size() + 1);

                X = margin;
                for (int i = 0; i < table[R].size(); ++i)
                {
                    tableCoord[R] << mapToItem(table[R][i]->parentItem(), X, Y);
                    tableSize[R] << size;

                    X += margin + size;
                }
                Y += constants->resourcesTableMargin * size + size;
            }
        }

        ResourceTableHeight = Y;
        BiggestOrderSize = W * (1 - 3 * constants->resourcesTableMargin) / 2;
    }
    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);

        recountTableCoordinates(W, H);
        for(QMap <Resource, QList <OrderPic *> >::iterator it = table.begin(); it != table.end(); ++it)
        {
            Resource R = it.key();
            for (int i = 0; i < table[R].size(); ++i)
            {
                table[R][i]->setPos(tableCoord[R][i]);
                table[R][i]->resize(tableSize[R][i], tableSize[R][i]);
            }
        }
    }
public:
    void showUndefinedUnits(QMap <GameUnit *, Unit *> all)
    {
        QList <Unit *> list;
        for (int i = 0; i < prototype->units.size(); ++i)
            if (prototype->units[i]->x == UNDEFINED)
                list << all[prototype->units[i]];

        if (list.isEmpty())
            return;

        qreal X = this->width() / 2 - constants->unitsSize / 2;
        qreal shift = (this->height() - list.size() * constants->unitsSize) / (list.size() + 1);
        qreal Y = shift;

        foreach (Unit * unit, list)
        {
            unit->setGeometry(this->x() + X, this->y() + Y,
                              constants->unitsSize, constants->unitsSize);

            Y += constants->unitsSize + shift;
        }
    }

private:
    QList <OrderPic *> to_burn;
public:
    void animateNewResources(QList<OrderPic *> resources, QList<bool> burn)
    {
        for (int i = 0; i < resources.size(); ++i)
        {
            if (burn[i])  // если сжигаем - то планируем сожжение
            {
                to_burn << resources[i];
            }
            else  // иначе просто добавляем в наш список
                table[resources[i]->type] << resources[i];
        }        
    }
    void turnResource(Resource type, bool turn)
    {
        int i = 0;
        while (table[type][i]->isTurnedOn == turn) { ++i; }
        table[type][i]->turnOn(turn);
    }
    void disappearAllTurnedOffOrders()
    {
        for(QMap <Resource, QList <OrderPic *> >::iterator it = table.begin(); it != table.end(); ++it)
        {
            Resource R = it.key();

            QList <OrderPic *> orders = table[R];
            foreach (OrderPic * order, orders)
            {
                if (!order->isTurnedOn)
                {
                    order->disappear();
                    table[R].removeAll(order);
                }
            }
        }
    }
    void burn(Resource type)
    {
        int i = 0;
        while (!table[type][i]->isTurnedOn) { ++i; }
        table[type][i]->burn();
        table[type].removeAt(i);
    }
    void reconfigureResources()
    {
        recountTableCoordinates(width(), height());

        for(QMap <Resource, QList <OrderPic *> >::iterator it = table.begin(); it != table.end(); ++it)
        {
            Resource R = it.key();
            for (int i = 0; i < table[R].size(); ++i)
            {
                table[R][i]->AnimationStart(X_POS,   tableCoord[R][i].x(), constants->orderReconfigureTime, true);
                table[R][i]->AnimationStart(Y_POS, tableCoord[R][i].y(), constants->orderReconfigureTime, true);
                table[R][i]->AnimationStart(WIDTH, tableSize[R][i], constants->orderReconfigureTime, true);
                table[R][i]->AnimationStart(HEIGHT, tableSize[R][i], constants->orderReconfigureTime, true);
            }
        }

        QMap <Resource, int> time;
        foreach (Resource R, game->rules->ordersInGame)
            time[R] = constants->orderReconfigureTime;

        foreach (OrderPic * burningOrder, to_burn)
        {
            Resource R = burningOrder->type;

            qreal size;
            if (table[R].size() == 0)
            {
                burningOrder->AnimationStart(Y_POS, y() + ResourceTableHeight, time[R], true);
                size = BiggestOrderSize;
            }
            else
            {
                burningOrder->AnimationStart(Y_POS, tableCoord[R][0].y(), time[R], true);
                size = tableSize[R][0];
            }
            burningOrder->AnimationStart(WIDTH, size, time[R], true);
            burningOrder->AnimationStart(HEIGHT, size, time[R], true);

            if (burningOrder->x() > x() + width())
                burningOrder->AnimationStart(X_POS,   x() + width(), time[R], true);
            else
                burningOrder->AnimationStart(X_POS,   x() - size, time[R], true);

            time[R] += constants->burningOrdersDelay;

            connect(burningOrder, SIGNAL(movieFinished()), burningOrder, SLOT(burn()));
        }
        to_burn.clear();
    }
};
