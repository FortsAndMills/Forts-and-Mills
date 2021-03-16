#ifndef LESSONMESSAGES_H
#define LESSONMESSAGES_H

#include "GameExecution/GameMessages.h"

enum ReactionType { DEFAULT, NEXT_PHASE, BLOCK };
class Reaction
{
public:
    ReactionType type;
    QMap<QString, QString> text;
    QString picture;

    Reaction(ReactionType type = DEFAULT, const QMap<QString, QString> text = QMap<QString, QString>(), QString picture = "")
    {
        this->type = type;
        this->text = text;
        this->picture = picture;
    }
};

enum LessonMessageType
{
    SHOW_MESSAGE,
    HIDE_PLAYER_PANELS,
    HIDE_GO,
    HIDE_DAY_TIMES,
    HIDE_FIELD_CONTROL,
    HIDE_HOMES,
    HIDE_PLAYER_FORCES,
    SHOW_FIELD_CONTROL,
    SHOW_DAY_TIMES,
    SHOW_GO,
    SHOW_START_PROGRESSBAR,
    ALLOW_HELP,
    ENABLE_GO,
    DISABLE_GO,
    ENABLE_NEXT,
    DISAPPEAR_NEXT,
    LESSON_PLAN,
    LESSON_CHOOSE_HEX,
    DEFAULT_UNIT_CLICK_REACTION,
    DEFAULT_ORDER_CHOICE_REACTION,
    DEFAULT_HEX_CHOICE_REACTION,
    REACTION_ON_UNIT_CLICK,
    REACTION_ON_ORDER_CHOICE,
    REACTION_ON_HEX_CHOICE,
    REACTION_ON_RIVER_CROSS_IN_PLAN,
    PAUSE,
    EXIT
};

class LessonMessage : public GameMessage
{
public:
    LessonMessageType ltype;

    QMap<QString, QString> text;
    QString pictureName;
    bool with_ok_button, isComment;
    GameUnit * unit;
    OrderType order_type;
    GameHex * hex;
    Reaction reaction;
    bool success;
    bool clearHistory;
    bool addToHistory;
    bool enable;

    explicit LessonMessage() : GameMessage() {}

    void ShowMessage(const QMap<QString, QString>& text, bool with_ok_button = true, bool isComment = false, bool clearHistory = false, QSet <GameUnit *> involved = QSet <GameUnit *>(), QString pictureName = "")
    {
        type = LESSON;
        ltype = SHOW_MESSAGE;
        this->text = text;
        this->with_ok_button = with_ok_button;
        this->isComment = isComment;
        this->fighters = involved;
        this->pictureName = pictureName;
        this->clearHistory = clearHistory;
        this->addToHistory = true;
    }
    void ShowMessageAndPicture(const QMap<QString, QString>& text, QString pictureName, bool with_ok_button = true, bool clearHistory = false)
    {
        ShowMessage(text, with_ok_button, false, clearHistory, QSet <GameUnit *>(), pictureName);
    }
    void ShowErrorMessage(const QMap<QString, QString>& text, QString pictureName = "")
    {
        ShowMessageAndPicture(text, pictureName, false);
        addToHistory = false;
    }
    void HidePlayerPanels()
    {
        type = LESSON;
        ltype = HIDE_PLAYER_PANELS;
    }
    void HideGo()
    {
        type = LESSON;
        ltype = HIDE_GO;
    }
    void HideDayTimes()
    {
        type = LESSON;
        ltype = HIDE_DAY_TIMES;
    }
    void HideFieldControl()
    {
        type = LESSON;
        ltype = HIDE_FIELD_CONTROL;
    }
    void HideHomes()
    {
        type = LESSON;
        ltype = HIDE_HOMES;
    }
    void HidePlayerForces()
    {
        type = LESSON;
        ltype = HIDE_PLAYER_FORCES;
    }
    void AllowHelp()
    {
        type = LESSON;
        ltype = ALLOW_HELP;
    }
    void ShowDayTimes()
    {
        type = LESSON;
        ltype = SHOW_DAY_TIMES;
    }
    void ShowFieldControl()
    {
        type = LESSON;
        ltype = SHOW_FIELD_CONTROL;
    }
    void ShowGo()
    {
        type = LESSON;
        ltype = SHOW_GO;
    }
    void ShowStartProgressBar()
    {
        type = LESSON;
        ltype = SHOW_START_PROGRESSBAR;
    }
    void DisableGo()
    {
        type = LESSON;
        ltype = DISABLE_GO;
    }
    void EnableGo()
    {
        type = LESSON;
        ltype = ENABLE_GO;
    }
    void EnableNext(QSet <GameUnit *> involved = QSet <GameUnit *>(), bool enable=true)
    {
        type = LESSON;
        ltype = ENABLE_NEXT;
        this->fighters = involved;
        this->enable = enable;
    }
    void DisappearNext()
    {
        type = LESSON;
        ltype = DISAPPEAR_NEXT;
    }
    void Plan() override
    {
        type = LESSON;
        ltype = LESSON_PLAN;
    }
    void ChooseHex(QList<Coord> variants) override
    {
        type = LESSON;
        ltype = LESSON_CHOOSE_HEX;
        this->variants = variants;
    }
    void DefaultUnitClickReaction(Reaction reaction)
    {
        type = LESSON;
        ltype = DEFAULT_UNIT_CLICK_REACTION;
        this->reaction = reaction;
    }
    void DefaultOrderChoiceReaction(Reaction reaction)
    {
        type = LESSON;
        ltype = DEFAULT_ORDER_CHOICE_REACTION;
        this->reaction = reaction;
    }
    void DefaultHexChoiceReaction(Reaction reaction)
    {
        type = LESSON;
        ltype = DEFAULT_HEX_CHOICE_REACTION;
        this->reaction = reaction;
    }
    void ReactionOnUnitClick(GameUnit * unit, Reaction reaction)
    {
        type = LESSON;
        ltype = REACTION_ON_UNIT_CLICK;
        this->unit = unit;
        this->reaction = reaction;
    }
    void ReactionOnOrderChoice(OrderType order_type, Reaction reaction)
    {
        type = LESSON;
        ltype = REACTION_ON_ORDER_CHOICE;
        this->order_type = order_type;
        this->reaction = reaction;
    }
    void ReactionOnHexChoice(GameHex * hex, Reaction reaction)
    {
        type = LESSON;
        ltype = REACTION_ON_HEX_CHOICE;
        this->hex = hex;
        this->reaction = reaction;
    }
    void ReactionOnRiverCrossInPlan(Reaction reaction)
    {
        type = LESSON;
        ltype = REACTION_ON_RIVER_CROSS_IN_PLAN;
        this->reaction = reaction;
    }
    void Pause()
    {
        type = LESSON;
        ltype = PAUSE;
    }
    void Exit(bool success = true)
    {
        type = LESSON;
        ltype = EXIT;
        this->success = success;
    }
};

#endif // LESSONMESSAGES_H
