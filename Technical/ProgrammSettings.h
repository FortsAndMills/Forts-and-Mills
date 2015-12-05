#ifndef SETTINGS
#define SETTINGS

#include "Technical\Headers.h"

class Settings : public QObject  // NOTE нужно ли вообще ProgrammSettings в итоге?
{
private:
    const QString PROGRAMM_SETTINGS_FILE_NAME = "ProgrammSettings.opt";
    const QString Host_Settings_File_Name = "Connection.txt";

public:
    // ОБЪЯВЛЕНИЕ ВСЕХ НАСТРОЕК----------------------------------------------------------
    quint16 PORT = 64125;
    QString HOST_NAME = "81.195.19.44";
    int W = 8, H = 5;

#define POWER_POINT_VERSION 1
#define REAL_CLIENT 2
    int PROGRAMM_VERSION = 2;

    QPoint APPLICATION_START_POSITION = QPoint(100, 100);
    QSize APPLICATION_START_SIZE = QSize(1200, 720);
    bool IS_APPLICATION_MAXIMIZED = false;



    // ВВОД-ВЫВОД ВСЕХ НАСТРОЕК---------------------------------------------------------
    friend QDataStream &operator << (QDataStream &stream, const Settings *settings)
    {
        return stream
                    << settings->APPLICATION_START_POSITION
                    << settings->APPLICATION_START_SIZE
                    << settings->IS_APPLICATION_MAXIMIZED;
    }
    friend QDataStream &operator >>(QDataStream &stream, Settings *settings)
    {
        return stream
                    >> settings->APPLICATION_START_POSITION
                    >> settings->APPLICATION_START_SIZE
                    >> settings->IS_APPLICATION_MAXIMIZED;
    }



    // СЧИТЫВАНИЕ И ЗАПИСЫВАНИЕ-------------------------------------------------------
    explicit Settings(QObject * parent) : QObject(parent)
    {
        //read();
    }

    void read()
    {
        QFile * hostFile = new QFile(Host_Settings_File_Name, this);
        if (hostFile->open(QIODevice::ReadOnly))
        {
            QTextStream tstream(hostFile);
            tstream >> PROGRAMM_VERSION >> PORT >> HOST_NAME >> H >> W;
            hostFile->close();
        }

        QFile * file = new QFile(PROGRAMM_SETTINGS_FILE_NAME, this);
        if (file->open(QIODevice::ReadOnly))
        {
            QDataStream stream(file);
            stream >> this;

            if(stream.status() != QDataStream::Ok)
            {
                qDebug() << "ERROR: Ошибка чтения файла программных настроек";
            }
            file->close();
            return;
        }
        qDebug() << "NOTE: Отсутствует файл программных настроек";
    }
    void write()
    {        
        QFile * hostFile = new QFile(Host_Settings_File_Name, this);
        if (hostFile->open(QIODevice::WriteOnly))
        {
            QTextStream tstream(hostFile);
            tstream << PROGRAMM_VERSION << " " << PORT << " " << HOST_NAME
                          << "\r\n" << H << " " << W;
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
