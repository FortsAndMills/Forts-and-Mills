#include "Server.h"
#include "ServerClient.h"

#include <QFile>

#define PORT 64124
#define VERSION 1320

void Server::setgeometry()
{
    this->setFixedSize(QSize(700, 400));

    text = new QTextEdit(this);
    text->setFont(QFont("Book Antiqua", 16));
    text->setReadOnly(true);
    text->setGeometry(0, 0, 600, 400);

    playing = new QLabel(this);
    playing->setFont(QFont("Book Antiqua", 14));
    playing->setAlignment(Qt::AlignCenter);
    playing->setGeometry(600, 0, 100, 150);
    playingInc();

    played = new QLabel(this);
    played->setFont(QFont("Book Antiqua", 14));
    played->setAlignment(Qt::AlignCenter);
    played->setGeometry(600, 150, 100, 150);
    playedInc();

    block = new QPushButton("block", this);
    block->setCheckable(true);
    block->setFont(QFont("Book Antiqua", 14));
    block->setGeometry(600, 300, 100, 100);
}
Server::Server() : QMainWindow()
{
    int port = PORT;
    QFile * hostFile = new QFile("port.txt", this);
    if (hostFile->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream tstream(hostFile);
        tstream >> port;
        hostFile->close();
    }

    setgeometry();

    server = new QTcpServer(this);
    if (!server->listen(QHostAddress("0.0.0.0"), port))
        this->say("ERROR: Can't listen!");
    else
        this->say("Good afternoon. FortsAndMills are working on port #" + QString::number(port));

    connect(server, SIGNAL(newConnection()), this, SLOT(getConnection()));
}
Server::~Server()
{
    foreach (ServerClient * client, clients)
        client->close();
}

bool Server::isVersionGood(ServerClient *client)
{
    return client->version >= VERSION;
}
void Server::wantToListenNews(ServerClient *client)
{
    QList<qint32> ids;
    QList < QList<qint32> > rules;
    QList <qint8> players;

    for (QMap<qint32, Game>::iterator it = games.begin(); it != games.end(); ++it)
    {
        ids.push_back(it.key());
        rules.push_back(it->rules);
        players.push_back(it->player_ids.size());
    }

    client->sendNews(ids, rules, players);
}
void Server::createGame(ServerClient * author, QList<qint32> rules)
{
    games[game_ids++] = Game(rules);
    say("Game #" + QString::number(game_ids - 1) + " created: " +
                QString::number(rules[0]) + " pl, " +
                QString::number(rules[1]) + "x" + QString::number(rules[2]) +
                ", code " + QString::number(rules[3]) + "/" + QString::number(rules[4]) + ", " +
                (rules[5] == 179 ? "CDT" : QString::number(rules[5]) + " dt"));

    foreach (ServerClient * client, clients)
        client->sendNewGameCreated(game_ids - 1, rules, client == author);
}
void Server::join(ServerClient *client, qint32 id)
{
    say(client->id() + " wants to play game #" + QString::number(id));
    games[id].player_ids.push_back(client->ID);

    foreach (ServerClient * client, clients)
        if (client->state == ServerClient::MENU_STATE)
            client->sendJoinMessage(id);

    if (games[id].player_ids.size() == games[id].players_needed())
    {
        QList <qint32> random;
        for (int i = 0; i < 1000; ++i)  // <undone> константа
            random << rand();

        QList <ServerClient *> opponents;
        for (int i = 0; i < games[id].player_ids.size(); ++i)
        {
            opponents.push_back(clients[games[id].player_ids[i]]);
        }

        for (int i = 0; i < opponents.size(); ++i)
        {
            opponents[i]->startPlaying(i, opponents, random);
        }

        QString phr = opponents[0]->id();
        for (int i = 1; i < opponents.size(); ++i)
            phr += " and " + opponents[i]->id();
        say(phr + " started game!", true);

        playingInc();

        removeGame(id);
    }
}
void Server::leave(ServerClient *client, qint32 i)
{
    say(client->id() + " leaves playing game #" + QString::number(i));
    games[i].player_ids.removeAll(client->ID);

    foreach (ServerClient * client, clients)
        if (client->state == ServerClient::MENU_STATE)
            client->sendUnjoinMessage(i);

    if (games[i].player_ids.size() == 0)
    {
        removeGame(i);
    }
}
void Server::removeGame(qint32 i)
{
    foreach (ServerClient * client, clients)
        if (client->state == ServerClient::MENU_STATE)
            client->sendRemoveGameMessage(i);

    say("game #" + QString::number(i) + " is removed.", true);
    games.remove(i);
}

void Server::getConnection()
{
    ServerClient * New = new ServerClient(server->nextPendingConnection(), this, ids++);

    if (block->isChecked())
    {
        say("New connection blocked");
        New->blocked();
    }
    else
    {
        say("New connection: " + New->id());

        clients[New->ID] = New;
    }
    qApp->alert(dynamic_cast<QWidget *>(this));
}
void Server::reconnected(ServerClient *client, qint32 ID)
{
    say(client->id() + " is reconnected " + QString::number(ID), true);
    if (clients.contains(ID))
    {
        clients[ID]->reconnect(client->socket);
        clients.remove(client->ID);
        client->socket = NULL;
        client->deleteLater();
    }
    else
    {
        say("...what happened?", true);
    }
}
void Server::leave(ServerClient *client)
{
    say(client->id() + " leaves!");
    foreach (ServerClient * sc, client->opponents)
        sc->opponents.removeAll(client);
    if (client->opponents.size() == 0 && client->state == ServerClient::PLAYING)
    {
        say("game ends here", true);
        playingDec();
        playedInc();
    }

    if (client->state == ServerClient::MENU_STATE)
    {
        for (QMap<qint32, Game>::iterator it = games.begin(); it != games.end(); ++it)
        {
            if (it->player_ids.contains(client->ID))
            {
                leave(client, it.key());
                break;
            }
        }
    }

    clients.remove(client->ID);
}
void Server::disconnected(ServerClient *client)
{
    foreach (ServerClient * sc, client->opponents)
        sc->opponentDisconnected(client);

    say(client->id() + " disconnected...");

    for (QMap<qint32, Game>::iterator it = games.begin(); it != games.end(); ++it)
        if (it->player_ids.contains(client->ID))
            leave(client, it.key());
}
