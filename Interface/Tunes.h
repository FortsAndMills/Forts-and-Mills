#ifndef TUNES_H
#define TUNES_H

#include "Technical/Headers.h"
#include "Technical/HelpManager.h"
#include "Technical/ProgrammSettings.h"
#include "Game/GameRules.h"
#include "BasicElements/StateObject.h"
#include "BasicElements/DigitObject.h"
#include "GameElements/Unit.h"
#include "DayTimePanel.h"

// Мини-картинки игровых элементов в панельках доступных игр-комнат
// и в меню создания игры, где они обычно интерактивны
class PlayersElement : public StateObject
{
    Q_OBJECT

public:
    bool isOn = true;
    bool active;
    int i;

    PlayersElement(GraphicObject * parent, int i, bool isOn, bool active) :
        StateObject(parent, "on", "PlayerOnline", (active * CLICKABLE) | RIGHT_CLICKABLE, "", "SimpleLayer")
    {
        this->i = i;
        this->active = active;
        addPicture("off", "PlayerOffline");
        turnOn(isOn);
    }

    void leftClick() { emit whenClicked(i); }
    void rightClick() { emit help->HelpAsked(active ? "PlayersTune" : "NumberOfPlayers"); }
    void turnOn(bool on)
    {
        isOn = on;
        setPictureState(isOn ? "on" : "off");
    }

signals:
    void whenClicked(int i);
};
class PlayersTune : public GraphicObject
{
    Q_OBJECT

    Object * Label;

public:
    QList < PlayersElement * > players;

    PlayersTune(GraphicObject * parent) : GraphicObject(parent, RIGHT_CLICKABLE)
    {
        is_rectangular = true;
        Label = new GraphicObject(this, 0, "PlayersLabel");
        for (int i = 0; i < settings->rules->AllPlayers.size(); ++i)
        {
            players.push_back(new PlayersElement(this, i, i < settings->rules->numOfPlayers, true));
            connect(players[i], SIGNAL(whenClicked(int)), SLOT(whenClicked(int)));
        }
    }
    virtual void Delete()
    {
        Label->Delete();
        foreach (PlayersElement * pe, players)
            pe->Delete();

        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        Label->setGeometry(0, 0, W, H / 2);

        int n = players.size();
        qreal size = H / 2 * (1 - 2 * constants->playersTuneMargin);
        qreal X = (W - size * n - size *(n - 1) * constants->playersTuneInsideMargin) / 2;
        qreal Y = H / 2 * (1 + constants->playersTuneMargin);
        for (int i = 0; i < n; ++i)
        {
            players[i]->setGeometry(X, Y, size, size);
            X += size * (1 + constants->playersTuneInsideMargin);
        }
    }

    void rightClick() { emit help->HelpAsked("PlayersTune"); }

private slots:
    void whenClicked(int i)
    {
        while (settings->rules->numOfPlayers <= i)
        {
            players[settings->rules->numOfPlayers]->turnOn(true);
            ++settings->rules->numOfPlayers;
        }

        while (settings->rules->numOfPlayers > qMax(i + 1, 2))
        {
            --settings->rules->numOfPlayers;
            players[settings->rules->numOfPlayers]->turnOn(false);
        }
    }
};

class TimerElement : public StateObject
{
    Q_OBJECT

public:
    bool isOn = true;
    bool active;
    TimerType type;
    int i;

    TimerElement(GraphicObject * parent, int i, TimerType type, bool isOn, bool active) :
        StateObject(parent, "on", "timer_" + type, (active * CLICKABLE) | RIGHT_CLICKABLE, "", "SimpleLayer")
    {
        this->type = type;
        this->i = i;
        this->active = active;
        addPicture("off", "timer_" + type + "_off");

        pictures["on"]->is_rectangular = true;
        pictures["off"]->is_rectangular = true;

        turnOn(isOn);
    }

    void leftClick() { emit whenClicked(i); }
    void rightClick() { emit help->HelpAsked(active ? "timer_" + type : "timer_" + type); }
    void turnOn(bool on)
    {
        isOn = on;
        setPictureState(isOn ? "on" : "off");
    }

signals:
    void whenClicked(int i);
};
class TimerTune : public GraphicObject
{
    Q_OBJECT

    Object * Label;
    int turned_i = 0;

public:
    QList < TimerElement * > options;

