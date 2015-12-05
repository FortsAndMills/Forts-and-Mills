#ifndef GAMEHEX
#define GAMEHEX

#include "Technical/Headers.h"

class GameHex : public QObject
{
    Q_OBJECT

public:
    HexType type;

    PlayerColor color = "Neutral";

    int x, y;

    QList <Resource> resources;

    bool canBeCaptured;
    bool canHaveResources;
    bool canBeChosenAsStartPoint;
    bool canGoHere;
    bool isBuilding;

    // вспомогательные переменные
    bool wasChosenAsStartPoint = false;

    explicit GameHex(QObject * parent, HexType type, int x, int y) : QObject(parent)
    {
        this->x = x;
        this->y = y;

        setType(type);
    }
    void setType(HexType type);
};
#endif
