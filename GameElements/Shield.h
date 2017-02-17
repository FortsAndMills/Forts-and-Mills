#ifndef SHIELD_H
#define SHIELD_H

#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

class Shield : public StateObject
{
    Q_OBJECT

public:
    bool isOn = true;
    PlayerColor color;

    explicit Shield(GraphicObject * parent, PlayerColor color) :
        StateObject(parent, "default", color + "Shield", 0, "", "", constants->shieldTurnTime)
    {
        this->color = color;

        addGeometry("small", QRectF((1 - constants->ShieldSmallStateWidthShift) / 2,
                                                           (1 - constants->ShieldSmallStateHeightShift) / 2,
                                                              constants->ShieldSmallStateWidthShift,
                                                              constants->ShieldSmallStateHeightShift));
        addPicture("small", "NeutralShield");
    }
    explicit Shield(Shield * another, GraphicObject * newParent) : Shield(newParent, another->color)
    {
        if (!another->isOn)
        {
            isOn = false;
            setGeometryState("small", true);
            setPictureState("small", true);
        }
    }

    void turnOn(bool turn = true)
    {
        isOn = turn;
        if (turn)
        {
            setGeometryState("default");
            setPictureState("default");
        }
        else
        {
            setGeometryState("small");
            setPictureState("small");
        }
    }
};

#endif // SHIELD_H
