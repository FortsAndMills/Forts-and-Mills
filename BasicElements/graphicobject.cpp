#include "GraphicObject.h"

GraphicObject::GraphicObject(GraphicObject *parent, Properties flags,
                             QString pictureName, QString framePictureName,
                             QString layerName, bool keepaspectratio) :
                         Object(parent, pictureName, keepaspectratio)
{
    // лэйр отображается в момент щелчка мыши
    layer = new Object(this, layerName);
    layer->setVisible(false);
    if (!isNull())
        layer->ClipWithItem(this);  // обрезка лэйра также, как и исходной фигуры

    // фрейм отображается при наведении мыши
    frame = new Object(this, framePictureName);
    frame->setVisible(false);
    // принято решение его не обрезать, так как это рамка и она может выходить за границы

    this->setProperties(flags);
}
void GraphicObject::setProperties(Properties newFlags)
{
    // установка технических qt-шных свойств согласно новым флагам
    this->flags = newFlags;

    if ((flags & CLICKABLE) && (flags & PUSHABLE))
    {
        debug << "NOTE: graphic object is clickable and pushable simulteniously!\n";
    }

    // принимаем наведение мыши, если есть дети или хотим обрабатывать наведение
    setAcceptHoverEvents((flags & (CHILD | HOVER)));

    // принимаем щелчки мышью раздельно для каждой из двух кнопок мыши
    this->setAcceptedMouseButtons((Qt::MouseButton)
              ((Qt::LeftButton * (bool)(flags & (CLICKABLE | DRAGABLE | CHILD | PUSHABLE))) |
               (Qt::RightButton * (bool)(flags & (RIGHT_CLICKABLE | CHILD)))));

    // технически, эта функция должна обрезать объект и его детей по форме картинки
    // однако всё равно понадобились костыли внутри класса Object :(
    this->setFlag(QGraphicsItem::ItemClipsToShape, !this->isNull() &&
                  (this->acceptHoverEvents() || this->acceptedMouseButtons() != 0));
}
void GraphicObject::Delete()
{
    frame->Delete();
    layer->Delete();
    Object::Delete();
}

void GraphicObject::resize(qreal W, qreal H)
{
    Object::resize(W, H);

    frame->setGeometry(0, 0, width(), height());
    layer->setGeometry(0, 0, width(), height());
}

void GraphicObject::mousePressEvent(QGraphicsSceneMouseEvent *qme)
{
    // передача сигнала детям
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mousePressEvent(qme);

    if (qme->button() == Qt::LeftButton)
    {
        dontRelease = false;

        // подготовка к переносу
        if (flags & DRAGABLE)
        {
            cursor_position = qme->screenPos();

            dragStart();
            emit dragStarted();
        }

        // простой щелчок
        if (flags & CLICKABLE)
        {
            // сигналы начала щелчка
            leftClickStart();
            emit leftClickStarted();
            // сигнал самого щелчка по традиции щлётся, когда кнопка отпускается

            isClicked = true;

            layer->setVisible(true);
        }

        // если объект выделяем и ещё не выделен, выделяем
        if ((flags & PUSHABLE) && !isClicked)
        {
            dontRelease = true;
            isClicked = true;

            push(true);
            emit pushed(true);

            layer->setVisible(true);
        }
    }
    else if (qme->button() == Qt::RightButton)
    {
        // для правой кнопки мыши только щелчок
        if (flags & RIGHT_CLICKABLE)
        {
            rightClick();
            emit rightClicked();
        }
    }
}
void GraphicObject::mouseMoveEvent(QGraphicsSceneMouseEvent *qme)
{
    // перемещение мыши используется только для драга

    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mouseMoveEvent(qme);

    if ((flags & DRAGABLE) && (qme->buttons() & Qt::LeftButton))
    {
        this->moveBy(qme->screenPos() - cursor_position);
        cursor_position = qme->screenPos();
    }
}
void GraphicObject::mouseReleaseEvent(QGraphicsSceneMouseEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->mouseReleaseEvent(qme);

    if (flags & PUSHABLE)
    {
        // переход в состояние "не нажато" для pushable-кнопки
        if (isClicked && !dontRelease)
        {
            isClicked = false;

            push(false);
            emit pushed(false);

            layer->setVisible(false);
        }
        dontRelease = false;
    }
    else
    {
        // сигнал щелчка левой кнопкой
        if (qme->button() == Qt::LeftButton)
        {
            if (flags & CLICKABLE)
            {
                leftClick();
                emit leftClicked();
            }
        }

        isClicked = false;
        layer->setVisible(false);
    }

    // завершение драга
    if (qme->button() == Qt::LeftButton)
    {
        if (flags & DRAGABLE)
        {
            dragFinish();
            emit dragFinished();
        }
    }
}

void GraphicObject::hoverEnterEvent(QGraphicsSceneHoverEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->hoverEnterEvent(qme);

    // появление фрейма при наведении
    if (HOVER & flags)
    {
        frame->setVisible(true);

        enter();
        emit entered();
    }
}
void GraphicObject::hoverLeaveEvent(QGraphicsSceneHoverEvent *qme)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->hoverLeaveEvent(qme);

    // убирание фрейма при выходе мышки
    if (HOVER & flags)
    {
        frame->setVisible(false);

        leave();
        emit left();
    }
}

void GraphicObject::wheelEvent(QGraphicsSceneWheelEvent *qwe)
{
    if (CHILD & flags)
        dynamic_cast<GraphicObject *>(parentItem())->wheelEvent(qwe);

    // прокручивание колёсика мыши
    if (flags & WHEEL)
    {
        wheel(qwe->delta());
        emit wheeled(qwe->delta());
    }
}
