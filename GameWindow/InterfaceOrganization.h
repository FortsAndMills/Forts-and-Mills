#ifndef INTERFACEORGANIZATION_H
#define INTERFACEORGANIZATION_H

#include "GameElementsOrganization.h"
#include "Interface/PlayerResources.h"
#include "Interface/DayTimePanel.h"
#include "Interface/GoButton.h"
#include "Interface/Dialog.h"
#include "Interface/LeftPanelsSwitch.h"
#include "Interface/SpecialButton.h"
#include "Technical/Client.h"

// Организация неигровых элементов интерфейса и их, фу-фу, геометрия

class InterfaceOrganization : public GameElementsOrganization
{
    Q_OBJECT

public:
    QMap <PlayerColor, PlayerResources *> player_windows;
    QString leftPanel;
    QList <LeftPanelSwitcher *> leftPanelSwitchers;

    GoButton * go;
    NextButton * next;

    DayTimePanel * DayTimeTable;
    FieldWindow * fieldControl;

    SpecialButton * whiteFlag;
    SpecialButton * homeButton;

    Hex * hexCopy;

    Dialog * dialog;

    explicit InterfaceOrganization(Game * game, qint8 PlayerIndex, GraphicObject * parent);
    virtual void Delete();

    QRectF leftPanelGeometry();
    QRectF closedLeftPanelGeometry();

    void resize(qreal W, qreal H);
    void resizeHexCopy();
    void resizeDayTimeTable(qreal W, qreal H);
    void resizeLeftPanelSwitchers(qreal W, qreal H);
    virtual void resizeDialog(qreal W, qreal H);

public slots:
    void leftPanelSwitcherClicked(QString name);
};

#endif // INTERFACEORGANIZATION_H
