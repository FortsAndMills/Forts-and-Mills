#ifndef ORDER_H
#define ORDER_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "BasicElements/SpriteObject.h"
#include "BasicElements/StateObject.h"
#include "Game/Game.h"
#include "Game/GameOrder.h"

class Order : public GraphicObject  // отображается на поле
{
    Q_OBJECT

public:
    GameOrder * prototype;

    int anchor_x, anchor_y, anchor_point;

    explicit Order(GraphicObject * parent, GameOrder * prototype) :
        GraphicObject(parent, DRAGABLE | RIGHT_CLICKABLE, prototype->type)
    {
        this->prototype = prototype;
        this->setZValue(constants->orderZPos);
    }

public:
    void rightClick()
    {
        emit rightClicked(this->prototype);
    }
    void disappear()
    {
        this->setEnabled(false);
        this->AnimationStart(WIDTH, 0, constants->orderDisappearTime);
        this->AnimationStart(HEIGHT, 0, constants->orderDisappearTime);
        this->AnimationStart(X_POS,   x() + width() / 2, constants->orderDisappearTime);
        this->AnimationStart(Y_POS, y() + height() / 2, constants->orderDisappearTime);
        connect(this, SIGNAL(movieFinished()), this, SLOT(Delete()));
    }

signals:
    void rightClicked(GameOrder *);
};

class ResourcePic : public GraphicObject  // отображается на гексах
{
public:
    explicit ResourcePic(GraphicObject * parent, Resource R) :
        GraphicObject(parent, 0, R) {}
};

class OrderPic : public GraphicObject  // отображается в таблице ресурсов
{
    Q_OBJECT

public:
    OrderType type;
    PlayerColor owner;

    bool isTurnedOn = true;

    explicit OrderPic(GraphicObject * parent, Game * game, PlayerColor owner, OrderType R) :
        GraphicObject(parent, CLICKABLE, R)
    {
        this->type = R;
        this->owner = owner;
        connect(this, SIGNAL(orderPicPressed(QString, QString, bool)), game, SLOT(orderPicPushed(QString, QString, bool)));
    }
    virtual void Delete()
    {
        if (burning != NULL)
            burning->Delete();
        GraphicObject::Delete();
    }

private:
    SpriteObject * burning = NULL;

public slots:
    void turnOn(bool on = true)
    {
        isTurnedOn = on;
        if (on)
            this->AnimationStart(OPACITY, 1, constants->ordersTurnTime);
        else
            this->AnimationStart(OPACITY,
                                 constants->orderTurnedOffOpacity,
                                 constants->ordersTurnTime);
    }
    void disappear()
    {
        this->setEnabled(false);
        this->AnimationStart(WIDTH, 0, constants->orderDisappearTime);
        this->AnimationStart(HEIGHT, 0, constants->orderDisappearTime);
        this->AnimationStart(X_POS,   x() + width() / 2, constants->orderDisappearTime);
        this->AnimationStart(Y_POS, y() + height() / 2, constants->orderDisappearTime);
        connect(this, SIGNAL(movieFinished()), SLOT(Delete()));
    }
    void burn()
    {
        if (burning == NULL)
        {
            this->setEnabled(false);
            this->AnimationStart(WIDTH, 0, constants->orderDisappearTime);
            this->AnimationStart(HEIGHT, 0, constants->orderDisappearTime);
            this->AnimationStart(X_POS,   x() + width() * constants->orderDisappearPointX,
                                 constants->orderDisappearTime);
            this->AnimationStart(Y_POS, y() + height() * constants->orderDisappearPointY,
                                 constants->orderDisappearTime);

            burning = new SpriteObject(dynamic_cast<GraphicObject *>(this->parentItem()), 0, "Burn", constants->orderBurnTime);  // TODO чуть-чуть рандома
            burning->setGeometry(x() + width() * constants->orderDisappearBurnPointX,
                                                    y() + height() * constants->orderDisappearBurnPointY,
                                                    width() * constants->orderBurnWidth,
                                                    height() * constants->orderBurnHeight);
            connect(burning, SIGNAL(looped()), SLOT(Delete()));
        }
    }

    void leftClick()
    {
        emit orderPicPressed(type, owner, isTurnedOn);
    }
signals:
    void orderPicPressed(OrderType, PlayerColor, bool);
};

class SpecialButton : public StateObject  // отображается в таблице вариантов
{
    Q_OBJECT

public:
    QString name;

    explicit SpecialButton(GraphicObject * parent, QString name, bool used = true) :
        StateObject(parent, "default", CLICKABLE * used, name, "", "SimpleLayer")
    {
        this->name = name;

        geometries["entered"] = QRectF(constants->specialButtonShiftX,
                                                               constants->specialButtonShiftY,
                                                               constants->specialButtonWidthShift,
                                                               constants->specialButtonHeightShift);
    }

    void enter()
    {
        setState("entered");
    }
    void leave()
    {
        setState("default");
    }

    void leftClick()
    {
        emit leftClicked(name);
    }
signals:
    void leftClicked(QString);
};

#endif
