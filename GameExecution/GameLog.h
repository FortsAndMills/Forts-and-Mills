#ifndef GAMELOG_H
#define GAMELOG_H

#include "Game.h"

// Данные игры сохраняются в файлик!
class GameLog
{
private:
    QFile * logFile;
    QTextStream log;
    Game * game;

public:
    void logStart();
    void logMillChoice();
    void logPlan();

    explicit GameLog(Game * game, qint8 PlayerIndex);
    ~GameLog();
};

#endif // GAMELOG_H
