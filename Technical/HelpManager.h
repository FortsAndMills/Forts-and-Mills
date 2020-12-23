#ifndef HELPMANAGER_H
#define HELPMANAGER_H

#include <QObject>
#include "Game/GameRules.h"

class HelpManager : public QObject
{
    Q_OBJECT

public:
    static QVector <QString> HelpInfo(QString name, GameRules * rules = NULL);
    static QString HelpPicture(QString name);

    explicit HelpManager() {}

signals:
    void HelpAsked(QString);
};

#endif // HELPMANAGER_H
