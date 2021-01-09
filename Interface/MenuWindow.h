#ifndef MENUWINDOW_H
#define MENUWINDOW_H

#include "Technical/Headers.h"
#include "Technical/Constants.h"
#include "Technical/Client.h"
#include "BasicElements/GraphicObject.h"
#include "SpecialButton.h"
#include "GameLabel.h"
#include "Dialog.h"

class MenuWindow : public GraphicObject
{
    Q_OBJECT

    Object * logo, *logo2;

    Dialog * dialog;

    SpecialButton * question;

    QVector < GameLabel * > games;
    GameLabel * game(int i)
    {
        foreach (GameLabel * gl, games)
            if (gl->index == i)
                return gl;

        debug << "Panic! GameLabel not found!";
        return NULL;
    }

public:
    GraphicObject * CreateNewGame;
    GraphicObject * HowToPlayFrame;
    Object * HowToPlayLabel;

    explicit MenuWindow(GraphicObject * parent) : GraphicObject(parent)
    {
        logo = new Object(this, "Logo");
        logo2 = new Object(this, "Logo");

        dialog = new Dialog(this);
        dialog->ClipWithItem(parent);
        dialog->set("Neutral", "ИДЁТ ПОДКЛЮЧЕНИЕ К СЕРВЕРУ...", true);

        connect(client, SIGNAL(connected()), SLOT(connected()));
        connect(client, SIGNAL(error(QAbstractSocket::SocketError)), SLOT(NetworkError(QAbstractSocket::SocketError)));
        connect(client, SIGNAL(disconnected()), SLOT(disconnected()));
        connect(client, SIGNAL(Blocked()), SLOT(blocked()));
        connect(client, SIGNAL(BadVersion()), SLOT(badVersion()));
        connect(client, SIGNAL(News(QList<qint32>, QList<QList<qint32> >,QList<qint8>)), SLOT(GetNews(QList<qint32>, QList<QList<qint32> >,QList<qint8>)));
        connect(client, SIGNAL(gameCreated(qint32, QList<qint32>, bool)), SLOT(gameCreated(qint32, QList<qint32>, bool)));
        connect(client, SIGNAL(game_joined(qint32)), SLOT(game_joined(qint32)));
        connect(client, SIGNAL(game_left(qint32)), SLOT(game_left(qint32)));
        connect(client, SIGNAL(game_removed(qint32)), SLOT(game_removed(qint32)));

        CreateNewGame = new GraphicObject(this, CLICKABLE | HOVER, "CreateNewGame", "NewGameFrameSelection", "SimpleLayer");
        CreateNewGame->setVisible(false);
        connect(CreateNewGame, SIGNAL(leftClicked()), SLOT(leaveJoined()));

        HowToPlayFrame = new GraphicObject(this, CLICKABLE | HOVER, "LessonFrame", "LessonFrameSelection", "SimpleLayer");
        connect(HowToPlayFrame, SIGNAL(leftClicked()), SLOT(leaveJoined()));
        HowToPlayLabel = new Object(this, "HowToPlayButton");

        question = new SpecialButton(this, "Question");
        connect(question, SIGNAL(leftClicked()), SLOT(questionClicked()));

        CheckForLessonsPassed();
    }
    void turn(bool on)
    {
        if (on)
        {
            AnimationStart(OPACITY, 1);
            dialog->showHelp = true;
        }
        else
        {
            AnimationStart(OPACITY, 0);
            dialog->showHelp = false;
        }
    }

    void resizeChildren(qreal W, qreal H)
    {
        logo->setGeometry(W / 2 - 2 * W / 9 - H / 6 - W / 15, H / 40, H / 6, H / 6);
        logo2->setGeometry(W / 2 + 2 * W / 9 + W / 15, H / 40, H / 6, H / 6);

        if (dialog)
            dialog->setGeometry(W * (1 - constants->dialogWidth) / 2,
                                H * (1 - constants->dialogHeight) / 2 + H / 10,
                                W * constants->dialogWidth, H * constants->dialogHeight);

        CreateNewGame->setGeometry(W / 2 - 2 * W / 9, H / 18, 4 * W / 9, H / 9);
        HowToPlayFrame->setGeometry(constants->howToPlayButtonX * W,
                                    constants->howToPlayButtonY * H,
                                    constants->howToPlayButtonWidth * W,
                                    constants->howToPlayButtonHeight * H);
        HowToPlayLabel->setGeometry(constants->howToPlayLabelX * W,
                                    constants->howToPlayLabelY * H,
                                    constants->howToPlayLabelWidth * W,
                                    constants->howToPlayLabelHeight * H);

        question->setGeometry(constants->questionX * W,
                              constants->questionY * H,
                              constants->questionWidth * W,
                              constants->questionHeight * H);

        resizeGameLabels();
    }

