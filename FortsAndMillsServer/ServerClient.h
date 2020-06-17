#ifndef SERVERCLIENT_H
#define SERVERCLIENT_H

#include "Server.h"

class ServerClient : public QObject
{
    Q_OBJECT

public:
    // Сообщения для клиента:
    static const quint8 BLOCKED = 1;  //
    static const quint8 BAD_VERSION = 2;  //
    static const quint8 NEWS = 3;  // QList <qint32> > game_ids, QList< QList <qint32> > rules, QList<qint8> players
    static const quint8 GAME_JOINED = 4;  // qint32 game_id
    static const quint8 GAME_LEFT = 5;  // qint32 game_id
    static const quint8 REMOVE_GAME = 6;  // qint32 game_id
    static const quint8 GAME_CREATED = 7;  // qint32 game_id, QList <qint32> rules, qint8 mine
    static const quint8 START_GAME_MESSAGE = 8;  // qint32 ID, qint8 индекс игрока, QList <qint32> рандом
    static const quint8 MESSAGE_FOR_GAME = 9;  // qint16 mes_key, сообщение для игры
    static const quint8 OPPONENT_DISCONNECTED = 10;  // qint8 индекс отвалившегося
    static const quint8 OPPONENT_RECONNECTED = 11;  // qint8 индекс подсоединившегося
    static const quint8 RESEND_MESSAGE_RECEIVED = 12;  // qint16 mes_key
    // Сообщения для сервера:
    static const quint8 WANT_TO_LISTEN_NEWS = 1;  // qint32 version
    static const quint8 JOIN_GAME = 2;  // qint32 game_id
    static const quint8 LEAVE_GAME = 3;  // qint32 game_id
    static const quint8 CREATE_GAME = 4;  // QList <qint32> rules
    static const quint8 RESEND_THIS_TO_OPPONENT = 5;  // qint16 mes_key, сообщение для клиента
    static const quint8 I_RECONNECTED = 6;  // qint32 ID
    static const quint8 MESSAGE_RECEIVED = 7;  // qint16 mes_key
    static const quint8 I_LEAVE = 8;  //
    static const quint8 CONNECTION_CHECK = 9; //

    // наследство от прабабушки
    static const quint8 _old_v_I_WANT_TO_PLAY = 2;  // qint32 version, QList <qint32> rules
    bool IS_OLD_VER = false;

    enum ClientState {INIT,MENU_STATE, PLAYING, FINAL_STATE};
    ClientState state = INIT;
    QList <ServerClient *> opponents;
    qint8 game_index;  // в игре

    qint32 version;
    QList <qint32> rules;

    QTcpSocket * socket;
    qint32 ID;
    QString id() { return QString::number(ID); }
private:
    Server * server;

public:
    explicit ServerClient(QTcpSocket * socket, Server * server, int ID);

public slots:
    void close()
    {
        socket->abort();
    }
    void startPlaying(qint8 player_index, QList <ServerClient *> opponents, QList<qint32> random);
    void reconnect(QTcpSocket * qts);

    void blocked();
    void badVersion();

    void sendNews(QList <qint32> ids, QList <QList <qint32> > rules, QList <qint8> players)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << NEWS << ids << rules << players;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendNewGameCreated(qint32 id, QList <qint32> rules, qint8 mine)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << GAME_CREATED << id << rules << mine;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendJoinMessage(qint32 index)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << GAME_JOINED << index;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendUnjoinMessage(qint32 index)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << GAME_LEFT << index;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendRemoveGameMessage(qint32 index)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << REMOVE_GAME << index;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }

    void opponentDisconnected(ServerClient *op)
    {
        int i = 0;
        while (opponents[i] != op)
            ++i;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << OPPONENT_DISCONNECTED << opponents[i]->game_index;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void opponentReconnected(ServerClient *op)
    {
        int i = 0;
        while (opponents[i] != op)
            ++i;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << OPPONENT_RECONNECTED << opponents[i]->game_index;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendRecievedMessage(qint16 mes_key)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << RESEND_MESSAGE_RECEIVED << mes_key;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        socket->write(block);
    }
    void sendMessage(QByteArray message)
    {        
        messages[mes_key++] = message;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)(message.size() + sizeof(quint8) + sizeof(qint16)) << MESSAGE_FOR_GAME << (qint16)(mes_key - 1);
        block.push_back(message);

        socket->write(block);
    }

private:
    qint16 BlockSize = 0;

    QMap<int, QByteArray> messages;
    qint16 mes_key = 0;

public slots:
    void read();
    void ProcessMessage(QDataStream &in, qint8 mes, qint64 message_size);
    void disconnected();
};

#endif // SERVERCLIENT_H
