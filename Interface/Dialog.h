#ifndef DIALOG_H
#define DIALOG_H

#include "Technical/Headers.h"
#include "Technical/HelpManager.h"
#include "BasicElements/TextObject.h"
#include "GoButton.h"
#include "SpecialButton.h"

class Dialog : public GraphicObject
{
    Q_OBJECT

protected:
    TextObject * message;
    QString signal_message;

    Object * WaitingPicture = NULL;
    Object * SupportPicture = NULL;

    EnlargingButton * ok = NULL, * cancel = NULL, * back = NULL;

    GameRules * rules;

public:
    QString color;
    explicit Dialog(GraphicObject * parent, GameRules * rules = NULL) :
        GraphicObject(parent, DRAGABLE | HOVER, "NeutralDialogWindow")
    {
        this->rules = rules;
        this->setZValue(0.99);  // <undone>        
        this->message = new TextObject(this, "");
        this->setVisible(false);
        connect(help, SIGNAL(HelpAsked(QString)), SLOT(helpAsked(QString)));
    }
    virtual void Delete()
    {
        message->Delete();
        if (WaitingPicture != NULL)
            WaitingPicture->Delete();
        if (SupportPicture != NULL)
            SupportPicture->Delete();
        if (ok != NULL)
            ok->Delete();
        if (cancel != NULL)
            cancel->Delete();
        if (back != NULL)
            back->Delete();
        GraphicObject::Delete();
    }

    // геометрия держится на неоформленных в константы цифрах...
    void resizeChildren(qreal W, qreal H)
    {
        if (SupportPicture == NULL && WaitingPicture == NULL && cancel == NULL && ok == NULL && back == NULL)
            message->setGeometry(W / 7, H / 8, 5 * W / 7, 3 * H / 4);
        else if (WaitingPicture != NULL || SupportPicture != NULL)
            message->setGeometry(W / 8, H / 16, 3 * W / 4, H / 2);
        else
            message->setGeometry(W / 16, H / 16, 7 * W / 8, 9 * H / 16);

        if (WaitingPicture != NULL)
        {
            WaitingPicture->setGeometry(W / 2 - H / 8, 5 * H / 8, H / 4, H / 4);  // <undone>
        }
        if (SupportPicture != NULL)
        {
            SupportPicture->setGeometry(W / 2 - H / 8, 5 * H / 8, H / 4, H / 4);  // <undone>
        }

        if (ok != NULL)
        {
            ok->setGeometry(W / 2 + W / 4, 11 * H / 16, H / 4, H / 4);
        }
        if (cancel != NULL)
            cancel->setGeometry(W / 2 - W / 4 - H / 4, 11 * H / 16, H / 4, H / 4);
        if (back != NULL)
            back->setGeometry(W / 2 - W / 4 - H / 4, 11 * H / 16, H / 4, H / 4);
    }

    void reset() { setVisible(false); }
    void set(PlayerColor color, QString message, bool Waiting = false, bool cancelButton = false, bool okButton = false, bool backButton = false, QString supportPic = "", QString signal_message = "")
    {
        this->color = color;
        this->signal_message = signal_message;
        this->setPicture(color + "DialogWindow");
        this->setVisible(true);
        this->message->setText(message);

        if (SupportPicture != NULL) SupportPicture->Delete();
        if (supportPic != "")
        {
            SupportPicture = new Object(this, supportPic, true);
        }
        else
            SupportPicture = NULL;

        if (ok != NULL)
        {
            ok->Delete();
            ok = NULL;
        }
        if (okButton)
        {
            ok = new EnlargingButton(this, color + "OkButton", false);
            connect(ok, SIGNAL(leftClicked()), SLOT(okClicked()));
        }

        if (cancel != NULL)
        {
            cancel->Delete();
            cancel = NULL;
        }
        if (cancelButton)
        {
            cancel = new EnlargingButton(this, color + "CancelButton", false);
            connect(cancel, SIGNAL(leftClicked()), SLOT(cancelClicked()));
        }
        else if (backButton)
        {
            cancel = new EnlargingButton(this, color + "BackButton", false);
            connect(cancel, SIGNAL(leftClicked()), SLOT(backClicked()));
        }

        if (Waiting && WaitingPicture == NULL)
        {
            WaitingPicture = new Object(this, "SandClock");
            rotateWaitingPicture();

            resizeChildren(width(), height());
        }
        else if (!Waiting && WaitingPicture != NULL)
        {
            WaitingPicture->Delete();
            WaitingPicture = NULL;
        }

        resizeChildren(width(), height());
    }

    // вспомогательные элементы для отображения справки
    // там может быть несколько текстов подряд
    QVector <QString> texts;
    int current_dialog = 0;
    QString picture;

    // эти настройки модифицируются в туториале:
    bool resetWhenExit = true;   // сворачиваем окно при нажатии на галочку
    bool showHelp = true;        // реагируем на запросы справки
    bool last_text_ok_button = true;  // показываем галочку в последнем тексте справки
public slots:
    void helpAsked(QString what)
    {
        if (showHelp)
        {
            this->color = "Neutral";
            texts = HelpManager::HelpInfo(what, rules);
            picture = HelpManager::HelpPicture(what);

            current_dialog = 0;
            set(color, texts[0], false, false, texts.size() > 1 || last_text_ok_button, false, picture);
        }
    }

    void rotateWaitingPicture()
    {
        WaitingPicture->setRotation(0);
        Animation * a = WaitingPicture->AnimationStart(ROTATION, 360, constants->waitingClockRotationTime);
        connect(a, SIGNAL(finished()), SLOT(rotateWaitingPicture()));
    }

    void okClicked()
    {
        if (current_dialog + 1 < texts.size())
        {
            ++current_dialog;
            set("Neutral", texts[current_dialog], false, false, current_dialog + 1 < texts.size() || last_text_ok_button, true, picture);
        }
        else
        {
            texts.clear();
            current_dialog = 0;

            if (resetWhenExit)
                reset();
            emit returned(true, signal_message);
        }
    }
    void cancelClicked()
    {
        if (resetWhenExit)
            reset();
        emit returned(false, signal_message);
    }
    void backClicked()
    {
        if (current_dialog > 0)
        {
            --current_dialog;
            set("Neutral", texts[current_dialog], false, false, true, current_dialog != 0, picture);
        }
        else
        {
            emit wantBack();
        }
    }

signals:
    void returned(bool ok, QString sig_message);
    void wantBack();
};

#endif // DIALOG_H
