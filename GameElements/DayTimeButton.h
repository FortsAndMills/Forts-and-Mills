#ifndef DAYTIMEBUTTON_H
#define DAYTIMEBUTTON_H

#include "Technical/Headers.h"
#include "Game/Game.h"
#include "BasicElements/StateObject.h"
#include "BasicElements/SpriteObject.h"

class DayTimeButton : public StateObject
{
    Q_OBJECT

    DayTime time;

    SpriteObject * selection = NULL;

public:
    explicit DayTimeButton(GraphicObject * parent, DayTime time, QString timeName) :
        StateObject(parent, "default", CLICKABLE, timeName, "", "SimpleLayer")
    {
        this->time = time;

        geometries["entered"] = QRectF(constants->specialButtonShiftX,
                                                               constants->specialButtonShiftY,
                                                               constants->specialButtonWidthShift,
                                                               constants->specialButtonHeightShift);
        geometries["selected"] = QRectF(constants->dayTimeButtonSelectedShiftX,
                                                             constants->dayTimeButtonSelectedShiftY,
                                                             constants->dayTimeButtonSelectedWidthShift,
                                                             constants->dayTimeButtonSelectedHeightShift);
    }
    virtual void Delete()
    {
        if (selection != NULL)
            selection->Delete();
        StateObject::Delete();
    }

    void resizeChildren(qreal W, qreal H)
    {
        if (selection != NULL)
            selection->setGeometry(-W / 2, -H / 2, 2 * W, 2 * H);  // <undone>
    }

private:
    bool selected = false;
public:
    void select(bool enable = true)
    {
        selected = enable;
        if (enable)
        {
            if (cur_geometry != "entered")
                setState("selected");
            if (selection == NULL)
            {  // <<undone>
                selection = new SpriteObject(this, 0, "DayTimeSelection");
                selection->setZValue(constants->selectionZpos);
                resizeChildren(width(), height());
            }
        }
        else
        {
            if (selection != NULL)
            {
                selection->Delete();
                selection = NULL;
            }

            if (cur_geometry == "selected")
                setState("default");
        }
    }
private:
    void enter()
    {
        setState("entered");
    }
    void leave()
    {
        if (selected)
        {
            setState("selected");
        }
        else
            setState("default");
    }

    void leftClick() { emit leftClicked(time); }
signals:
    void leftClicked(DayTime time);
};

#endif // DAYTIMEBUTTON_H
