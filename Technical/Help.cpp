#include "HelpManager.h"
#include "Game/GameRules.h"
#include "Game/GameOrder.h"
#include "Technical/ProgrammSettings.h"

QVector <QString> HelpManager::HelpInfo(QString name, GameRules * rules)
{
    if (settings->language == "rus")
    {
        if (name == "Capture")
        {
            QVector<QString> ans;
            ans << "<u>Захват</u> - захватывает клетку, в которой находится юнит.";
            ans << "В случае боя юнит получает штраф -1.";
            return ans;
        }
        if (name == "Recruit")
        {
            QVector<QString> ans;
            ans << "<u>Рекрут</u> - рекрутирует в клетке нового юнита выбранного типа."
                << "Клетка должна быть захвачена и соединятся вашими территориями с мельницей."
                << "Новый юнит появится в клетке в конце раунда. Враг не должен нарушить соединение до конца раунда."
                << "В каждой клетке можно создать юнита лишь однажды за всю игру.";

            ans << "В случае боя юнит получает штраф -1.";

            return ans;
        }
        if (name == "Liberate")
        {
            QVector<QString> ans;
            ans << "<u>Освобождение</u> - клетка, в которой находится юнит, перестаёт принадлежать противнику.";
            ans << "В конце раунда юнит противника, созданный в освобождённой клетке, погибнет.";
            ans << "В случае боя юнит получает штраф -1.";
            return ans;
        }
        if (name == "Agite")
        {
            QVector<QString> ans;
            ans << "<u>Агитация</u> - выбранная соседняя клетка не принесёт оппонентам никаких ресурсов в конце раунда";
            ans << "Также запрещает оппонентам рекрутирование на этой клетке до конца раунда";
            ans << "В случае боя юнит получает штраф -1.";

            return ans;
        }
        if (name == "Attack") return QVector<QString>()
                    << "<u>Атака</u> - боевое перемещение в соседнюю клетку.";
        if (name == "Go") return QVector<QString>()
                    << "<u>Ход</u> - перемещение в соседнюю клетку со штрафом в бою -1.";
        if (name == "Retreat") return QVector<QString>()
                    << "<u>Отступление</u> - юнит, отстреливаясь, отступает в соседнюю клетку."
                    << "Если противник в этот момент атакует исходную клетку, он получит 1 единицу урона."
                    << "В клетке, в которую юнит отступает, юнит имеет в бою штраф -2.";
        if (name == "Fire") return QVector<QString>()
                    << "<u>Выстрел</u> - наносит 2 единицы урона в выбранную клетку. Может использовать только хрюшка."
                    << "Стрелять можно не далее чем на две клетки по прямой. Нельзя стрелять через форты, горы и мельницы."
                    << "Если юнит перемещается, выстрел попадает, если стрельба велась по клетке, в которую юнит шёл."
                    //<< "Если выстрел был по соседней клетке, с которой стрелявшего атаковали, выстрел попадает по атаковавшим."
                    << "При попадании учитываются все боевые штрафы и бонусы."
                    << "В случае ближнего боя юнит получает штраф -1.";
        if (name == "Siege") return QVector<QString>()
                    << "<u>Штурм</u> - перемещение в соседнюю клетку с бонусом в бою +2. Может использовать только Каркун.";
        if (name == "Pursue") return QVector<QString>()
            << "<u>Преследование</u> - Если выбранный вражеский юнит оказывается в соседней клетке, атакует её, после чего возвращается обратно.";
        if (name == "Fortify") return QVector <QString>()
                    << "<u>Укрепление</u> - строительство в клетке щита. В случае боя юнит получает штраф -1."
                    << "Уже построенный щит даёт единоразовый бонус в обороне +1 и учитывается до бонусов в обороне фортов и бегемота.";
        if (name == "Cure") return QVector<QString>()
                    << "<u>Лечение</u> - восстанавливает 1 единицу здоровья. Здоровье не может стать больше исходного."
                    << "В случае боя юнит получает штраф -1.";
        if (name == DefaultOrder) return QVector<QString>()
                    << "<u>Пропуск хода</u> - ничего не делать. Выставляется по умолчанию.";
        if (name == DeleteLastOrder) return QVector<QString>()
                    << "Удаляет последний отданный приказ.";
        if (name == "Unknown") return QVector<QString>()
                    << "Приказ этого юнита пока остаётся сюрпризом...";


        if (name == "Scrat") return QVector<QString>()
                << "<u>Белка</u> - стандартный юнит. Полное здоровье - 5.";
        if (name == "Pig")  return QVector<QString>()
                << "<u>Хрюшка</u> - может использовать приказ выстрела. Полное здоровье - 4.";
        if (name == "Hippo") return QVector<QString>()
                << "<u>Бегемот</u> - имеет бонус в обороне +2. Полное здоровье - 4."
                << "Считается, что юнит находился в обороне, если он не перемещается в момент боя."
                << "Бонус восполняется в конце раунда.";
        if (name == "Karkun") return QVector<QString>()
                << "<u>Каркун</u> - может использовать приказ штурма. Полное здоровье - 4.";
        if (name == "Rabbit") return QVector<QString>()
                << "<u>Кролик</u> - появляется сразу два юнита с полным здоровьем 3.";
        if (name == "Mumusha") return QVector<QString>()
                << "<u>Корова</u> - Полное здоровье - 7, но появляется с 3 здоровья.";

        if (name == "Fort") return QVector<QString>()
                << "<u>Форт</u>. Количество принадлежащих игроку фортов определяет лимит ресурсов одного типа. Излишки сжигаются в конце раунда. Даёт +2 в обороне.";
        if (name == "Mill") return QVector<QString>()
                << "<u>Мельница</u>. На соединённых с мельницами территориях можно проводить рекрутирование.";
        if (name == "Mountain") return QVector<QString>()
                << "<u>Гора</u>. По этой клетке нельзя перемещаться.";
        if (name == "Lake") return QVector<QString>()
                << "<u>Озеро</u>. По этой клетке нельзя перемещаться.";
        if (name == "Simple") return QVector<QString>()
                << "<u>Клетка с ресурсами</u>. Владелец этой клетки получает указанные на ней ресурсы в конце каждого раунда.";
        if (name == "River") return QVector<QString>()
                << "<u>Река</u>. После пересечения реки юнит не может выполнять других приказов до конца раунда.";


        if (name == "Fortification") return QVector<QString>()
                << "<u>Щит</u>. Даёт единоразовый бонус в обороне +1. Этот бонус учитывается до бонуса форта и бонусов юнита."
                << "Если противник входит в не защищённую юнитами клетку, все щиты на ней исчезают.";
        if (name == "UnitIsGoingToRecruit") return QVector<QString>()
                << "В этой клетке рекрутирован юнит. Он появится в конце раунда, если в клетку не войдёт противник или не будет нарушено соединение с мельницами.";
        if (name == "NotUnitHome")
        {
            QVector<QString> ans;
            ans << "Юнит с домом в этой клетке ещё не был создан. Владелец может использовать приказ рекрута, чтобы получить нового юнита.";
            return ans;
        }
        if (name == "Tombstone") return QVector<QString>()
                << "Юнит с домом в этой клетке героически пал. Создавать юнитов здесь больше нельзя.";
        if (name == "NotConnected") return QVector<QString>()
                << "Клетка не подсоединена к мельнице. Рекрутировать здесь пока нельзя.";

        if (name == "cantrecruit_already_provided") return QVector<QString>()
                << "Юнит в этой клетке уже был создан. Больше рекрутировать здесь нельзя.";
        if (name == "cantrecruit_not_captured") return QVector<QString>()
                << "Перед использованием рекрутирования клетку нужно захватить. Это может сделать одновременно другой юнит.";
        if (name == "cantrecruit_not_connected") return QVector<QString>()
                << "Перед использованием рекрутирования клетку нужно подсоединить к одной из ваших мельниц.";
        if (name == "cantcapture_not_liberated") return QVector<QString>()
                << "Перед захватом клетку нужно освободить. Это может сделать одновременно другой юнит.";

        if (name == "PlayersTune") return QVector<QString>()
                << "На этой панели можно изменить количество игроков - от 2 до 4.";
        if (name == "NumberOfPlayers") return QVector<QString>()
                << "Здесь отображается количество игроков в партии.";
        if (name == "FieldTune") return QVector<QString>()
                << "На этой панели можно изменить размер игрового поля.";
        if (name == "FieldSize") return QVector<QString>()
                << "Здесь отображается размер игрового поля.";
        if (name == "TimerTune") return QVector<QString>()
                << "На этой панели можно изменить тип таймера в игре.";
        if (name == "UnitsInGame") return QVector<QString>()
                << "Здесь отображается, какие юниты участвуют в игре.";
        if (name == "OrdersInGame") return QVector<QString>()
                << "Здесь отображается, какие приказы участвуют в игре.";
        if (name == "DayTimesTune") return QVector<QString>()
                << "На этой панели можно настроить количество времён дня в игре: от 1 до 5 или изменяющийся режим, в котором количество времён дня будет циклически изменяться от 1 до 5.";

        if (name == "DayTimesInGame") return QVector<QString>()
                << "Здесь отображается, сколько времён дня в игре.";
        if (name == "Timer") return QVector<QString>()
                << "Здесь отображается оставшееся время.";

        if (name == "timer_1") return QVector<QString>()
                << "<u>Блиц</u><br />Время выбора старта: 30 секунд<br />Время планирования: 2 минуты";
        if (name == "timer_2") return QVector<QString>()
                << "<u>Умеренный таймер</u><br />Время выбора старта: 60 секунд<br />Время планирования: 4 минуты";
        if (name == "timer_3") return QVector<QString>()
                << "<u>Стандартный таймер</u><br />Время выбора старта: 90 секунд<br />Время планирования: 6 минуты";
        if (name == "timer_sp") return QVector<QString>()
                << "<u>Таймер по кнопке</u><br />Таймер включается как только один из оппонентов сделал свой ход.<br/>Время выбора старта: 30 секунд<br />Время планирования: 2 минуты";
        if (name == "timer_no") return QVector<QString>()
                << "<u>Выключенный таймер</u><br />Игра будет проводиться без ограничений на время хода.";


        if (name == "HowToAskHelp") return QVector<QString>()
                << "Помните: чтобы получить справку о любом элементе игры, нажмите на нём правой кнопкой мыши!";

        return QVector<QString>() << "Ой! Справка по этому вопросу куда-то делась!";
    }
    else
    {
        if (name == "Capture")
        {
            QVector<QString> ans;
            ans << "<u>Capture</u> - captures hex.";
            ans << "Penalty in combat -1.";
            return ans;
        }
        if (name == "Recruit")
        {
            QVector<QString> ans;
            ans << "<u>Recruit</u> - new unit of selected type will be recruited at the end of round."
                << "Hex must be captured and connected with your territories to one of your mills."
                << "Enemy must not break the connection or enter this hex until the end of round."
                << "Every hex provides only one unit per game."
                << "Penalty in combat -1.";

            return ans;
        }
        if (name == "Liberate")
        {
            QVector<QString> ans;
            ans << "<u>Liberation</u> - enemy loses control of current hex.";
            ans << "At the end of the round enemy's unit born in this hex will die.";
            ans << "Penalty in combat -1.";
            return ans;
        }
        if (name == "Agite")
        {
            QVector<QString> ans;
            ans << "<u>Agitate</u> - selected adjacent hex will not bring any resources to your opponents at the end of this round.";
            ans << "Also forbids opponents to recruit in this hex until the end of round.";
            ans << "Penalty in combat -1.";

            return ans;
        }
        if (name == "Attack") return QVector<QString>()
                    << "<u>Attack</u> - moves unit to adjacent hex.";
        if (name == "Go") return QVector<QString>()
                    << "<u>Move</u> - moves unit to adjacent hex with penalty in combat -1.";
        if (name == "Retreat") return QVector<QString>()
                    << "<u>Retreat</u> - moves unit to adjacent hex with penalty in combat -2."
                    << "If enemy attacks abandoned hex, he receives 1 damage.";
        if (name == "Fire") return QVector<QString>()
                    << "<u>Shoot</u> - units in selected hex receive 2 damage. Can be used only by Pig."
                    << "One can shoot any hex in the range of two hexes in a straight line. You can't shoot over forts, mills and mountains."
                    << "If unit enters shooted hex, he is hit; if unit leaves shooted hex, shoot is considered missed."
                    << "Penalty in combat -1.";
        if (name == "Siege") return QVector<QString>()
                    << "<u>Siege</u> - moves unit to adjacent hex with combat bonus +2. Can be used only by Karkun.";
        if (name == "Pursue") return QVector<QString>()
            << "<u>Pursue</u> - If selected enemy unit moves or stays in adjacent hex, attacks this hex and returns back.";
        if (name == "Fortify") return QVector <QString>()
                    << "<u>Fortify</u> - builds a shield. Penalty in combat -1."
                    << "Already created shield gives a disposable defense bonus +1. It is used before other defense bonuses of forts and Hippos.";
        if (name == "Cure") return QVector<QString>()
                    << "<u>Cure</u> - restores 1 health. Health can't become greater than initial."
                    << "Penalty in combat -1.";
        if (name == DefaultOrder) return QVector<QString>()
                    << "<u>Inaction</u> - do nothing. This is order executed by default.";
        if (name == DeleteLastOrder) return QVector<QString>()
                    << "Removes last given order.";
        if (name == "Unknown") return QVector<QString>()
                    << "This order still remains a mystery...";


        if (name == "Scrat") return QVector<QString>()
                << "<u>Scrat</u> - default unit. Full health is 5.";
        if (name == "Pig")  return QVector<QString>()
                << "<u>Pig</u> - can use shooting order. Full health is 4.";
        if (name == "Hippo") return QVector<QString>()
                << "<u>Hippo</u> - has defense bonus +2. Full health is 4."
                << "Unit is considered defending if he is not moving."
                << "Defense bonus is restored at the end of each round.";
        if (name == "Karkun") return QVector<QString>()
                << "<u>Karkun</u> - can use siege order. Full health is 4.";
        if (name == "Rabbit") return QVector<QString>()
                << "<u>Rabbit</u> - full health is 3, but appears two of them.";
        if (name == "Mumusha") return QVector<QString>()
                << "<u>Cow</u> - Full health is 7, but appears with 3.";

        if (name == "Fort") return QVector<QString>()
                << "<u>Fort</u>. The number of captured forts determines the limit of resources of one type. Excess are lost at the end of each round."
                << "Gives defense bonus +2 when captured. This defense bonus restores at the end of each round.";
        if (name == "Mill") return QVector<QString>()
                << "<u>Mills</u>. Allows recruiting on connected terrotories.";
        if (name == "Mountain") return QVector<QString>()
                << "<u>Mountain</u>. Can't move here.";
        if (name == "Lake") return QVector<QString>()
                << "<u>Lake</u>. Can't move here.";
        if (name == "Simple") return QVector<QString>()
                << "<u>Resource Hex</u>. Owner of this hex gathers shown resources at the end of each round.";
        if (name == "River") return QVector<QString>()
                << "<u>River</u>. The order of crossing river must be the last unit's order in the round.";


        if (name == "Fortification") return QVector<QString>()
                << "<u>Shield</u>. Gives a disposable defense bonus +1. It is used before other defense bonuses of forts and Hippos."
                << "If enemy enters this hex while no unit protect it, all shields are destroyed.";
        if (name == "UnitIsGoingToRecruit") return QVector<QString>()
                << "Unit has been recruited in this hex. It will appear at the end of the round if enemy does not enter the hex or the connection to mills is broken.";
        if (name == "NotUnitHome")
        {
            QVector<QString> ans;
            ans << "This hex hasn't provided unit yet. The owner can use recruit order to create new unit here.";
            return ans;
        }
        if (name == "Tombstone") return QVector<QString>()
                << "Unit created in this hex has died. Players can't recruit new units here.";
        if (name == "NotConnected") return QVector<QString>()
                << "This hex hasn't provided unit yet, but the owner has not connected it to his mill yet, so recruiting is yet unavailable.";

        if (name == "cantrecruit_already_provided") return QVector<QString>()
                << "This hex has already provided a unit. Can't recruit again here.";
        if (name == "cantrecruit_not_captured") return QVector<QString>()
                << "You have to capture the hex before recruiting. It can be done by another unit at the same time.";
        if (name == "cantrecruit_not_connected") return QVector<QString>()
                << "You have to connect this hex to one of your mills through your territories before recruiting here.";
        if (name == "cantcapture_not_liberated") return QVector<QString>()
                << "You have to liberate the hex before capturing it. It can be done by another unit at the same time.";

        if (name == "PlayersTune") return QVector<QString>()
                << "Choose number of players here.";
        if (name == "NumberOfPlayers") return QVector<QString>()
                << "The number of players is displayed here.";
        if (name == "FieldTune") return QVector<QString>()
                << "Choose the size of field here.";
        if (name == "FieldSize") return QVector<QString>()
                << "The field size is displayed here.";
        if (name == "TimerTune") return QVector<QString>()
                << "Choose a timer here.";
        if (name == "UnitsInGame") return QVector<QString>()
                << "Units included in the game are displayed here.";
        if (name == "OrdersInGame") return QVector<QString>()
                << "Orders included in the game are displayed here.";
        if (name == "DayTimesTune") return QVector<QString>()
                << "Choose the number of time slots per round: from 1 to 5 or changing mode, in which the number of slots will cycle from 1 to 5.";

        if (name == "DayTimesInGame") return QVector<QString>()
                << "Number of time slots per round is displayed here.";
        if (name == "Timer") return QVector<QString>()
                << "Remaining time is displayed here.";

        if (name == "timer_1") return QVector<QString>()
                << "<u>Blitz timer</u><br />Time to choose starting position: 30 seconds<br />Time to plan: 2 minutes";
        if (name == "timer_2") return QVector<QString>()
                << "<u>Moderate timer</u><br />Time to choose starting position: 60 seconds<br />Time to plan: 4 minutes";
        if (name == "timer_3") return QVector<QString>()
                << "<u>Default timer</u><br />Time to choose starting position: 90 seconds<br />Time to plan: 6 minutes";
        if (name == "timer_sp") return QVector<QString>()
                << "<u>Adaptive timer</u><br />Timer turns on when one of the opponents sends his plan.<br/>Time to choose starting position: 30 seconds<br />Time to plan: 2 minutes";
        if (name == "timer_no") return QVector<QString>()
                << "<u>No timer</u><br />There will be no time limitations during the game.";


        if (name == "HowToAskHelp") return QVector<QString>()
                << "Remember: you can get help about any game object by clicking on it with right mouse button!";

        return QVector<QString>() << "Oh! Help on this question got lost!";
    }
}
QString HelpManager::HelpPicture(QString name)
{
    if (name == "Capture") return "Capture";
    if (name == "Recruit") return "Recruit";
    if (name == "Liberate") return "Liberate";
    if (name == "Agite") return "Agite";
    if (name == "Attack" ) return "Attack";
    if (name == "Go") return "Go";
    if (name == "Retreat") return "Retreat";
    if (name == "Fire") return "Fire";
    if (name == "Siege") return "Siege";
    if (name == "Pursue") return "Pursue";
    if (name == "Fortify") return "Fortify";
    if (name == "Cure") return "Cure";
    if (name == "Unknown") return "Unknown";
    if (name == DefaultOrder) return DefaultOrder;

    if (name == "Scrat") return "Scrat";
    if (name == "Pig") return "Pig";
    if (name == "Karkun") return "Karkun";
    if (name == "Hippo") return "Hippo";
    if (name == "Rabbit") return "Rabbit";
    if (name == "Mumusha") return "Mumusha";

    if (name == "Fort") return "Fort";
    if (name == "Mill") return "Mill";
    if (name == "Mountain") return "Mountain";
    if (name == "Lake") return "Lake";
    if (name == "River") return "River";
    if (name == "Simple") return "NeutralHex";

    if (name == "Fortification") return "BlueFortification";
    if (name == "UnitIsGoingToRecruit") return "Scrat";
    if (name == "NotUnitHome") return "NotBlueUnitHome";
    if (name == "Tombstone") return "Tombstone";
    if (name == "NotConnected") return "NotConnected";

    //if (name == "cantrecruit_already_provided") return ;
    if (name == "cantrecruit_not_captured") return "Capture";
    if (name == "cantrecruit_not_connected") return "NotConnected";
    if (name == "cantcapture_not_liberated") return "Liberate";

    if (name == "PlayersTune") return "PlayerOnline";
    if (name == "NumberOfPlayers") return "PlayerOnline";
    if (name == "FieldTune") return "NeutralHex";
    if (name == "FieldSize") return "NeutralHex";
    if (name == "UnitsInGame") return "Scrat";
    if (name == "OrdersInGame") return DefaultOrder;
    if (name == "DayTimesTune") return "Supper";
    if (name == "DayTimesInGame") return "Supper";

    if (name == "timer_1") return "timer_1";
    if (name == "timer_2") return "timer_2";
    if (name == "timer_3") return "timer_3";
    if (name == "timer_sp") return "timer_sp";
    if (name == "timer_no") return "timer_no";
    if (name == "TimerTune") return "timer_1";
    if (name == "Timer") return "timer_1";

    if (name == "HowToAskHelp") return "Unknown";

    return "Error";
}
