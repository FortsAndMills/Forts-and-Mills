#ifndef FIELD
#define FIELD

#include "Technical/Headers.h"
#include "GameElements/Hex.h"

class Field : public GraphicObject
{
    Q_OBJECT

public:
    QList <QList <Hex *> > hexes;

    explicit Field(GraphicObject *parent,  Game *game);
    virtual void Delete();

public:
    bool isSplitted(int x, int y, WAY way);
    QMap <WAY, QPointF> oneHexShift;  // смещение на один гекс в данном направлении
private:
    QPointF coordinates(int i, int j);  // возвращает координаты данного гекса с учётом сдвига
    void resize(qreal W, qreal H);

public:
    Hex * HexAt(QPointF point);  // найти гекс под этой точкой


private:
    int x_shift = 0, y_shift = 0;  // Перемещение поля
    int want_x_shift = 0, want_y_shift = 0;
    void AnimateHexes();
public slots:
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
private slots:
    void fieldXmoveFinished();
    void fieldYmoveFinished();
    void shiftDown();
};

#endif
