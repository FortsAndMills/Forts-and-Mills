#ifndef ARTIFICIALGAMECONTROL
#define ARTIFICIALGAMECONTROL

#include "Technical/Headers.h"
#include "BasicElements/GraphicObject.h"
#include "Technical/Constants.h"
#include "Game/Game.h"

class ArtificialButton : public GraphicObject
{
    Q_OBJECT

    Object * picture;

public:
    explicit ArtificialButton(GraphicObject * parent, Properties flags, QString pictureName,
                              QString clickedPictureName, QString inside_picture) :
        GraphicObject(parent, flags, pictureName, "", clickedPictureName)
    {
        picture = new Object(this, inside_picture);
    }

    void resizeChildren(qreal W, qreal H)
    {
        picture->setGeometry(W / 6, H / 6, 2 * W / 3, 2 * H / 3);
    }
};

class ArtificialControlPanel : public GraphicObject
{
    Q_OBJECT

    ArtificialButton * ToBlue;
    ArtificialButton * ToRed;
    ArtificialButton * ToNeutral;

    ArtificialButton * StartGame;

    ArtificialButton * GatherResources;

public:
    explicit ArtificialControlPanel(GraphicObject * parent) : GraphicObject(parent)
    {
        ToBlue = new ArtificialButton(this, PUSHABLE, "BlueButton", "BlueButtonClicked", "BlueHex");
        ToRed = new ArtificialButton(this, PUSHABLE, "RedButton", "RedButtonClicked", "RedHex");
        ToNeutral = new ArtificialButton(this, PUSHABLE, "NeutralButton", "NeutralButtonClicked", "NeutralHex");

        StartGame = new ArtificialButton(this, PUSHABLE, "NeutralButton", "NeutralButtonClicked", "StartLabel");

        GatherResources = new ArtificialButton(this, CLICKABLE, "NeutralButton", "NeutralButtonClicked", "GatherLabel");
        GatherResources->setEnabled(false);
    }

    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);

        ToBlue->setGeometry(0, 0, 3 * W / 10, 3 * H / 10);
        ToRed->setGeometry(7 * W / 20, 0, 3 * W / 10, 3 * H / 10);
        ToNeutral->setGeometry(14 * W / 20, 0, 3 * W / 10, 3 * H / 10);

        StartGame->setGeometry(0, 7 * H / 20, W, 3 * H / 10);
        GatherResources->setGeometry(0, 14 * H / 20, W, 3 * H / 10);
    }

    void startGame()
    {
        StartGame->setEnabled(false);
        GatherResources->setEnabled(true);
    }
};

class ArtificialUnitCreatorButton : public ArtificialButton
{
    Q_OBJECT

    PlayerColor color;
    UnitType type;

public:
    explicit ArtificialUnitCreatorButton(GraphicObject * parent,
                                         PlayerColor color, UnitType type) :
        ArtificialButton(parent, CLICKABLE, color + "Button", color + "ButtonClicked", color + type)
    {
        this->color = color;
        this->type = type;
    }

    void leftClick() { emit leftClicked(color, type); }
signals:
    void leftClicked(PlayerColor color, UnitType type);
};

class ArtificialUnitCreator : public GraphicObject
{
    Q_OBJECT

    Game * game;

    QMap <PlayerColor, QList <ArtificialUnitCreatorButton *> > buttons;

public:
    explicit ArtificialUnitCreator(GraphicObject * parent, Game * game) :
        GraphicObject(parent)
    {
        this->game = game;

        foreach (PlayerColor color, game->rules->players)
        {
            foreach (UnitType type, game->rules->unitsInGame)
            {
                buttons[color] << new ArtificialUnitCreatorButton(this, color, type);
            }
        }
    }

    void resize(qreal W, qreal H)
    {
        GraphicObject::resize(W, H);

        qreal margin = constants->unitCreatorMargin;

        qreal X, Y = 0;
        qreal Xsize;
        qreal Ysize = H / buttons.size();
        foreach (PlayerColor color, game->rules->players)
        {
            X = 0;
            Xsize = W / buttons[color].size();
            for (int i = 0; i < buttons[color].size(); ++i)
            {
                buttons[color][i]->setGeometry(X + Xsize * margin,
                                                                    Y + Ysize * margin,
                                                                    Xsize * (1 - 2 * margin),
                                                                    Ysize * (1 - 2 * margin));
                X += Xsize;
            }
            Y += Ysize;
        }
    }
};

#endif
