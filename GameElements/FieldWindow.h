#ifndef FIELDWINDOW
#define FIELDWINDOW

#include "Field.h"
#include "Technical/Constants.h"

class FieldWindow : public GraphicObject
{
    Q_OBJECT
public:
    GraphicObject * up, * down, * right, * left;

    explicit FieldWindow(GraphicObject * parent, Field * field) : GraphicObject(parent)
    {
        up = new GraphicObject(this, CLICKABLE, "up", "", "SimpleLayer");
        connect(up, SIGNAL(leftClicked()), field, SLOT(moveUp()));
        down = new GraphicObject(this, CLICKABLE, "down", "", "SimpleLayer");
        connect(down, SIGNAL(leftClicked()), field, SLOT(moveDown()));
        right = new GraphicObject(this, CLICKABLE, "right", "", "SimpleLayer");
        connect(right, SIGNAL(leftClicked()), field, SLOT(moveRight()));
        left = new GraphicObject(this, CLICKABLE, "left", "", "SimpleLayer");
        connect(left, SIGNAL(leftClicked()), field, SLOT(moveLeft()));
    }
    virtual void Delete()
    {
        up->Delete();
        down->Delete();
        left->Delete();
        right->Delete();
        GraphicObject::Delete();
    }

private:
    void resizeChildren(qreal W, qreal H)
    {
        left->setGeometry(0, H / 3, W / 3, H / 3);
        right->setGeometry(2 * W / 3, H / 3, W / 3, H / 3);
        up->setGeometry(W / 3, 0, W / 3, H / 3);
        down->setGeometry(W / 3, 2 * H / 3, W / 3, H / 3);
    }
};

#endif
