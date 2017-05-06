#ifndef LEFTPANELSSWITCH_H
#define LEFTPANELSSWITCH_H

#include "Technical/Headers.h"
#include "BasicElements/GraphicObject.h"

class LeftPanelSwitcher : public GraphicObject
{
    Q_OBJECT

public:
    QString name;

    explicit LeftPanelSwitcher(GraphicObject * parent, QString name) :
        GraphicObject(parent, CLICKABLE | HOVER, name + "PanelButton", "SimpleLayer")
    {
        this->name = name;
    }

    void leftClick()
    {
        emit leftPanelSwitcherClicked(name);
    }

signals:
    void leftPanelSwitcherClicked(QString);
};

#endif // LEFTPANELSSWITCH_H
