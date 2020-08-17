#include "GameLog.h"

GameLog::GameLog(Game *game, qint8 PlayerIndex, GraphicObject *parent) :
    Interaction(game, PlayerIndex, parent)
{
    QDir().mkpath(QApplication::applicationDirPath() + "/PlayedGames");
    QString logFileName = QApplication::applicationDirPath() + "/PlayedGames/game " + QDate::currentDate().toString("MMMM dd (yy), ") + QTime::currentTime().toString("hh.mm") + ".fam";
    logFile = new QFile(logFileName);
    if (!logFile->open(QFile::Text | QFile::WriteOnly))
    {
        debug << "Logfile creation error!\n";
    }

    log.setDevice(logFile);

    log << "Rules:\n" << game->rules << "\n";

    int i = 0;
    while (game->rules->players[i] != mainPlayerColor) { ++i; }
    log << "MyIndex: " << i << "\n\n";

    log << "FIELD:\n";

    for (int i = 0; i < game->rules->fieldW; ++i)
    {
        for (int j = 0; j < game->rules->fieldH; ++j)
        {
            GameHex * h = game->hexes[j][i];

            log << h->type;
            if (h->canHaveResources)
                log << " : " << h->resources;
            //else if (h->canBeChosenAsStartPoint)
            //    log << " : " << h->livingNation << "\n";
            else
                log << "\n";
        }
        log << "\n";
    }

    log.flush();
}
void GameLog::Delete()
{
    logFile->close();
    delete logFile;
    Interaction::Delete();
}

void GameLog::logMillChoice()
{
    log << "MILLS CHOICE:\n";
    for (int i = 0; i < game->rules->players.size(); ++i)
    {
        QString name = game->rules->players[i];
        if (game->players[name]->GiveUp)
            log << name << ") gives up!\n";
        else
            log << name << ") " << game->chosenHex[game->rules->players[i]] << "\n";
    }
    log << "\n";

    log.flush();
}
void GameLog::logPlan()
{
    log << "PLANS:\n";
    for (int i = 0; i < game->rules->players.size(); ++i)
    {
        QString name = game->rules->players[i];
        if (game->players[name]->GiveUp)
            log << name << " gives up!";
        else
        {
            log << name << " plan:\n";

            foreach (GameUnit * unit, game->players[name]->units)
            {
                log << "  unit #" << unit->id << "\n";
                for (int t = 0; t < game->rules->dayTimes; ++t)
                {
                    log << "    daytime " << t << ": " << unit->plan[t];
                }
            }
        }

        log << "\n";
    }
    log << "\n";
    log << "\n";

    log.flush();
}
