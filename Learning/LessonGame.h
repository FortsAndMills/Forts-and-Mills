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
                          // (они ещё не созданы и их нельзя вызвать)
                          // поэтому приходится создавать поле по обычным правилам
                          // затем очищать и заполнять как надо для урока
    }


    LessonMessage * AddEvent() override
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

    void GenerateField() override;

    // вспомогательные переменные
    int round = 0;  // номер раунда
    bool antirepeat = false;  // если игрок ошибается, нужно показывать ему разные
                              // сообщения об ошибке
    QString final_message;    // переменная для туториала 4

private:
    GameUnit * unit1;
    GameUnit * unit2;
    GameUnit * unit3;
    GameUnit * enemy_unit1;
    GameUnit * enemy_unit2;
    GameUnit * enemy_unit3;

public:
    void StartGame() override;

    bool checkIfPlanIsGood();

    int NextStage() override;

private:
    // пара утилит
    // переносит дом юнита из клетки, где он появился, в другую
    // удобно делать так, чем переносить самого юнита
    void SwitchHome(GameUnit * unit, Coord new_home, GameHex::HexStatus status = GameHex::TO_BE_CONQUERED)
    {
        hex(unit->home)->status = status;
        hex(new_home)->status = GameHex::HOME;
        unit->home = new_home;
    }
    bool checkIfEmpty()  // проверка пустоты плана
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
};



#endif // LESSONGAME_H

