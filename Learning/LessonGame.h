#ifndef LESSONGAME_H
#define LESSONGAME_H

#include "GameExecution/Game.h"
#include "LessonMessages.h"

class LessonGame : public Game
{
public:
    int N;

    explicit LessonGame(GameRules * rules, int N) :
        Game(rules, new Random(QList<int>() << 0))
    {
        this->N = N;
        GenerateField();  // из конструктора Games виртуальные функции нас подводят
    }

    int round = 0;
    LessonMessage * AddEvent()
    {
        LessonMessage * Le = new LessonMessage();
        events << Le;
        return Le;
    }
    LessonMessage * AddEvent(int i)
    {
        LessonMessage * Le = new LessonMessage();
        events.insert(i, Le);
        return Le;
    }
    bool antirepeat = false;

    void GenerateField();

private:
    GameUnit * unit1;
    GameUnit * unit2;
    GameUnit * unit3;
    GameUnit * enemy_unit1;
    GameUnit * enemy_unit2;
    GameUnit * enemy_unit3;

public:
    void StartGame();

    void SwitchHome(GameUnit * unit, Coord new_home, GameHex::HexStatus status = GameHex::TO_BE_CONQUERED)
    {
        hex(unit->home)->status = status;
        hex(new_home)->status = GameHex::HOME;
        unit->home = new_home;
    }
    bool checkIfCaptured(Coord c)
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            Coord unit_c = unit->position;
            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] == NULL)
                    break;

                if (unit->plan[i]->type == "Attack" ||
                     unit->plan[i]->type == "Siege" ||
                     unit->plan[i]->type == "Go")
                {
                    unit_c = unit->plan[i]->actions[1].target;
                }
                if (unit->plan[i]->type == "Capture")
                {
                    if (c == unit_c)
                        return true;
                }
            }
        }
        return false;
    }
    bool checkIfBlocked(Coord c)
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            Coord unit_c = unit->position;

            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] == NULL)
                    break;

                if (unit->plan[i]->type == "Attack" ||
                     unit->plan[i]->type == "Siege" ||
                     unit->plan[i]->type == "Go")
                {
                    unit_c = unit->plan[i]->actions[1].target;
                }
            }

            if (unit_c == c)
                return true;
        }
        return false;
    }
    bool checkIfExecuted(Coord c, OrderType order)
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            Coord unit_c = unit->position;
            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] == NULL)
                    break;

                if (unit->plan[i]->type == "Attack" ||
                     unit->plan[i]->type == "Siege" ||
                     unit->plan[i]->type == "Go")
                {
                    unit_c = unit->plan[i]->actions[1].target;
                }
                if (unit->plan[i]->type == order)
                {
                    if (c == unit_c)
                        return true;
                }
            }
        }
        return false;
    }
    bool checkIfEmpty()
    {
        foreach (GameUnit * unit, players[rules->players[0]]->units)
        {
            for (int i = 0; i < rules->dayTimes; ++i)
            {
                if (unit->plan[i] != NULL && unit->plan[i]->type != DefaultOrder)
                    return false;
            }
        }
        return true;
    }
    bool checkIfPlanIsGood();

    void PlanRealisation();
};



#endif // LESSONGAME_H

