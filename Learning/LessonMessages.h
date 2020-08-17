#ifndef LESSONMESSAGES_H
#define LESSONMESSAGES_H

#include "GameExecution/GameMessages.h"

enum ReactionType { DEFAULT, NEXT_PHASE, BLOCK };
class Reaction
{
public:
    ReactionType type;
    QString text;
    QString picture;

    Reaction(ReactionType type = DEFAULT, QString text = "", QString picture = "")
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
    SHOW_FIELD_CONTROL,
    SHOW_DAY_TIMES,
    SHOW_GO,
    ENABLE_GO,
    DISABLE_GO,
    ENABLE_NEXT,
    GET_READY_TO_PLAN,
    GET_READY_TO_CHOOSE_HEX,
    DEFAULT_UNIT_CLICK_REACTION,
    DEFAULT_ORDER_CHOICE_REACTION,
    DEFAULT_HEX_CHOICE_REACTION,
    REACTION_ON_UNIT_CLICK,
    REACTION_ON_ORDER_CHOICE,
    REACTION_ON_HEX_CHOICE,
    EXIT
};

class LessonMessage : public GameMessage
{
public:
    LessonMessageType ltype;

    QString text;
    QString pictureName;
    bool with_ok_button, isComment;
    GameUnit * unit;
    OrderType order_type;
    GameHex * hex;
    Reaction reaction;
    bool success;
    bool clearHistory;
    bool addToHistory;

    explicit LessonMessage() : GameMessage() {}

    void ShowMessage(QString text, bool with_ok_button = true, bool isComment = false, bool clearHistory = false, QSet <GameUnit *> involved = QSet <GameUnit *>(), QString pictureName = "")
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
    void ShowMessageAndPicture(QString text, QString pictureName, bool with_ok_button = true, bool clearHistory = false)
    {
        ShowMessage(text, with_ok_button, false, clearHistory, QSet <GameUnit *>(), pictureName);
    }
    void ShowErrorMessage(QString text, QString pictureName = "")
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
    void EnableNext(QSet <GameUnit *> involved = QSet <GameUnit *>())
    {
        type = LESSON;
        ltype = ENABLE_NEXT;
        this->fighters = involved;
    }
    void GetReadyToPlan()
    {
        type = LESSON;
        ltype = GET_READY_TO_PLAN;
    }
    void GetReadyToChooseHex(GameMessage * message)
    {
        type = LESSON;
        ltype = GET_READY_TO_CHOOSE_HEX;
        this->variants = message->variants;
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
    void Exit(bool success = true)
    {
        type = LESSON;
        ltype = EXIT;
        this->success = success;
    }
};

#endif // LESSONMESSAGES_H
