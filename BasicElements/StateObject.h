#ifndef STATEOBJECT_H
#define STATEOBJECT_H

#include "Technical/Constants.h"
#include "GraphicObject.h"

// объект с несколькими "состояниями"
// "состояние" предполагает свою картинку или свой размер
// смена состояний - плавная, одна картинка должна появится, другая исчезнуть
// поэтому приходится хранить объекты для каждой картинки

class StateObject : public GraphicObject
{
    Q_OBJECT
    qreal ChangeTime;

protected:
    QString cur_picture;  // Текущее положение
    QMap <QString, GraphicObject *> pictures; // Карта всех состояний
public:
    void addPicture(QString state, QString pictureName)  // Функция добавления нового состояния
    {
        pictures[state] = new GraphicObject(this, (this->flags != 0) * CHILD, pictureName);
        pictures[state]->setOpacity(0);
        pictures[state]->setZValue(constants->statesZpos);
    }
    void setPictureState(QString state, bool isImmediate = false)  // Функция переключения
    {
        if (isImmediate)  // мгновенная смена без анимации
            pictures[cur_picture]->setOpacity(0);
        else
            pictures[cur_picture]->AnimationStart(OPACITY, 0, ChangeTime);

        cur_picture = state;

        if (isImmediate)
            pictures[cur_picture]->setOpacity(1);
        else
            pictures[cur_picture]->AnimationStart(OPACITY, 1, ChangeTime);
    }
public:
    GraphicObject * cur_object() const
    {
        return pictures[cur_picture];
    }

protected:
    // всё тоже самое, но для геометрии
    QString cur_geometry = "";
    QMap <QString, QRectF> geometries;
    void addGeometry(QString state, QRectF geometry)
    {
        geometries[state] = geometry;
    }
    void setGeometryState(QString state, bool isImmediate = false)
    {
        if (geometries.contains(state))
        {
            if (cur_geometry != state && !isImmediate)
            {
                cur_geometry = state;
                foreach (GraphicObject * pic, pictures)
                {
                    pic->AnimationStart(X_POS, geometries[cur_geometry].x() * width(), ChangeTime);
                    pic->AnimationStart(Y_POS, geometries[cur_geometry].y() * height(), ChangeTime);
                    pic->AnimationStart(WIDTH, geometries[cur_geometry].width() * width(), ChangeTime);
                    pic->AnimationStart(HEIGHT, geometries[cur_geometry].height() * height(), ChangeTime);
                }

                layer->AnimationStart(currentRect(), ChangeTime);
                frame->AnimationStart(currentRect(), ChangeTime);
            }
            else if (isImmediate)
            {
                cur_geometry = state;
                setInsideGeometry();
            }
        }
    }

public:
    explicit StateObject(GraphicObject *parent,
                         QString defaultStateName,
                         QString defaultPictureName,
                         Properties properties = 0,
                         QString frameName = "",
                         QString layerName = "",
                         qreal time = constants->stateObjectChangeTime) :
        GraphicObject(parent, properties, "", frameName, layerName)
    {
        addPicture(defaultStateName, defaultPictureName);
        pictures[defaultStateName]->setOpacity(1);
        cur_picture = defaultStateName;

        // дефолтное геометрическое состояние всегда 0 0 1 1
        addGeometry(defaultStateName, QRectF(0, 0, 1, 1));
        cur_geometry = defaultStateName;

        ChangeTime = time;  // время смены состояний

        // по умолчанию, фрейм и лэйр обрезаются родителем
        // но этот родитель пуст и не является картинкой
        // поэтому по умолчанию в GraphicObject обрезки не происходит
        layer->ClipWithItem(this);
        frame->ClipWithItem(this);
    }
    virtual void Delete()
    {
        foreach (GraphicObject * obj, pictures)
            obj->Delete();
        GraphicObject::Delete();
    }

    QRectF currentRect() const
    {
        return QRectF(geometries[cur_geometry].x() * width(),
                                  geometries[cur_geometry].y() * height(),
                                  geometries[cur_geometry].width() * width(),
                                  geometries[cur_geometry].height() * height());
    }
    virtual void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);
        setInsideGeometry();
    }
    void setInsideGeometry()
    {
        // при изменении размера приходятся менять размер всех возможных состояний
        foreach (GraphicObject * pic, pictures)
        {
            pic->setGeometry(currentRect());
        }

        if (layer != NULL)
            this->layer->setGeometry(currentRect());
        if (frame != NULL)
            this->frame->setGeometry(currentRect());
    }

    QRectF boundingRect() const
    {
        return currentRect();
    }
    QPainterPath shape() const
    {
        return mapFromItem(cur_object(), (QPainterPath)cur_object()->shape());
    }
    bool contains(const QPointF &point) const
    {
        return pictures[cur_picture]->contains(point);
    }
};

#endif // STATEOBJECT_H
