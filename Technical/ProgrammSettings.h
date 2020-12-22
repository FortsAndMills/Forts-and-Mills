#ifndef SETTINGS
#define SETTINGS

#include "Technical/Headers.h"
#include "Game/GameRules.h"

class Settings : public QObject
{
private:
    const QString PROGRAMM_SETTINGS_FILE_NAME = "ProgrammSettings.opt";
    const QString Host_Settings_File_Name = "Connection.connection";
public:
    const QString Debug_File_Name = "log.log";
    const qint32 version = 1320;

public:
    // ОБЪЯВЛЕНИЕ ВСЕХ НАСТРОЕК----------------------------------------------------------
    quint16 PORT = 64124;
    QString HOST_NAME = "fortsandmills.got-game.org";
    GameRules * rules;

    QPoint APPLICATION_START_POSITION = QPoint(100, 100);
    QSize APPLICATION_START_SIZE = QSize(1200, 720);
    bool IS_APPLICATION_MAXIMIZED = false;
    bool lessonsPassed[LESSONS_AMOUNT];


    // ВВОД-ВЫВОД ВСЕХ НАСТРОЕК---------------------------------------------------------
    friend QDataStream &operator << (QDataStream &stream, const Settings *settings)
    {
        stream << settings->APPLICATION_START_POSITION
                    << settings->APPLICATION_START_SIZE
                    << settings->IS_APPLICATION_MAXIMIZED
                    << settings->rules;

        for (int i = 0; i < LESSONS_AMOUNT; ++i)
                    stream << settings->lessonsPassed[i];

        return stream;
    }
    friend QDataStream &operator >>(QDataStream &stream, Settings *settings)
    {
        stream >> settings->APPLICATION_START_POSITION
                    >> settings->APPLICATION_START_SIZE
                    >> settings->IS_APPLICATION_MAXIMIZED
                    >> settings->rules;

        for (int i = 0; i < LESSONS_AMOUNT; ++i)
                    stream >> settings->lessonsPassed[i];

        return stream;
    }



    // СЧИТЫВАНИЕ И ЗАПИСЫВАНИЕ-------------------------------------------------------
    explicit Settings(QObject * parent) : QObject(parent)
    {
        rules = new GameRules();
        for (int i = 0; i < LESSONS_AMOUNT; ++i)
                    lessonsPassed[i] = false;

        read();
    }

    void read()
    {
        QFile * hostFile = new QFile(Host_Settings_File_Name, this);
        if (hostFile->open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream tstream(hostFile);
            QString s;
            tstream >> s >> s >> PORT
                          >> s >> s >> s >> HOST_NAME;

            if(tstream.status() != QTextStream::Ok)
            {
                qDebug() << "ERROR: Ошибка чтения файла подключения\n";
            }
            hostFile->close();
        }

        QFile * file = new QFile(PROGRAMM_SETTINGS_FILE_NAME, this);
        if (file->open(QIODevice::ReadOnly))
        {
            QDataStream stream(file);
            stream >> this;

            if(stream.status() != QDataStream::Ok)
            {
                qDebug() << "ERROR: Ошибка чтения файла программных настроек\n";
            }
            file->close();
        }
    }
    void write()
    {        
        QFile * hostFile = new QFile(Host_Settings_File_Name, this);
        if (hostFile->open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream tstream(hostFile);
            tstream << "PORT = " << PORT << Qt::endl <<
                               "HOST NAME = " << HOST_NAME << Qt::endl;
            hostFile->close();
        }

        QFile * file = new QFile(PROGRAMM_SETTINGS_FILE_NAME);
        if (file->open(QIODevice::WriteOnly))
        {
            QDataStream stream(file);
            stream << this;

            // <?> Возможны ли проблемы при записи?

            file->close();
            return;
        }
        qDebug() << "NOTE: Не создал файл программных настроек, гад!";
    }
};

#endif
