#ifndef GAMEEVENT_H
#define GAMEEVENT_H

#include "Technical/Headers.h"
#include "GameStructures.h"

// Техническая структура для передачи данных о событиях
// из игрового модуля в графический

enum GameEventType
{
    CHOOSE_HEX,
    START_CHOICE_MADE,
    NEW_UNIT_APPEAR,
    HEX_CAPTURED,
    RESOURCE_APPEAR,
    GATHER_RESOURCES,
    PLAN,
    HEX_DEFENSE_FILLED,
    UNIT_DEFENSE_FILLED,
    TIME_STARTED,
    ORDER_REALIZATION_STARTED,
    AGITATED,
    UNIT_CAPTURES_HEX,
    DEFENSE_BONUS_APPEARS,
    UNIT_IS_GOING_TO_RECRUIT,
    RECRUIT_FAILS_BECAUSE_OF_AGITE,
    UNIT_LEAVES,
    PURSUE_TRIGGERED,
    PURSUE_NOT_TRIGGERED,
    UNIT_ENTERS,
    UNITS_ARE_GOING_TO_FIGHT,
    ORDER_BURNS_IN_FIGHT,
    UNITS_FIGHT,
    HEX_STATUS_CHANGE,
    UNIT_DIES,
    UNIT_FINISHES_ENTER,
    RECRUITED_KILLED,
    UNIT_LOOSES_HOME,
    UNIT_DECAPTURES_HEX,
    DEFENSE_BONUS_DISAPPEARS,
    FORTIFICATION_DISAPPEARS,
    SHOOT_LEFT,
    UNIT_CURES,
    FORTIFIED,
    ORDER_REALIZATION_FINISHED,
    TIME_FINISHED,
    PLAN_REALIZATION_FINISHED,
    HOMELESS_UNIT_DAMAGED,
    AGITATION_ENDS,
    BURN_RESOURCE,
    DAY_TIMES_CHANGED,
    PLAYER_LOSES,
    WIN,
    LESSON
};

class GameMessage
{
public:
    GameEventType type;

    // всевозможные параметры
    QList <Coord> variants;
    GameUnit * unit = NULL;
    GameUnit * unit2 = NULL;
    GameUnit * bruter = NULL;
    QSet <GameUnit *> relevants;
    QSet <GameUnit *> fighters;
    GameHex::HexStatus hexStatus;
    Strike strike;
    int amount;
    GameHex * hex;
    GameHex * fort;
    PlayerColor color;
    QList<Resource> resources;
    QList<bool> burn;
    bool isActiveOrderBurns;
    DayTime time;
    Resource R;
    UnitType unitType;
    QMap<GameUnit *, OrderType> plan;

