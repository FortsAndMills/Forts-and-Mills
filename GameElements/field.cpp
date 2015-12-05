#include "Field.h"
#include "Game/GameRules.h"
#include "Game/Game.h"
#include "Hex.h"
#include "Technical/Constants.h"
#include "Unit.h"
#include "Game/GamePlayer.h"

Field::Field(GraphicObject *parent, Game * game) : GraphicObject(parent)
{
    for (int i = 0; i < game->rules->fieldH; ++i)
    {
        hexes << QList <Hex *> ();

        for (int j = 0; j < game->rules->fieldW; ++j)
        {
            hexes[i] << new Hex(this, game, game->field[i][j]);
        }
    }
}
void Field::Delete()
{
    foreach (QList <Hex *> list, hexes)
    {
        foreach (Hex * hex, list)
        {
            hex->Delete();
        }
    }
    GraphicObject::Delete();
}

bool Field::isSplitted(int x, int y, WAY way)
{
    if (way == "UP")
        return (x + y_shift) % hexes.size() == 0;
    if (way == "DOWN")
        return (x + y_shift + 1) % hexes.size() == 0;
    if (way == "RIGHT_UP")
        return (y % 2 == 0 && (x + y_shift) % hexes.size() == 0) || (y + x_shift + 1) % hexes[0].size() == 0;
    if (way == "RIGHT_DOWN")
        return (y % 2 && (x + y_shift + 1) % hexes.size() == 0) || (y + x_shift + 1) % hexes[0].size() == 0;
    if (way == "LEFT_UP")
        return (y % 2 == 0 && (x + y_shift) % hexes.size() == 0) || (y + x_shift) % hexes[0].size() == 0;
    if (way == "LEFT_DOWN")
        return (y % 2 && (x + y_shift + 1) % hexes.size() == 0) || (y + x_shift) % hexes[0].size() == 0;

    qDebug() << "ERROR: there is no such way!!!";
    return false;
}
QPointF Field::coordinates(int i, int j)
{
    int org_j = j;
    i = (i + y_shift) % hexes.size();
    j = (j + x_shift) % hexes[0].size();

    return QPointF((constants->hexWidth * (1 - constants->hexShift)) * j,
                                constants->hexHeight * (i + (org_j % 2) / 2.0));
}
void Field::resize(qreal W, qreal H)
{
    GraphicObject::resize(W, H);

    for (int i = 0; i < hexes.size(); ++i)
    {
        for (int j = 0; j < hexes[i].size(); ++j)
        {
           hexes[i][j]->setPos(coordinates(i, j));
           hexes[i][j]->resize(constants->hexWidth, constants->hexHeight);
        }
    }

    oneHexShift["UP"] = QPointF(0, constants->hexHeight);
    oneHexShift["DOWN"] = QPointF(0, -constants->hexHeight);
    oneHexShift["LEFT_UP"] = QPointF(constants->hexWidth * (1 - constants->hexShift),
                                                                    constants->hexHeight / 2);
    oneHexShift["LEFT_DOWN"] = QPointF(constants->hexWidth * (1 - constants->hexShift),
                                                                    -constants->hexHeight / 2);
    oneHexShift["RIGHT_UP"] = QPointF(-constants->hexWidth * (1 - constants->hexShift),
                                                                    constants->hexHeight / 2);
    oneHexShift["RIGHT_DOWN"] = QPointF(-constants->hexWidth * (1 - constants->hexShift),
                                                                    -constants->hexHeight / 2);
}

Hex *Field::HexAt(QPointF point)
{
    foreach (QList <Hex *> list, hexes)
    {
        foreach (Hex * hex, list)
        {
            if (hex->contains(hex->mapFromItem(this, point)))
                return hex;
        }
    }
    return NULL;
}

