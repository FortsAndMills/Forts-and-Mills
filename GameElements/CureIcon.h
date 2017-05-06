#ifndef HEALICON_H
#define HEALICON_H

#include "BasicElements/Object.h"
#include "Technical/Constants.h"

// Полупрозрачный крестик "лечения", который всплывает при соотв. действии

class CureIcon : public Object
{
    Q_OBJECT

public:
    explicit CureIcon(Object * parent) :
        Object(parent, "CureIcon")
    {
        this->setZValue(constants->cureIconZPos);
        this->setOpacity(0);
    }

    void startAnimation()
    {
        this->AnimationStart(OPACITY, 1, constants->cureIconAppearTime);
        this->AnimationStart(X_POS, x() - width() * (constants->cureIconTargetPointWidth - 1) / 2 + width() * constants->cureIconTargetPointX, constants->cureIconAnimationTime);
        this->AnimationStart(Y_POS, y() - height() * (constants->cureIconTargetPointHeight - 1) / 2 + height() * constants->cureIconTargetPointY, constants->cureIconAnimationTime);
        this->AnimationStart(WIDTH, width() * constants->cureIconTargetPointWidth, constants->cureIconAnimationTime);
        this->AnimationStart(HEIGHT, height() * constants->cureIconTargetPointHeight, constants->cureIconAnimationTime);

        // за некоторое время до финала начинаем исчезать
        QTimer::singleShot(constants->cureIconAnimationTime - constants->cureIconAppearTime, this, SLOT(finishAnimation()));
    }

private slots:
    void finishAnimation()
    {
        this->AnimationStart(OPACITY, 0, constants->cureIconAppearTime);
        connect(this, SIGNAL(movieFinished()), SLOT(Delete()));
    }
};

#endif // HEALICON_H