    QRectF gameLabelGeometry(int i)
    {
        return QRectF(width() * constants->gameLabelX,
                                 height() * (constants->gameLabelsYStart + i * (constants->gameLabelsMarginY + constants->gameLabelsHeight)),
                                 width() * (1 - 2 * constants->gameLabelX),
                                height() * constants->gameLabelsHeight);
    }
    void resizeGameLabels()
    {
        for (int i = 0; i < games.size(); ++i)
        {
            games[i]->setGeometry(gameLabelGeometry(i));
        }
    }
    void reconfigureGameLabels()
    {
        for (int i = 0; i < games.size(); ++i)
        {
            games[i]->AnimationStart(gameLabelGeometry(i), constants->defaultAnimationTime);
            games[i]->AnimationStart(OPACITY, 1);
        }
    }

    GameRules * getRules()
    {
        for (int j = 0; j < games.size(); ++j)
        {
            if (games[j]->isOn)
            {
                return games[j]->rules;
            }
        }
        return NULL;
    }

private:
    bool isBlocked = false;
public slots:
    void connected()
    {
        dialog->setVisible(false);
        client->sendWantNewsMessage();
        CreateNewGame->setVisible(true);
    }
    void recreateList()
    {
        foreach (GameLabel * label, games)
            label->Delete();
        games.clear();

        client->sendWantNewsMessage();
    }
    void NetworkError(QAbstractSocket::SocketError)
    {
        if (isBlocked)
            return;

        dialog->set("Neutral", "ОШИБКА!<br>НЕВОЗМОЖНО ПОДКЛЮЧИТЬСЯ К СЕРВЕРУ :(");

        debug << "Connection error: " << client->errorString() << "\n";
    }
    void disconnected()
    {
        if (isBlocked)
            return;

        dialog->set("Neutral", "ОШИБКА!<br>СОЕДИНЕНИЕ С СЕРВЕРОМ РАЗОРВАНО!");

        debug << "Server disconnected\n";
    }

    void blocked()
    {
        dialog->set("Neutral", "СЕРВЕР ОБНОВЛЯЕТСЯ.");

        isBlocked = true;
        debug << "Server is upgrading\n";
    }
    void badVersion()
    {
        dialog->setVisible(true);
        dialog->set("Neutral", "ВЕРСИЯ СЛИШКОМ СТАРАЯ!<br>Пожалуйста, скачайте новую версию по ссылке в readme-файле.");

        isBlocked = true;
        debug << "Version is too old\n";
    }

    void questionClicked()
    {
        help->HelpAsked("HowToAskHelp");
    }

    // РАБОТА СО СПИСКОМ КОМНАТ
    void GetNews(QList<qint32> ids, QList <QList <qint32> > rules, QList <qint8> players)
    {
        for (int i = 0; i < rules.size(); ++i)
            addGame(ids[i], rules[i], players[i]);
    }
    void gameCreated(qint32 id, QList <qint32> rules, bool mine)
    {
        addGame(id, rules, 0);
        if (mine)
            games.last()->joinClicked();
    }
    void addGame(qint32 id, QList<qint32> rules, qint8 players)
    {
        games.push_back(new GameLabel(this, id, rules, players, games.size() % 2));
        games.last()->setOpacity(0);
        games.last()->setGeometry(width() * constants->gameLabelX,
                                                     (games.size() == 1 ? height() * constants->gameLabelsYStart : games[games.size() - 2]->y()) + 100 + rand() % 200,
                                                     width() * (1 - 2 * constants->gameLabelX),
                                                     height() * constants->gameLabelsHeight);

        connect(games.last(), SIGNAL(joinClicked(bool,int)), SLOT(joinClicked(bool,int)));

        reconfigureGameLabels();
    }
    void game_joined(qint32 i)
    {
        game(i)->turnPlayer(true);
    }
    void game_left(qint32 i)
    {
        game(i)->turnPlayer(false);
    }
    void game_removed(qint32 i)
    {
        game(i)->setEnabled(false);
        game(i)->AnimationStart(OPACITY, 0);
        connect(game(i), SIGNAL(movieFinished()), game(i), SLOT(Delete()));
        games.removeAll(game(i));
        reconfigureGameLabels();
    }

    void leaveJoined(int exception = -1)
    {
        for (int j = 0; j < games.size(); ++j)
        {
            if (games[j]->index != exception && games[j]->isOn)
            {
                games[j]->joinClicked();
            }
        }
    }
    void joinClicked(bool on, int i)
    {
        if (on)
        {
            leaveJoined(i);
            client->sendJoinMessage(i);
        }
        else
        {
            client->sendUnjoinMessage(i);
        }
    }

    void CheckForLessonsPassed()
    {
        int i = 0;
        while (i < LESSONS_AMOUNT && settings->lessonsPassed[i]) { ++i; }
        if (i == LESSONS_AMOUNT)
            HowToPlayFrame->setOpacity(0.7);
    }
};

#endif // MENUWINDOW_H
