#ifndef GOBUTTON_H
#define GOBUTTON_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

class GoButton : public StateObject
{
public:
    explicit GoButton(GraphicObject * parent) :
        StateObject(parent, "disabled", 0, "DisabledGoButton")
    {
        addState("enabled", "GoButton", PUSHABLE);
        addState("pushed", "PressedGoButton", 0);
    }
    void enable(bool enable = true)
    {
        if (enable)
        {
            setState("enabled");
        }
        else
        {
            setState("disabled");
        }
    }

    void push(bool newState)
    {
        if (newState)
            setState("pushed");
        else
            qDebug() << "ERROR; GoButton depushed!";
    }
};

#endif // GOBUTTON_H
