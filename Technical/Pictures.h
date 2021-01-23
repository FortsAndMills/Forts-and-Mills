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

class Images
{
private:
    QList < PictureNameFolder > names;

public:
    explicit Images()
    {
        InitPictures();
        for (int n = 0; n < names.size(); ++n)
        {
            foreach (QString name, names[n].names)
            {
                pictures[name] = new QPixmap(":/Content/" + names[n].prefix + "/" + name + names[n].suffix);
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
    QMap<QString, QString> designPictures;
    QString designTransformation(QString name)
    {
        if (designPictures.contains(name))
            return designPictures[name];
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

        debug << "ERROR: no picture with name \"" + name + "\"!!!" << Qt::endl;
        return QPixmap();
    }
};

#endif
