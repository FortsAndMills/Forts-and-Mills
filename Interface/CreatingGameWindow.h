#ifndef CREATINGGAMEWINDOW_H
#define CREATINGGAMEWINDOW_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "Technical/Client.h"
#include "BasicElements/GraphicObject.h"
#include "Tunes.h"
#include "SpecialButton.h"
#include "Dialog.h"

// Меню создания игры
class CreatingGameWindow : public GraphicObject
{
    Q_OBJECT

    PlayersTune * players;    
    EnlargingButton * question;
    FieldTune * field;
    TimerTune * timers;
    UnitsTune * units;
    Object * border;
    OrdersTune * orders;
    DayTimesTune * dayTimes;
    Dialog * dialog;

public:
    EnlargingButton * cancel;
    GraphicObject * create;

    explicit CreatingGameWindow(GraphicObject * parent) : GraphicObject(parent)
    {
        setOpacity(0);

        players = new PlayersTune(this);

        question = new EnlargingButton(this, "Question");
        connect(question, SIGNAL(leftClicked()), SLOT(questionClicked()));

        field = new FieldTune(this);

        timers = new TimerTune(this);

        units = new UnitsTune(this);
        border = new Object(this, "Border");
        orders = new OrdersTune(this);

        foreach (UnitsElement * u, units->units)
        {
            connect(u, SIGNAL(whenClicked(bool,QString)), SLOT(unitClicked(bool,QString)));
        }
        foreach (OrdersElement * ord, orders->orders)
        {
            connect(ord, SIGNAL(whenClicked(bool,QString)), SLOT(orderClicked(bool,QString)));
        }

        dayTimes = new DayTimesTune(this);

        cancel = new EnlargingButton(this, "HomeButton");
        create = new GraphicObject(this, CLICKABLE | HOVER, "CreateButton_" + settings->language, "CreateGameFrameSelection", "SimpleLayer");

        dialog = new Dialog(this);
        dialog->showHelp = false;
    }
    void turn(bool on)
    {
        if (on)
        {
            AnimationStart(OPACITY, 1);
            dialog->showHelp = true;
        }
        else
        {
            AnimationStart(OPACITY, 0);
            dialog->showHelp = false;
        }
    }

    void resizeChildren(qreal W, qreal H)
    {
        players->setGeometry(constants->playersTuneX * W,
                                             constants->playersTuneY * H,
                                             constants->playersTuneWidth * W,
                                             constants->playersTuneHeight * H);
        question->setGeometry(W * (1 - constants->questionWidth) / 2,
                              H * (constants->fieldTuneY + (constants->fieldTuneHeight - constants->questionHeight) / 2),
                              constants->questionWidth * W,
                              constants->questionHeight * H);
        field->setGeometry(constants->fieldTuneX * W,
                                             constants->fieldTuneY * H,
                                             constants->fieldTuneWidth * W,
                                             constants->fieldTuneHeight * H);
        timers->setGeometry(constants->timerTuneX * W,
                                             constants->timerTuneY * H,
                                             constants->timerTuneWidth * W,
                                             constants->timerTuneHeight * H);
        units->setGeometry(0, constants->unitsTuneY * H, W, constants->unitsTuneHeight * H);
        border->setGeometry(W * (1 - constants->borderWidth) / 2,
                                            H * constants->borderY,
                                            W * constants->borderWidth,
                                            H * constants->borderHeight);
        orders->setGeometry(0, constants->ordersTuneY * H, W, constants->ordersTuneHeight * H);
        dayTimes->setGeometry(W * (1 - constants->dayTimesTuneWidth) / 2,
                                                 constants->dayTimesTuneY * H,
                                                 W * constants->dayTimesTuneWidth,
                                                 constants->dayTimesTuneHeight * H);
        cancel->setGeometry(constants->cancelButtonX * W,
                                             constants->cancelButtonY * H,
                                             constants->cancelButtonHeight * H,
                                             constants->cancelButtonHeight * H);
        create->setGeometry(constants->createButtonX * W,
                                             constants->createButtonY * H,
                                             constants->createButtonWidth * W,
                                             constants->createButtonHeight * H);

        resizeDialog(W, H);
    }
    void resizeDialog(qreal W, qreal H)
    {
        dialog->setGeometry(W * (1 - constants->dialogWidth) / 2,
                                           H * (1 - constants->dialogHeight) / 2,
                                            W * constants->dialogWidth,
                                            H * constants->dialogHeight);
    }

private slots:
    bool unitClicked(bool on, UnitType type)
    {
        // проверки на корректность добавления или удаления типа юнита из игры

        int i = 0;
        while (units->units[i]->type != type) { ++i; }

        if (units->units[i]->isOn == on)
            return false;
        if (!on && settings->default_rules->unitsInGame.size() <= 1)
            return true;

        units->units[i]->turnOn(on);
        if (on)
        {
            settings->default_rules->unitsInGame.push_back(type);
        }
        else
        {
            settings->default_rules->unitsInGame.removeAll(type);
        }

        bool fail = false;
        if (type == "Karkun")
            fail = orderClicked(on, "Siege");
        if (type == "Pig")
            fail = orderClicked(on, "Fire");
        if (type == "Mumusha" && on)
            fail = orderClicked(on, "Cure");

        if (fail)
            unitClicked(!on, type);
        return fail;
    }
    bool orderClicked(bool on, OrderType type)
    {
        // проверки на корректность добавления или удаления вида приказов в игру

        int i = 0;
        while (orders->orders[i]->type != type) { ++i; }

        if (orders->orders[i]->isOn == on)
            return false;

        if (!on)
            if (type == "Capture" || type == "Attack" || type == "Go" || type == "Liberate" || type == "Recruit")
                return true;

        orders->orders[i]->turnOn(on);
        if (on)
        {
            settings->default_rules->ordersInGame.push_back(type);
        }
        else
            settings->default_rules->ordersInGame.removeAll(type);

        bool fail = false;
        if (type == "Siege")
            fail = unitClicked(on, "Karkun");
        if (type == "Fire")
            fail = unitClicked(on, "Pig");
        if (!on && type == "Cure")
            fail = unitClicked(on, "Mumusha");

        if (fail)
            orderClicked(!on, type);
        return fail;
    }

    void languageChanged()
    {
        create->setPicture("CreateButton_" + settings->language);
    }
    void questionClicked()
    {
        help->HelpAsked("HowToAskHelp");
    }
};

#endif // CREATINGGAMEWINDOW_H
