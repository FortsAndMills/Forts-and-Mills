#ifndef MERGINGOBJECT_H
#define MERGINGOBJECT_H

#include "GraphicObject.h"

class MergingObject : public GraphicObject
{
    Q_OBJECT

public:
    QString name;
    bool wantToDelete = false;

    explicit MergingObject(GraphicObject * parent, QString name, Properties flags = 0) :
        GraphicObject(parent, flags, name)
    {
        this->name = name;
        this->setOpacity(0);
        merge();
    }

    void DeleteOnMerging()
    {
        wantToDelete = true;
    }

private slots:
    void merge()
    {
        int target = opacity() == 0 ? 1 : 0;
        if (wantToDelete && target == 1)
            Delete();
        else
            connect(AnimationStart(OPACITY, target, constants->mergeTime), SIGNAL(finished()), SLOT(merge()));
    }
};

#endif // MERGINGOBJECT_H
