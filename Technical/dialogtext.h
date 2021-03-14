#ifndef DIALOGTEXT_H
#define DIALOGTEXT_H

#include "Headers.h"

class DialogText
{    
    QMap<QString, QMap<QString, QString> > text;

public:
    DialogText();

    QString get(QString key);

    QString playerGaveUp(QString player);
    QString playerLost(QString player);
    QString lostConnection(QString player);
    QString otherPlayerReconnected(QString player);
};

#endif // DIALOGTEXT_H
