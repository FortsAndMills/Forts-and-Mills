#ifndef APPLICATION_H
#define APPLICATION_H

#include "Constants.h"
#include "ProgrammSettings.h"
#include "Pictures.h"
#include "AnimationManager.h"
#include "Client.h"
#include "Headers.h"

class Application : public QApplication
{
public:
    explicit Application(int argc, char *argv[]) : QApplication(argc, argv)
    {
        QCoreApplication::setApplicationName("Forts And Mills");  // настроечки

        constants = new Constants();  // инициализируем ключевые классы
        settings = new Settings(this);
        pictures = new Pictures();
        animations = new AnimationManager();
        client = new Client();
    }

    // TODO сохранение приложения при внезапном выключении компа
    /*void commitData(QSessionManager * qsm)
    void saveState(QSessionManager * qsm)*/
};

#endif // APPLICATION_H
