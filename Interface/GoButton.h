#ifndef GOBUTTON_H
#define GOBUTTON_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

class GoButton : public StateObject
{
    Q_OBJECT

public:
    explicit GoButton(GraphicObject * parent, PlayerColor color) :
        StateObject(parent, "disabled", "DisabledGoButton", 0, "SimpleLayer")
    {
        addPicture("enabled", color + "GoButton");
        addPicture("pushed", "PressedGoButton");
    }
    void enable(bool enable = true)
    {
        if (enable)
        {
            setPictureState("enabled");
            setProperties(CLICKABLE);
        }
        else
        {
            setPictureState("disabled");
            setProperties(0);
        }
    }

    void leftClick()
    {
        setPictureState("pushed");
        setProperties(0);
    }
};

class NextButton : public GraphicObject
{
    Q_OBJECT

public:
    explicit NextButton(GraphicObject * parent, PlayerColor mainPlayerColor) :
        GraphicObject(parent, CLICKABLE, mainPlayerColor + "Next", "", "SimpleLayer")
    {
    }

    bool noCheckMode = false;

public slots:
    void enableWithCheck()
    {
        if (isClicked && !noCheckMode)
        {
            leftClickStart();
            emit leftClickStarted();
        }
        else
            enable(true);
    }
    void enable(bool enable)
    {
        if (enable)
        {
            layer->setVisible(false);
            setProperties(CLICKABLE);
            isClicked = false;
            disableWhenReleased = false;
        }
        else
        {
            layer->setVisible(true);
            setProperties(0);
            isClicked = false;
        }
    }

protected:
    bool disableWhenReleased = true;
    void leftClickStart()
    {
        disableWhenReleased = true;
    }
    void leftClick()
    {
        if (disableWhenReleased)
            enable(false);
    }
};

#endif // GOBUTTON_H
