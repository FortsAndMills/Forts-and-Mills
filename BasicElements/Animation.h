#ifndef ANIMATION
#define ANIMATION

#include "Technical/Headers.h"
#include "Technical/Constants.h"

class Animation : public QObject
{
    Q_OBJECT

    qreal target;  // Цель
private:
    Object * work_object;  // Объект, с которым проводятся изменения
    ANIMATION_TYPE type;  // Что анимирует

    qreal (Object::*getValue)() const;  // Функции получения и изменения значения
    void (Object::*setValue)(qreal newValue);

public:
    qreal V;  // Скорость

public:
    explicit Animation(Object * work_object,
              qreal (Object::*getValue)() const,
              void (Object::*setValue)(qreal newValue),
              ANIMATION_TYPE type, qreal target, int time, bool isMain = false);
    friend AnimationManager;
    friend Object;

    void stop();  // Останавливает анимацию и сразу переключает на конечное значение
    void start(qreal target_value, int time, bool isMain = false);  // Начинает анимацию с новым target
public slots:
    void frame();  // Реализация одного кадра

signals:
    void finished();  // посылается по завершении анимации
};

#endif
