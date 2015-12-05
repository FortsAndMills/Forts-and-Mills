#include "Technical/Headers.h"
#include "BasicElements/GraphicObject.h"

class SystemControlPanel : public GraphicObject
{
    Q_OBJECT

    GraphicObject * close_button;
    GraphicObject * minimize_button;
    GraphicObject * maximize_button;

public:
    explicit SystemControlPanel(GraphicObject * parent = 0) : GraphicObject(parent)
    {
        close_button = new GraphicObject(this, CLICKABLE,
                                        "CloseButton", "ButtonLayer");
        connect(close_button, SIGNAL(leftClicked()), SIGNAL(close_clicked()));

        maximize_button = new GraphicObject(this, CLICKABLE,
                                            "MaximizeButton", "ButtonLayer");
        connect(maximize_button, SIGNAL(leftClicked()), SIGNAL(maximize_clicked()));

        minimize_button = new GraphicObject(this, CLICKABLE,
                                            "MinimizeButton", "ButtonLayer");
        connect(minimize_button, SIGNAL(leftClicked()), SIGNAL(minimize_clicked()));
    }

    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);

        close_button->setGeometry(2 * W / 3, 0, W / 3, H);
        maximize_button->setGeometry(W / 3, 0, W / 3, H);
        minimize_button->setGeometry(0, 0, W / 3, H);
    }

signals:
    void minimize_clicked();
    void maximize_clicked();
    void close_clicked();
};
