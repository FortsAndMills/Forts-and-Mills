#include "Field.h"
#include "Technical/Constants.h"

class FieldWindow : public GraphicObject
{
    Q_OBJECT
public:
    Field * field;

private:
    GraphicObject * up, * down, * right, * left;

public:
    explicit FieldWindow(Game * game, GraphicObject * parent) : GraphicObject(parent)
    {
        field = new Field(this, game);

        this->setZValue(constants->fieldZPos);

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
        field->Delete();
        up->Delete();
        down->Delete();
        left->Delete();
        right->Delete();
        GraphicObject::Delete();
    }

private:
    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);

        qreal arrowSize = constants->fieldSideMargin * W;
        qreal marginX = (W - constants->fieldWidth - 2 * arrowSize) / 2;
        qreal marginY = (H - constants->fieldHeight - 2 * arrowSize) / 2;
        field->setGeometry(marginX + arrowSize,
                                         marginY + arrowSize,
                                         constants->fieldWidth, constants->fieldHeight);

        left->setGeometry(marginX, H / 2 - arrowSize / 2, arrowSize, arrowSize);
        right->setGeometry(W - arrowSize - marginX, H / 2 - arrowSize / 2, arrowSize, arrowSize);
        up->setGeometry(W / 2 - arrowSize / 2, marginY, arrowSize, arrowSize);
        down->setGeometry(W / 2 - arrowSize / 2, H - arrowSize - marginY, arrowSize, arrowSize);
    }
};
