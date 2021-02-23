#ifndef WHITEFLAGBUTTON_H
#define WHITEFLAGBUTTON_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

// Кнопка, которая увеличивается при наведении.
class EnlargingButton : public StateObject
{
    Q_OBJECT

public:
    explicit EnlargingButton(GraphicObject * parent, QString Name, bool simpleLayer = true, bool keepaspectratio=true) :
        StateObject(parent, "default", Name, CLICKABLE | HOVER, "", simpleLayer ? "SimpleLayer" : "", keepaspectratio)
    {        
        this->set_rectangular_boundig_box();
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
