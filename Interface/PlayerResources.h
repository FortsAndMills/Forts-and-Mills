#ifndef PLAYER_RESOURCES
#define PLAYER_RESOURCES

#include <Game/GamePlayer.h>
#include <GameElements/Unit.h>
#include <GameElements/Order.h>
#include <Game/GameRules.h>
#include <Technical/Pictures.h>
#include <Game/GameUnit.h>
#include <Technical/Constants.h>

class PlayerResources : public GraphicObject
{
    Q_OBJECT

    QMap <Resource, QList <OrderPic *> > table;
public:
    Game * game;
    GamePlayer * prototype;

    int isRight;  // +1 если справа, -1 если слева

    PlayerResources(Game * game, GraphicObject * parent, GamePlayer * prototype, bool isMain) :
        GraphicObject(parent, 0, (isMain ? "Left" : "Right") + prototype->color + "PlayerPanel")
    {
        this->game = game;
        this->prototype = prototype;
        this->isRight = isMain ? -1 : 1;
    }
    void Delete()
    {
        foreach (QList <OrderPic *> list, table)
        {
            foreach (OrderPic * op, list)
                op->Delete();
        }

        GraphicObject::Delete();
    }

private:
    QMap <Resource, QList <QPointF> > tableCoord;
    QMap <Resource, QList <qreal> > tableSize;
    qreal ResourceTableHeight;
    qreal BiggestOrderSize;
    void recountTableCoordinates(qreal W, qreal H)
    {
        qreal X, Xmargin = constants->resourcesTableMarginX * W;
        qreal Y = constants->resourcesTableMarginY * H;

        foreach (Resource R, game->rules->ordersInGame)
        {
            tableCoord[R].clear();
            tableSize[R].clear();

            if (table[R].size() != 0)
            {
                int in_row = qMax(table[R].size(), constants->minOrdersInRow);
                qreal size = (W - 2 * Xmargin) / (in_row + (in_row + 1) * constants->resourcesTableInsideMargin);

                qreal margin = (W - 2 * Xmargin - size * table[R].size()) / (table[R].size() + 1);

                X = Xmargin + margin;
                for (int i = 0; i < table[R].size(); ++i)
                {
                    tableCoord[R] << mapToItem(table[R][i]->parentItem(), X, Y);
                    tableSize[R] << size;

                    X += margin + size;
                }
                Y += constants->resourcesTableInsideMargin * size + size;
            }
        }

        ResourceTableHeight = Y;
        BiggestOrderSize = W * (1 - 3 * constants->resourcesTableInsideMargin) / 2;
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
        int i = (isRight == 1 ? 0 : (table[type].size() - 1));
        if (turn)  // ищем ближайший к краю ресурс указанного вида
        {              // для которого можно сделать turn
            i -= isRight;
            while (i + isRight >= 0 && i + isRight < table[type].size() &&
                   table[type][i + isRight]->isTurnedOn != true)
            {
                i += isRight;
            }

            if (i < 0 || i >= table[type].size())
                debug << "turning on resource gone wrong!\n";
            else
                table[type][i]->turnOn();
        }
        else
        {
            while (i >= 0 && i < table[type].size() &&
                   table[type][i]->isTurnedOn == false) { i += isRight; }

            if (i >= 0 && i < table[type].size())
                table[type][i]->turnOn(false);
        }
    }
    void lightResource(Resource type, bool turn)
    {
        int i = (isRight == 1 ? 0 : (table[type].size() - 1));
        if (turn)
        {
            while (i >= 0 && i < table[type].size() &&
                   (table[type][i]->isLighten || !table[type][i]->isTurnedOn))
            {
                i += isRight;
            }

            if (i < 0 || i >= table[type].size())
                debug << "lighting on resource gone wrong!\n";
            else
                table[type][i]->light();
        }
        else
        {
            while (i >= 0 && i < table[type].size() &&
                   table[type][i]->isLighten == false) { i += isRight; }

            if (i >= 0 && i < table[type].size())
                table[type][i]->light(false);
        }
    }
    void turnOffLightenResource(Resource type)
    {
        int i = (isRight == 1 ? 0 : (table[type].size() - 1));
        while (i >= 0 && i < table[type].size() &&
                   !table[type][i]->isLighten)
        {
            i += isRight;
        }

        if (i >= 0 && i < table[type].size())
        {
            table[type][i]->light(false);
            table[type][i]->turnOn(false);
        }
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
        while (i >= 0 && i < table[type].size() &&
                !table[type][i]->isTurnedOn)
        {
            ++i;
        }

        if (i >= 0 && i < table[type].size())
        {
            table[type][i]->burn();
            table[type].removeAt(i);
        }
    }
    void burnLighten(Resource type)
    {
        int i = 0;
        while (i >= 0 && i < table[type].size() &&
            (!table[type][i]->isTurnedOn || !table[type][i]->isLighten))
        {
            ++i;
        }

        if (i >= 0 && i < table[type].size())
        {
            table[type][i]->burn();
            table[type].removeAt(i);
        }
    }
    void reconfigureResources()
    {
        recountTableCoordinates(width(), height());

        for(QMap <Resource, QList <OrderPic *> >::iterator it = table.begin(); it != table.end(); ++it)
        {
            Resource R = it.key();
            for (int i = 0; i < table[R].size(); ++i)
            {
                table[R][i]->AnimationStart(X_POS,  tableCoord[R][i].x(), constants->orderReconfigureTime);
                table[R][i]->AnimationStart(Y_POS, tableCoord[R][i].y(), constants->orderReconfigureTime);
                table[R][i]->AnimationStart(WIDTH, tableSize[R][i], constants->orderReconfigureTime);
                table[R][i]->AnimationStart(HEIGHT, tableSize[R][i], constants->orderReconfigureTime);
            }
        }

        // разборки с сожжением лишних прилетающих ресурсов
        // первый ресурс вида R сжигается через константу
        // для каждого следующего задержка увеличивается
        QMap <Resource, int> time;
        foreach (Resource R, game->rules->ordersInGame)
            time[R] = constants->orderReconfigureTime;

        foreach (OrderPic * burningOrder, to_burn)
        {
            Resource R = burningOrder->type;

            qreal size;  // если такого вида ресурсов вообще нет (это бывает, когда игрок потерял все мельницы)
            if (table[R].size() == 0)  // то летит в верхний угол панели
            {
                burningOrder->AnimationStart(Y_POS, y() + ResourceTableHeight, time[R]);
                size = BiggestOrderSize;
            }
            else  // иначе в ряд, соотв. данному виду ресурсов
            {
                burningOrder->AnimationStart(Y_POS, tableCoord[R][0].y(), time[R]);
                size = tableSize[R][0];
            }
            burningOrder->AnimationStart(WIDTH, size, time[R]);
            burningOrder->AnimationStart(HEIGHT, size, time[R]);

            // ищем координату по x в зависимости от того, правая панель или левая
            if (burningOrder->x() > x() + width())
                burningOrder->AnimationStart(X_POS,   x() + width(), time[R]);
            else
                burningOrder->AnimationStart(X_POS,   x() - size, time[R]);

            time[R] += constants->burningOrdersDelay;

            connect(burningOrder, SIGNAL(movieFinished()), burningOrder, SLOT(burn()));
        }

        // забываем про всё, что сгорит
        to_burn.clear();
    }
};

#endif
