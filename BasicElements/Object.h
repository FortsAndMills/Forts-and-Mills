#ifndef OBJECT
#define OBJECT

#include "Technical/Headers.h"
#include "Technical/Pictures.h"
#include "Technical/AnimationManager.h"
#include "Animation.h"

// Базовый графический объект. Объединяет QObject и QGraphicsPixmapItem
// Основные настройки графики и размеров
// Анимации и контроль анимаций

class Object : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

// Инициализация и удаление
public:
    explicit Object(Object * parent = NULL, QString pictureName = "");
public slots:
    virtual void Delete();
public:
    bool isDeleted = false;  // удалён ли объект


// Контроль размера объекта и его картинки
public:
    virtual void resizeChildren(qreal, qreal) {}
    virtual void resize(qreal w, qreal h);

private:
    QPixmap original;
public:
    bool isNull() { return original.isNull(); }
    void setPixmap(const QPixmap &pixmap);  // установка картинки

// Недети
private:
    QSet <Object *> psevdo_children;
    QSet <Object *> psevdo_parent;
public:
    void deanchorFrom(Object * anchor);
    void anchorTo(Object * anchor);
    void setPos(qreal x, qreal y);
    void setPos(QPointF pos) { setPos(pos.x(), pos.y()); }
    void setX(qreal x) { setPos(x, y()); }
    void setY(qreal y) { setPos(x(), y); }

// Геометрия - удобные функции
private:
    qreal Width, Height;  // FAIL: объекты без картинки всегда имеют нулевой размер...
public:
    qreal width() const { return Width; }
    qreal height() const { return Height; }
    void setWidth(qreal W) { resize(W, height()); }
    void setHeight(qreal H) { resize(width(), H); }

    void setGeometry(qreal x, qreal y, qreal w, qreal h)
    {
        setPos(x, y);
        resize(w, h);
    }
    void setGeometry(QRectF rect)
    {
        setGeometry(rect.x(), rect.y(), rect.width(), rect.height());
    }

    void moveBy(qreal dx, qreal dy);  // FAIL где анимация, где реальность?
    void moveBy(QPointF delta) { moveBy(delta.x(), delta.y()); }


// А вот и аналог знаменитейшего ItemClipsChildrenToShape
private:
    QSet <Object *> shapeClips;
public:
    void ClipWithItem(Object * item);
    void UnclipWithItem(Object * item);
    virtual void paint(QPainter * painter, const QStyleOptionGraphicsItem * option,
               QWidget * widget = 0);


// Создание анимаций
private:
    QMap<ANIMATION_TYPE, qreal (Object::*)() const > getFunctions;
    QMap<ANIMATION_TYPE, void (Object::*)(qreal) > setFunctions;
    void InitAnimationTypes();
public:
    Animation * AnimationStart(ANIMATION_TYPE type,
                               qreal target_value, int time = constants->defaultAnimationTime,
                               bool isMain = false);
    Animation * AnimationStart(QRectF tar, int time, bool isMain = false);


// проблема анимированности
public:
    bool isAnimatedNow = false;  // чтобы знать, анимирована ли в данный момент
    void RecheckIsAnimated(bool for_sure = false);
protected:
    virtual void startedAnimation() {}  // для переопределения
    virtual void finishedAnimation() {}
private slots:
    void animationFinished();
signals:
    void movieFinished();
};

#endif
