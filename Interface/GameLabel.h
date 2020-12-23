#ifndef GAMELABEL_H
#define GAMELABEL_H

#include "Technical/Headers.h"
#include "Game/GameRules.h"
#include "BasicElements/StateObject.h"
#include "BasicElements/MergingObject.h"
#include "Tunes.h"

// Прямоугольник игры в "комнатах"
class GameLabel : public StateObject
{
    Q_OBJECT

    MergingObject * light;

    QVector <PlayersElement *> players;
    FieldTune * field;
    UnitsTune * units;
    OrdersTune * orders;
    DayTimesTune * dayTimes;
    StateObject * join;

public:
    GameRules * rules;
    bool isOn = false;
    int index;

    GameLabel(GraphicObject * parent, int index, QList <qint32> rules_hash, qint8 pl, bool type) :
        StateObject(parent, "off", "GameLabelPanel" + QString::number(type + 1))
    {
        this->index = index;
        this->rules = new GameRules(rules_hash);

        addPicture("on", "ChosenGameLabelPanel");
        light = NULL;

        for (int i = 0; i < rules->numOfPlayers; ++i)
            players.push_back(new PlayersElement(this, i < pl, false));
        field = new FieldTune(this, rules, false);
        units = new UnitsTune(this, rules, false);
        orders = new OrdersTune(this, rules, false);
        dayTimes = new DayTimesTune(this, rules, false);

        join = new StateObject(this, "join", "JoinButton", CLICKABLE, "SimpleLayer");
        join->addPicture("leave", "LeaveButton");
        connect(join, SIGNAL(leftClicked()), SLOT(joinClicked()));
    }
    virtual void Delete()
    {
        if (light != NULL)
            light->Delete();
        foreach (PlayersElement * pe, players)
            pe->Delete();
        field->Delete();
        units->Delete();
        orders->Delete();
        dayTimes->Delete();
        join->Delete();

        StateObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        if (light != NULL)
            light->setGeometry(0, 0, W, H);

        qreal X = constants->labelsMarginX / 2 * W;
        qreal length = constants->unitsLabelX * W - 2 * X;
        qreal size = qMin(H * (1 - 2 * constants->playersTuneMargin),
                            (length + players.size() * constants->playersTuneInsideMargin) / (players.size() * (1 + constants->playersTuneInsideMargin)));
        qreal real_length = players.size() * size + (players.size() - 1) * size * constants->playersTuneInsideMargin;

        X += (length - real_length) / 2;
        for (int i = 0; i < players.size(); ++i)
        {
            players[i]->setGeometry(X, (H - size) / 2, size, size);
            X += size * (1 + constants->playersTuneInsideMargin);
        }

        units->setGeometry(W * (constants->unitsLabelX + constants->labelsMarginX / 2), 0,
                           W * (constants->fieldLabelX - constants->labelsMarginX - constants->unitsLabelX), H);
        field->setGeometry(W * (constants->fieldLabelX + constants->labelsMarginX / 2), 0,
                           W * (constants->ordersLabelX - constants->labelsMarginX - constants->fieldLabelX), H);
        orders->setGeometry(W * (constants->ordersLabelX + constants->labelsMarginX / 2), 0,
                           W * (constants->dayTimesLabelX - constants->labelsMarginX - constants->ordersLabelX), H);
        dayTimes->setGeometry(W * (constants->dayTimesLabelX + constants->labelsMarginX / 2), 0,
                           W * (constants->joinButtonX - constants->labelsMarginX - constants->dayTimesLabelX), H);
        join->setGeometry(W * (constants->joinButtonX + constants->labelsMarginX / 2), H * constants->joinButtonY,
                           W * (1 - constants->labelsMarginX - constants->joinButtonX), H * (1 - 2 * constants->joinButtonY));
    }

public slots:
    void joinClicked()
    {
        if (isOn)
        {
            setPictureState("off");
            light->DeleteOnMerging();
            light = NULL;
            join->setPictureState("join");
        }
        else
        {
            setPictureState("on");
            light = new MergingObject(this, "ChosenGameLabelLayer");
            resize(width(), height());
            join->setPictureState("leave");
        }
        isOn = !isOn;

        emit joinClicked(isOn, index);
    }
    void turnPlayer(bool on)
    {
        if (on)
        {
            int i = 0;
            while (players[i]->isOn) ++i;
            players[i]->turnOn(true);
        }
        else
        {
            int i = players.size() - 1;
            while (!players[i]->isOn) --i;
            players[i]->turnOn(false);
        }
    }

signals:
    void joinClicked(bool, int);
};

#endif // GAMELABEL_H
