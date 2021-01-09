#ifndef GAME_WINDOW
#define GAME_WINDOW

#include "Technical/Client.h"
#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Technical/AnimationManager.h"

#include "GameExecution/Game.h"
#include "GameExecution/GameLog.h"
#include "GameElements/FieldWindow.h"
#include "GameElements/Unit.h"
#include "GameElements/Rocket.h"
#include "GameElements/CureIcon.h"

#include "Interface/PlayerResources.h"
#include "Interface/DayTimePanel.h"
#include "Interface/GoButton.h"
#include "Interface/Dialog.h"
#include "Interface/LeftPanelsSwitch.h"
#include "Interface/SpecialButton.h"
#include "Interface/StartChoiceProgress.h"
#include "Interface/Timer.h"
#include "Interface/Tunes.h"

class GameWindow : public GraphicObject
{
    Q_OBJECT

    // --------------------------------------------------------------------------------
    // упорядочивание событий и их исполнение
    // GameWindow.cpp
public:
    explicit GameWindow(Game * game, qint8 PlayerIndex, GraphicObject * parent);
    virtual void Delete();

protected:
    bool isRealizingFinished = false;

    // упорядочивание событий от игры!
    void MixEvents();

protected slots:
    void NextPhase();  // Обработка очереди событий

protected:
    GameMessage * lastDayTimeEvent(int base);  // находит последнее событие на данное время дня

    // --------------------------------------------------------------------------------
    // реализация игровых событий
    // EventsRealization.cpp

    // По окончании реализации также выдаются указания к дальнейшим действиям
    // например, можно попросить подождать какого-то действия пользователя
    // или нажатия на кнопку go
    // или через некоторое время перейти к...
    // либо к следующему действию, либо к продолжению выполнения текущего
    // за последний случай отвечает эта переменная
    int Substate = 0;
    enum W_TYPE {WAIT_FOR_INTERACTION, NO_WAIT, BUTTON};

    class WaitingType  // эта информация хранится вот в такой структуре
    {
    public:
        W_TYPE type;
        bool finished_this_event;
        int time;

        WaitingType(W_TYPE type = NO_WAIT, int time = 0, bool finished_this_event = true)
        {
            this->type = type;
            this->time = time;
            this->finished_this_event = finished_this_event;
        }
    };
    virtual WaitingType RealizeEvent();  // обработать игровое событие

    // реализации события сражения достойны выделения в отдельные функции
    void UnitsFightObject(QSet <GameUnit *> fighters, const Strike & strike,
                          QMap <GameUnit *, QPointF > & ans, QMap <PlayerColor, QSet<QPointF> > & team_ans);
    void UnitsFight(QSet <GameUnit *> fighters, const Strike & strike,
                    const QMap <GameUnit *, QPointF > & points, const QMap <PlayerColor, QSet<QPointF> > & team_points);

    // --------------------------------------------------------------------------------
    // реагирование на действие пользователей
    // Interaction.cpp
protected:
    void initialConnections();
    void unitConnections(Unit * unit);  // проведение всех connect-ов
    void orderPicsConnections(OrderPic * pic);

    bool planned_to_go_home = false;
    GameLog * log;

    void CheckNextPhase();  // проверяет, нужно ли просчитать следующий раунд игры

protected slots:
    virtual void hexClicked(Coord c);
    void hexLeft(Coord c);
    virtual void hexEntered(Coord c);

    void orderPicEntered(OrderType type, PlayerColor color);
    void orderPicLeft(OrderType type, PlayerColor color);

    virtual void orderVariantClicked(OrderType type);  // выбран приказ с панели
    virtual void unitTypeChosen(UnitType type);

    virtual void unitHoverEntered(GameUnit * unit);
    virtual void unitHoverLeft(GameUnit * unit);
    virtual void unitLeftClicked(GameUnit *unit);

    void dayTimeClicked(DayTime time);
    void startUnitTypeClicked(bool on, QString type);

    void timerExpired();

    void readFromOpponent(QByteArray in); // пришла посылка от противника
    void sendPlan();
    virtual void GoButtonPushed();
    void DialogReturned(bool, QString sig_mes);
    void NextButtonClicked();

    void serverDisconnected();
    void opponentDisconnected(qint8 index);
    void opponentReconnected(qint8 index);

    void whiteFlagClicked();
    virtual void homeButtonClicked();

public:
    virtual void giveup();

signals:
    void writeToOpponent(QByteArray);
    void GoHome();

protected:
    // --------------------------------------------------------------------------------
    // Контроль текущего состояния интерфейса и переключения между ними
    // То есть что включено, что интерактивно, какие действия валидны в текущий момент, и т.д.
    // StateControl.cpp
    enum State {CHOOSING_HEX,                // выбор гекса
                WAIT_FOR_ENEMY_START_POINT,  // ожидание выбора гекса оппонентов
                PLANNING,                    // планирование
                CHOOSE_ORDER_PARAMETER,      // выбор параметров приказа
                ARE_YOU_SURE_DIALOG,         // диалог "вы уверены?"
                WAIT_FOR_ENEMY_PLAN,         // ожидание планов оппонентов
                REALIZATION_PHASE}           // реализация планов
                   state = CHOOSING_HEX;
    State prev_state;

protected:
    bool isSelectingEnemyUnit = false;

protected:
    void getReadyToChooseHex(QList<Coord> variants);  // подготовить интерфейс к выбору стартовой позиции
    void getReadyToPlanning();  // подготовиться к планированию
    void getReadyToRealization();  // подготовиться к реализации