    TimerTune(GraphicObject * parent) : GraphicObject(parent, RIGHT_CLICKABLE)
    {
        is_rectangular = true;
        Label = new GraphicObject(this, 0, "TimerLabel");
        for (int i = 0; i < settings->rules->AllTimerTypes.size(); ++i)
        {
            bool on = settings->rules->timer == settings->rules->AllTimerTypes[i];
            options.push_back(new TimerElement(this, i, settings->rules->AllTimerTypes[i], on, true));
            connect(options[i], SIGNAL(whenClicked(int)), SLOT(whenClicked(int)));

            if (on)
                this->turned_i = i;
        }
    }
    virtual void Delete()
    {
        Label->Delete();
        foreach (TimerElement * te, options)
            te->Delete();

        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        Label->setGeometry(0, 0, W, H / 2);

        int n = options.size();
        qreal size = H / 2 * (1 - 2 * constants->timerTuneMargin);
        qreal X = (W - size * n - size *(n - 1) * constants->timerTuneInsideMargin) / 2;
        qreal Y = H / 2 * (1 + constants->timerTuneMargin);
        for (int i = 0; i < n; ++i)
        {
            options[i]->setGeometry(X, Y, size, size);
            X += size * (1 + constants->timerTuneInsideMargin);
        }
    }

    void rightClick() { emit help->HelpAsked("TimerTune"); }

private slots:
    void whenClicked(int i)
    {
        if (settings->rules->timer != options[i]->type)
        {
            options[turned_i]->turnOn(false);
            turned_i = i;
            options[turned_i]->turnOn(true);
            settings->rules->timer = options[i]->type;
        }
    }
};

class FieldTune : public GraphicObject
{
    Q_OBJECT
    bool active;

    QVector < DigitObject * > digits;

public:
    FieldTune(GraphicObject * parent, GameRules * rules = settings->rules, bool active = true) :
        GraphicObject(parent, RIGHT_CLICKABLE)
    {
        is_rectangular = true;
        this->active = active;

        digits.push_back(new DigitObject(this, rules->fieldH / 10, "Fort", active * (WHEEL | CLICKABLE)));
        digits.push_back(new DigitObject(this, rules->fieldH % 10, "Mill", active * (WHEEL | CLICKABLE)));
        digits.push_back(new DigitObject(this, -1, "Fort", 0));
        digits.push_back(new DigitObject(this, rules->fieldW / 10 == 0 ? rules->fieldW % 10 : rules->fieldW / 10, "Mill", active * (WHEEL | CLICKABLE)));
        digits.push_back(new DigitObject(this, rules->fieldW % 10, "Fort", active * (WHEEL | CLICKABLE)));

        if (rules->fieldW / 10 == 0)
            digits[4]->setVisible(false);
        if (digits[0]->n == 0)
            digits[0]->setVisible(false);

        connect(digits[0], SIGNAL(wheeled(int)), SLOT(HeightWheeled(int)));
        connect(digits[1], SIGNAL(wheeled(int)), SLOT(HeightWheeled(int)));
        connect(digits[3], SIGNAL(wheeled(int)), SLOT(WidthWheeled(int)));
        connect(digits[4], SIGNAL(wheeled(int)), SLOT(WidthWheeled(int)));
        connect(digits[0], SIGNAL(leftClicked()), SLOT(HeightClicked()));
        connect(digits[1], SIGNAL(leftClicked()), SLOT(HeightClicked()));
        connect(digits[3], SIGNAL(leftClicked()), SLOT(WidthClicked()));
        connect(digits[4], SIGNAL(leftClicked()), SLOT(WidthClicked()));
    }
    virtual void Delete()
    {
        foreach (DigitObject * d, digits)
            d->Delete();

        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        qreal sizeX = W / (5 - 4 * constants->textXShift);
        qreal X = 0;
        for (int i = 0; i < 5; ++i)
        {
            digits[i]->setGeometry(X, 0, sizeX, H);
            X += sizeX * (1 - constants->textXShift);
        }
    }

