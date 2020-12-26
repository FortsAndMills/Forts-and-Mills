#ifndef ORDER_H
#define ORDER_H

#include "Technical/Headers.h"
#include "Technical/HelpManager.h"
#include "Technical/Constants.h"
#include "BasicElements/SpriteObject.h"
#include "BasicElements/StateObject.h"
#include "GameExecution/Game.h"

// Укрепление (щиты)
class Fortification : public GraphicObject
{
    Q_OBJECT

public:
    int point;

public:
    explicit Fortification(GraphicObject * parent, PlayerColor color, int point) :
        GraphicObject(parent, RIGHT_CLICKABLE, color + "Fortification")
    {
        this->point = point;
        this->setZValue(constants->fortificationZPos);
    }

    void rightClick() { emit help->HelpAsked("Fortification"); }
};

// отображается на поле возле юнитов при планировании
class Order : public GraphicObject
{
    Q_OBJECT

public:
    GameOrder * prototype;

    Coord anchor;
    int anchor_point;

    explicit Order(GraphicObject * parent, GameOrder * prototype) :
        GraphicObject(parent, RIGHT_CLICKABLE, prototype->type)
    {
        this->prototype = prototype;
        this->setZValue(constants->orderZPos);
    }

    void disappear()
    {
        GraphicObject::disappear(constants->orderDisappearTime);
    }

    void rightClick()
    {
        emit help->HelpAsked(this->prototype->type);
    }

signals:
    void rightClicked(GameOrder *);
};

// отображается на поле при реализации
// анимация открытия реализуется в спрайтах
class MainOrder : public SpriteObject
{
    Q_OBJECT

public:
    bool opened = false;

    OrderType type;

    explicit MainOrder(GraphicObject * parent, OrderType type, bool opened) :
        SpriteObject(parent, RIGHT_CLICKABLE, type + "Turn", constants->mainOrderOpenTime, true, false)
    {
        this->opened = opened;
        if (opened)
            setFrame(frames() - 1);

        this->type = type;
        this->setZValue(constants->orderZPos);
    }
    void Delete()
    {
        if (burning != NULL)
            burning->Delete();
        SpriteObject::Delete();
    }

    void rightClick()
    {
        emit help->HelpAsked(opened ? type : "Unknown");
    }

private:
    SpriteObject * burning = NULL;
public:
    void open()
    {
        opened = true;
        start();

        RealWidth = width();
        RealHeight = height();
        this->AnimationStart(WIDTH, width() * constants->mainOrderTurnCoeff, constants->mainOrderOpenTime / 2);
        this->AnimationStart(HEIGHT, height() * constants->mainOrderTurnCoeff, constants->mainOrderOpenTime / 2);

        QTimer::singleShot(constants->mainOrderOpenTime / 2, this, SLOT(finishOpening()));
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
    void light()
    {
        SpriteObject * Lighting = new SpriteObject(this, 0, "OrdersFrameRed");
        Lighting->setGeometry(0, 0, width(), height());
        connect(Lighting, SIGNAL(looped()), Lighting, SLOT(Delete()));
    }
private:
    qreal RealWidth, RealHeight;
private slots:
    void finishOpening()
    {
        this->AnimationStart(WIDTH, RealWidth, constants->mainOrderOpenTime / 2);
        this->AnimationStart(HEIGHT, RealHeight, constants->mainOrderOpenTime / 2);
    }
};

// отображается на гексах
class ResourcePic : public GraphicObject
{
public:
    Resource R;

    explicit ResourcePic(GraphicObject * parent, Resource R) :
        GraphicObject(parent, RIGHT_CLICKABLE, R)
    {
        this->R = R;
    }

    void rightClick()
    {
        emit help->HelpAsked(R);
    }
};

// отображается в таблице ресурсов
class OrderPic : public GraphicObject
{
    Q_OBJECT

public:
    OrderType type;
    PlayerColor owner;

    bool isTurnedOn = true;
    bool isLighten = false;

    SpriteObject * Lighting;

    explicit OrderPic(GraphicObject * parent, PlayerColor owner, OrderType R) :
        GraphicObject(parent, HOVER | RIGHT_CLICKABLE, R)
    {
        this->type = R;
        this->owner = owner;

        Lighting = NULL;
    }
    virtual void Delete()
    {
        if (isDeleted)
            return;

        if (burning != NULL)
            burning->Delete();
        if (Lighting != NULL)
            Lighting->Delete();
        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        if (Lighting != NULL)
            Lighting->setGeometry(0, 0, W, H);
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
    void light(bool on = true)
    {
        isLighten = on;
        if (on)
        {
            if (Lighting == NULL)
            {
                Lighting = new SpriteObject(this, 0, "OrdersFrame");
                resize(width(), height());
            }
        }
        else
        {
            connect(Lighting, SIGNAL(looped()), Lighting, SLOT(Delete()));
            Lighting = NULL;
        }
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

    void disappear()
    {
        GraphicObject::disappear(constants->orderDisappearTime);
    }

    void enter() { emit orderPicEntered(type, owner);}
    void leave() { emit orderPicLeft(type, owner);}
    void rightClick() { emit help->HelpAsked(type); }
signals:
    void orderPicLeft(OrderType, PlayerColor);
    void orderPicEntered(OrderType, PlayerColor);
};

// отображается в таблице вариантов
class OrderVariant : public StateObject
{
    Q_OBJECT

public:
    QString name;

    explicit OrderVariant(GraphicObject * parent, QString name) :
        StateObject(parent, "default", name == DeleteLastOrder ? "RedCancelButton" : name, HOVER | CLICKABLE | RIGHT_CLICKABLE)
    {
        this->name = name;

        addGeometry("entered", QRectF(constants->specialButtonShiftX,
                                      constants->specialButtonShiftY,
                                      constants->specialButtonWidthShift,
                                      constants->specialButtonHeightShift));
    }

    void enter()
    {
        setGeometryState("entered");
    }
    void leave()
    {
        setGeometryState("default");
    }

    void rightClick()
    {
        emit help->HelpAsked(name);
    }
    void leftClick()
    {
        emit leftClicked(name);
    }
signals:
    void leftClicked(QString);
};

#endif