    bool setNextTime();  // установка времени: следующего, предыдущего, конкретного...
    void setPreviousTime();
    void setTime(DayTime time);
    void setEarlyTime(DayTime time);
    void setLateTime(DayTime time);

    void getReadyToChooseOrderParameter();  // начать выбирать параметр приказа
    void breakChoosingOrderParameter();  // прервать выбор параметра
    void finishedChoosingOrderParameter();  // завершить выбор параметра

    void showPlannedOrder(GameUnit * unit);  // показать исполнение приказа как запланированный в данный момент
    void deshowPlannedOrder(GameUnit * unit, bool several = false);  // обратная операция
    void finishPlannedOrder(GameUnit * unit);  // считать исполнение приказа завершённым
    void definishPlannedOrder(GameUnit * unit);  // обратная операция

    void deplanOrder(GameUnit *unit, int time);  // убрать запланированный план

    // --------------------------------------------------------------------------------
    // Отдельные игровые события
    // Events.cpp
protected:
    Unit * selectedUnit = NULL;  // выделенный юнит
    DayTime dayTime = -1;  // текущее время, -1 если не выбрано

    void disableWholeField();
    void delightWholeField();

    void newUnit(GameUnit * unit, Coord where);
    void blowUnit(GameUnit * unit);

    // перемещения юнита при планировании и не только
    bool moveUnit(Unit * unit, Coord cord, Hex::POSITION_STATE position_type = Hex::STAY);
    void moveUnitBack(Unit * unit);
    void moveUnitForward(Unit * unit);
    void moveToWayUnit(Unit * unit, Coord to);

    void anchorOrderFromUnit(Unit *unit, Order *order);
    void anchorOrderToUnit(Unit *unit, Order *order);

    void fortify(Hex * hex, int amount, PlayerColor color = "Neutral", QPointF origin = QPointF(UNDEFINED, UNDEFINED));
    void deleteAllFortification(Hex * hex);

    void createWay(Unit * unit);
    void shoot(QString color, QPointF base, QPointF top, int value, Rocket::RocketType type = Rocket::ROCKET);

    void newResources(GameHex * hex, PlayerColor color, QList<Resource> resources, QList<bool> burn);
    void disappearAllTurnedOffOrders();  // все картинки использованных приказов исчезают
    void reconfigureResources();     // обновляет положение ресурсов на панелях игроков

    // --------------------------------------------------------------------------------
    // Организация неигровых элементов интерфейса и их, фу-фу, геометрия
    // InterfaceOrganization.cpp

protected:
    QMap <PlayerColor, PlayerResources *> player_windows;
    QString leftPanel;
    QList <LeftPanelSwitcher *> leftPanelSwitchers;

    GoButton * go;
    NextButton * next;

    Timer * timer;
    DayTimePanel * dayTimeTable;
    UnitsTune * startUnitsChoice;
    FieldWindow * fieldControl;

    StartChoiceProgress * startChoiceProgress;

    SpecialButton * whiteFlag;
    SpecialButton * homeButton;

    Hex * hexCopy;

    Dialog * dialog;

    void initInterface();
    void deleteInterface();

    QRectF leftPanelGeometry();
    QRectF closedLeftPanelGeometry();

    void resize(qreal W, qreal H);
    void resizeHexCopy();
    void resizeDayTimeTable(qreal W, qreal H);
    void resizeLeftPanelSwitchers(qreal W, qreal H);
    virtual void resizeDialog(qreal W, qreal H);

protected slots:
    void leftPanelSwitcherClicked(QString name);


    // --------------------------------------------------------------------------------
    // графическая основа интерфейса, геометрия и прочая технотень
    // GameElementsOrganization.cpp

protected:
    Game * game;

    PlayerColor mainPlayerColor = "";
    quint8 PlayerIndex = UNDEFINED;

    Field * field;
    QMap <GameHex *, Hex *> hexes;
    Hex * hex(Coord coord)
    {
        return hexes[game->hex(coord)];
    }
    Hex * hex(int i, int j)
    {
        return hex(Coord(i, j));
    }
    inline Hex * hex(Unit * u)
    {
        return hex(u->where());
    }

    QMap <GameUnit *, Unit *> units;

    QMap <GameOrder *, Order *> orders;

    QMap <Hex *, QList <Fortification *> > fortifications;

    QMap <Unit *, QStack <UnitWay *> > ways_to;
    QMap <Unit *, QStack <UnitWay *> > ways_from;

    QMap <Unit *, QList<Rocket *> > rockets;

    QPointF adjacentHexCoord(Hex * hex, WAY way);

    QPointF unitCoordinate(Unit * u);
    void resizeUnits();
    QPointF wayToStartCoordinate(UnitWay * way);
    QPointF wayToEndCoordinate(UnitWay * way);
    QPointF wayFromStartCoordinate(UnitWay * way);
    QPointF wayFromEndCoordinate(UnitWay * way);
    void resizeWays();
    void resizeRockets();
    QRectF fortificationGeometry(Hex * hex, int index);
    void resizeFortifications();

    void allHexesToReconfigurePoints();
    void reconfigureUnits();
    void reconfigureWays();
    void reconfigureRockets();
    void reconfigureFortifications();

protected slots:
    void hexPointsChanged();
};

#endif
