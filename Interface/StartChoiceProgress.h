#ifndef STARTCHOICEPROGRESS_H
#define STARTCHOICEPROGRESS_H

#include "BasicElements/GraphicObject.h"

class StartChoiceProgress : public GraphicObject
{
    Q_OBJECT

private:
    Object * frame;
    Object * progress;
    int stage = 0, stages;

public:
    StartChoiceProgress(GraphicObject * parent, PlayerColor color, int stages) :
        GraphicObject(parent, RIGHT_CLICKABLE, "startChoiceProgressBase")
    {
        this->progress = new Object(this, "startChoiceBar" + color);
        this->frame = new Object(this, "startChoiceProgressFrame");

        this->setFlag(ItemClipsChildrenToShape);
        this->stages = stages;
    }
    void Delete() override
    {
        frame->Delete();
        progress->Delete();
    }

    void resize(qreal W, qreal H) override
    {
        GraphicObject::resize(W, H);

        this->frame->setGeometry(0, 0, W, H);
        this->progress->setGeometry(5, 2, W * stage / stages - 10, H - 4);
    }

    void expand()
    {
        ++stage;
        this->progress->AnimationStart(QRectF(5, 2, width() * stage / stages - 10, height() - 4), constants->progressBarUpdateTime);

    }
};

#endif // STARTCHOICEPROGRESS_H
