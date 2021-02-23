#ifndef DAYTIMEBUTTON_H
#define DAYTIMEBUTTON_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"
#include "BasicElements/SpriteObject.h"

// Одна кнопка на панели времён дня внизу игрового окна

class DayTimeButton : public StateObject
{
    Q_OBJECT

    DayTime time;

    // надо найти красивый спрайт выделения))
    SpriteObject * selection = NULL;

public:
    // active отвечает за то, отображается ли панель в игровом окне или в главменю
    // в последнем случае, она неинтерактивна и просто висит картинкой
    explicit DayTimeButton(GraphicObject * parent, DayTime time, QString timeName, bool active) :
        StateObject(parent, "default", timeName, active * (CLICKABLE | HOVER), "", "SimpleLayer", false)
    {
        this->time = time;

        addPicture("selected", timeName + " selected");

        addGeometry("entered", QRectF(constants->specialButtonShiftX,
                                                              constants->specialButtonShiftY,
                                                              constants->specialButtonWidthShift,
                                                              constants->specialButtonHeightShift));
        addGeometry("selected", QRectF(constants->dayTimeButtonSelectedShiftX,
                                                               constants->dayTimeButtonSelectedShiftY,
                                                               constants->dayTimeButtonSelectedWidthShift,
                                                               constants->dayTimeButtonSelectedHeightShift));
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
        {
            selection->setGeometry(W * constants->dayTimeSelectionX,
                                   H * constants->dayTimeSelectionY,
                                   W * constants->dayTimeSelectionWidth,
                                   H * constants->dayTimeSelectionHeight);
        }
    }

private:
    bool selected = false;
public:
    // при выделении меняем геометрию (чуть увеличиваем картинку)
    // и включаем спрайт
    void select(bool enable = true)
    {
        selected = enable;
        if (enable)
        {
            setPictureState("selected");
            if (cur_geometry == "default")
            {
                setGeometryState("selected");
            }

            if (selection == NULL)
            {
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

            setPictureState("default");
            if (cur_geometry == "selected")
            {
                setGeometryState("default");
            }
        }
    }
private:
    // при наведении увеличиваем картинку слегка
    void enter()
    {
        setGeometryState("entered");
    }
    void leave()
    {
        if (selected)
        {
            setGeometryState("selected");
        }
        else
            setGeometryState("default");
    }

    void leftClick() { emit leftClicked(time); }
signals:
    void leftClicked(DayTime time);
};

#endif // DAYTIMEBUTTON_H
