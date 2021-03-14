#ifndef LESSONGAMEWINDOW_H
#define LESSONGAMEWINDOW_H

#include "GameWindow/GameWindow.h"
#include "LessonGame.h"
#include "LessonMessages.h"
#include "Technical/ProgrammSettings.h"

class LessonGameWindow : public GameWindow
{
    Q_OBJECT

public:
    explicit LessonGameWindow(GraphicObject * parent, LessonGame * game) :
        GameWindow(game, 0, parent)
    {
        whiteFlag->setVisible(false);
        startChoiceProgress->setVisible(false);

        connect(dialog, SIGNAL(returned(bool,QString)), SLOT(lessonDialogReturned(bool,QString)));
        connect(dialog, SIGNAL(wantBack()), SLOT(ShowPreviousLesson()));
        dialog->resetWhenExit = false;
        dialog->showHelp = false;
        next->noCheckMode = true;
    }
    void resize(qreal W, qreal H)
    {
        GameWindow::resize(W, H);
        homeButton->setGeometry(W / 2 * (constants->playerPanelWidth - constants->homeButtonWidth),
                                                    (constants->upperPanelSize - constants->homeButtonHeight) * H,
                                                    constants->homeButtonWidth * W,
                                                    constants->homeButtonHeight * H);
    }
    void resizeDialog(qreal W, qreal H)
    {
        GameWindow::resizeDialog(W, H);
        dialog->setPos(constants->lessonDialogX * W, constants->lessonDialogY * H);
    }

    class Lesson
    {
    public:
        QMap<QString, QString> text;
        bool with_ok_button;
        QString pictureName;

        Lesson() {}
        Lesson(const QMap<QString, QString>& text, bool with_ok_button, QString pictureName)
        {
            this->text = text;
            this->with_ok_button = with_ok_button;
            this->pictureName = pictureName;
        }
    };
    QVector <Lesson> lessons;
    int lesson_shown = 0;

