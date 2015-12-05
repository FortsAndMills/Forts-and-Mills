#ifndef PICTURES
#define PICTURES

#include "Headers.h"

class PictureNameFolder
{
public:
    QString prefix;
    QList <QString> names;
    QString suffix;

    PictureNameFolder(QString pfx, QList <QString> list, QString sfx)
    {
        prefix = pfx;
        names = list;
        suffix = sfx;
    }
};

class Pictures
{
public:
    enum DesignStyle {CLASSIC};

    QList < PictureNameFolder > names;

    explicit Pictures()
    {
        style = CLASSIC;

        InitPictures();
        for (int n = 0; n < names.size(); ++n)
        {
            foreach (QString name, names[n].names)
            {
                pictures[name] = new QPixmap(names[n].prefix + name + names[n].suffix);
            }
        }

        InitDesignPictures();
    }
private:
    void InitPictures();
    void InitDesignPictures();

private:
    QMap <QString, QPixmap *> pictures;
public:
    QMap <QString, int> picture_rows;
    QMap <QString, int> picture_columns;

private:
    DesignStyle style;
    QMap <DesignStyle, QMap<QString, QString> > designPictures;
    QString designTransformation(QString name)
    {
        if (designPictures[style].contains(name))
            return designPictures[style][name];
        return name;
    }

public:
    const QPixmap get(QString name)
    {
        if (name == "")
        {
            return QPixmap();
        }

        name = designTransformation(name);
        if (pictures.contains(name))
            return *pictures[name];

        qDebug() << "ERROR: no picture with name \"" + name + "\"!!!";
        return QPixmap();
    }
};

#endif
