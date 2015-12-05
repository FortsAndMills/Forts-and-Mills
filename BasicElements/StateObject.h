#ifndef STATEOBJECT_H
#define STATEOBJECT_H

#include "Technical/Constants.h"
#include "GraphicObject.h"

// TODO спасите меня от этого безумия
class StateObject : public GraphicObject
{
    Q_OBJECT
    qreal ChangeTime;

    // РАЗЛИЧНЫЕ ПАРАМЕТРЫ СОСТОЯНИЙ И БАЗОВЫЕ ФУНКЦИИ------------
protected:
    QString cur_picture = "";  // Текущее положение
    QMap <QString, GraphicObject *> objects; // Карта всех состояний
protected:
    void addPicture(QString state, QString pictureName)  // Функция добавления
    {
        objects[state] = new GraphicObject(this, CHILD, pictureName);
        objects[state]->setOpacity(0);
        objects[state]->setZValue(constants->statesZpos);
    }
    void setPictureState(QString state, bool isImmediate = false)  // Функция переключения
    {
        if (objects.contains(state))
        {
            if (cur_picture != "")
            {
                if (isImmediate)
                    objects[cur_picture]->setOpacity(0);
                else
                    objects[cur_picture]->AnimationStart(OPACITY, 0, ChangeTime);
            }

            cur_picture = state;
            if (isImmediate)
                objects[cur_picture]->setOpacity(1);
            else
                objects[cur_picture]->AnimationStart(OPACITY, 1, ChangeTime);
        }
    }
public:
    GraphicObject * cur_object() const
    {
        return objects[cur_picture];
    }

protected:
    QString cur_properties = "";
    QMap <QString, Properties> properties;
    void setPropertyState(QString state)
    {
        if (properties.contains(state))
        {
            cur_properties = state;
            this->setProperties(properties[cur_properties]);
        }
    }

    QString cur_frame = "";
    QMap <QString, QString> frames;
    void setFrameState(QString state)
    {
        if (frames.contains(state))
        {
            cur_frame = state;
            this->frame->setPixmap(pictures->get(frames[cur_frame]));
        }
    }

    QString cur_geometry = "";
    QMap <QString, QRectF> geometries;
    void setGeometryState(QString state, bool isImmediate = false)
    {
        if (geometries.contains(state))
        {
            if (cur_geometry != "" && cur_geometry != state && !isImmediate)
            {
                cur_geometry = state;
                foreach (GraphicObject * pic, objects)
                {
                    pic->AnimationStart(X_POS, geometries[cur_geometry].x() * width(), ChangeTime);
                    pic->AnimationStart(Y_POS, geometries[cur_geometry].y() * height(), ChangeTime);
                    pic->AnimationStart(WIDTH, geometries[cur_geometry].width() * width(), ChangeTime);
                    pic->AnimationStart(HEIGHT, geometries[cur_geometry].height() * height(), ChangeTime);
                }
                layer->AnimationStart(currentRect(), ChangeTime);
            }
            else
            {
                cur_geometry = state;
                setInsideGeometry();
            }
        }
    }

public:
    explicit StateObject(GraphicObject *parent, QString defaultStateName,
                         Properties defaultProperties = 0,
                         QString defaultPictureName = "",
                         QString defaultFrameName = "",
                         QString layerName = "",
                         qreal time = constants->objectStateChangeTime) :
        GraphicObject(parent, 0, "", "", layerName)
    {
        addState(defaultStateName, defaultPictureName, defaultProperties, defaultFrameName,
                 QRectF(0, 0, 1, 1));
        setState(defaultStateName, true);

        ChangeTime = time;

        layer->ClipWithItem(objects[defaultStateName]);
    }
    virtual void Delete()
    {
        foreach (GraphicObject * obj, objects)
            obj->Delete();
        GraphicObject::Delete();
    }

    QRectF boundingRect() const
    {
        return currentRect();
    }
    QPainterPath shape() const
    {
        return (QPainterPath)cur_object()->shape();
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
        foreach (GraphicObject * pic, objects)
        {
            pic->setGeometry(currentRect());
        }

        if (layer != NULL)
            this->layer->setGeometry(currentRect());
    }

    bool contains(const QPointF &point) const
    {
        return objects[cur_picture]->contains(point);
    }

    // ФУНКЦИИ ДЛЯ УДОБСТВА ОПРЕДЕЛЕНИЯ--------------------------------------------
    void addState(QString state, QString pictureName)
    {
        addPicture(state, pictureName);
    }
    void addState(QString state, QString pictureName, Properties flags)
    {
        addState(state, pictureName);
        properties[state] = flags;
    }
    void addState(QString state, QString pictureName, Properties flags, QString frameName)
    {
        addState(state, pictureName, flags);
        frames[state] = frameName;
    }
    void addState(QString state, QString pictureName, Properties flags,
                  QString frameName, QRectF pos)
    {
        addState(state, pictureName, flags, frameName);
        geometries[state] = pos;
    }

    // ФУНКЦИИ ПЕРЕКЛЮЧЕНИЯ-------------------------------------------------------------------
public:
    virtual void setState(QString state, bool isImmediate = false)
    {
        setPictureState(state, isImmediate);
        setFrameState(state); // сначала фрэйм, потом пропертис
        setPropertyState(state);
        setGeometryState(state, isImmediate);
    }
};

#endif // STATEOBJECT_H
