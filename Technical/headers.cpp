#include "Headers.h"

Settings * settings;
Constants * constants;
Images * images;
AnimationManager * animations;
DialogText * dialogtext;
HelpManager * help;
Client * client;
QFile * logFile;
QTextStream debug;


QTextStream &operator <<(QTextStream &stream, const QList<QString> list)
{
    stream << list.size();
    for (int i = 0; i < list.size(); ++i)
        stream << " " << list[i];
    stream << "\n";
    return stream;
}
QTextStream &operator >>(QTextStream &stream, QList<QString> &list)
{
    list.clear();
    int n;
    stream >> n;
    for (int i = 0; i < n; ++i)
    {
        list.append("");
        stream >> list[i];
    }
    return stream;
}
