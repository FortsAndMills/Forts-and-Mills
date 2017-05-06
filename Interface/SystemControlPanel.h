#include "Technical/Headers.h"
#include "BasicElements/StateObject.h"

class SystemControlPanel : public GraphicObject
{
    Q_OBJECT

    StateObject * close_button;
    StateObject * minimize_button;
    StateObject * maximize_button;

public:
    explicit SystemControlPanel(GraphicObject * parent = 0) : GraphicObject(parent)
    {
        close_button = new StateObject(this, "Neutral", "NeutralCloseButton", CLICKABLE | HOVER, "NeutralButtonLayer");
        connect(close_button, SIGNAL(leftClicked()), SIGNAL(close_clicked()));

        maximize_button = new StateObject(this, "Neutral", "NeutralMaximizeButton", CLICKABLE | HOVER, "NeutralButtonLayer");
        connect(maximize_button, SIGNAL(leftClicked()), SIGNAL(maximize_clicked()));

        minimize_button = new StateObject(this, "Neutral", "NeutralMinimizeButton", CLICKABLE | HOVER, "NeutralButtonLayer");
        connect(minimize_button, SIGNAL(leftClicked()), SIGNAL(minimize_clicked()));

        close_button->addPicture("Blue", "BlueCloseButton");
        maximize_button->addPicture("Blue", "BlueMaximizeButton");
        minimize_button->addPicture("Blue", "BlueMinimizeButton");
        close_button->addPicture("Red", "RedCloseButton");
        maximize_button->addPicture("Red", "RedMaximizeButton");
        minimize_button->addPicture("Red", "RedMinimizeButton");
        close_button->addPicture("Green", "GreenCloseButton");
        maximize_button->addPicture("Green", "GreenMaximizeButton");
        minimize_button->addPicture("Green", "GreenMinimizeButton");
        close_button->addPicture("Yellow", "YellowCloseButton");
        maximize_button->addPicture("Yellow", "YellowMaximizeButton");
        minimize_button->addPicture("Yellow", "YellowMinimizeButton");
    }

    void setState(PlayerColor color)
    {
        close_button->setPictureState(color);
        close_button->layer->setPixmap(color + "ButtonLayer");
        minimize_button->setPictureState(color);
        minimize_button->layer->setPixmap(color + "ButtonLayer");
        maximize_button->setPictureState(color);
        maximize_button->layer->setPixmap(color + "ButtonLayer");
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
