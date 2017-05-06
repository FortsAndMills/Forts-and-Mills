#include "SpriteObject.h"

SpriteObject::SpriteObject(GraphicObject *parent, Properties flags,
                           QString pictureName, int speed, bool single, bool start) :
    GraphicObject(parent, flags)
{
    this->FullPicture = images->get(pictureName);
    this->rows = images->picture_rows[pictureName];
    this->columns = images->picture_columns[pictureName];
    this->single = single;  // прокручивается только один раз и удаляется
    this->speed = speed;
    if (speed == -1)  // передача -1 означает дефолтную скорость одного изменения
        this->speed = constants->spriteFrameChangeSpeed * frames();

    if (columns < 2 && rows < 2)
        debug << "NOTE: very strange sprite object!!!\n";
    else if (FullPicture.isNull())
        debug << "ERROR: sprite_object is NULL picture?!?\n";

    this->FrameWidth = (qreal)FullPicture.width() / columns;
    this->FrameHeight = (qreal)FullPicture.height() / rows;
    setFrame(0);

    movie = NULL;
    if (start)
        this->start();
}
void SpriteObject::Delete()
{
    if (movie != NULL)
        movie->stop();
    GraphicObject::Delete();
}

void SpriteObject::setPixmap(QString name)
{
    // для спрайтов предполагаем, что размеры исходных картинок одинаковы
    FullPicture = images->get(name);
    setFrame(frame);
}

void SpriteObject::start()
{
    // начало смены картинок
    movie = new QTimer(this);
    movie->start(speed / (columns * rows));
    connect(movie, SIGNAL(timeout()), SLOT(frameChange()));
}
void SpriteObject::setFrame(int frameIndex)
{
    // установка конкретного кадра
    this->frame = frameIndex % (columns * rows);
    GraphicObject::setPixmap(FullPicture.copy((frame % columns) * FrameWidth,
                                                                             (frame / columns) * FrameHeight,
                                                                                       FrameWidth, FrameHeight));
}

void SpriteObject::frameChange()
{
    if (frame == columns * rows - 1)  // проверка на конец цикла
    {
        emit looped();
        if (single)  // сингл означает удаление в конце первого цикла
        {
            movie->stop();
            return;
        }
    }
    setFrame(frame + 1);
}