    explicit GameMessage(){}
    virtual ~GameMessage(){}
    virtual void ChooseHex(QList <Coord> variants)
    {
        type = CHOOSE_HEX;
        this->variants = variants;
    }
    void StartChoiceMade()
    {
        type = START_CHOICE_MADE;
    }
    void NewUnitAppear(GameUnit * unit, GameHex * hex)
    {
        type = NEW_UNIT_APPEAR;
        this->unit = unit;
        this->hex = hex;
    }
    void HexCaptured(GameHex * hex, PlayerColor color)
    {
        type = HEX_CAPTURED;
        this->hex = hex;
        this->color = color;
    }
    void NewResource(Resource R, GameHex * hex)
    {
        type = RESOURCE_APPEAR;
        this->hex = hex;
        this->R = R;
    }
    void GatherResources(GameHex * hex, PlayerColor color, QList<Resource> resources, QList<bool> burn)
    {
        type = GATHER_RESOURCES;
        this->hex = hex;
        this->color = color;
        this->resources = resources;
        this->burn = burn;
    }
    virtual void Plan()
    {
        type = PLAN;
    }
    void HexDefenseFilled(GameHex * hex, int amount)
    {
        type = HEX_DEFENSE_FILLED;
        this->hex = hex;
        this->amount = amount;
    }
    void UnitDefenseFilled(GameUnit * unit, int amount)
    {
        type = UNIT_DEFENSE_FILLED;
        this->unit = unit;
        this->amount = amount;
    }
    void TimeStarted(DayTime time, QMap<GameUnit *, OrderType> plan)
    {
        type = TIME_STARTED;
        this->time = time;
        this->plan = plan;
    }
    void OrderRealizationStarted(GameUnit * unit, PlayerColor color, Resource R)
    {
        type = ORDER_REALIZATION_STARTED;
        this->unit = unit;
        this->color = color;
        this->R = R;
    }
    void Agitated(GameUnit * unit, GameHex * hex)
    {
        type = AGITATED;
        this->hex = hex;
        this->unit = unit;
    }
    void UnitCapturesHex(GameUnit * unit, GameHex * hex, PlayerColor color)
    {
        type = UNIT_CAPTURES_HEX;
        this->hex = hex;
        this->unit = unit;
        this->color = color;
    }
    void DefenseBonusAppears(GameUnit * unit, GameHex * hex, PlayerColor color, int amount)
    {
        type = DEFENSE_BONUS_APPEARS;
        this->unit = unit;
        this->hex = hex;
        this->color = color;
        this->amount = amount;
    }
    void UnitIsGoingToRecruit(GameUnit * unit, GameHex * hex, UnitType unitType)
    {
        type = UNIT_IS_GOING_TO_RECRUIT;
        this->hex = hex;
        this->unit = unit;
        this->unitType = unitType;
    }
    void RecruitFailsBecauseOfAgite(GameUnit * unit, OrderType R, GameHex * hex)
    {
        type = RECRUIT_FAILS_BECAUSE_OF_AGITE;
        this->hex = hex;
        this->R = R;
        this->unit = unit;
    }
    void UnitLeaves(GameUnit * unit, GameHex * hex)
    {
        type = UNIT_LEAVES;
        this->hex = hex;
        this->unit = unit;
    }
    void PursueTriggered(GameUnit * unit, GameUnit * unit2)
    {
        type = PURSUE_TRIGGERED;
        this->unit = unit;
        this->unit2 = unit2;
    }
    void PursueNotTriggered(GameUnit * unit, GameUnit * unit2)
    {
        type = PURSUE_NOT_TRIGGERED;
        this->unit = unit;
        this->unit2 = unit2;
    }
    void UnitEnters(GameUnit * unit, GameHex * hex, QSet <GameUnit *> relevants)
    {
        type = UNIT_ENTERS;
        this->hex = hex;
        this->unit = unit;
        this->relevants = relevants;
    }
    void UnitsAreGoingToFight(QSet <GameUnit *> fighters)
    {
        type = UNITS_ARE_GOING_TO_FIGHT;
        this->fighters = fighters;
    }
    void OrderBurnsInFight(GameUnit * unit, PlayerColor color, Resource R)
    {
        type = ORDER_BURNS_IN_FIGHT;
        this->unit = unit;
        this->color = color;
        this->R = R;
    }
    void UnitsFight(QSet <GameUnit *> fighters, Strike & strike)
    {
        type = UNITS_FIGHT;
        this->fighters = fighters;
        this->strike = strike;
    }
    void HexStatusChanged(GameHex * hex, GameHex::HexStatus hexStatus, PlayerColor color, GameUnit * unit, QSet <GameUnit *> authors = QSet <GameUnit *>())
    {
        type = HEX_STATUS_CHANGE;
        this->hex = hex;
        this->hexStatus = hexStatus;
        this->color = color;
        this->unit = unit;
        this->relevants = authors;
    }
    void UnitDies(GameUnit * unit, QList <Resource> resources, bool isActiveOrderBurns, QSet <GameUnit *> relevants)
    {
        type = UNIT_DIES;
        this->unit = unit;
        this->resources = resources;
        this->isActiveOrderBurns = isActiveOrderBurns;
        this->relevants = relevants;
    }
    void UnitFinishesEnter(GameUnit * unit, GameHex * hex)
    {
        type = UNIT_FINISHES_ENTER;
        this->unit = unit;
        this->hex = hex;
    }
    void RecruitedKilled(GameUnit * unit, GameHex * hex)
    {
        type = RECRUITED_KILLED;
        this->unit = unit;
        this->hex = hex;
    }
    void UnitLoosesHome(GameUnit * unit, GameHex * hex, GameUnit * reason)
    {
        type = UNIT_LOOSES_HOME;
        this->unit = unit;
        this->hex = hex;
        this->unit2 = reason;
    }
    void UnitDecapturesHex(GameUnit * unit, GameHex * hex)
    {
        type = UNIT_DECAPTURES_HEX;
        this->unit = unit;
        this->hex = hex;
    }
    void DefenseBonusDisappears(GameUnit * unit, GameHex * hex)
    {
        type = DEFENSE_BONUS_DISAPPEARS;
        this->unit = unit;
        this->hex = hex;
    }
    void FortificationDisappears(GameUnit * unit, GameHex * hex)
    {
        type = FORTIFICATION_DISAPPEARS;
        this->unit = unit;
        this->hex = hex;
    }
    void ShootLeft(GameUnit * unit, GameHex * hex, int amount)
    {
        type = SHOOT_LEFT;
        this->unit = unit;
        this->hex = hex;
        this->amount = amount;
    }
    void UnitCures(GameUnit * unit, int amount)
    {
        type = UNIT_CURES;
        this->unit = unit;
        this->amount = amount;
    }
    void Fortified(GameUnit * unit, GameHex * hex, int amount)
    {
        type = FORTIFIED;
        this->unit = unit;
        this->hex = hex;
        this->amount = amount;
    }
    void OrderRealizationFinished(GameUnit * unit, PlayerColor color, Resource R)
    {
        type = ORDER_REALIZATION_FINISHED;
        this->unit = unit;
        this->color = color;
        this->R = R;
    }
    void TimeFinished(DayTime time)
    {
        type = TIME_FINISHED;
        this->time = time;
    }
    void PlanRealizationFinished()
    {
        type = PLAN_REALIZATION_FINISHED;
    }
    void HomelessUnitDamaged(GameUnit * unit, int damage)
    {
        type = HOMELESS_UNIT_DAMAGED;
        this->unit = unit;
        this->amount = damage;
    }
    void AgitationEnds(GameHex * hex, PlayerColor color)
    {
        type = AGITATION_ENDS;
        this->hex = hex;
        this->color = color;
    }
    void BurnResource(PlayerColor color, Resource R)
    {
        type = BURN_RESOURCE;
        this->color = color;
        this->R = R;
    }
    void DayTimesChanged()
    {
        type = DAY_TIMES_CHANGED;
    }
    void PlayerLoses(PlayerColor color)
    {
        type = PLAYER_LOSES;
        this->color = color;
    }
    void Win(PlayerColor color)
    {
        type = WIN;
        this->color = color;
    }

    // объединение всех полей с юнитами в одно множество
    // для фиксации того, какие юниты "участвуют" в этом событии
    QSet <GameUnit *> involved()
    {
        QSet <GameUnit *> ans = relevants;
        foreach (GameUnit * u, fighters)
            ans << u;

        ans << unit << unit2 << bruter;

        ans.remove(NULL);
        return ans;
    }
};

#endif // GAMEEVENT_H
