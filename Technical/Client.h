#ifndef CLIENT_H
#define CLIENT_H

#include "Headers.h"
#include "ProgrammSettings.h"

class Client : public QTcpSocket
{
    Q_OBJECT
public:
    // Сообщения для клиента:
    static const quint8 START_GAME_MESSAGE = 1;  // qint8 индекс игрока, QList <qint32> рандом
    static const quint8 MESSAGE_FOR_GAME = 2;  // сообщение для игры
    // Сообщения для сервера:
    static const quint8 RESEND_THIS_TO_OPPONENT = 1;  // сообщение для клиента


private:
    qint16 BlockSize = 0;

public:
    explicit Client(QObject *parent = 0) : QTcpSocket(parent)
    {
        this->setLocalPort(settings->PORT);
        connect(this, SIGNAL(readyRead()), SLOT(read()));

        // TODO внезапное отключение от сервера
        // TODO отключение противника
        // TODO ошибка подключения к серверу
        // TODO авторизация при подключении
    }

public slots:
    void TryToConnect()
    {
        this->connectToHost(settings->HOST_NAME, settings->PORT);
    }

    void read()
    {
        QDataStream in(this);
        if (BlockSize == 0)
        {
            if (this->bytesAvailable() < (int)sizeof(quint16))
                    return;
            in >> BlockSize;

            if (BlockSize == 0)
                qDebug() << "NET ERROR: BlockSize is 0!";
         }

        if (this->bytesAvailable() < BlockSize)
            return;
        BlockSize = 0;

        qint8 MessageId;
        in >> MessageId;
        ProcessMessage(in, MessageId);
    }
    void ProcessMessage(QDataStream &in, qint8 mes)
    {
        switch (mes)  // TODO проверка на корректность принимаемых данных и дефолтный кейс
        {
        case START_GAME_MESSAGE:
        {
            qint8 PlayerIndex;
            QList <qint32> random;
            in >> PlayerIndex >> random;

            emit CreateGame(PlayerIndex, random);
            return;
        }
        case MESSAGE_FOR_GAME:
        {
            emit MessageToGame(in.device()->readAll());
            return;
        }
        }
    }

    void sendToOpponent(QByteArray message)
    {
        QByteArray block;
        QDataStream out(&block, QIODevice::WriteOnly);
        out << (quint16)(message.size() + sizeof(quint8)) << RESEND_THIS_TO_OPPONENT;
        message.push_front(block);
        this->write(message);
    }

signals:
    void CreateGame(qint8 PlayerIndex, QList <qint32> random);  // создание игры
    void MessageToGame(QByteArray message);  // получение сообщения
};

#endif // CLIENT_H
