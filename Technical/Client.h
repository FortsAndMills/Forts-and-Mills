#ifndef CLIENT_H
#define CLIENT_H

#include "Headers.h"
#include "ProgrammSettings.h"
#include "Constants.h"

class Client : public QTcpSocket
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

private:
    qint16 BlockSize = 0;
    qint32 ID = -1;
    bool blocked = false;

public:
    explicit Client(QObject *parent = 0) : QTcpSocket(parent)
    {
        this->setLocalPort(settings->PORT);
        connect(this, SIGNAL(readyRead()), SLOT(read()));
        connect(this, SIGNAL(disconnected()), SLOT(TryToReconnect()));
        connect(this, SIGNAL(connected()), SLOT(reconnected()));

        ConnectionCheckTimer = new QTimer(this);
        ConnectionCheckTimer->start(constants->serverConnectionCheckTime);
        connect(ConnectionCheckTimer, SIGNAL(timeout()), SLOT(ConnectionCheck()));
        // TODO авторизация при подключении
    }
    
private:
    QMap<int, QByteArray> messages;
    qint16 mes_key = 0;
    QTimer * ConnectionCheckTimer;

    void error_happened()
    {
        // TODO
    }

public slots:
    void TryToConnect()
    {
        this->connectToHost(settings->HOST_NAME, settings->PORT);
    }
    void reconnected()
    {
        if (ID != -1)
        {
            sendReconnectionMessage();
            emit SuccesfullyReconnected();
            
            QMap<int, QByteArray> mes = messages;
            messages.clear();
            foreach (QByteArray qba, mes)
            {
                sendToOpponent(qba);
            }
        }
    }
    void TryToReconnect()
    {
        if (blocked)
            return;

        if (this->state() == UnconnectedState)
        {
            this->abort();
            TryToConnect();
            QTimer::singleShot(constants->serverReconnectionTime, this, SLOT(TryToReconnect()));
        }
        else if (this->state() != SocketState::ConnectedState)
        {
            QTimer::singleShot(constants->serverReconnectionTime, this, SLOT(TryToReconnect()));
        }
    }
    void ConnectionCheck()
    {
        if (this->state() != SocketState::ConnectedState)
            return;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << CONNECTION_CHECK;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }

    // чтение сообщений
    void read()
    {
        QDataStream in(this);
        if (BlockSize == 0)
        {
            if (this->bytesAvailable() < (int)sizeof(quint16))
                    return;
            in >> BlockSize;

            if (BlockSize == 0)
                debug << "NET ERROR: BlockSize is 0!\n";

            if (in.status() != QDataStream::Ok)
            {
                debug << "NET ERROR: can't read size of message!\n";
                error_happened();
                return;
            }
         }

        if (this->bytesAvailable() < BlockSize)
            return;
        qint64 message_size = BlockSize - sizeof(qint8);
        BlockSize = 0;

        qint8 MessageId;
        in >> MessageId;

        if (in.status() != QDataStream::Ok)
        {
            debug << "NET ERROR: error while reading message ID\n";
            error_happened();
            return;
        }

        ProcessMessage(in, MessageId, message_size);

        if (in.status() != QDataStream::Ok)
        {
            debug << "NET ERROR: message with id" + QString::number(MessageId) + " came wrong!\n";
            error_happened();
            return;
        }

        read();
    }
private:
    void ProcessMessage(QDataStream &in, qint8 mes, qint64 message_size)
    {
        switch (mes)
        {
        case BLOCKED:
        {
            blocked = true;
            emit Blocked();
            return;
        }
        case BAD_VERSION:
        {
            blocked = true;
            emit BadVersion();
            return;
        }
        case NEWS:
        {
            QList <qint32> ids;
            QList <QList <qint32> > rules;
            QList <qint8> players;

            in >> ids >> rules >> players;
            emit News(ids, rules, players);

            return;
        }
        case GAME_JOINED:
        {
            qint32 index;
            in >> index;

            emit game_joined(index);
            return;
        }
        case GAME_LEFT:
        {
            qint32 index;
            in >> index;

            emit game_left(index);
            return;
        }
        case REMOVE_GAME:
        {
            qint32 index;
            in >> index;

            emit game_removed(index);
            return;
        }
        case GAME_CREATED:
        {
            qint32 id;
            qint8 mine;
            QList <qint32> rules;
            in >> id >> rules >> mine;

            emit gameCreated(id, rules, mine);
            return;
        }
        case START_GAME_MESSAGE:
        {
            qint8 PlayerIndex;
            QList <qint32> random;
            in >> ID >> PlayerIndex >> random;

            emit CreateGame(PlayerIndex, random);
            return;
        }
        case MESSAGE_FOR_GAME:
        {
            qint16 mes_key;
            in >> mes_key;
            sendRecievedMessage(mes_key);
            message_size -= sizeof(qint16);

            emit MessageToGame(in.device()->read(message_size));
            return;
        }   
        case OPPONENT_DISCONNECTED:
        {
            qint8 PlayerIndex;
            in >> PlayerIndex;

            emit OpponentDisconnected(PlayerIndex);
            return;
        }
        case OPPONENT_RECONNECTED:
        {
            qint8 PlayerIndex;
            in >> PlayerIndex;

            emit OpponentReconnected(PlayerIndex);
            return;
        }
        case RESEND_MESSAGE_RECEIVED:
        {
            qint16 key;
            in >> key;
            messages.remove(key);
            return;
        }
        default:
        {
            debug << "ERROR: strange message " << mes << " from server!\n";
        }
        }
    }

public slots:
    // отправки сообщений
    void sendToOpponent(QByteArray message)
    {        
        messages[mes_key++] = message;

        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)(message.size() + sizeof(quint8) + sizeof(qint16)) << RESEND_THIS_TO_OPPONENT << (qint16)(mes_key - 1);
        message.push_front(block);
        this->write(message);
    }

public:
    void sendWantNewsMessage()
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << WANT_TO_LISTEN_NEWS << settings->version;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }
    void sendCreateGameMessage()
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << CREATE_GAME << settings->rules->get();
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }
    void sendJoinMessage(qint32 game_id)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << JOIN_GAME << game_id;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }
    void sendUnjoinMessage(qint32 game_id)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << LEAVE_GAME << game_id;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }

    void sendReconnectionMessage()
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << I_RECONNECTED << ID;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }
    void sendRecievedMessage(qint16 key)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << MESSAGE_RECEIVED << key;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }
    void sendLeaveMessage()
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)0 << I_LEAVE;
        out.device()->seek(0);
        out << (quint16)(block.size() - sizeof(quint16));
        this->write(block);
    }

signals:
    void Blocked();
    void BadVersion();
    void OpponentDisconnected(qint8 index);
    void OpponentReconnected(qint8 index);
    void SuccesfullyReconnected();

    void News(QList <qint32> ids, QList <QList <qint32> > rules, QList <qint8> players);
    void gameCreated(qint32 id, QList <qint32> rules, bool mine);
    void game_joined(qint32 index);
    void game_left(qint32 index);
    void game_removed(qint32 index);

    void CreateGame(qint8 PlayerIndex, QList <qint32> random);  // создание игры
    void MessageToGame(QByteArray message);  // получение сообщения
};

#endif // CLIENT_H
