#include "Object.h"

Object::Object(Object *parent, QString pictureName) :
    QObject(), QGraphicsPixmapItem(images->get(pictureName), parent)
{
    this->setTransformationMode(Qt::SmoothTransformation);  // просто настроечки
    this->setCacheMode(QGraphicsItem::NoCache);
    this->setAcceptHoverEvents(false);
    this->setAcceptedMouseButtons(0);

    original = pixmap();
    Width = original.width();
    Height = original.height();

    InitAnimationTypes();
}
void Object::Delete()
{
    foreach (QGraphicsItem * child, this->childItems())
    {
        Object * obj = dynamic_cast<Object *>(child);
        if (obj)
            obj->Delete();
    }

    foreach (Object * anchor, psevdo_parent)
        anchor->psevdo_children.remove(this);

    isDeleted = true;
    animations->stopAll(this);

    prepareGeometryChange();
    if (this->scene())
        this->scene()->removeItem(this);
    this->deleteLater();
}

void Object::anchorTo(Object *anchor)
{
    anchor->psevdo_children << this;
    this->psevdo_parent << anchor;
}
void Object::deanchorFrom(Object *anchor)
{
    anchor->psevdo_children.remove(this);
    if (this->psevdo_parent.contains(anchor))
        this->psevdo_parent.remove(anchor);
}

void Object::setPos(qreal x, qreal y)
{
    foreach (Object * object, psevdo_children)
        object->moveBy(x - this->x(), y - this->y());
    QGraphicsPixmapItem::setPos(x, y);
}

void Object::updatePicture()
{
    prepareGeometryChange();
    QGraphicsPixmapItem::setPixmap(original.scaled(Width, Height, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
}
void Object::resize(qreal w, qreal h)
{
    Width = w;
    Height = h;

    if (!isNull())
        updatePicture();

    this->setTransformOriginPoint(w / 2.0, h / 2.0);  // для вращений и прочего

    resizeChildren(w, h);
}
void Object::setPixmap(const QPixmap &pixmap)
{
    qreal OldWidth = width(), OldHeight = height();
    original = pixmap;
    resize(OldWidth, OldHeight);  // нужно вернуться к старым размерам
}
void Object::setPicture(QString name)
{
    setPixmap(images->get(name));
}

void Object::moveBy(qreal dx, qreal dy)
{
    Animation * X_Animation = animations->contains(this, X_POS);
    Animation * Y_Animation = animations->contains(this, Y_POS);

    if (X_Animation != NULL)
        X_Animation->target += dx;
    if (Y_Animation != NULL)
        Y_Animation->target += dy;

    setPos(x() + dx, y() + dy);
}

void Object::ClipWithItem(Object *item)
{
    shapeClips << item;  // обрезаем себя
    foreach (QGraphicsItem * qgi, this->childItems())  // обрезаем детей
    {
        Object * obj = dynamic_cast<Object *>(qgi);
        if (obj != NULL)
            obj->ClipWithItem(item);
    }
}
void Object::UnclipWithItem(Object *item)
{
    shapeClips.remove(item);
    foreach (QGraphicsItem * qgi, this->childItems())
    {
        Object * obj = dynamic_cast<Object *>(qgi);
        if (obj != NULL)
            obj->UnclipWithItem(item);
    }
}
void Object::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QPainterPath exodus = QPainterPath();  // изначально пустой, что нехорошо
    foreach (Object * obj, shapeClips)
    {
        if (exodus.isEmpty())  // первая обрезка и будет основой
            exodus = mapFromItem(obj, obj->shape());
        else  // остальные пересекаем
            exodus = exodus.intersected(mapFromItem(obj, obj->shape()));
    }

    if (shapeClips.size())  // если изначальная обрезка не ноль, то обрезаем painter
        painter->setClipPath(exodus);

    QGraphicsPixmapItem::paint(painter, option, widget);
}

void Object::InitAnimationTypes()
{
    getFunctions[X_POS] = &Object::x;
    setFunctions[X_POS] = &Object::setX;
    getFunctions[Y_POS] = &Object::y;
    setFunctions[Y_POS] = &Object::setY;
    getFunctions[WIDTH] = &Object::width;
    setFunctions[WIDTH] = &Object::setWidth;
    getFunctions[HEIGHT] = &Object::height;
    setFunctions[HEIGHT] = &Object::setHeight;
    getFunctions[OPACITY] = &Object::opacity;
    setFunctions[OPACITY] = &Object::setOpacity;
    getFunctions[ROTATION] = &Object::rotation;
    setFunctions[ROTATION] = &Object::setRotation;
}
Animation * Object::AnimationStart(ANIMATION_TYPE type,
                                           qreal target_value, int time)
{
    if (isDeleted)
        debug << "FATAL ERROR: deleted object animated!!!";

    Animation * a = animations->contains(this, type);
    if (a == NULL)
    {
        a = new Animation(this, getFunctions[type], setFunctions[type],
                                         type, target_value, time);
        connect(a, SIGNAL(finished()), (Object *)this, SLOT(animationFinished()));
    }
    else
    {
        a->start(target_value, time);
    }
    RecheckIsAnimated();

    return a;
}
Animation * Object::AnimationStart(QRectF tar, int time)
{
    AnimationStart(X_POS, tar.x(), time);
    AnimationStart(Y_POS, tar.y(), time);
    AnimationStart(WIDTH, tar.width(), time);
    return AnimationStart(HEIGHT, tar.height(), time);
}

void Object::RecheckIsAnimated()
{
    bool NewData = animations->contains(this);

    if (NewData && !isAnimatedNow)  // если значение изменилось, вызываем вирт. ф.
        startedAnimation();
    else if (!NewData && isAnimatedNow)
    {
        finishedAnimation();
        emit movieFinished();
    }

    isAnimatedNow = NewData;
}
void Object::animationFinished()
{
    RecheckIsAnimated();
}

void Object::disappear(int time)
{
    this->setEnabled(false);
    this->AnimationStart(QRectF(x() + width() / 2, y() + height() / 2, 0, 0), time);
    connect(this, SIGNAL(movieFinished()), this, SLOT(Delete()));
}
