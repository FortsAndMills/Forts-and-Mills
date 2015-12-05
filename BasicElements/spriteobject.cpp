#include "SpriteObject.h"
#include "Technical/Constants.h"

SpriteObject::SpriteObject(GraphicObject *parent, Properties flags,
                           QString pictureName, int speed) :
    GraphicObject(parent, flags)
{
    this->FullPicture = pictures->get(pictureName);
    this->rows = pictures->picture_rows[pictureName];
    this->columns = pictures->picture_columns[pictureName];
    if (columns < 2 && rows < 2)
        qDebug() << "NOTE: very strange sprite object!!!";

    this->FrameWidth = (qreal)FullPicture.width() / columns;
    this->FrameHeight = (qreal)FullPicture.height() / rows;
    setFrame(0);

    movie = new QTimer(this);
    if (speed == -1)
        movie->start(constants->spriteFrameChangeSpeed);
    else
        movie->start(speed / (columns * rows));
    connect(movie, SIGNAL(timeout()), SLOT(frameChange()));
}
void SpriteObject::Delete()
{
    movie->stop();
    GraphicObject::Delete();
}

void SpriteObject::setFrame(int frameIndex)
{
    this->frame = frameIndex % (columns * rows);
    this->setPixmap(FullPicture.copy((frame % columns) * FrameWidth,
                                                               (frame / columns) * FrameHeight,
                                                                FrameWidth, FrameHeight));
}
void SpriteObject::frameChange()
{
    if (frame == columns * rows - 1)
        emit looped();
    setFrame(frame + 1);
}
