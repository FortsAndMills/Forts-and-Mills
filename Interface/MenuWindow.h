#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "Technical/Client.h"
#include "BasicElements/GraphicObject.h"

#include "Dialog.h"

class MenuWindow : public GraphicObject
{
    Q_OBJECT

    Dialog * dialog;

public:
    explicit MenuWindow(GraphicObject * parent) : GraphicObject(parent)
    {
        dialog = new Dialog(this, "ERROR 01\n NO CONNECTION TO SERVER");
        dialog->ClipWithItem(parent);

        connect(client, SIGNAL(connected()), SLOT(connected()));
    }

    void resizeChildren(qreal W, qreal H)
    {
        dialog->setGeometry(W * (1 - constants->dialogWidth) / 2,
                                            H * (1 - constants->dialogHeight) / 2,
                                            W * constants->dialogWidth, H * constants->dialogHeight);
    }

public slots:
    void connected()
    {
        dialog->setText("CONNECTED TO SERVER\nWAITING FOR SECOND PLAYER");
    }
};

#endif // MENUWINDOW_H
