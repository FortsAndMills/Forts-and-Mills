#include "Object.h"

Animation::Animation(Object *work_object,
                                      qreal (Object::*getValue)() const,
                                      void (Object::*setValue)(qreal newValue),
                                      ANIMATION_TYPE type,
                                       qreal target, int time) : QObject(work_object)
{
    this->type = type;
    this->work_object = work_object;
    this->getValue = getValue;
    this->setValue = setValue;

    // анимация стартует при инициализации
    this->start(target, time);
}

void Animation::start(qreal target_value, int time)  // TODO анимации с ускорением
{
    this->target = target_value;

    // Выбираем нужную скорость
    qreal value = (work_object->*getValue)();
    V = (target - value) / time * constants->framesPerSecond;

      // проверка на то, что анимация уже завершена
    if (qAbs(V) < constants->minAnimationVelocity)
    {
        (work_object->*setValue)(target_value);
        emit finished();  // посылаем сигнал завершения
    }
    else
    {
        animations->start(this);  // Добавляемся в список анимаций
    }
}
void Animation::stop()
{
    animations->stop(this);  // удаляем из списка анимаций
    (work_object->*setValue)(target);  // устанавливаем конечное значение

    emit finished();  // посылаем сигнал завершения
}
void Animation::frame()
{
    qreal newValue;
    //try
    //{
    newValue = (work_object->*getValue)() + V;  // новое значение
    //}
    //catch (...) { animations->stop(this); return; }

    if ((V >= 0 && newValue >= target) ||  // проверка на завершение
         (V <= 0 && newValue <= target))
    {
        stop();
    }
    else  // установка нового значения
    {
        (work_object->*setValue)(newValue);
    }
}