// TODO безобразие с полем
void Field::moveDown()  // FAIL четыре одинаковые функции
{
    int moved = (hexes.size() - 1 - y_shift) % hexes.size();
    for (int i = 0; i < hexes[moved].size(); ++i)
    {
        Hex * Imaginarium = new Hex(hexes[moved][i]);  // А вот и призрак!
        Imaginarium->AnimationStart(OPACITY, 0);
        connect(Imaginarium, SIGNAL(movieFinished()), Imaginarium, SLOT(Delete()));
        Imaginarium->AnimationStart(Y_POS, Imaginarium->y() + constants->hexWidth);

        hexes[moved][i]->moveBy(0, -constants->hexHeight * hexes.size());
        hexes[moved][i]->setOpacity(0);
        hexes[moved][i]->AnimationStart(OPACITY, 1);
    }

    y_shift = (y_shift + 1 + hexes.size()) % hexes.size();

    AnimateHexes();
}
void Field::moveUp()
{
    int moved = (hexes.size() - y_shift) % hexes.size();
    for (int i = 0; i < hexes[moved].size(); ++i)
    {
        Hex * Imaginarium = new Hex(hexes[moved][i]);  // А вот и призрак!
        Imaginarium->AnimationStart(OPACITY, 0);
        connect(Imaginarium, SIGNAL(movieFinished()), Imaginarium, SLOT(Delete()));
        Imaginarium->AnimationStart(Y_POS, Imaginarium->y() - constants->hexWidth);

        hexes[moved][i]->moveBy(0, constants->hexHeight * hexes.size());

        hexes[moved][i]->setOpacity(0);
        hexes[moved][i]->AnimationStart(OPACITY, 1);
    }

    y_shift = (y_shift - 1 + hexes.size()) % hexes.size();

    AnimateHexes();
}
void Field::moveRight()
{
    int moved = (hexes[0].size() - 1 - x_shift) % hexes[0].size();
    for (int i = 0; i < hexes.size(); ++i)
    {
        Hex * Imaginarium = new Hex(hexes[i][moved]);  // А вот и призрак!
        Imaginarium->AnimationStart(OPACITY, 0);
        connect(Imaginarium, SIGNAL(movieFinished()), Imaginarium, SLOT(Delete()));
        Imaginarium->AnimationStart(X_POS,   Imaginarium->x() + constants->hexHeight);

        hexes[i][moved]->moveBy(
                                 -(constants->hexWidth * (1 - constants->hexShift)) * hexes[0].size(), 0);

        hexes[i][moved]->setOpacity(0);
        hexes[i][moved]->AnimationStart(OPACITY, 1);
    }

    x_shift = (x_shift + 1 + hexes[0].size()) % hexes[0].size();

    AnimateHexes();
}
void Field::moveLeft()
{
    int moved = (hexes[0].size() - x_shift) % hexes[0].size();
    for (int i = 0; i < hexes.size(); ++i)
    {
        Hex * Imaginarium = new Hex(hexes[i][moved]);  // А вот и призрак!
        Imaginarium->AnimationStart(OPACITY, 0);
        connect(Imaginarium, SIGNAL(movieFinished()), Imaginarium, SLOT(Delete()));
        Imaginarium->AnimationStart(X_POS,   Imaginarium->x() - constants->hexHeight);

        hexes[i][moved]->moveBy(
                                 (constants->hexWidth * (1 - constants->hexShift)) * hexes[0].size(), 0);

        hexes[i][moved]->setOpacity(0);
        hexes[i][moved]->AnimationStart(OPACITY, 1);
    }

    x_shift = (x_shift - 1 + hexes[0].size()) % hexes[0].size();

    AnimateHexes();
}

void Field::fieldXmoveFinished()
{

}
void Field::fieldYmoveFinished()
{

}
void Field::shiftDown()
{
    int moved = (hexes.size() - 1 - y_shift) % hexes.size();
    for (int i = 0; i < hexes[moved].size(); ++i)
    {
        Hex * Imaginarium = new Hex(hexes[moved][i]);  // А вот и призрак!
        Imaginarium->AnimationStart(OPACITY, 0);
        connect(Imaginarium, SIGNAL(movieFinished()), Imaginarium, SLOT(Delete()));
        Imaginarium->AnimationStart(Y_POS, Imaginarium->y() + constants->hexWidth);

        hexes[moved][i]->moveBy(0, -constants->hexHeight * hexes.size());
        hexes[moved][i]->setOpacity(0);
        hexes[moved][i]->AnimationStart(OPACITY, 1);
    }

    y_shift = (y_shift + 1 + hexes.size()) % hexes.size();

    AnimateHexes();
}
void Field::AnimateHexes()
{
    for (int i = 0; i < hexes.size(); ++i)
    {
        for (int j = 0; j < hexes[i].size(); ++j)
        {
            hexes[i][j]->AnimationStart(X_POS, coordinates(i, j).x());
            hexes[i][j]->AnimationStart(Y_POS, coordinates(i, j).y());
        }
    }
}
