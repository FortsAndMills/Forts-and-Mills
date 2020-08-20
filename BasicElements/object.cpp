#include "Object.h"

Object::Object(Object *parent, QString pictureName) :
    QObject(), QGraphicsPixmapItem(images->get(pictureName), parent)
{
    this->setTransformationMode(Qt::SmoothTransformation);  // просто настроечки
    this->setCacheMode(QGraphicsItem::NoCache);  // вроде так лучше
    this->setAcceptHoverEvents(false);  // по умолчанию, не интерактивен
    this->setAcceptedMouseButtons(0);

    original = pixmap();  // сохранение картинки
    Width = original.width();
    Height = original.height();

    // инициализация ссылок на функции для передачи в класс анимации
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

    foreach (Object * anchor, pseudo_parent)
        anchor->pseudo_children.remove(this);

    isDeleted = true;
    animations->stopAll(this);

    prepareGeometryChange();
    if (this->scene())
        this->scene()->removeItem(this);
    this->deleteLater();
}

// привязка и отвязка к другим объектам
void Object::anchorTo(Object *anchor)
{
    anchor->pseudo_children << this;
    this->pseudo_parent << anchor;
}
void Object::deanchorFrom(Object *anchor)
{
    anchor->pseudo_children.remove(this);
    if (this->pseudo_parent.contains(anchor))
        this->pseudo_parent.remove(anchor);
}

// при изменении позиции перемещаются и псевдодети
void Object::setPos(qreal x, qreal y)
{
    foreach (Object * object, pseudo_children)
        object->moveBy(x - this->x(), y - this->y());
    QGraphicsPixmapItem::setPos(x, y);
}

void Object::updatePicture()
{
    prepareGeometryChange();  // документация рекомендует
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
    // функция меняет картинку с сохранением текущего размера объекта

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
    // Если этот объект движется в некотором направлении,
    // а его нужно сместить, то целевая переменная также смещается
    Animation * X_Animation = animations->contains(this, X_POS);
    Animation * Y_Animation = animations->contains(this, Y_POS);

    if (X_Animation != NULL)
        X_Animation->target += dx;
    if (Y_Animation != NULL)
        Y_Animation->target += dy;

    setPos(x() + dx, y() + dy);
}

// костыли для корректной обрезки
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
// общая функция старта анимации для этого объекта
Animation * Object::AnimationStart(ANIMATION_TYPE type,
                                           qreal target_value, int time)
{
    if (isDeleted)
        debug << "FATAL ERROR: deleted object animated!!!";

    Animation * a = animations->contains(this, type);
    if (a == NULL)  // создание новой анимации
    {
        a = new Animation(this, getFunctions[type], setFunctions[type],
                                         type, target_value, time);

        // хотим получить оповещение по завершению анимации
        connect(a, SIGNAL(finished()), (Object *)this, SLOT(animationFinished()));
    }
    else
    {
        a->start(target_value, time);  // перезагрузка старой
    }
    // пересчитываем, анимируется ли сейчас объект
    RecheckIsAnimated();

    return a;
}
// удобные обобщения
Animation * Object::AnimationStart(QRectF tar, int time)
{
    AnimationStart(X_POS, tar.x(), time);
    AnimationStart(Y_POS, tar.y(), time);
    AnimationStart(WIDTH, tar.width(), time);
    return AnimationStart(HEIGHT, tar.height(), time);
}
void Object::disappear(int time)
{
    // "сжимание в точку":
    this->setEnabled(false);
    this->AnimationStart(QRectF(x() + width() / 2, y() + height() / 2, 0, 0), time);
    // по завершении удаление
    connect(this, SIGNAL(movieFinished()), this, SLOT(Delete()));
}

void Object::RecheckIsAnimated()
{
    // пересчёт текущего состояния анимаций
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
    // ри оповещении о завершении какой-то анимации, производим пересчёт
    RecheckIsAnimated();
}