    void ShowLesson(Lesson L)
    {
        dialog->set("Neutral", L.text[settings->language], false, false, L.with_ok_button || lesson_shown < lessons.size() - 1, lesson_shown != 0, L.pictureName);
        resizeDialog(width(), height());
    }
    void ShowLesson(const QMap<QString, QString>& text, bool with_ok_button = false, QString pictureName = "", bool addToHistory = false)
    {        
        if (addToHistory)
        {
            lessons.push_back(Lesson(text, with_ok_button, pictureName));
            lesson_shown = lessons.size() - 1;
        }
        else
            lesson_shown = lessons.size();

        ShowLesson(Lesson(text, with_ok_button, pictureName));
    }
    void ShowLesson(const QMap<QString, QString>& text, QString picName)
    {
        ShowLesson(text, false, picName);
    }

public slots:
    void ShowPreviousLesson()
    {
        --lesson_shown;
        ShowLesson(lessons[lesson_shown]);
    }

public:
    virtual WaitingType RealizeEvent()
    {
        GameMessage * e = game->events[0];
        if (e->type == LESSON)
        {
            LessonMessage * Le = dynamic_cast<LessonMessage *>(e);
            if (Le->ltype == HIDE_PLAYER_PANELS)
            {
                foreach (PlayerResources * PR, player_windows)
                {
                    PR->setVisible(false);
                }
                return WaitingType();
            }
            else if (Le->ltype == HIDE_GO)
            {
                go->setOpacity(0);
                return WaitingType();
            }
            else if (Le->ltype == HIDE_DAY_TIMES)
            {
                dayTimeTable->setVisible(false);
                return WaitingType();
            }
            else if (Le->ltype == HIDE_FIELD_CONTROL)
            {
                fieldControl->setVisible(false);
                return WaitingType();
            }
            else if (Le->ltype == HIDE_HOMES)
            {
                allowHomes = false;
                return WaitingType();
            }
            else if (Le->ltype == HIDE_PLAYER_FORCES)
            {
                foreach (PlayerResources * PR, player_windows)
                {
                    PR->forces->setVisible(false);
                }
                return WaitingType();
            }
            else if (Le->ltype == SHOW_FIELD_CONTROL)
            {
                fieldControl->setVisible(true);
                fieldControl->setOpacity(0);
                fieldControl->AnimationStart(OPACITY, 1);
                return WaitingType();
            }
            else if (Le->ltype == SHOW_DAY_TIMES)
            {
                dayTimeTable->setVisible(true);
                dayTimeTable->setOpacity(0);
                dayTimeTable->AnimationStart(OPACITY, 1);
                return WaitingType();
            }
            else if (Le->ltype == SHOW_GO)
            {
                go->setVisible(true);
                go->setOpacity(0);
                go->AnimationStart(OPACITY, 1);
                return WaitingType();
            }
            else if (Le->ltype == SHOW_START_PROGRESSBAR)
            {
                startChoiceProgress->setVisible(true);
                startChoiceProgress->setOpacity(0);
                startChoiceProgress->AnimationStart(OPACITY, 1);
                return WaitingType();
            }
            else if (Le->ltype == DISABLE_GO)
            {
                go->enable(false);
                return WaitingType();
            }
            else if (Le->ltype == ENABLE_GO)
            {
                go->enable(true);
                return WaitingType();
            }
            else if (Le->ltype == ENABLE_NEXT)
            {
                next->enable(Le->enable);
                return WaitingType();
            }
            else if (Le->ltype == DISAPPEAR_NEXT)
            {
                next->AnimationStart(OPACITY, 0);
                return WaitingType();
            }
            else if (Le->ltype == LESSON_PLAN)
            {
                getReadyToPlanning();
                return WaitingType();
            }
            else if (Le->ltype == LESSON_CHOOSE_HEX)
            {
                getReadyToChooseHex(Le->variants);
                return WaitingType();
            }
            else if (Le->ltype == SHOW_MESSAGE)
            {
                if (Le->clearHistory)
                    lessons.clear();

                ShowLesson(Le->text, Le->with_ok_button, Le->pictureName, Le->addToHistory);

                if (Le->isComment)
                    return WaitingType();
                next->enable(true);
                return WaitingType(WAIT_FOR_INTERACTION);
            }
            else if (Le->ltype == DEFAULT_UNIT_CLICK_REACTION)
            {
                DefaultUnitClickReaction = Le->reaction;
                UnitClickReactions.clear();
                return WaitingType();
            }
            else if (Le->ltype == DEFAULT_ORDER_CHOICE_REACTION)
            {
                DefaultOrderChoiceReaction = Le->reaction;
                OrderChoiceReactions.clear();
                return WaitingType();
            }
            else if (Le->ltype == DEFAULT_HEX_CHOICE_REACTION)
            {
                DefaultHexChoiceReaction = Le->reaction;
                HexChoiceReactions.clear();
                return WaitingType();
            }
            else if (Le->ltype == REACTION_ON_RIVER_CROSS_IN_PLAN)
            {
                RiverCrossReaction = Le->reaction;
                return WaitingType();
            }
            else if (Le->ltype == REACTION_ON_UNIT_CLICK)
            {
                UnitClickReactions[Le->unit] = Le->reaction;
                return WaitingType();
            }
            else if (Le->ltype == REACTION_ON_ORDER_CHOICE)
            {
                OrderChoiceReactions[Le->order_type] = Le->reaction;
                return WaitingType();
            }
            else if (Le->ltype == REACTION_ON_HEX_CHOICE)
            {
                HexChoiceReactions[Le->hex] = Le->reaction;
                return WaitingType();
            }
            else if (Le->ltype == PAUSE)
            {
                return WaitingType(WAIT_FOR_INTERACTION);
            }
            else if (Le->ltype == EXIT)
            {
                if (Le->success)
                    emit LessonPassed(dynamic_cast<LessonGame*>(game)->N);

                emit GoHome();

                return WaitingType(WAIT_FOR_INTERACTION);
            }
        }

        WaitingType WT = GameWindow::RealizeEvent();

        if (!allowHomes)
        {
            if (e->type == UNIT_IS_GOING_TO_RECRUIT)
            {
                hexes[e->hex]->information->setVisible(false);
                hexes[e->hex]->hideInformation();
            }
            if (e->type == HEX_STATUS_CHANGE)
            {
                hexes[e->hex]->information->setVisible(false);
                hexes[e->hex]->hideInformation();
            }
        }

        return WT;
    }

