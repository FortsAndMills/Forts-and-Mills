#ifndef DAYTIMEPANEL_H
#define DAYTIMEPANEL_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "Game/Game.h"
#include "GameElements/DayTimeButton.h"

class DayTimePanel : public GraphicObject
{
    Q_OBJECT

    Game * game;

    QMap <DayTime, DayTimeButton *> DayTimePictures;
    friend GameWindow;  // FAIL сигнальчики, блин

public:
    explicit DayTimePanel(GraphicObject * parent, Game * game) : GraphicObject(parent)
    {
        this->game = game;
        for (int time = 0; time < game->rules->dayTimes.size(); ++time)
        {
            DayTimePictures[time] = new DayTimeButton(this, time, game->rules->dayTimes[time]);
        }
    }
    virtual void Delete()
    {
        foreach (DayTimeButton * button, DayTimePictures)
            button->Delete();
        GraphicObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        qreal size = W / (DayTimePictures.size() +
                          (DayTimePictures.size() - 1) * constants->dayTimeTableMargin);

        qreal X = 0;
        for (int i = 0; i < game->rules->dayTimes.size(); ++i)
        {
            DayTimePictures[i]->setGeometry(X, 0, size, H);
            X += size * (1 + constants->dayTimeTableMargin);
        }
    }

    void appear()
    {
        this->setVisible(true);
        this->setOpacity(0);
        this->AnimationStart(OPACITY, 1, constants->dayTimePanelAppearTime);
    }
    void select(DayTime time, bool enable)
    {
        DayTimePictures[time]->select(enable);
    }
};

#endif // DAYTIMEPANEL_H
