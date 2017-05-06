#ifndef GRAPHICOBJECT_H
#define GRAPHICOBJECT_H

#include "Technical/Headers.h"
#include "Technical/Pictures.h"
#include "BasicElements/Object.h"

// Базовый класс для интерактивных объектов

#define Properties unsigned int
#define DRAGABLE 1
#define CLICKABLE 2
#define RIGHT_CLICKABLE 4
#define PUSHABLE 8
#define WHEEL 16
#define HOVER 32
#define CHILD 64
class GraphicObject : public Object
{
    Q_OBJECT

    Properties flags;  // Свойства графического объекта
public:
    void setProperties(Properties newFlags);

public:
    Object * frame;  // Выводится при наведении мыши
    Object * layer;  // Выводится при клике
    void setFrame(QString frameName)
    {
        frame->setPixmap(images->get(frameName));
    }

public:
    explicit GraphicObject(GraphicObject *parent = 0, Properties flags = 0,
                           QString pictureName = "", QString framePictureName = "",
                           QString layerName = "");
    virtual void Delete();


public:
    void resize(qreal w, qreal h);

    bool isClicked = false;  // находимся ли в состоянии нажатой клавиши
private:
    bool dontRelease = false;  // нужно ли отпускать в момент отпускания мышки
    QPointF cursor_position;  // для драга
private:
    // обработки сигналов
    void mousePressEvent(QGraphicsSceneMouseEvent *);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *);
    void wheelEvent(QGraphicsSceneWheelEvent *);

protected:
    virtual void leftClick() {}  // Для переопределения
    virtual void leftClickStart() {}
    virtual void rightClick() {}
    virtual void enter() {}
    virtual void leave() {}
    virtual void dragStart() {}
    virtual void dragFinish() {}
    virtual void push(bool) {}
    virtual void wheel(int) {}

signals:
    void leftClicked();  // Сигналы для внешнего мира
    void leftClickStarted();
    void rightClicked();
    void dragStarted();
    void dragFinished();
    void entered();
    void left();
    void pushed(bool newState);
    void wheeled(int);
};

#endif
