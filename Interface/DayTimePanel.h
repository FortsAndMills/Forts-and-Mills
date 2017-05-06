#ifndef DAYTIMEPANEL_H
#define DAYTIMEPANEL_H

#include "Technical/Headers.h"
#include "GameExecution/Game.h"
#include "Technical/Constants.h"
#include "GameElements/DayTimeButton.h"

// Эта панель используется и в игре, и в меню создания игры, и на панельке комнаты игры в главменю
class DayTimePanel : public GraphicObject
{
    Q_OBJECT
public:
    QMap <DayTime, DayTimeButton *> DayTimePictures;

    enum DayTimePanelState {FREEZE, BUTTON, GAME_STATE};
    explicit DayTimePanel(GraphicObject * parent, int dt, DayTimePanelState state = GAME_STATE, bool MaximizeDayTimes = false) :
        GraphicObject(parent, (state == BUTTON) * (CLICKABLE | CHILD) | (state == FREEZE) * CHILD, "", "", "SimpleLayer")
    {
        this->MaximizeDayTimes = MaximizeDayTimes;
        QList <QString> DayTimeNames = {"Breakfast", "Dinner", "Supper", "Night", "Sleep"};

        for (int time = 0; time < dt; ++time)
        {
            DayTimePictures[time] = new DayTimeButton(this, time, DayTimeNames[time], state == GAME_STATE);
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

    QRectF boundingRect() const
    {
        return QRectF(0, 0, width(), height());
    }
    QPainterPath shape() const
    {
        QPainterPath qp;
        qp.addRect(boundingRect());
        return qp;
    }
    bool contains(const QPointF &point) const
    {
        return boundingRect().contains(point);
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
