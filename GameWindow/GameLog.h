#ifndef GAMELOG_H
#define GAMELOG_H

#include "Interaction.h"

// Данные игры сохраняются в файлик!
class GameLog : public Interaction
{
    Q_OBJECT

public:
    QFile * logFile;
    QTextStream log;

    void logStart();
    void logMillChoice();
    void logPlan();

    explicit GameLog(Game * game, qint8 PlayerIndex, GraphicObject * parent);
public slots:
    virtual void Delete();
};

#endif // GAMELOG_H
