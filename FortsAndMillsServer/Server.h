#ifndef EVENTWINDOW_H
#define EVENTWINDOW_H

#include <QMap>
#include <QMainWindow>
#include <QTextEdit>
#include <QLabel>
#include <QPushButton>
#include <QTcpServer>
#include <QTime>
#include <QTcpSocket>
#include <QObject>
#include <QApplication>
#include <QTimer>

class ServerClient;
class Server : public QMainWindow
{
    Q_OBJECT

    QTcpServer * server;

    QTextEdit * text;
    int now = -1;
    QLabel * playing;
    int done = -1;
    QLabel * played;
    QPushButton * block;

private:
    void playingInc()
    {
        ++now;
        playing->setText("NOW\nPLAYING:\n" + QString::number(now));
    }
    void playingDec()
    {
        --now;
        playing->setText("NOW\nPLAYING:\n" + QString::number(now));
    }
    void playedInc()
    {
        ++done;
        played->setText("PLAYED:\n" + QString::number(done));
    }

public:
    void say(QString NewText, bool sub = false)
    {
        if (sub)
            NewText = "    " + NewText;
        else
            NewText = QDate::currentDate().toString() + " " + QTime::currentTime().toString() + ")\n    " + NewText + "\n";

        text->setText(NewText + "\n" + text->toPlainText());
    }
    explicit Server();
    ~Server();

    void setgeometry();

    bool isVersionGood(ServerClient * client);
    void wantToListenNews(ServerClient * client);
    void createGame(ServerClient *client, QList <qint32> rules);
    void join(ServerClient * client, qint32 i);
    void leave(ServerClient * client, qint32 i);
    void removeGame(qint32 i);

private:
    QMap <qint32, ServerClient *> clients;
    int ids = 0;

    int game_ids = 0;
    struct Game
    {
        QList <qint32> rules;
        QList <int> player_ids;

        int players_needed() { return rules[0]; }
        Game(QList <qint32> _rules) : rules(_rules) {}
        Game() {}
    };

    QMap <qint32, Game > games;
public slots:
    void getConnection();
    void reconnected(ServerClient * client, qint32 ID);
    void leave(ServerClient * client);
    void disconnected(ServerClient * client);
};

#endif // EVENTWINDOW_H



