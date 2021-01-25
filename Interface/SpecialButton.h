#ifndef WHITEFLAGBUTTON_H
#define WHITEFLAGBUTTON_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

// Кнопка, которая увеличивается при наведении.
class SpecialButton : public StateObject
{
    Q_OBJECT

public:
    explicit SpecialButton(GraphicObject * parent, QString Name, bool simpleLayer = true) :
        StateObject(parent, "default", Name, CLICKABLE | HOVER, "", simpleLayer ? "SimpleLayer" : "")
    {        
        is_rectangular = true;
        addGeometry("entered", QRectF(constants->specialButtonShiftX,
                                      constants->specialButtonShiftY,
                                      constants->specialButtonWidthShift,
                                      constants->specialButtonHeightShift));
    }

private:
    void enter()
    {
        setGeometryState("entered");
    }
    void leave()
    {
        setGeometryState("default");
    }
};

#endif // WHITEFLAGBUTTON_H
