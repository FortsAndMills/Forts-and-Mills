#include "GraphicObject.h"
#include "Technical/Pictures.h"

GraphicObject::GraphicObject(GraphicObject *parent, Properties flags,
                             QString pictureName, QString framePictureName,
                             QString layerName) : Object(parent, pictureName)
{
    layer = new Object(this, layerName);
    layer->setVisible(false);
    if (!isNull())
        layer->ClipWithItem(this);

    frame = new Object(this, framePictureName);
    frame->setVisible(false);

    this->setProperties(flags);
}
void GraphicObject::setProperties(Properties newFlags)
{
    this->flags = newFlags;

    if ((flags & CLICKABLE) && (flags & PUSHABLE))
    {
        qDebug() << "NOTE: graphic object is clickable and pushable simulteniously!";
    }

    setAcceptHoverEvents(!frame->isNull() ||(flags & (CHILD | WHEEL)));
    this->setAcceptedMouseButtons((Qt::MouseButton)
              ((Qt::LeftButton * (bool)(flags & (CLICKABLE | DRAGABLE | CHILD | PUSHABLE))) |
               (Qt::RightButton * (bool)(flags & (RIGHT_CLICKABLE | CHILD)))));

    this->setFlag(QGraphicsItem::ItemClipsToShape, !this->isNull() &&
                  (this->acceptHoverEvents() || this->acceptedMouseButtons() != 0));
}
void GraphicObject::Delete()
{
    frame->Delete();
    layer->Delete();
    Object::Delete();
}

void GraphicObject::resize(qreal W, qreal H)
{
    Object::resize(W, H);

    frame->setGeometry(0, 0, width(), height());
    layer->setGeometry(0, 0, width(), height());
}

void GraphicObject::mousePressEvent(QGraphicsSceneMouseEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mousePressEvent(qme);

    if (qme->button() == Qt::LeftButton)
    {
        dontRelease = false;

        if (flags & DRAGABLE)
        {
            cursor_position = qme->screenPos();

            dragStart();
            emit dragStarted();
        }

        if (flags & CLICKABLE)
        {
            leftClick();
            emit leftClicked();

            isClicked = true;

            layer->setVisible(true);
        }

        if ((flags & PUSHABLE) && !isClicked)
        {
            dontRelease = true;
            isClicked = true;

            push(true);
            emit pushed(true);

            layer->setVisible(true);
        }
    }
    else if (qme->button() == Qt::RightButton)
    {
        if (flags & RIGHT_CLICKABLE)
        {
            rightClick();
            emit rightClicked();
        }
    }
}
void GraphicObject::mouseMoveEvent(QGraphicsSceneMouseEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mouseMoveEvent(qme);

    if ((flags & DRAGABLE) && (qme->buttons() & Qt::LeftButton))
    {
        this->moveBy(qme->screenPos() - cursor_position);
        cursor_position = qme->screenPos();
    }
}
void GraphicObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mouseReleaseEvent(qme);

    if (flags & PUSHABLE)
    {
        if (isClicked && !dontRelease)
        {
            isClicked = false;

            push(false);
            emit pushed(false);

            layer->setVisible(false);
        }
        dontRelease = false;
    }
    else
    {
        isClicked = false;
        layer->setVisible(false);
    }

    if (qme->button() == Qt::LeftButton)
    {
        if (flags & DRAGABLE)
        {
            dragFinish();
            emit dragFinished();
        }
    }
}

void GraphicObject::hoverEnterEvent(QGraphicsSceneHoverEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->hoverEnterEvent(qme);

    frame->setVisible(true);

    enter();
    emit entered();
}
void GraphicObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->hoverLeaveEvent(qme);

    frame->setVisible(false);

    leave();
    emit left();
}

void GraphicObject::wheelEvent(QGraphicsSceneWheelEvent *qwe)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->wheelEvent(qwe);

    if (flags & WHEEL)
    {
        wheel(qwe->delta());
        emit wheeled(qwe->delta());
    }
}
