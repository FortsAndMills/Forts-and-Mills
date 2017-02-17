#include "SpriteObject.h"

SpriteObject::SpriteObject(GraphicObject *parent, Properties flags,
                           QString pictureName, int speed, bool single, bool start) :
    GraphicObject(parent, flags)
{
    this->FullPicture = images->get(pictureName);
    this->rows = images->picture_rows[pictureName];
    this->columns = images->picture_columns[pictureName];
    this->single = single;
    this->speed = speed;
    if (speed == -1)
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
    FullPicture = images->get(name);
    setFrame(frame);
}

void SpriteObject::start()
{
    movie = new QTimer(this);
    movie->start(speed / (columns * rows));
    connect(movie, SIGNAL(timeout()), SLOT(frameChange()));
}
void SpriteObject::setFrame(int frameIndex)
{
    this->frame = frameIndex % (columns * rows);
    GraphicObject::setPixmap(FullPicture.copy((frame % columns) * FrameWidth,
                                                               (frame / columns) * FrameHeight,
                                     FrameWidth, FrameHeight));
}

void SpriteObject::frameChange()
{
    if (frame == columns * rows - 1)
    {
        emit looped();
        if (single)
        {
            movie->stop();
            return;
        }
    }
    setFrame(frame + 1);
}
