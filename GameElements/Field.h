#ifndef FIELD
#define FIELD

#include "Technical/Headers.h"
#include "GameElements/Hex.h"

// смещение на один гекс в данном направлении - полезные константы
// не пригодились?
//static const QPointF oneHexShift(WAY way)
//{
//    switch (way)
//    {
//    case UP: return QPointF(0, constants->hexHeight);
//    case DOWN: return QPointF(0, -constants->hexHeight);
//    case LEFT_UP: return QPointF(constants->hexWidth * (1 - constants->hexShift),
//                                     constants->hexHeight / 2);
//    case LEFT_DOWN: return QPointF(constants->hexWidth * (1 - constants->hexShift),
//                                   -constants->hexHeight / 2);
//    case RIGHT_UP: return QPointF(-constants->hexWidth * (1 - constants->hexShift),
//                                  constants->hexHeight / 2);
//    case RIGHT_DOWN: return QPointF(-constants->hexWidth * (1 - constants->hexShift),
//                                    -constants->hexHeight / 2);
//    default: qDebug() << "ERROR: no way hex shift";
//    };
//    return QPointF();
//};


// Игровое поле
class Field : public GraphicObject
{
    Q_OBJECT

public:
    QList <QList <Hex *> > hexes;

    explicit Field(GraphicObject *parent,  Game *game);
    virtual void Delete();

public:
    bool isSplitted(int x, int y, WAY way);  // приводит ли перемещение из данного гекса в данном направлении к разрыву поля
    Hex * HexAt(QPointF point);  // найти гекс под этой точкой экрана

private:
    QPointF coordinates(int i, int j);  // возвращает координаты данного гекса с учётом сдвига
    void resize(qreal W, qreal H);

private:
    int x_shift = 0, y_shift = 0;  // Перемещение поля
    void AnimateHexes();
public slots:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
};

#endif
