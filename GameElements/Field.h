#ifndef FIELD
#define FIELD

#include "Technical/Headers.h"
#include "GameElements/Hex.h"

// Игровое поле

class Field : public GraphicObject
{
    Q_OBJECT

public:
    QList <QList <Hex *> > hexes;

    explicit Field(GraphicObject *parent,  Game *game);
    virtual void Delete();

public:
    QMap <WAY, QPointF> oneHexShift;  // смещение на один гекс в данном направлении - полезные константы

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
