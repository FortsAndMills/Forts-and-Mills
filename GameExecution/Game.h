#ifndef GAME_H
#define GAME_H

#include "GameSteps.h"

/* Структура класса игровой логики:
 * Game               - функции для внешнего вызова
 * GameSteps          - реализации отдельных шагов игры
                        ("собрать ресурсы", "убить бездомных юнитов")
 * GameOrderExecution - функция выполнения одного приказа
 * GameFight          - бой
 * GameEvents         - микро-события ("юнит умирает", "юнит теряет N здоровья")
 * GameField          - поле и генерация поля
 * GameTurns          - вспомогательные утилиты для внешнего пользования
 * GameHelp           - утилиты вида "найти союзников в той же клетке"
 * GameBase           - хранение данных, базовая геометрия
*/

class Game : public GameSteps
{
public:
    explicit Game(GameRules * rules, Random * rand);

    virtual void StartGame();

    virtual void HexChosen();

    virtual void PlanRealisation();
};

#endif
