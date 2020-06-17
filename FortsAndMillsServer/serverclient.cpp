#include "ServerClient.h"

ServerClient::ServerClient(QTcpSocket *socket, Server *server, int ID) : QObject()
{
    this->socket = socket;
    this->server = server;
    this->ID = ID;

    connect(socket, SIGNAL(readyRead()), SLOT(read()));
    connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));
}
void ServerClient::startPlaying(qint8 player_index, QList<ServerClient *> opponents, QList<qint32> random)
{
    this->opponents = opponents;
    this->opponents.removeAll(this);
    this->state = PLAYING;
    this->game_index = player_index;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << START_GAME_MESSAGE << (qint32)ID << (qint8)game_index << random;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);
}
void ServerClient::blocked()
{
    state = FINAL_STATE;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << BLOCKED;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);

    socket->disconnectFromHost();
    this->deleteLater();
}
void ServerClient::badVersion()
{
    state = FINAL_STATE;

    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    out << (quint16)0 << BAD_VERSION;
    out.device()->seek(0);
    out << (quint16)(block.size() - sizeof(quint16));
    socket->write(block);

    server->say(id() + " has too old version, version = " + QString::number(version), true);
    socket->disconnectFromHost();
    this->deleteLater();
}


void ServerClient::disconnected()
{
    if (state == FINAL_STATE)
    {
        socket->deleteLater();
        deleteLater();
    }
    else
    {
        server->disconnected(this);
    }
}
void ServerClient::reconnect(QTcpSocket *qts)
{
    QObject::disconnect(socket, SIGNAL(readyRead()), this, SLOT(read()));
    QObject::disconnect(socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    socket->deleteLater();
    socket = qts;
    connect(socket, SIGNAL(readyRead()), SLOT(read()));
    connect(socket, SIGNAL(disconnected()), SLOT(disconnected()));

    QMap<int, QByteArray> mes = messages;
    messages.clear();
    foreach (QByteArray m, mes)
    {
        sendMessage(m);
    }
    foreach (ServerClient * sc, opponents)
        sc->opponentReconnected(this);

    read();
}

void ServerClient::read()
{
    if (socket == NULL)
        return;

    QDataStream in(socket);
    if (BlockSize == 0)
    {
        if (socket->bytesAvailable() < (int)sizeof(quint16))
                return;
        in >> BlockSize;

        if (BlockSize == 0)
            server->say("NET ERROR: BlockSize is 0!");
     }

    if (socket->bytesAvailable() < BlockSize)
        return;
    qint64 message_size = BlockSize - sizeof(quint8);
    BlockSize = 0;

    quint8 MessageId;
    in >> MessageId;
    ProcessMessage(in, MessageId, message_size);

    read();
}
void ServerClient::ProcessMessage(QDataStream &in, qint8 mes, qint64 message_size)
{
    if (IS_OLD_VER)
        return;

    switch (mes)
    {
    case WANT_TO_LISTEN_NEWS:
    {
        in >> version;
        if (!in.atEnd() || in.status() != QDataStream::Ok)
            server->say("ERROR: WANT_TO_LISTEN_NEWS message came wrong!");

        if (server->isVersionGood(this))
        {
            state = MENU_STATE;
            server->wantToListenNews(this);
        }
        else
        {
            badVersion();
        }
        return;
    }
    case JOIN_GAME:
    {
        qint32 index;
        in >> index;

        if (!in.atEnd())  // проблемы со старыми версиями
        {
            server->say(this->id() + " is possibly old version: " + QString::number(index));
            if (state != INIT)
                server->say("though state is not INIT...", true);

            if (true)
            {
                QByteArray block;
                QDataStream out(&block, QIODevice::WriteOnly);
                out << (quint16)0 << (quint8)4;
                out.device()->seek(0);
                out << (quint16)(block.size() - sizeof(quint16));
                socket->write(block);

                IS_OLD_VER = true;
                return;
            }
        }

        server->join(this, index);

        return;
    }
    case LEAVE_GAME:
    {
        qint32 index;
        in >> index;

        server->leave(this, index);
        return;
    }
    case CREATE_GAME:
    {
        in >> rules;
        if (!in.atEnd() || in.status() != QDataStream::Ok)
            server->say("ERROR: CREATE_GAME message came wrong!");

        server->createGame(this, rules);
        return;
    }
    case RESEND_THIS_TO_OPPONENT:
    {
        qint16 mes_key;
        in >> mes_key;

        this->sendRecievedMessage(mes_key);

        message_size -= sizeof(qint16);
        QByteArray message = in.device()->read(message_size);
        foreach (ServerClient * opponent, opponents)
            opponent->sendMessage(message);
        return;
    }
    case MESSAGE_RECEIVED:
    {
        qint16 mes_key;
        in >> mes_key;
        messages.remove(mes_key);
        return;
    }
    case I_RECONNECTED:
    {
        qint32 ID;
        in >> ID;
        server->reconnected(this, ID);
        return;
    }
    case I_LEAVE:
    {
        server->leave(this);
        state = FINAL_STATE;
        opponents.clear();
        return;
    }
    case CONNECTION_CHECK:
    {
        return;
    }
    default:
        server->say("ERROR: unknown message from " + id() + ", code " + QString::number(mes));
    }
}