    bool allowHomes = true;
    void unitHoverEntered(GameUnit *unit)
    {
        if (allowHomes)
            GameWindow::unitHoverEntered(unit);
    }
    void hexEntered(Coord c)
    {
        if (allowHomes)
            GameWindow::hexEntered(c);
    }
    void giveup() override {}
    void ask_for_close() override
    {
        emit DecidedToQuit();
    }

    Reaction RiverCrossReaction;
    void riverCrossedInPlan()
    {
        if (!RiverCrossReaction.text.empty())
            ShowLesson(RiverCrossReaction.text, RiverCrossReaction.picture);
    }

    QMap<GameUnit *, Reaction> UnitClickReactions;
    Reaction DefaultUnitClickReaction;
    void unitLeftClicked(GameUnit * unit)
    {
        Reaction R = UnitClickReactions.contains(unit) ? UnitClickReactions[unit] : DefaultUnitClickReaction;

        if (!R.text.empty())
            ShowLesson(R.text, R.picture);

        if (R.type == DEFAULT || R.type == NEXT_PHASE)
            GameWindow::unitLeftClicked(unit);

        if (R.type == NEXT_PHASE)
            NextPhase();
    }

    QMap<OrderType, Reaction> OrderChoiceReactions;
    Reaction DefaultOrderChoiceReaction;
    void orderVariantClicked(Game::PossibleChoice pos_order)
    {
        if (pos_order.status != Game::POSSIBLE)
            return;

        Reaction R = OrderChoiceReactions.contains(pos_order.variant) ? OrderChoiceReactions[pos_order.variant] : DefaultOrderChoiceReaction;

        if (!R.text.empty())
            ShowLesson(R.text, R.picture);

        if (R.type == DEFAULT || R.type == NEXT_PHASE)
            GameWindow::orderVariantClicked(pos_order);

        if (R.type == NEXT_PHASE)
            NextPhase();
    }

    QMap<GameHex *, Reaction> HexChoiceReactions;
    Reaction DefaultHexChoiceReaction;
    void hexClicked(Coord c)
    {
        Reaction R = HexChoiceReactions.contains(game->hex(c)) ? HexChoiceReactions[game->hex(c)] : DefaultHexChoiceReaction;

        if (!R.text.empty())
            ShowLesson(R.text, R.picture);

        if (R.type == DEFAULT || R.type == NEXT_PHASE)
            GameWindow::hexClicked(c);

        if (R.type == NEXT_PHASE)
            NextPhase();
    }

    void GoButtonPushed()
    {
        if (state == CHOOSE_ORDER_PARAMETER)
        {
            breakChoosingOrderParameter();
        }

        if (dynamic_cast<LessonGame*>(game)->checkIfPlanIsGood())
        {
            if (state == PLANNING || state == CHOOSE_ORDER_PARAMETER)
                getReadyToRealization();
            game->NextStage();
        }
        else
            go->enable();

        NextPhase();
    }
    void homeButtonClicked()
    {
        emit GoHome();
    }

private slots:
    void lessonDialogReturned(bool, QString)
    {
        if (lesson_shown < lessons.size() - 1)
        {
            ++lesson_shown;
            ShowLesson(lessons[lesson_shown]);
        }
        else
            NextPhase();
    }

signals:
    void LessonPassed(int);
};

#endif // LESSONGAMEWINDOW_H
