#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include "GraphicObject.h"
#include "Technical/Constants.h"

class SpriteObject : public GraphicObject
{
    Q_OBJECT
private:
    QPixmap FullPicture;  // всё просто
    int rows, columns;
    qreal FrameWidth, FrameHeight;
    int frame;

    QTimer * movie;

public:
    explicit SpriteObject(GraphicObject *parent = 0, Properties flags = 0,
                          QString pictureName = "", int speed = -1);
    virtual void Delete();
    void setFrame(int frameIndex);

public slots:
    void frameChange();
    void setVisibleFalse() { setVisible(false); }  // для отключения в конце лупа.
signals:
    void looped();  // вызывается, когда возвращается на нулевой фрэйм.
};

#endif // SPRITEOBJECT_H
