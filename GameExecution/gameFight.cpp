#include "GameFight.h"

GameFight::GameFight(GameRules *rules, Random *rand) :
    GameEvents(rules, rand)
{
}

// защищается ли юнит или обороняется
FightStatus GameFight::getFightStatus(GameUnit *unit)
{
    if (unit->going_to != NOWHERE)
        return FS_ATTACK;
    return FS_DEFEND;
}
// чем сражается каждый юнит. При игре в несколько человек units.size() >= 2
QList <FightBrid> GameFight::getStrikeTypes(QList <GameUnit *> units, QList <FightStatus> fs, int &val)
{
    // сначала нужно найти, кто будет сражаться "штрафами" противника
    // например, если у кого-то был захват, то 1 единицу боевой силы
    // технически получают все остальные игроки

    QSet <int> Penalties;  // список игроков со штрафом
    int min_penalty = 0;    // размер штрафа
    for (int i = 0; i < units.size(); ++i)
    {
        // нужно проверить, что за игрока не сражаются щиты или бонусы гекса
        // в таком случае не считается, что юнит сражается сам, и штраф "не работает"
        if (fs[i] != FS_DEFEND ||
            ((hex(units[i]->position)->fortification <= 0 || hex(units[i]->position)->fortificationColor != units[i]->color) &&
            (hex(units[i]->position)->defence <= 0)))
        {
                // источником штрафа может являться только приказ
                if (units[i]->plan[time]->fightInfluence < 0 && fs[i] != FS_DISTANT)
                {
                    int k = units[i]->plan[time]->fightInfluence;

                    Penalties << i;
                    min_penalty = min_penalty == 0 ? -k : qMin(min_penalty, -k);
                }
        }
    }

    // вопрос отображения: если штраф у всех, то все будут сражаться чужими штрафами
    if (Penalties.size() == units.size())
        Penalties.clear();

    // теперь составляем ответ на задачу
    QList <FightBrid> ans;
    QList <int> v;
    for (int i = 0; i < units.size(); ++i)
    {
        if (min_penalty != 0 && !Penalties.contains(i))
        {  // сражающиеся "чужими приказами"
            v << min_penalty;
            ans << ENEMY_ORDER_PENALTY;
        }
        else if (fs[i] == FS_DEFEND && hex(units[i]->position)->fortification > 0 && hex(units[i]->position)->fortificationColor == units[i]->color)
        {  // сражающиеся щитами
            v << hex(units[i]->position)->fortification;
            ans << FORTIFICATION;
        }
        else if (fs[i] == FS_DEFEND && hex(units[i]->position)->defence > 0)
        {  // сражающиеся защитой на гексе
            v << hex(units[i]->position)->defence;
            ans << HEX_DEFENCE;
        }
        else if (fs[i] == FS_DISTANT && units[i]->distantAttack > 0)
        {  // дистанционная атака
            v << units[i]->distantAttack;
            ans << DISTANT;
        }
        else if (fs[i] == FS_DISTANT)
        {  // если силы для дистанционной атаки нет, то бой прекращается
            v << 0;
            ans << STOP_FIGHT;
        }
        else if (units[i]->plan[time]->fightInfluence > 0)
        {  // бонус приказа
            v << units[i]->plan[time]->fightInfluence;
            ans << ORDER_BONUS;
        }
        else if (fs[i] == FS_DEFEND && units[i]->defenceBonus > 0)
        {  // защита юнита (бегемота)
            v << units[i]->defenceBonus;
            ans << UNIT_DEFENCE;
        }
        else
        {  // собственно, юнит
            v << units[i]->health;
            ans << UNIT_HEALTH;
        }
    }

    // очередная итерация боя - пока у кого-либо из юнитов не исчерпается
    // то, чем он сражается
    val = INF;
    foreach (int V, v)
        if (V < val)
            val = V;
    return ans;
}
void GameFight::checkIfGetToFight(GameUnit * unit, FightBrid fb)
{
    // проверка на то, что юнит попал в бой с вытекающими последствиями
    if (fb != FORTIFICATION && fb != HEX_DEFENCE)
    {
        if (!unit->plan[time]->wasInFight)  // на случай нескольких боёв за раунд
        {
            unit->plan[time]->wasInFight = true;

            // сжигание мирных приказов
            if (unit->plan[time]->burnsWhenFight)
            {
                players[unit->color]->resources[unit->plan[time]->type]--;
                unit->plan[time]->realizationFinished = true;
                AddEvent()->OrderBurnsInFight(unit, unit->color, unit->plan[time]->type);
            }
        }
    }
}
void GameFight::CountStrike(QList <GameUnit *> fighters, QList <FightBrid> types, int value)
{
    // Реализация боя (обмена ударами)

    // Подсчёт, сколько игроков сражаются "штрафами" других
    int penalty = 0;
    for (int i = 0; i < types.size(); ++i)
    {
        if (types[i] == ENEMY_ORDER_PENALTY)
            ++penalty;
    }

    for (int i = 0; i < fighters.size(); ++i)
    {
        // если кто-то сражается штрафами, но не этот юнит
        // значит, у него есть штраф!
        // и его надо восполнить, т.е. "использовать".
        if ((types[i] != ENEMY_ORDER_PENALTY && penalty > 0) || penalty == fighters.size())
            fighters[i]->plan[time]->fightInfluence += value;

        // юзание различных бонусов
        if (types[i] == FORTIFICATION)
            hex(fighters[i]->position)->fortification -= value;
        else if (types[i] == HEX_DEFENCE)
           hex(fighters[i]->position)->defence -= value;
        else if (types[i] == UNIT_DEFENCE)
            fighters[i]->defenceBonus -= value;
        else if (types[i] == ORDER_BONUS)
            fighters[i]->plan[time]->fightInfluence -= value;
        else if (types[i] == UNIT_HEALTH)
        {
            // для анимации требуется хранить список тех, кто "убил" данного юнита
            QSet <GameUnit *> authors;
            foreach (GameUnit * u, fighters)
                if (u != fighters[i])
                    authors << u;

            // этот список требуется функции нанесения юниту урона
            Damage(fighters[i], value, authors);
        }
        else if (types[i] == DISTANT)
            fighters[i]->distantAttack -= value;
    }
}

void GameFight::UnitsFight(QList <GameUnit *> fighters, QList <FightStatus> fs)
{
    while (fs.size() < fighters.size())
        fs << getFightStatus(fighters[fs.size()]);

    // пока бой не закончен явно
    while (fighters.size() > 1)
    {
        int val;
        QList <FightBrid> fb = getStrikeTypes(fighters, fs, val);

        // неявное завершение боя (напр. при дистанционной атаке)
        if (fb.contains(STOP_FIGHT))
        {
            return;
        }

        // делаем поверку на то, не попал ли юнит в бой (не нужно ли ему приказ сжечь, наприер)
        // и составляем список гексов, в которых находятся юниты, на случай бонуса в гексе
        QList <GameHex *> positions;
        for (int i = 0; i < fighters.size(); ++i)
        {
            checkIfGetToFight(fighters[i], fb[i]);
            positions << hex(fighters[i]->position);
        }

        // реализация итерации боя
        AddEvent()->UnitsFight(fighters, fb, positions, val);
        CountStrike(fighters, fb, val);

        // убираем мёртвых из списка бойцов
        for (int i = 0; i < fighters.size(); ++i)
        {
            if (fighters[i]->health <= 0)
            {
                fighters.removeAt(i);
                fs.removeAt(i);
                --i;
            }
        }
    }
}