    void rightClick() { emit help->HelpAsked(active ? "FieldTune" : "FieldSize"); }

private slots:
    void HeightWheeled(int n)
    {
        // TODO ограничения на размер поля перенести в константы!
        if (n > 0 &&
            settings->rules->fieldH < settings->rules->fieldW &&
            settings->rules->fieldH < 12)
        {
            ++settings->rules->fieldH;
        }
        if (n < 0 &&
             settings->rules->fieldH > 3)
        {
            --settings->rules->fieldH;
        }

        digits[0]->setN(settings->rules->fieldH / 10);
        digits[0]->setVisible(digits[0]->n > 0);
        digits[1]->setN(settings->rules->fieldH % 10);
    }
    void WidthWheeled(int n)
    {
        if (n > 0 &&
            settings->rules->fieldW < 20)
        {
            settings->rules->fieldW += 2;
        }
        if (n < 0 &&
             settings->rules->fieldW - 2 >= settings->rules->fieldH &&
             settings->rules->fieldW - 2 >= 4)
        {
            settings->rules->fieldW -= 2;
        }

        if (settings->rules->fieldW / 10)
        {
            digits[3]->setN(settings->rules->fieldW / 10);
            digits[4]->setVisible(true);
            digits[4]->setN(settings->rules->fieldW % 10);
        }
        else
        {
            digits[3]->setN(settings->rules->fieldW);
            digits[4]->setVisible(false);
        }
    }
    void HeightClicked()
    {
        int H = settings->rules->fieldH;
        HeightWheeled(1);

        if (H == settings->rules->fieldH)
        {
            settings->rules->fieldH = 3;
            digits[0]->setN(0);
            digits[0]->setVisible(false);
            digits[1]->setN(settings->rules->fieldH % 10);
        }
    }
    void WidthClicked()
    {
        int W = settings->rules->fieldW;
        WidthWheeled(1);

        if (W == settings->rules->fieldW)
        {
            settings->rules->fieldW = qMax(4, settings->rules->fieldH + settings->rules->fieldH % 2);

            if (settings->rules->fieldW / 10)
            {
                digits[3]->setN(settings->rules->fieldW / 10);
                digits[4]->setVisible(true);
                digits[4]->setN(settings->rules->fieldW % 10);
            }
            else
            {
                digits[3]->setN(settings->rules->fieldW);
                digits[4]->setVisible(false);
            }
        }
    }
};

class UnitsElement : public GraphicObject
{
    Q_OBJECT

public:
    bool active;
    bool isOn = true;
    Unit * on, * off;
    UnitType type;

    UnitsElement(GraphicObject * parent, UnitType type, bool isOn, bool active, PlayerColor color = "") :
        GraphicObject(parent, (active * CLICKABLE) | RIGHT_CLICKABLE)
    {
        this->active = active;
        this->type = type;
        on = new Unit(this, color == "" ? settings->rules->AllPlayers[rand() % settings->rules->AllPlayers.size()] : color, type);
        off = new Unit(this, "Neutral", type);

        turnOn(isOn);
    }
    virtual void Delete()
    {
        on->Delete();
        off->Delete();

        GraphicObject::Delete();
    }

    void resize(qreal w, qreal h)
    {
        on->setGeometry(0, 0, w, h);
        off->setGeometry(0, 0, w, h);
    }

    void rightClick() { emit help->HelpAsked(active ? type : "UnitsInGame"); }
    void leftClick()
    {
        emit whenClicked(!isOn, type);
    }
    void turnOn(bool isOn)
    {
        this->isOn = isOn;

        if (isOn)
        {
            on->AnimationStart(OPACITY, 1, constants->stateObjectChangeTime);
            off->AnimationStart(OPACITY, 0, constants->stateObjectChangeTime);
        }
        else
        {
            off->AnimationStart(OPACITY, 1, constants->stateObjectChangeTime);
            on->AnimationStart(OPACITY, 0, constants->stateObjectChangeTime);
        }
    }

signals:
    void whenClicked(bool on, QString type);
};
class UnitsTune : public GraphicObject
{
    Q_OBJECT
public:
    QVector <UnitsElement *> units;

    explicit UnitsTune(GraphicObject * parent, GameRules * rules = settings->rules, bool active = true) :
        GraphicObject(parent)
    {
        this->active = active;
        for (int i = 0; i < rules->AllUnits.size(); ++i)
        {
            bool on = rules->unitsInGame.contains(rules->AllUnits[i]);
            if (active || on)
            {
                units.push_back(new UnitsElement(this, rules->AllUnits[i], on, active));
            }
        }
    }
    explicit UnitsTune(GraphicObject * parent, GameRules * rules, PlayerColor color, UnitType active_unit) :
        GraphicObject(parent)
    {
        this->active = true;
        for (int i = 0; i < rules->unitsInGame.size(); ++i)
        {
            UnitType type = rules->unitsInGame[i];
            units.push_back(new UnitsElement(this, type, type == active_unit, active, color));
        }
    }
    virtual void Delete()
    {
        foreach (UnitsElement * ue, units)
            ue->Delete();
        GraphicObject::Delete();
    }

    bool active;
    void resizeChildren(qreal W, qreal H)
    {
        int n = units.size();
        qreal size = qMin(H * (active ? 1 : constants->unitsMaxHeight),
                          (W + n * constants->unitsTuneInsideMargin) / (n * (1 + constants->unitsTuneInsideMargin)));
        qreal real_length = n * size + (n - 1) * size * constants->unitsTuneInsideMargin;

        qreal X = (W - real_length) / 2;
        for (int i = 0; i < units.size(); ++i)
        {
            units[i]->setGeometry(X, (H - size) / 2 + size / 2 * constants->unitsTuneYshift, size, size);
            X += size * (1 + constants->unitsTuneInsideMargin);
        }
    }
};

