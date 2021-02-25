#ifndef LANGUAGESELECTOR_H
#define LANGUAGESELECTOR_H

#include "Technical/Headers.h"
#include "Technical/ProgrammSettings.h"
#include "BasicElements/StateObject.h"

const QList<QString> LANGUAGES({"eng", "rus"});

class LanguageVariant : public GraphicObject
{
    Q_OBJECT

    QString name;

public:
    explicit LanguageVariant(GraphicObject *parent, QString name) :
        GraphicObject(parent, CLICKABLE, name, "", "SimpleLayer", true)
    {
        this->name = name;
    }

public:
    void leftClick() { emit clicked(name);}

signals:
    void clicked(QString);
};

class LanguageSelectorPanel : public StateObject
{
    Q_OBJECT

    QList <LanguageVariant *> variants;

public:
    explicit LanguageSelectorPanel(GraphicObject *parent) :
        StateObject(parent, "default", "NeutralDialogWindow", 0, "", "", false, constants->unitPanelAppearTime)
    {
        this->setOpacity(0);
        addGeometry("open", QRectF(0, -1, 1, 1));

        foreach (QString var, LANGUAGES)
        {
            variants << new LanguageVariant(this->cur_object(), var);
            connect(variants.last(), SIGNAL(clicked(QString)), parent, SLOT(chosen(QString)));
        }
    }

public:
    void resizeChildren(qreal W, qreal H)
    {
        // TODO: перенести в константы?
        qreal margin = 0.3;
        qreal h = H / (variants.size() * (margin + 1) + margin);
        qreal y = margin * h;

        for (int i = 0; i < variants.size(); ++i)
        {
            variants[i]->setGeometry(0.1 * W, y, 0.8 * W, h);
            y += h * (1 + margin);
        }
    }

    void appear()
    {
        this->setEnabled(true);
        this->AnimationStart(OPACITY, 1, constants->unitPanelAppearTime);
        this->setGeometryState("open");
    }
    void disappear()
    {
        this->setEnabled(false);
        this->AnimationStart(OPACITY, 0, constants->unitPanelAppearTime);
        this->setGeometryState("default");
    }
};

class LanguageSelector : public StateObject
{
    Q_OBJECT

    LanguageSelectorPanel * panel;
    bool panel_opened = false;

public:
    explicit LanguageSelector(GraphicObject * parent) :
        StateObject(parent, settings->language, settings->language,
                    CLICKABLE, "", "SimpleLayer", true)
    {
        foreach (QString var, LANGUAGES)
        {
            if (var != settings->language)
            {
                addPicture(var, var);
            }
        }
        panel = new LanguageSelectorPanel(this);
    }

    void resizeChildren(qreal W, qreal H)
    {
        panel->resize(W, H * 1.4);
    }

public slots:
    void leftClick() override
    {
        if (panel_opened)
        {
            panel->disappear();
            panel_opened = false;
        }
        else
        {
            panel->appear();
            panel_opened = true;
        }
    }
    void chosen(QString new_language)
    {
        panel->disappear();
        panel_opened = false;
        settings->language = new_language;
        this->setPictureState(new_language);
        emit languageChanged();
    }

signals:
    void languageChanged();
};

#endif // LANGUAGESELECTOR_H
