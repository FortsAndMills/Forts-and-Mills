#include "Technical/Constants.h"
#include "Technical/ProgrammSettings.h"
#include "Game/Game.h"
#include "GameElements/FieldWindow.h"
#include "GameElements/Unit.h"
#include "Interface/PlayerResources.h"
#include "Interface/DayTimePanel.h"
#include "Interface/ArtificialGameControl.h"
#include "Interface/GoButton.h"
#include "Technical/Client.h"
#include "Technical/AnimationManager.h"

class GameWindow : public GraphicObject
{
    Q_OBJECT

private:
    enum State {CHOOSE_START_POINT,
                        WAIT_FOR_ENEMY_START_POINT,
                         WAITING_FOR_LAST_UNITS_ON_START_POINTS,
                         GETTING_RESOURCES,
                         PLANNING,
                         CHOOSE_ORDER_PARAMETER,
                         MOVING_CENTRAL_UNIT}
                   state = CHOOSE_START_POINT;
    Game * game;

    PlayerColor mainPlayerColor = "Neutral";
    quint8 PlayerIndex = UNDEFINED;

    // ГРАФИЧЕСКИЕ ИГРОВЫЕ ЭЛЕМЕНТЫ---------------------------------
    QMap <GameHex *, Hex *> hexes;
    Hex * hex(int x, int y)
    {
        return hexes[game->field[x][y]];
    }
    QMap <PlayerColor, PlayerResources *> player_windows;
    QMap <GameUnit *, Unit *> units;
    Hex * hex(Unit * u)
    {
        return hex(u->x_stack.top(), u->y_stack.top());
    }

    QMap <GameOrder *, Order *> orders;

    QMap <Unit *, QStack <UnitWay *> > ways;

    // Окна и кнопки интерфейса
    FieldWindow * field;

    GoButton * go;

    DayTimePanel * DayTimeTable;

    ArtificialControlPanel * ArtificialPanel;
    ArtificialUnitCreator * UnitCreator;

    // ВСПОМОГАТЕЛЬНЫЕ ПЕРЕМЕННЫЕ------------------------------------------------------------------
    Unit * selectedUnit = NULL;  // выделенный юнит
    DayTime dayTime = -1;  // текущее время, -1 если не выбрано
    
    QMap <Resource, int> spended; // сколько затрачено ресурсов

    bool ToBlue = false;  // нажаты ли клавиши
    bool ToRed = false;  // TODO а это нам теперь не нужно
    bool ToNeutral = false;

    bool WasUnitHomeless = true;  // был ли при переносе юнит бездомным


    // ИНИЦИАЛИЗАЦИЯ ИНТЕРФЕЙСА И ГЕОМЕТРИЯ------------------
public:
    void unitConnections(Unit * unit);
    explicit GameWindow(Game * game, qint8 PlayerIndex, GraphicObject * parent);
    // TODO деструктор

    void resize(qreal W, qreal H);

    QPointF unitCoordinate(Unit * u);
    void resizeUnits();
    QPointF wayStartCoordinate(UnitWay * way);
    QPointF wayEndCoordinate(UnitWay * way);
    void resizeWays();

    void allHexesToReconfigurePoints();
    void reconfigureUnits();
    void reconfigureWays();
    
    // ПОЛУЧЕНИЕ СИГНАЛОВ ОТ ИГРЫ-----------------------------------------------------------
public slots:
    void newResources(GameHex * hex, PlayerColor player_id,
                      QList<Resource> resources, QList<bool> burn);
    
    void anchorUnitToField(GameUnit * unit);

    void blowUnit(GameUnit * unit);
    

    // ПОЛУЧЕНИЕ СИГНАЛОВ ОТ ИНТЕРФЕЙСА----------------------------------
    void mainAnimationFinished();  // срабатывает при завершении игровых анимаций
    
    void hexClicked(int i, int j);
    
    void readFromOpponent(QByteArray in); // пришла посылка от противника
    void GoButtonPushed(bool);

    void orderVariantClicked(OrderType type);  // выбран приказ с панели
    void unitTypeChosen(UnitType type);

    void unitHoverEntered(GameUnit * unit);
    void unitHoverLeft(GameUnit * unit);
    void unitRightClicked(GameUnit *unit);
    void unitLeftClicked(GameUnit *unit);

    void dayTimeClicked(DayTime time);

    void orderPicPushed(OrderType type, QString owner, bool state);  // в табличке
    void OrderRightClicked(GameOrder *order);



    void toBluePushed(bool state);        // искусственная часть
    void toRedPushed(bool state);
    void toNeutralPushed(bool state);
    void startGamePressed(bool);
    void unitCreatorClicked(PlayerColor color, UnitType type);

    void unitWheeled(GameUnit * unit, int delta);
    void unitDragStarted(GameUnit *unit);
    void unitDragFinished(GameUnit *unit, GameHex * hex);
    
    // ФУНКЦИИ ГРАФИЧЕСКОГО ИНТЕРФЕЙСА---------------------------------------
    // основные игровые события
private:
    void startNextPhase();  // начать следующую игровую фазу
    
    void getReadyToChooseStartPoint();  // подготовить интерфейс к выбору стартовой позиции
    
    void getReadyToPlanning();  // подготовиться к планированию
    
    bool setNextTime();  // установка времени
    void setPreviousTime();
    void setTime(DayTime time);
    void setEarlyTime(DayTime time);
    void setLateTime(DayTime time);
    
    void getReadyToChooseOrderParameter();  // начать выбирать параметр приказа
    void breakChoosingOrderParameter();
    void finishedChoosingOrderParameter();

    void showPlannedOrder(GameUnit * unit);  // показать исполнение приказа как запланированный в данный момент
    void deshowPlannedOrder(GameUnit * unit, bool several = false);  // обратная операция
    void finishPlannedOrder(GameUnit * unit);  // считать исполнение приказа завершённым
    void definishPlannedOrder(GameUnit * unit);  // обратная операция

    void deplanOrder(GameUnit *unit, int time);  // убрать запланированный план
    
    void roundEnd();  // окончание одного раунда
    void disappearAllTurnedOffOrders();
    void reconfigureResources(); 
    

    // вспомогательные функции
    bool DoesEveryoneChoosedStartPoint();  // проверяет, все ли сделали ход, и если да...

    void disableWholeField();  // делает гексы недоступными для выбора
    void delightWholeField();

    void moveUnit(Unit * unit, int x, int y, Hex::POSITION_STATE position_type = Hex::STAY);
    void moveUnitBack(Unit * unit);

    void anchorOrderFromUnit(Unit *unit, Order *order);
    void anchorOrderToUnit(Unit *unit, Order *order);

    void newUnit(GameUnit * unit);

    void createWay(Unit * unit);

    // искусственные функции
    void createFreeResource(GameOrder * order);  // создаёт иконку безвольного ресурса
    void unitGetOutOfField(GameUnit * unit, int x, int y);  // юнит откалывается от поля

signals:
    void writeToOpponent(QByteArray);
};