class OrdersElement : public GraphicObject
{
    Q_OBJECT

public:
    bool active;
    bool isOn = true;
    OrderType type;

    OrdersElement(GraphicObject * parent, OrderType type, bool isOn, bool active) :
        GraphicObject(parent, (active * CLICKABLE) | RIGHT_CLICKABLE, type)
    {
        this->active = active;
        this->type = type;
        turnOn(isOn);
    }

    void rightClick() { emit help->HelpAsked(active ?  type : "OrdersInGame"); }
    void leftClick()
    {
        emit whenClicked(!isOn, type);
    }
    void turnOn(bool isOn)
    {
        this->isOn = isOn;

        if (isOn)
        {
            AnimationStart(OPACITY, 1, constants->stateObjectChangeTime);
        }
        else
        {
            AnimationStart(OPACITY, constants->orderTurnedOffOpacity, constants->stateObjectChangeTime);
        }
    }

signals:
    void whenClicked(bool on, QString type);
};
class OrdersTune : public GraphicObject
{
    Q_OBJECT
public:
    QList < OrdersElement * > orders;

    OrdersTune(GraphicObject * parent, GameRules * rules = settings->rules, bool active = true) : GraphicObject(parent)
    {
        this->active = active;
        for (int i = 0; i < rules->AllOrders.size(); ++i)
        {
            bool on = rules->ordersInGame.contains(rules->AllOrders[i]);
            if (on || active)
            {
                orders.push_back(new OrdersElement(this, rules->AllOrders[i], on, active));
            }
        }
    }
    virtual void Delete()
    {
        foreach (OrdersElement * r, orders)
            r->Delete();
        GraphicObject::Delete();
    }

    bool active;
    void resizeChildren(qreal W, qreal H)
    {
        int n = orders.size();
        qreal size = qMin(H * (active ? 1 : constants->ordersMaxHeight),
                          (W + n * constants->ordersTuneInsideMargin) / (n * (1 + constants->ordersTuneInsideMargin)));
        qreal real_length = n * size + (n - 1) * size * constants->ordersTuneInsideMargin;

        qreal X = (W - real_length) / 2;
        for (int i = 0; i < orders.size(); ++i)
        {
            orders[i]->setGeometry(X, (H - size) / 2, size, size);
            X += size * (1 + constants->ordersTuneInsideMargin);
        }
    }
};

class DayTimesTune : public GraphicObject
{
    Q_OBJECT

    DayTimePanel * panel;
    int dayTimes;

    QTimer * timer;
    bool active;

public:
    explicit DayTimesTune(GraphicObject * parent, GameRules * rules = settings->rules, bool active = true) :
        GraphicObject(parent, RIGHT_CLICKABLE | (active * HOVER) | (active * CLICKABLE), "", "LessonFrameSelection", "SimpleLayer")
    {
        is_rectangular = true;
        this->active = active;
        dayTimes = rules->dayTimes;
        createPanel(rules->changingDayTimes);

        timer = new QTimer(this);
        connect(timer, SIGNAL(timeout()), SLOT(inc()));
        if (rules->changingDayTimes)
            timer->start(constants->dayTimesChangeAnimationTime);
    }
    virtual void Delete()
    {
        panel->Delete();
        GraphicObject::Delete();
    }
    void createPanel(bool cdt)
    {
        panel = new DayTimePanel(this, dayTimes, DayTimePanel::FREEZE, cdt);
        resizeChildren(width(), height());
    }

    void resizeChildren(qreal W, qreal H)
    {
        panel->setGeometry(0, 0, W, H);
    }

    void rightClick() { emit help->HelpAsked(active ? "DayTimesTune" : "DayTimesInGame"); }

    void leftClick()  // переключение ведётся не только между вариантами 1...5, но и шестым вариантом - их динамическим переключением
    {
        panel->Delete();

        if (settings->rules->changingDayTimes)
        {
            settings->rules->changingDayTimes = false;
            settings->rules->dayTimes = 1;
            dayTimes = 1;
        }
        else if (dayTimes < 5)
        {
            ++dayTimes;
            ++settings->rules->dayTimes;
        }
        else
        {
            settings->rules->changingDayTimes = true;
            settings->rules->dayTimes = 1;
        }

        createPanel(settings->rules->changingDayTimes);

        if (settings->rules->changingDayTimes)
            timer->start(constants->dayTimesChangeAnimationTime);
        else
            timer->stop();
    }

public slots:
    void inc()
    {
        panel->Delete();
        dayTimes = (dayTimes) % 5 + 1;
        createPanel(true);

        timer->start(constants->dayTimesChangeAnimationTime);
    }
};

#endif // TUNES_H
