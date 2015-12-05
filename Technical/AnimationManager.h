#ifndef ANIMATIONMANAGER_H
#define ANIMATIONMANAGER_H

#include "BasicElements/Object.h"
#include "BasicElements/Animation.h"
#include "Technical/Constants.h"

class AnimationManager : public QTimer  // для менеджмента анимациями
{
    Q_OBJECT

    QSet <Animation *> animations;  // все работающие анимации
    QSet <Animation *> main_animations;  // главные анимации

public:
    explicit AnimationManager() : QTimer()
    {
        this->setInterval(constants->framesPerSecond);
        connect(this, SIGNAL(timeout()), this, SLOT(frame()));
    }
private:
    void start(Animation * New, bool isMain = false)
    {
        animations.insert(New);
        if (isMain)
            main_animations.insert(New);

        if (!this->isActive())
            QTimer::start();
    }
    void stop(Animation * Old)
    {
        animations.remove(Old);
        if (main_animations.remove(Old) && main_animations.size() == 0)
            emit mainAnimationFinished();

        if (animations.size() == 0)
            QTimer::stop();
    }

public slots:
    void stopAll()
    {
        QSet <Animation *> all = animations;  // с целью корректной работы foreach и QSet
        foreach (Animation * a, all)
            a->stop();

        if (main_animations.size() != 0)
        {
            main_animations.clear();
            emit mainAnimationFinished();
        }
    }
    void stopAll(Object * obj)  // для данного объекта
    {
        QSet <Animation *> all = animations;
        foreach (Animation * a, all)
            if (a->work_object == obj)
                stop(a);
    }

    void frame()
    {
        foreach (Animation * an, animations)
            an->frame();
    }

private:
    bool contains(Object * obj)  // является ли данный объект анимированным сейчас
    {
        foreach (Animation * a, animations)
            if (a->work_object == obj)
                return true;
        return false;
    }
    Animation * contains(Object * obj, ANIMATION_TYPE type)
    {  // найти анимацию данного объекта данного типа
        foreach (Animation * a, animations)
            if (a->work_object == obj && a->type == type)
                return a;
        return NULL;
    }

public:
    void checkIfMainAnimationFinished()  // если нет главных анимаций, высылает сигнал об их завершении
    {
        if (main_animations.size() == 0)
            emit mainAnimationFinished();
    }

    friend Animation;  // всё частное - для друзей
    friend Object;

signals:
    void mainAnimationFinished();
};

#endif // ANIMATIONMANAGER_H
