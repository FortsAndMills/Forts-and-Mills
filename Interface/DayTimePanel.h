#ifndef DAYTIMEPANEL_H
#define DAYTIMEPANEL_H

#include "Technical/Headers.h"
#include "GameExecution/Game.h"
#include "Technical/Constants.h"
#include "GameElements/DayTimeButton.h"

// Эта панель используется и в игре, и в меню создания игры,
// и на панельке комнаты игры в главменю
class DayTimePanel : public GraphicObject
{
    Q_OBJECT
public:
    QMap <DayTime, DayTimeButton *> DayTimePictures;

    enum DayTimePanelState {FREEZE, GAME_STATE};
    explicit DayTimePanel(GraphicObject * parent, int dt, DayTimePanelState state = GAME_STATE, bool MaximizeDayTimes = false) :
        GraphicObject(parent, 0, "", "", "SimpleLayer")
    {        
        is_rectangular = true;
        this->MaximizeDayTimes = MaximizeDayTimes;

        for (int time = 0; time < dt; ++time)
        {
            DayTimePictures[time] = new DayTimeButton(this, time, "Round" + QString::number(time + 1), state == GAME_STATE);
        }
    }
    virtual void Delete()
    {
        foreach (DayTimeButton * button, DayTimePictures)
            button->Delete();
        GraphicObject::Delete();
    }

    bool MaximizeDayTimes;
    void resizeChildren(qreal W, qreal H)
    {
        int n = MaximizeDayTimes ? 5 : DayTimePictures.size();
        qreal size = W / (n + (n - 1) * constants->dayTimeTableMargin);
        size = qMin(size, H);

        qreal X = (W - (DayTimePictures.size() + (DayTimePictures.size() - 1) * constants->dayTimeTableMargin) * size) / 2;
        for (int i = 0; i < DayTimePictures.size(); ++i)
        {
            DayTimePictures[i]->setGeometry(X, (H - size) / 2, size, size);
            X += size * (1 + constants->dayTimeTableMargin);
        }
    }

    void appear()
    {
        this->setVisible(true);
        this->setOpacity(0);
        this->AnimationStart(OPACITY, 1, constants->gameMainPhaseStartPanelsAppearTime);
    }
    void disappear()
    {
        GraphicObject::disappear(constants->gameMainPhaseStartPanelsAppearTime);
    }
    void select(DayTime time, bool enable)
    {
        DayTimePictures[time]->select(enable);
    }
    void deselectAll()
    {
        foreach (DayTimeButton * dtb, DayTimePictures)
            dtb->select(false);
    }
};

#endif // DAYTIMEPANEL_H
