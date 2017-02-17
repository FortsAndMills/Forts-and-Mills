#ifndef GAMEELEMENTSORGANIZATION_H
#define GAMEELEMENTSORGANIZATION_H

#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "GameExecution/Game.h"
#include "GameElements/FieldWindow.h"
#include "GameElements/Unit.h"
#include "GameElements/Rocket.h"
#include "GameElements/CureIcon.h"
#include "Technical/AnimationManager.h"

class GameElementsOrganization : public GraphicObject
{
    Q_OBJECT

public:
    Game * game;

    PlayerColor mainPlayerColor = "";
    quint8 PlayerIndex = UNDEFINED;

    Field * field;
    QMap <GameHex *, Hex *> hexes;
    Hex * hex(Coord coord)
    {
        return hexes[game->hex(coord)];
    }
    Hex * hex(int i, int j)
    {
        return hex(Coord(i, j));
    }
    inline Hex * hex(Unit * u)
    {
        return hex(u->where());
    }

    QMap <GameUnit *, Unit *> units;

    QMap <GameOrder *, Order *> orders;

    QMap <Hex *, QList <Fortification *> > fortifications;

    QMap <Unit *, QStack <UnitWay *> > ways_to;
    QMap <Unit *, QStack <UnitWay *> > ways_from;

    QMap <Unit *, QList<Rocket *> > rockets;

    explicit GameElementsOrganization(Game * game, qint8 PlayerIndex, GraphicObject * parent);
    virtual void Delete();

    QPointF adjacentHexCoord(Hex * hex, WAY way);

    QPointF unitCoordinate(Unit * u);
    void resizeUnits();
    QPointF wayToStartCoordinate(UnitWay * way);
    QPointF wayToEndCoordinate(UnitWay * way);
    QPointF wayFromStartCoordinate(UnitWay * way);
    QPointF wayFromEndCoordinate(UnitWay * way);
    void resizeWays();
    void resizeRockets();
    QRectF fortificationGeometry(Hex * hex, int index);
    void resizeFortifications();

    void allHexesToReconfigurePoints();
    void reconfigureUnits();
    void reconfigureWays();
    void reconfigureRockets();
    void reconfigureFortifications();

public slots:
    void hexPointsChanged();
};

#endif // GAMEELEMENTSORGANIZATION_H
