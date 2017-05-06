#ifndef MERGINGOBJECT_H
#define MERGINGOBJECT_H

#include "GraphicObject.h"

// Мигающий объект (информация на гексах, например)

class MergingObject : public GraphicObject
{
    Q_OBJECT

public:
    QString name;

    explicit MergingObject(GraphicObject * parent, QString name, Properties flags = 0) :
        GraphicObject(parent, flags, name)
    {
        this->name = name;

        // изначально не виден и начинает мигать
        this->setOpacity(0);
        merge();
    }    

    // при сигнале удаления этот объект удаляется при очередном исчезновении
    bool wantToDelete = false;
    void DeleteOnMerging()
    {
        wantToDelete = true;
    }

private slots:
    // мигание
    void merge()
    {
        // если видны - то следующее целевое значение прозрачности 0, иначе 1
        int target = opacity() == 0 ? 1 : 0;

        // проверка на удаление
        if (wantToDelete && target == 1)
            Delete();
        else
            connect(AnimationStart(OPACITY, target, constants->mergeTime), SIGNAL(finished()), SLOT(merge()));
    }
};

#endif // MERGINGOBJECT_H
