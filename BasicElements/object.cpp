#include "Object.h"

Object::Object(Object *parent, QString pictureName) :
    QObject(), QGraphicsPixmapItem(pictures->get(pictureName), parent)
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
    foreach (Object * anchor, psevdo_parent)
        anchor->psevdo_children.remove(this);

    isDeleted = true;
    animations->stopAll(this);

    prepareGeometryChange();
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

void Object::resize(qreal w, qreal h)
{
    prepareGeometryChange();

    if (!isNull())
        QGraphicsPixmapItem::setPixmap(original.scaled(w, h, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

    Width = w;
    Height = h;

    this->setTransformOriginPoint(w / 2.0, h / 2.0);  // для вращений и прочего

    resizeChildren(w, h);
}
void Object::setPixmap(const QPixmap &pixmap)
{
    qreal OldWidth = width(), OldHeight = height();
    original = pixmap;
    resize(OldWidth, OldHeight);  // нужно вернуться к старым размерам
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
        obj->ClipWithItem(item);
    }
}
void Object::UnclipWithItem(Object *item)
{
    shapeClips.remove(item);
    foreach (QGraphicsItem * qgi, this->childItems())
    {
        Object * obj = dynamic_cast<Object *>(qgi);
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
                                           qreal target_value, int time, bool isMain)
{
    if (isDeleted)
        qDebug() << "FATAL ERROR: deleted object animated!!!";

    Animation * a = animations->contains(this, type);
    if (a == NULL)
    {
        a = new Animation(this, getFunctions[type], setFunctions[type],
                                         type, target_value, time, isMain);
        connect(a, SIGNAL(finished()), SLOT(animationFinished()));
    }
    else
    {
        a->start(target_value, time, isMain);
    }
    RecheckIsAnimated(true);

    return a;
}
Animation * Object::AnimationStart(QRectF tar, int time, bool isMain)
{
    AnimationStart(X_POS, tar.x(), time, isMain);
    AnimationStart(Y_POS, tar.y(), time, isMain);
    AnimationStart(WIDTH, tar.width(), time, isMain);
    return AnimationStart(HEIGHT, tar.height(), time, isMain);
}

void Object::RecheckIsAnimated(bool for_sure)
{
    bool NewData = for_sure || animations->contains(this);

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
