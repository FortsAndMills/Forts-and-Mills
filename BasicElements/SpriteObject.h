#ifndef SPRITEOBJECT_H
#define SPRITEOBJECT_H

#include "GraphicObject.h"
#include "Technical/Constants.h"

// для объектов, состоящих из анимированных спрайтов

class SpriteObject : public GraphicObject
{
    Q_OBJECT
private:
    QPixmap FullPicture;  // всё просто: спрайт это табличка картинок
    int rows, columns;
    qreal FrameWidth, FrameHeight;  // размеры кадра

    int frame_index;  // текущий кадр
protected:
    int frames() { return rows * columns; }

private:
    bool single;  // по кругу крутить анимацию или только один раз
    int speed = -1;
    QTimer * movie;

public:
    explicit SpriteObject(GraphicObject *parent = 0, Properties flags = 0,
                          QString pictureName = "", int speed = -1, bool single = false, bool start = true);
    virtual void Delete();
    void setFrame(int frameIndex);

    void setPixmap(QString name);

    void start();

private slots:
    void frameChange();
signals:
    void looped();  // вызывается, когда возвращается на нулевой фрэйм.
};

#endif // SPRITEOBJECT_H
