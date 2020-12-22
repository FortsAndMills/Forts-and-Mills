#ifndef ANIMATION
#define ANIMATION

#include "Technical/Headers.h"
#include "Technical/Constants.h"

// Базовый класс всех анимаций, происходящих в игре
class Animation : public QObject
{
    Q_OBJECT

private:
    qreal target;  // Целевое значение
    Object * work_object;  // Объект, с которым проводятся изменения
    ANIMATION_TYPE type;  // Что анимирует

    qreal (Object::*getValue)() const;  // Функции получения и изменения значения
    void (Object::*setValue)(qreal newValue);

    qreal V;  // Скорость

public:
    explicit Animation(Object * work_object,
              qreal (Object::*getValue)() const,
              void (Object::*setValue)(qreal newValue),
              ANIMATION_TYPE type, qreal target, int time);
    friend AnimationManager;

    void stop();  // Останавливает анимацию и сразу переключает на конечное значение
    void start(qreal target_value, int time);  // Начинает анимацию с новым target

    void move_by(qreal dx, qreal dy) // если объект переместился на (dx, dy)
    {
        if (type == X_POS)
            target += dx;
        if (type == Y_POS)
            target += dy;
    }

private slots:
    void frame();  // Реализация одного кадра

signals:
    void finished();  // посылается по завершении анимации
};

#endif
