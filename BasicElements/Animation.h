#ifndef ANIMATION
#define ANIMATION

#include "Technical/Headers.h"
#include "Technical/Constants.h"

// Базовый класс всех анимаций, происходящих в игре
class Animation : public QObject
{
    Q_OBJECT

    qreal target;  // Цель
private:
    Object * work_object;  // Объект, с которым проводятся изменения
    ANIMATION_TYPE type;  // Что анимирует

    qreal (Object::*getValue)() const;  // Функции получения и изменения значения
    void (Object::*setValue)(qreal newValue);

private:
    qreal V;  // Скорость

public:
    explicit Animation(Object * work_object,
              qreal (Object::*getValue)() const,
              void (Object::*setValue)(qreal newValue),
              ANIMATION_TYPE type, qreal target, int time);
    friend AnimationManager;
    friend Object;  // для доступа к target :(

    void stop();  // Останавливает анимацию и сразу переключает на конечное значение
    void start(qreal target_value, int time);  // Начинает анимацию с новым target
private slots:
    void frame();  // Реализация одного кадра

signals:
    void finished();  // посылается по завершении анимации
};

#endif
